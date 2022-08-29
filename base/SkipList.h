#ifndef SKIPLIST_H
#define SKIPLIST_H
#include <iostream>
#include <string>
#include <mutex>
#include <fstream>
#include "node.h"

#define STORE_FILE "./store/dumpFile"

std::mutex mtx;

template<typename K, typename V>
class SkipList{
public:
    SkipList() = default;
    SkipList(int maxLevel);
    ~SkipList();

    int getRandomLevel();
    Node<K,V>* createNode(K,V,int);
    void displayList();
    int insertElement(K, V);
    bool searchElement(K, V&);
    bool deleteElement(K);
    int size(){ return _elementCount; };
    
    void dumpFile();
    void loadFile();

private:
    void get_key_value_from_string(const std::string& str, std::string* key, std::string* value);
    bool is_valid_string(const std::string& str);

private:
    // max level of the skip list
    int _maxLevel;
    // curlevel of the skip list
    int _curLevel;
    // head ptr
    Node<K, V>* _header;
    // cur element num
    int _elementCount;
    // file operator
    std::ofstream _fileWriter;
    std::ifstream _fileReader;
};



// init of SkipList
template<typename K, typename V>
SkipList<K,V>::SkipList(int level){
    _maxLevel = level;
    _curLevel = 0;
    _elementCount = 0;
    K k;
    V v;
    _header = new Node<K,V>(k,v,_maxLevel);
}


// destroy of SkipList
template<typename K, typename V>
SkipList<K,V>::~SkipList(){
    if(_fileWriter.is_open())
        _fileWriter.close();
    if(_fileReader.is_open())
        _fileReader.close();
    delete _header;
}


// random level of the node
template<typename K, typename V>
int SkipList<K,V>::getRandomLevel(){
    int k = 1;
    while(rand()%2)
        k++;
    k = (k<_maxLevel)?k:_maxLevel;
    return k;
}


// create Node<K,V>
template<typename K, typename V>
Node<K,V>* SkipList<K,V>::createNode(K k, V v, int level){
    Node<K,V>* node = new Node<K,V>(k,v,level);
    return node;
}


// insert element
template<typename K, typename V>
int SkipList<K,V>::insertElement(K k, V v){

    // lock 
    mtx.lock();
    Node<K,V>* cur = _header;

    // track the parent of the inserted-Node 
    Node<K,V>* update = new Node<K,V>[_maxLevel+1];
    for(int i = _curLevel; i >= 0; i--){
        while(cur->_forward[i] && cur->_forward[i]->getKey()<k)
            cur = cur->_forward[i];
        update[i] = *cur;
    }

    // the position of key
    cur = cur->_forward[0];

    // the key is already in the skiplist, modify its value
    if(cur && cur->getKey()==k){
        cur->setValue(v);
        mtx.unlock();
        return 1;
    }

    // insert the new Node
    if(cur==nullptr || cur->getKey()==k){
        int randomLevel = getRandomLevel();
        // the new Node's level is higher than _curLevel
        if(randomLevel > _curLevel){
            for(int i = _curLevel+1; i <= randomLevel; i++){
                update[i] = *_header;
            }
            _curLevel = randomLevel;
        }
        // insert
        Node<K,V>* insertNode = createNode(k,v,randomLevel);
        for(int i = 0; i <= randomLevel; i++){
            insertNode->_forward[i] = update[i]._forward[i];
            update[i]._forward[i] = insertNode;
        }
        std::cout<<"Successfully inserted key: "<<k<<", value: "<<v<<std::endl;
        _elementCount++;
    }

    // unlock the mutex
    mtx.unlock();
    return 0;
}


// search the given key, and return its value
template<typename K, typename V>
bool SkipList<K,V>::searchElement(K k, V& v){
    Node<K,V>* cur = _header;
    for(int i = _curLevel; i >= 0; i--){
        while(cur->_forward[i] && cur->_forward[i]->getKey()<k)
            cur = cur->_forward[i];
    }
    cur = cur->_forward[0];
    if(cur && cur->getKey()==k){
        std::cout << "Found key: " << k << ", value: " << cur->getValue() << std::endl;
        return true;
    }
    std::cout << "Not Found Key:" << k << std::endl;
    return false;
}


// delete the given key element
template<typename K, typename V>
bool SkipList<K,V>::deleteElement(K k){
    // lock the mutex
    mtx.lock();
    Node<K,V>* cur = _header;
    // track the parent
    Node<K,V>* update = new Node<K,V>[_maxLevel+1];
    for(int i = _curLevel; i >= 0; i--){
        while(cur->_forward[i] && cur->_forward[i]->getKey()<k)
            cur = cur->_forward[i];
        update[i] = *cur;
    }
    cur = cur->_forward[0];

    // if find the key-element, delete
    if(cur && cur->getKey()==k){
        for(int i = 0; i <= _curLevel; i++){
            if(update[i]._forward[i] != cur)
                break;
            update[i]._forward[i] = cur->_forward[i];
        }
        std::cout<<"Delete key: "<<k<<" value: "<<cur->getValue()<<std::endl;
        delete cur;
        while(_curLevel>0 && _header->_forward[_curLevel]==nullptr)
            _curLevel--;
        _elementCount--;
        mtx.unlock();
        return true;
    }
    mtx.unlock();
    return false;
}


// display the skip list
// in level mode
template<typename K, typename V>
void SkipList<K,V>::displayList(){
    std::cout<<"\n**********Display SkipList**********\n";
    Node<K,V>* cur;
    for(int i = _curLevel; i>=0; i--){
        cur = _header->_forward[i];
        std::cout<<"Level "<<i<<std::endl;
        while(cur != nullptr){
            std::cout << cur->getKey() << ":" << cur->getValue() << "; ";
            cur = cur->_forward[i];
        }
        std::cout<<std::endl;
    }
}


// write return disk
template<typename K, typename V>
void SkipList<K,V>::dumpFile(){
    std::cout<<"\ndump file\n";
    _fileWriter.open(STORE_FILE);
    if(!_fileWriter.is_open()){
        std::cout<<"file not open"<<std::endl;
        return;
    }
    Node<K,V>* cur = _header->_forward[0];
    while(cur != nullptr){
        _fileWriter<<cur->getKey()<<":"<<cur->getValue()<<std::endl;
        cur = cur->_forward[0];
    }
    _fileWriter.flush();
    _fileWriter.close();
}


// load the data from disk
template<typename K, typename V>
void SkipList<K,V>::loadFile(){
    std::cout<<"\nload file\n";
    _fileReader.open(STORE_FILE);
    if(!_fileReader.is_open()){
        std::cout<<"file not open"<<std::endl;
        return;
    }
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while(getline(_fileReader,line)){
        get_key_value_from_string(line,key,value);
        if(key->empty() || value->empty())
            continue;
        insertElement(stoi(*key),*value);
        std::cout << "load item key: " << *key << " value: " << *value << std::endl;
    }
    _fileReader.close();
}


// recover the KV-item from string 
template<typename K, typename V>
void SkipList<K,V>::get_key_value_from_string(const std::string& str, std::string* key, std::string* value){
    if(str.empty() || str.find(':')==std::string::npos)
        return;
    *key = str.substr(0,str.find(':'));
    *value = str.substr(str.find(':')+1);
}

#endif