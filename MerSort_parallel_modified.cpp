#include <cstdio>
#include <cstdlib>
#include <thread>
#include <algorithm>
#include "./code/lib/new_Alice_Lib.hpp"

template<typename It>
void print(It b, It e)
{
    while(b < e)
    {
        printf("%d ", *b++);
    }
    puts("");
}

void print(std::vector<int> vec)
{
    for(auto i : vec)
    {
        printf("%d ", i);
    }
    puts("");
}


template<typename It>
void Merge(It b1, It e1, It b2, It e2, It sp)
{
    while(b1 < e1 && b2 < e2)
    {
        if(*b1 < *b2) *sp++ = *b1++;
        else *sp++ = *b2++;
    }
    while(b1 < e1) *sp++ = *b1++;
    while(b2 < e2) *sp++ = *b2++;
}

template<typename It>
void MergeSort(It b, It e, It sp)
{
    const size_t length = std::distance(b, e);
    if(length == 0 || length == 1) return;
    It mid = b + length / 2;
    MergeSort(b, mid, sp);
    MergeSort(mid, e, sp);
    Merge(b, mid, mid, e, sp);
    while(b < e) *b++ = *sp++;
}

template<typename It>
void MergeSort_parallel(It b, It e)
{
    typedef typename It::value_type T;

    const size_t length = std::distance(b, e);
    const size_t minPerThreads = 1024;
    const size_t maxThreads = (length + minPerThreads - 1) / minPerThreads;
    const size_t hardwareThreads = std::thread::hardware_concurrency();
    const size_t threadsCnt = std::min(hardwareThreads != 0 ? hardwareThreads : 2, maxThreads);
    size_t block = (length + threadsCnt - 1) / threadsCnt;

    std::vector<T> space(length);
    std::vector<std::thread> threads(threadsCnt - 1);

    It bb, ee, mm, sp;
    bb = b, ee = b + block, sp = space.begin();
    for(size_t k = 0; k < threadsCnt - 1; k++)
    {
        threads[k] = std::thread(MergeSort<It>, bb, ee, sp);
        std::advance(bb, block);
        std::advance(ee, block);
        std::advance(sp, block);
    }
    MergeSort(bb, e, sp);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    size_t cnt = threadsCnt;
    while(cnt > 1)
    {
        bb = b, mm = bb + block, ee = mm + block, sp = space.begin();
        for(size_t k = 0; k < cnt / 2 - 1; k++)
        {
            threads[k] = std::thread(Merge<It>, bb, mm, mm, ee, sp);
            std::advance(bb, 2 * block);
            std::advance(mm, 2 * block);
            std::advance(ee, 2 * block);
            std::advance(sp, 2 * block);
        }
        Merge(bb, mm, mm, e, sp);
        std::for_each(threads.begin(), threads.begin() + cnt / 2 - 1, std::mem_fn(&std::thread::join));
        for(It it = b, res = space.begin(); it < ee;)
        {
            *it++ = *res++;
        }
        block *= 2;
        cnt >>= 1;
    }
}

void check(std::vector<int> a, std::vector<int> b)
{
    if(a.size() != b.size())
    {
        printf("wrong size, expect %llu get %llu\n", b.size(), a.size());
        return;
    }
    size_t length = a.size();
    for(size_t i = 0; i < length; i++)
    {
        if(a[i] != b[i])
        {
            printf("wrong answer at %llu, expect %d get %d\n", i, b[i], a[i]);
            return;
        }
    }
    puts("correct!");
}


int main(void)
{
    puts("start");
    using std::vector;
    const int maxn = 112345678;
    vector<int> vec(maxn);
    for(int i = 0; i < maxn; i++) vec[i] = i;
    shuffle(vec.begin(), vec.end());
    vector<int> judge1(vec), judge2(vec);
    //puts("vec");
    if(maxn <= 212) print(vec);
    puts("sorting");
    double t;

    /*
     *puts("qsort start..");
     *auto cmp = [](const void* _a, const void* _b)->int{
     *    const int *a = (const int*)_a, *b = (const int*)_b;
     *    if(*a > *b) return 1;
     *    else if(*a < *b) return -1;
     *    else return 0;
     *};
     *t = clock();
     *qsort(&judge2[0], maxn, sizeof(int), cmp);
     *t = clock() - t;
     *printf("qsort done in %lf\n", t / CLOCKS_PER_SEC);
     */

    puts("std::sort start..");
    t = clock();
    std::sort(judge2.begin(), judge2.end());
    t = clock() - t;
    printf("std::sort done in %lf\n", t / CLOCKS_PER_SEC);

    puts("MergeSort start..");
    t = clock();
    MergeSort(judge1.begin(), judge1.end(), vector<int>(maxn).begin());
    t = clock() - t;
    printf("MergeSort done in %lf\n", t / CLOCKS_PER_SEC);

    puts("MergeSort_parallel start..");
    t = clock();
    MergeSort_parallel(vec.begin(), vec.end());
    t = clock() - t;
    printf("MergeSort_parallel done in %lf\n", t / CLOCKS_PER_SEC);

    if(maxn <= 212)
    {
        puts("vec");
        print(vec);
        puts("judge");
        print(judge2);
    }
    check(vec, judge2);
}



