#ifndef NODE_H
#define NODE_H

template<typename K, typename V>
class SkipList;

template<typename K, typename V>
class Node{
    friend class SkipList<K,V>;
public:
    Node() = default;
    Node(K k, V v, int level);
    ~Node();

    K getKey() const { return _key; };
    V getValue() const { return _value; };
    void setValue(V v){ _value = v; };

    Node<K,V> **_forward;
    int _nodeLevel;
private:
    K _key;
    V _value;
};

template<typename K, typename V>
Node<K,V>::Node(const K k, const V v, int level){
    _key = k;
    _value = v;
    _nodeLevel = level;
    _forward = new Node<K,V>*[_nodeLevel+1]();
}

template<typename K, typename V>
Node<K,V>::~Node(){ delete[] _forward; }

#endif