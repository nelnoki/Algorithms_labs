#include <cstdlib>
#include <utility>

template <typename T>
class Array final {

    //declarations
    public:
        explicit Array(const int capacity = 8);
        ~Array();

        Array(const Array & other);
        Array(Array && other) noexcept;

        Array& operator=(const Array & other);
        Array& operator=(Array && other) noexcept;

        const T& operator[](int index) const;
        T& operator[](int index);

        int insert(const T& value);
        int insert(int index, const T& value);

        void remove(int index);

        int size() const;
    
        class Iterator {
            public:
                Iterator()
                {
                    ptr_ = nullptr;
                }
                const T& get() const;
                void set(const T& value);
                void next();
                bool hasNext() const;
            private:
                T* ptr_;
        };

        class ConstIterator {
            public:
                ConstIterator(){
                    ptr_ = nullptr;
                };
                const T& get() const;
                void set(const T& value);
                void next();
                bool hasNext() const;
            private:
                T* ptr_;
        };

        
        Iterator iterator() {return iterator();};
        ConstIterator iterator() const;

        Iterator reverseIterator();
        ConstIterator reverseIterator() const;

    private:
        int size_;
        int capacity_;
        T* data_;
        void swap_(Array other);
        
};

    
    template<typename T>
    Array<T>::Array(const int capacity) :   size_(0), capacity_(capacity),
                                            data_( static_cast<T*>(malloc( capacity * sizeof(T) )) ) {};

    template<typename T>
    Array<T>::~Array() {
        for (int i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        free(data_);
    };

    template<typename T>
    Array<T>::Array(const Array & other) :  size_(other.size_), capacity_(other.capacity_),
                                            data_( static_cast<T*>(malloc( other.capacity_ * sizeof(T) )) ) {
        for (int i = 0; i < size_; ++i) {
            new (data_ + i) T(other.data_[i]);
        }
    };

    template<typename T>
    Array<T>::Array(Array && other) noexcept : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    template<typename T>
    Array<T>& Array<T>::operator=(const Array & other) {
        Array tmp(other);
        swap(tmp);
        return *this;
    }

    template<typename T>
    Array<T>& Array<T>::operator=(Array && other) noexcept {
        swap(other);
        return *this;
    }

    template<typename T>
    const T& Array<T>::operator[](int index) const {
        return data_ + index;
    }

    template<typename T>
    T& Array<T>::operator[](int index) {
        return data_ + index;
    }

    template<typename T>
    int Array<T>::insert(const T& value) {
        return insert(size_, value);
        
    }

    template<typename T>
    int Array<T>::insert(int index, const T& value) {
        if (index < size_ || index > size_) return -1;
        if (size_ >= capacity_) {
            int new_capacity = capacity_ * 2;
            T* new_data = static_cast<T*>(malloc(new_capacity * sizeof(T)));
            for(int i = 0; i < size_; ++i) {
                new (new_data + i) T(std::move(data_[i]));
                data_[i].~T();
            }
            free(data_);
            data_ = new_data;
            capacity_ = new_capacity;
        }
        for(int i = size_; i > index; --i) {
            new (data_ + i) T(std::move(data_[i - 1]));
            data_[i - 1].~T();
        }
        new (data_ + index) T(value);
        ++size_;
        return index;
    }

    template<typename T>
    void Array<T>::remove(int index){
        data_[index].~T();
        for (int i = index; i < size_; ++i) {
            new (data_ + i) T(std::move(data_[i + 1]));
            data_[i + 1].~T();
        }
        --size_;
    }

    template<typename T>
    int Array<T>::size() const {
        return size_;
    }

    template<typename T>
    void Array<T>::swap_(Array other) {
        std::swap(other.size_);
        std::swap(other.capacity_);
        std::swap(other.data_);
    }

