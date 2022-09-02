#include <iostream>
#include <pthread.h>
#include <chrono>
#include <unistd.h>
#include <time.h>
#include "../base/SkipList.h"

#define FILE_PATH "./store/dumpFile"

SkipList<int, std::string> mySkipList(6);

void* cycle_del(void*) {
    while(true){
        usleep(50000);
        auto start = std::chrono::high_resolution_clock::now();
        mySkipList.cycle_del();
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        // std::cout << "cycle del time:" << elapsed.count() << std::endl;
    }

    pthread_exit(NULL);
}

int main(int argv, char* argc[]) {
    srand(time(NULL));
    // create a 6-level skip list
    // Node<K=int, V=string>
    // SkipList<int, std::string> mySkipList(6);
    mySkipList.insertElement(100, "Alex");
    mySkipList.insertElement(300, "Bob");
    mySkipList.insertElement(500, "Caster");
    mySkipList.insertElement(700, "David");
    mySkipList.insertElement(900, "Ella");

    // reset the value
    mySkipList.insertElement(500, "Ford");
    mySkipList.insertElement(700, "George");
    mySkipList.insertElement(900, "Harry");

    // LRU full
    mySkipList.insertElement(200, "Iris");
    mySkipList.insertElement(400, "Jack");
    mySkipList.insertElement(600, "Kate");
    mySkipList.insertElement(800, "Lee");

    mySkipList.printLRU();

    std::cout << "SkipList's size : " << mySkipList.size() << std::endl;

    mySkipList.dumpFile();

    mySkipList.loadFile();
    std::string value;
    if(mySkipList.searchElement(100, value)) {
        std::cout << "find value of key 100:" << value << std::endl;
    }
    else {
        std::cout << "skipList can not find the key" << std::endl;
    }

    if(mySkipList.searchElement(400, value)) {
        std::cout << "find value of key 400:" << value << std::endl;
    }
    else {
        std::cout << "skipList can not find the key" << std::endl;
    }

    mySkipList.displayList();

    mySkipList.deleteElement(300);
    mySkipList.deleteElement(1000);

    std::cout << "SkipList's size : " << mySkipList.size() << std::endl;

    // create thread to handle the cycle delete
    pthread_t trd;
    int rc = pthread_create(&trd, NULL, cycle_del, NULL);

    if(rc) {
        std::cout << "Error:unable to create thread," << rc << std::endl;
        exit(-1);
    }

    pthread_detach(trd);

    mySkipList.displayList();

    mySkipList.element_expire_time(100, 3);
    std::cout<<"sleep 1s"<<std::endl;
    sleep(1);

    mySkipList.element_ttl(100);
    std::cout<<"sleep 1s"<<std::endl;
    sleep(1);

    mySkipList.element_expire_time(500, 5);
    std::cout<<"sleep 1s"<<std::endl;
    sleep(1);

    mySkipList.element_ttl(500);
    std::cout<<"sleep 1s"<<std::endl;
    sleep(1);

    mySkipList.element_expire_time(700, 1);
    std::cout<<"sleep 1s"<<std::endl;
    sleep(1);
    mySkipList.searchElement(700,value);
    mySkipList.element_expire_time(900, 2);

    std::cout<<"sleep 3s"<<std::endl;
    sleep(3);
    mySkipList.displayList();
} 