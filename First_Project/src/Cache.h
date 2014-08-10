#ifndef _CACHE_H_
#define _CACHE_H_
#include <iostream>
#include <string>
#include <vector>
#include <ext/hash_map>
#include <unordered_map>
using namespace std;
using namespace __gnu_cxx;

template <class K, class T>
struct Node{
    K key;
    T data;
    Node *prev, *next;
};

template <class K, class T>
class Cache{
public:
    Cache(int size){
        entries_ = new Node<K,T>[size];
        for(int i = 0; i < size; ++i)// 存储可用结点的地址
            free_entries_.push_back(entries_ + i);
        head_ = new Node<K,T>;
        tail_ = new Node<K,T>;
        head_->prev = NULL;
        head_->next = tail_;
        tail_->prev = head_;
        tail_->next = NULL;
    }
    ~Cache(){
        delete head_;
        delete tail_;
        delete[] entries_;
    }
    void put(K key, T data){
        Node<K,T> *node = hashmap_[key];
        if(node){ // node exists
            detach(node);
            node->data = data;
            attach(node);
        }
        else{
            if(free_entries_.empty()){// 可用结点为空，即cache已满
                node = tail_->prev;
                detach(node);
                hashmap_.erase(node->key);
            }
            else{
                node = free_entries_.back();
                free_entries_.pop_back();
            }
            node->key = key;
            node->data = data;
            hashmap_[key] = node;
            attach(node);
        }
    }
    T get(K key){
        Node<K,T> *node = hashmap_[key];
        if(node){
            detach(node);
            attach(node);
            return node->data;
        }
        else{// 如果cache中没有，返回T的默认值。与hashmap行为一致
            return T();
        }
    }
private:
    // 分离结点
    void detach(Node<K,T>* node){
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    // 将结点插入头部
    void attach(Node<K,T>* node){
        node->next = head_->next;
        head_->next->prev = node;
        node->prev = head_;
        head_->next = node;
        /*
        node->prev = head_;
        node->next = head_->next;
        head_->next = node;
        node->next->prev = node;
        */
    }
private:
    unordered_map<K, Node<K,T>* > hashmap_;
//    hash_map<K, Node<K,T>* > hashmap_;
    vector<Node<K,T>* > free_entries_; // 存储可用结点的地址
    Node<K,T> *head_, *tail_;
    Node<K,T> *entries_; // 双向链表中的结点
};

#endif 
/*
#ifndef _CACHE_H_
#define _CACHE_H_
#include <iostream>
#include <vector>
#include <ext/hash_map>
using namespace __gnu_cxx;
using namespace stdext;

struct Node
{
    std::string key; //查询的单词
    std::string data;//搜索到的单词
    Node *pre, *next;
};

class Cache
{
    public:
        Cache(size_t size);
        ~Cache();
        void put(std::string key, std::string data);
        std::string get(std::string key);

    private:
        void detach(Node *cur);
        void attach(Node *cur);
        hash_map<std::string, Node *> hashmap_;
        std::vector<Node * >  free_entries_;
        Node *head_, *tail_; //头尾节点
        Node *entries_;
}

#endif
*/
