#include "Array.h"

int main()
{
    Array<int> a;
    for (int i = 0; i < 5; ++i)
        a.insert(i + 1);

    print(a);

    reverse_print(a);

    a.remove(4);
    print(a);

    Array<int> b = Array<int>(a);
    print(b);

    for (int i = 0; i < b.size(); ++i)
        b[i] *= 2;
    print(b);

    return 0;
}
