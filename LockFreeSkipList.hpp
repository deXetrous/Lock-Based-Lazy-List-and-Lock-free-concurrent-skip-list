#include <iostream>       
#include <thread>
#include <ctime>
#include <chrono>
#include <atomic>
#include <stdlib.h>
#include <fstream>
#include <random>
#include <bits/stdc++.h>
using namespace std;
#define MAX_LEVEL 5
atomic<int>check(0);
atomic<int>check1(0);
atomic<int>check2(0);
atomic<int>check3(0);
atomic<int>check4(0);
class Node_LF
{
    public:
        int value;
        int key;
        
        atomic<Node_LF*> next[MAX_LEVEL+1];
        bool nextBool[MAX_LEVEL+1];
        int topLevel;
        Node_LF()
        {}
        Node_LF(int key1)
        {
            value = INT_MIN;
            key=key1;
            //vector<atomic<Node_LF*>>temp(MAX_LEVEL+1);
            Node_LF *T = new Node_LF;
            
            for(int i=0;i<MAX_LEVEL+1;i++)
            {
                next[i] = T;
                nextBool[i] = false;
            }
            topLevel = MAX_LEVEL;
        }
        Node_LF(int x, int height)
        {
            value = x;
            //check for hashCode()
            key = x;
            Node_LF *T =new Node_LF;
            for(int i=0;i<MAX_LEVEL+1;i++)
            {
                next[i] = T;
                nextBool[i] = false;
            }
            topLevel = height;
        }
};

class LockFreeSkipList
{
    public:
        Node_LF* head = new Node_LF(INT_MIN);
        Node_LF* tail = new Node_LF(INT_MAX);
        LockFreeSkipList()
        {
            for(int i=0;i < MAX_LEVEL+1;i++)
            {
                head->next[i] = tail;
                //cout << tail->key << endl;
            }
        }

        bool find(int x, Node_LF* preds[], Node_LF* succs[])
        {
            int bottomLevel = 0;
            //check for hashCode()
            int key = x;
            bool marked = false;
            bool snip;
            Node_LF* pred = NULL;
            Node_LF* curr = NULL;
            Node_LF* succ = NULL;
            retry:
                while(true)
                {
                    pred = head;
                    for(int level = MAX_LEVEL; level >= bottomLevel; level--)
                    {
                       // cout << "hi2" << endl;
                        curr = pred->next[level];
                        
                        while(true)
                        {
                            //cout << "hi3" << endl;
                            succ = curr->next[level];
                            marked = curr->nextBool[level];
                            while(marked)
                            {
                                int t4=0;
                                while(!atomic_compare_exchange_weak(&check4,&t4,1))
                                {
                                    t4=0;
                                }
                                if(pred->next[level] == curr && pred->nextBool[level] == false)
                                {
                                    snip = true;
                                    pred->next[level] = succ;
                                    pred->nextBool[level] = false;
                                }
                                else
                                {
                                    
                                    snip = false;
                                }

                                check4 = 0;
                                
                                //snip = pred->next[level].compare_exchange_strong(curr,succ);
                                if(!snip)
                                    goto retry;
                                curr = pred->next[level];
                                succ = curr->next[level];
                                marked = curr->nextBool[level];
                            }
                            //cout << curr->key << "   " << key << endl;
                            if(curr->key < key)
                            {
                                pred = curr;
                                curr = succ;
                            }
                            else
                            {
                                break;
                            }
                        }
                        preds[level]=pred;
                        succs[level]=curr;
                    }
                    return(curr->key == key);
                }
        

        }
        bool add(int x, int id, FILE * out)
        {
            int topLevel = rand()%(MAX_LEVEL+1);
            int bottomLevel=0;
            Node_LF* preds [MAX_LEVEL+1];
            Node_LF* succs [MAX_LEVEL+1];
            for(int i=0;i<MAX_LEVEL+1;i++)
            {
                preds[i] = new Node_LF;
                succs[i] = new Node_LF;
            }
            while(true)
            {
                //cout << "hi1" << endl;
                bool found = find(x,preds,succs);
                if(found)
                    return false;
                else
                {
                    Node_LF* newNode = new Node_LF(x,topLevel);
                    for(int level=bottomLevel;level<=topLevel;level++)
                    {
                        Node_LF* succ = succs[level];
                        // check
                        newNode->next[level] = succ;
                        newNode->nextBool[level] = false;
                    }
                    Node_LF *pred = preds[bottomLevel];
                    Node_LF *succ = succs[bottomLevel];
                    //cout << "here " << check << endl;
                    int t=0;
                    while(!atomic_compare_exchange_weak(&check,&t,1))
                    {
                        t=0;
                    }

                    if(pred->next[bottomLevel] != succ || pred->nextBool[bottomLevel] != false)
                    {
                        check = 0;
                        continue;
                    }
                    else if(pred->next[bottomLevel] == succ && pred->nextBool[bottomLevel] == false)
                    {
                        pred->next[bottomLevel] = newNode;
                        pred->nextBool[bottomLevel] = false;
                        check = 0;
                    }
                    // if(!pred->next[bottomLevel].compare_exchange_strong(succ,newNode))
                    // {
                    //     pred->next[bottomLevel] = newNode;
                    //     pred->nextBool[bottomLevel] = false;
                    //     check = 0;
                    //     //cout << "here -- " << check << endl;
                    //     continue;
                    // }
                    else
                    {
                        check = 0;
                    }
                    

                    for(int level = bottomLevel+1;level<=topLevel;level++)
                    {
                        while(true)
                        {
                            pred = preds[level];
                            succ = succs[level];

                            int t1=0;
                            while(!atomic_compare_exchange_weak(&check1,&t1,1))
                            {
                                t1=0;
                            }
                            if(pred->next[level] == succ && pred->nextBool[level]==false)
                            {
                                pred->next[level] = newNode;
                                pred->nextBool[level] = false;
                                check1 = 0;
                                break;
                            }
                            else
                            {
                                check1=0;
                            }
                            
                            find(x,preds,succs);
                        }
                    }
                    //cout << "hi" << endl;
                    // cout << find(5,preds,succs) << endl;
                    // cout << find(20,preds,succs) << endl;
                    // cout << find(15,preds,succs) << endl;
                    // cout << find(10,preds,succs) << endl;
                    timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    fprintf(out,"%dth thread added : %d at time : %lld:%ld\n",id, x, (long long)ts.tv_sec, ts.tv_nsec);
                    return true;
                }
                
            }
            
        }

        bool contains(int x)
        {

            int bottomLevel=0;
            int v = x;
            bool marked = false;
            Node_LF* pred = head;
            Node_LF* curr = NULL;
            Node_LF* succ = NULL;

            for(int level = MAX_LEVEL; level >= bottomLevel;level--)
            {
                curr = pred->next[level];
                while(true)
                {   
                    marked = curr->nextBool[level];
                    succ = curr->next[level];
                    while(marked)
                    {
                        cout << marked << endl;
                        curr = pred->next[level];
                        succ = curr->next[level];
                        marked = curr->nextBool[level];
                        // marked = false;
                    }
                    if(curr->key < v)
                    {
                        pred = curr;
                        curr = succ;
                    }
                    else
                    {
                        break;
                    }
                    
                }
            }
            return (curr->key == v);
        }

        bool remove(int x, int id, FILE * out)
        {
            int bottomLevel = 0;
            Node_LF* preds [MAX_LEVEL+1];
            Node_LF* succs [MAX_LEVEL+1];
            // for(int i=0;i<MAX_LEVEL+1;i++)
            // {
            //     preds[i] = new Node_LF;
            //     succs[i] = new Node_LF;
            // }
            Node_LF* succ;
            while(true)
            {
                bool found = find(x,preds,succs);
                if(!found)
                    return false;
                else
                {
                    //cout << "hi1" << endl;
                    Node_LF * nodeToRemove = succs[bottomLevel];
                    cout << "val = " <<nodeToRemove->key << endl;
                    for(int level = nodeToRemove->topLevel; level >= bottomLevel+1 ; level--)
                    {
                        bool marked = false;
                        succ = nodeToRemove->next[level];
                        marked = nodeToRemove->nextBool[level];
                        while(!marked)
                        {
                            //cout << "hi2" << endl;
                            int t2=0;
                            while(!atomic_compare_exchange_weak(&check2,&t2,1))
                            {
                                t2=0;
                            }
                            if(nodeToRemove->next[level] == succ && nodeToRemove->nextBool[level] == false)
                            {
                                nodeToRemove->next[level] = succ;
                                nodeToRemove->nextBool[level] = true;
                                marked = nodeToRemove->nextBool[level];
                                succ = nodeToRemove->next[level];
                            }
                            check2 = 0;
                            
                        }
                    }
                    bool marked = false;
                    succ = nodeToRemove->next[bottomLevel];
                    marked = nodeToRemove->nextBool[bottomLevel];
                    while(true)
                    {
                        //cout << "hi3" << endl;
                        bool iMarkedIt = false;
                        int t3=0;
                        while(!atomic_compare_exchange_weak(&check3,&t3,1))
                        {
                            t3=0;
                        }
                        if(nodeToRemove->next[bottomLevel] == succ && nodeToRemove->nextBool[bottomLevel] == false)
                        {
                            nodeToRemove->next[bottomLevel] = succ;
                            nodeToRemove->nextBool[bottomLevel] = true;
                            iMarkedIt = true;
                            marked = nodeToRemove->nextBool[bottomLevel];
                            succ = succs[bottomLevel]->next[bottomLevel];
                        }
                        check3=0;
                        //bool iMarkedIt = nodeToRemove->next[bottomLevel].compare_exchange_strong(succ,succ);
                        //succ = succs[bottomLevel]->next[bottomLevel];
                        if(iMarkedIt)
                        {
                            find(x,preds,succs);
                            timespec ts;
                            clock_gettime(CLOCK_REALTIME, &ts);
                            fprintf(out,"%dth thread removed : %d at time : %lld:%ld\n",id, x, (long long)ts.tv_sec, ts.tv_nsec);
                            return true;
                        }
                        else if(marked)
                            return false;
                    }

                }
                
            }   
        }

};
