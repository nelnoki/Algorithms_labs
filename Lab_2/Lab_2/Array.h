#pragma once
#include <iostream>
#include <cstdlib>
#include <utility>
#include <cassert>

template <typename T>
class Array final {
    public:
        explicit Array(const int capacity = 8);
        ~Array();

        Array(const Array& other);
        Array(Array&& other) noexcept;

        Array& operator=(const Array& other);
        Array& operator=(Array&& other) noexcept;

        const T& operator[](int index) const;
        T& operator[](int index);

        int insert(const T& value);
        int insert(int index, const T& value);

        void remove(int index);

        int size() const { return size_; };
        int capacity() const { return capacity_; };

        class Iterator;
        class ConstIterator;

        Iterator iterator() { return Iterator(this, data_, size_); };
        ConstIterator iterator() const { return ConstIterator(data_, size_); };

        Iterator reverseIterator() { return Iterator(this, data_, size_, true); };
        ConstIterator reverseIterator() const { return ConstIterator(data_, size_, true); };

    private:
        int size_;
        int capacity_;
        T* data_;
        void swap_(Array & other);

    public:
        class Iterator {
        public:
            Iterator(const Array* arr, T* data, int size, bool rev = false) : array_(arr), size_(size), reverse_(rev) {
                if (rev) {
                    cur_ = data + size - 1;
                    end_ = data - 1;
                }
                else {
                    cur_ = data;
                    end_ = data + size;
                }
            };
            const T& get() const {
                check_();
                return *cur_;
            };
            void set(const T& value) {
                check_();
                *cur_ = value;
            };
            void next() {
                check_();
                reverse_ ? --cur_ : ++cur_;
            };
            bool hasNext() const {
                return cur_ != end_;
            };
        private:
            const Array* array_;
            T* cur_;
            T* end_;
            int size_;
            bool reverse_;
            void check_() const {
                if (size_ != array_->size()) {
                    throw std::runtime_error("Array was changed during iteration");
                }
            }
        };

        class ConstIterator {
        public:
            ConstIterator(const T* data, int size, bool rev = false) : size_(size), reverse_(rev) {
                if (rev) {
                    cur_ = data + size - 1;
                    end_ = data - 1;
                }
                else {
                    cur_ = data;
                    end_ = data + size;
                }
            };
            const T& get() const {
                return *cur_;
            }
            void next() {
                reverse_ ? --cur_ : ++cur_;
            }
            bool hasNext() const {
                return cur_ != end_;
            }
        private:
            const T* cur_;
            const T* end_;
            int size_;
            bool reverse_;
        };
};


template<typename T>
Array<T>::Array(const int capacity) :   size_(0), capacity_(capacity),
                                        data_(static_cast<T*>(malloc(capacity * sizeof(T)))) {};

template<typename T>
Array<T>::~Array() {
    for (int i = 0; i < size_; ++i) {
        data_[i].~T();
    }
    free(data_);
};

template<typename T>
Array<T>::Array(const Array& other) :   size_(other.size_), capacity_(other.capacity_),
                                        data_(static_cast<T*>(malloc(other.capacity_ * sizeof(T)))) {
    for (int i = 0; i < size_; ++i) {
        new (data_ + i) T(other.data_[i]);
    }
};

template<typename T>
Array<T>::Array(Array&& other) noexcept : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& other) {
    Array tmp(other);
    swap_(tmp);
    return *this;
}

template<typename T>
Array<T>& Array<T>::operator=(Array&& other) noexcept {
    swap_(other);
    return *this;
}

template<typename T>
const T& Array<T>::operator[](int index) const {
    return data_[index];
}

template<typename T>
T& Array<T>::operator[](int index) {
    return data_[index];
}

template<typename T>
int Array<T>::insert(const T& value) {
    return insert(size_, value);

}

template<typename T>
int Array<T>::insert(int index, const T& value) {
    assert(index >= 0 && index <= size_ && "Index is out of range");
    if (size_ >= capacity_) {
        int new_capacity = capacity_ * 2;
        T* new_data = static_cast<T*>(malloc(new_capacity * sizeof(T)));
        for (int i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        free(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
    for (int i = size_; i > index; --i) {
        new (data_ + i) T(std::move(data_[i - 1]));
        data_[i - 1].~T();
    }
    new (data_ + index) T(value);
    ++size_;
    return index;
}

template<typename T>
void Array<T>::remove(int index) {
    assert(index >= 0 && index < size_ && "Index is out of range");
    data_[index].~T();
    for (int i = index; i < size_ - 1; ++i) {
        new (data_ + i) T(std::move(data_[i + 1]));
        data_[i + 1].~T();
    }
    --size_;
}


template<typename T>
void Array<T>::swap_(Array & other) {
    std::swap(this->size_, other.size_);
    std::swap(this->capacity_, other.capacity_);
    std::swap(this->data_, other.data_);
}


template <typename T>
void print(Array<T> a) {
    for (auto it = a.iterator(); it.hasNext(); it.next())
        std::cout << it.get() << std::endl;
    std::cout << std::endl;
}

template <typename T>
void reverse_print(Array<T> a) {
    for (auto it = a.reverseIterator(); it.hasNext(); it.next())
        std::cout << it.get() << std::endl;
    std::cout << std::endl;
}