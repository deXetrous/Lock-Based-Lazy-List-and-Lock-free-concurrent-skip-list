#include<bits/stdc++.h>
#include<fstream>
#include<chrono>
#include<sys/time.h>
#include<thread>
#include<ctime>
#include<cstdlib>
#include<atomic>
using namespace std;
#define MAX_LEVEL 5
recursive_mutex mtx;
recursive_mutex mtx1;
class Node {
    public:
    recursive_mutex mtx;
    int item;
    int key;
    Node* next[MAX_LEVEL + 1];
    volatile bool marked = false;
    volatile bool fullyLinked = false;
    int topLevel;
    Node() {
        this->item = 0;
        this->key = 0;
        topLevel = 0;
    }
    void set(int k) {
        this->item = 0;
        this->key = k;
        if(k == INT_MIN) {
            for(int i = 0; i < MAX_LEVEL + 1; i++) {
            next[i] = new Node;
        }
        } else {
            for(int i = 0; i < MAX_LEVEL + 1; i++) {
            next[i] = new Node;
            }
        }
            
        
        topLevel = MAX_LEVEL;
    }
    Node(int k) {
        this->item = 0;
        this->key = k;
        for(int i = 0; i < MAX_LEVEL + 1; i++) {
            next[i] = new Node;
        }
        topLevel = MAX_LEVEL;
    }
    Node(int x, int height) {
        item = x;
        // hash<int> h;
        key = x;
        // Node temp[height + 1];
        for(int i = 0; i < height + 1; i++) {
            this->next[i] = new Node;
        }
        topLevel = height;
    }
    void Lock() {
        mtx.lock();
    }
    void Unlock() {
        mtx.unlock();
    }
};
class LazySkipList {
    public:
    Node head;
    Node tail;
    LazySkipList() {
        head.set(INT_MIN);
        tail.set(INT_MAX);
        for(int i = 0; i < MAX_LEVEL + 1; i++) {
            head.next[i] = &tail;
        }
    }
    ~ LazySkipList() {}
    int find(int x, Node** preds, Node** succs){
        int k = x;
        int found = -1;
        Node * pred;
        pred = &head;
        for(int level = MAX_LEVEL; level >= 0; level--) {
            Node * curr = pred->next[level];
            while(k > curr->key) {
                pred = curr;
                curr = pred->next[level];
            }
            if(found == -1 && k == curr->key) {
                found = level;
            }
            preds[level] = pred;
            succs[level] = curr;
        }
        return found;
    }
    bool add(int x, int id, FILE * out) {
        int tl = rand()%(MAX_LEVEL+1);
        Node * preds[MAX_LEVEL + 1];
        Node * succs[MAX_LEVEL + 1];
        vector<int> count;
        while(true) {
            int found = find(x, preds, succs);
            if(found != -1) {
                Node * nodeFound = succs[found];
                if(!nodeFound->marked) {
                    while(!nodeFound->fullyLinked) {}
                    return false;
                }
                continue;
            }
            int highestLocked = -1;
            // Node * pred;
            // Node * succ;
            bool valid = true;
            for(int level = 0; valid && (level <= tl); level++) {
                preds[level]->mtx.lock();
                highestLocked = level;
                valid = !preds[level]->marked && !succs[level]->marked && preds[level]->next[level] == succs[level];      
            }
            if(!valid) {
                for(int level = 0; level <= highestLocked; level++)
                {
                    preds[level]->Unlock();
                }
                continue;
            }
            Node * newNode;
            newNode = new Node(x, tl);
            for(int level = 0; level <= tl; level++)
                newNode->next[level] = succs[level];
            for(int level = 0; level <= tl; level++)
                preds[level]->next[level] = newNode;
            // Linearization point
            newNode->fullyLinked = true;
            timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            fprintf(out,"%dth thread added : %d at time : %lld:%ld\n",id, x, (long long)ts.tv_sec, ts.tv_nsec);
            for(int level = 0; level <= highestLocked; level++)
            {
                preds[level]->Unlock();
            }
            
            return true;
        }
    }
    bool contains(int x) {
        Node * preds[MAX_LEVEL + 1];
        Node * succs[MAX_LEVEL + 1];
        int found = find(x, preds, succs);
        return (found != -1 && succs[found]->fullyLinked && !succs[found]->marked);
    }
    bool remove(int x, int id, FILE * out) {
        Node * victim;
        bool isMarked = false;
        int tl = -1;
        int count = 0;
        Node * preds[MAX_LEVEL + 1];
        Node * succs[MAX_LEVEL + 1];
        while(true) {
            int found = find(x, preds, succs);
            if(found != -1) victim = succs[found];
            if(isMarked || ((found != -1) && (victim->fullyLinked && victim->topLevel == found && !victim->marked))) {
                if(!isMarked) {
                    tl = victim->topLevel;
                    victim->mtx.lock();
                    if(victim->marked) {
                        victim->mtx.unlock();
                        return false;
                    }
                    victim->marked = true;
                    isMarked = true;
                }
                int highestLocked = -1;
                Node * pred;
                Node * succ;
                bool valid = true;
                for(int level = 0; valid && (level <= tl); level++) {
                    pred = preds[level];
                    pred->mtx.lock();
                    highestLocked = level;
                    valid = !pred->marked && pred->next[level]==victim;
                }
                if(!valid) {
                    for(int level = 0; level <= highestLocked; level++)
                    {
                        preds[level]->Unlock();
                    }
                    continue;
                }
                for(int level = tl; level >= 0; level--) {
                    preds[level]->next[level] = victim->next[level];
                }
                victim->mtx.unlock();               
                timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                fprintf(out,"%dth thread removed : %d at time : %lld:%ld\n",id, x, (long long)ts.tv_sec, ts.tv_nsec);
                for(int level = 0; level <= highestLocked; level++)
                {
                    preds[level]->Unlock();

                }
                return true;
            }
            else {
                return false;
            }
        }
    }
};