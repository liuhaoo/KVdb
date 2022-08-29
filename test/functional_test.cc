#include <iostream>
#include "../base/SkipList.h"

#define FILE_PATH "./store/dumpFile"

int main(int argv, char* argc[]) {

    // create a 6-level skip list
    // Node<K=int, V=string>
    SkipList<int, std::string> mySkipList(6);
    mySkipList.insertElement(100, "Alex");
    mySkipList.insertElement(300, "Bob");
    mySkipList.insertElement(500, "Caster");
    mySkipList.insertElement(700, "David");
    mySkipList.insertElement(900, "Ella");

    // reset the value
    mySkipList.insertElement(500, "Ford");
    mySkipList.insertElement(700, "George");
    mySkipList.insertElement(900, "Harry");

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

    mySkipList.deleteElement(100);
    mySkipList.deleteElement(400);

    std::cout << "SkipList's size : " << mySkipList.size() << std::endl;

    mySkipList.displayList();
} 