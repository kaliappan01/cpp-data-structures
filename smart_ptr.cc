#include <iostream>

template <class T>
class SmartPtr
{
    T *ptr;

public:
    explicit SmartPtr(T *p = NULL) { ptr = p; }
    ~SmartPtr() { delete (ptr); }

    T &operator*() { return *ptr; }

    T *operator->() { return ptr; }
};

void f()
{
    SmartPtr<int> s(new int());
    // int* ptr = new int();
    // uncomment above line to check mem-leak
    /*
    ==20005== Memcheck, a memory error detector
==20005== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==20005== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==20005== Command: ./sp
==20005==
20
==20005==
==20005== HEAP SUMMARY:
==20005==     in use at exit: 40 bytes in 10 blocks
==20005==   total heap usage: 13 allocs, 3 frees, 73,772 bytes allocated
==20005==
==20005== 40 bytes in 10 blocks are definitely lost in loss record 1 of 1
==20005==    at 0x483BE63: operator new(unsigned long) (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==20005==    by 0x10923E: f() (in /home/kaliappan/Documents/workspace/cpp/sp)
==20005==    by 0x109279: main (in /home/kaliappan/Documents/workspace/cpp/sp)
==20005==
==20005== LEAK SUMMARY:
==20005==    definitely lost: 40 bytes in 10 blocks
==20005==    indirectly lost: 0 bytes in 0 blocks
==20005==      possibly lost: 0 bytes in 0 blocks
==20005==    still reachable: 0 bytes in 0 blocks
==20005==         suppressed: 0 bytes in 0 blocks
==20005==
==20005== For lists of detected and suppressed errors, rerun with: -s
==20005== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
    */
}

int main()
{
    std::unique_ptr<int> u;
    int i = 0;
    while (i < 10)
    {
        f();
        i++;
    }
    return 0;
}

/*
valgrind ouput

==19466== Memcheck, a memory error detector
==19466== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==19466== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==19466== Command: ./sp
==19466==
^C==19466==
==19466== Process terminating with default action of signal 2 (SIGINT)
==19466==    at 0x109203: f() (in /home/kaliappan/Documents/workspace/cpp/sp)
==19466==    by 0x109245: main (in /home/kaliappan/Documents/workspace/cpp/sp)
==19466==
==19466== HEAP SUMMARY:
==19466==     in use at exit: 4 bytes in 1 blocks
==19466==   total heap usage: 4,965,416 allocs, 4,965,415 frees, 19,934,364 bytes allocated
==19466==
==19466== LEAK SUMMARY:
==19466==    definitely lost: 0 bytes in 0 blocks
==19466==    indirectly lost: 0 bytes in 0 blocks
==19466==      possibly lost: 0 bytes in 0 blocks
==19466==    still reachable: 4 bytes in 1 blocks
==19466==         suppressed: 0 bytes in 0 blocks
==19466== Rerun with --leak-check=full to see details of leaked memory
==19466==
==19466== For lists of detected and suppressed errors, rerun with: -s
==19466== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/