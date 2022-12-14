#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>
#include <time.h>

#include "../base/SkipList.h"

#define NUM_THREADS 2
#define TEST_COUNT 100000
SkipList<int, std::string> testSkipList(18);

void* insertElement(void* threadId) {
    long tid;
    tid = reinterpret_cast<long> (threadId);
    std::cout << "tid : " << tid << std::endl;
    int temp = TEST_COUNT / NUM_THREADS;

    for(int i = tid * temp, cnt = 0; cnt < temp; ++i) {
        ++cnt;
        testSkipList.insertElement(rand() % TEST_COUNT, "a");
    }

    pthread_exit(NULL);
}

void* searchElement(void* threadId) {
    long tid;
    tid = reinterpret_cast<long> (threadId);
    std::cout << "tid : " << tid << std::endl;
    int temp = TEST_COUNT / NUM_THREADS;
    std::string value;

    for(int i = tid * temp, cnt = 0; cnt < temp; ++i) {
        ++cnt;
        testSkipList.searchElement(rand() % TEST_COUNT, value);
    }

    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
    srand(time(NULL));

    pthread_t threads[NUM_THREADS];
    int rc;
    int i;

    auto start = std::chrono::high_resolution_clock::now();

    for(i = 0; i < NUM_THREADS; ++i) {
        std::cout << "main() : creating thread, " << i << std::endl;
        rc = pthread_create(&threads[i], NULL, insertElement, &i);

        if(rc) {
            std::cout << "Error:unable to create thread," << rc << std::endl;
            exit(-1);
        }
    }

    void* ret;
    for(i = 0; i < NUM_THREADS; ++i) {
        if(pthread_join(threads[i], &ret) != 0) {
            perror("pthread_create() error");
            exit(3);
        }
    }
    
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "insert elapsed:" << elapsed.count() << std::endl;

    std::cout<<"SkipList size: "<<testSkipList.size()<<std::endl;
    // testSkipList.displayList();

    pthread_t threads2[NUM_THREADS];

    auto sstart = std::chrono::high_resolution_clock::now();

    for(i = 0; i < NUM_THREADS; ++i) {
        std::cout << "main() : creating thread, " << i << std::endl;
        rc = pthread_create(&threads2[i], NULL, searchElement, &i);

        if(rc) {
            std::cout << "Error:unable to create thread," << rc << std::endl;
            exit(-1);
        }
    }

    void* sret;
    for(i = 0; i < NUM_THREADS; ++i) {
        if(pthread_join(threads2[i], &sret) != 0) {
            perror("pthread_create() error");
            exit(3);
        }
    }
    
    auto sfinish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> selapsed = sfinish - sstart;
    std::cout << "search elapsed:" << selapsed.count() << std::endl;

    pthread_exit(NULL);

    return 0;
}