//
// Created by 97250 on 28/05/2023.
//


#include "concurrentQueue.cpp"

typedef struct ActiveObject {
    concurrentQueue *queue;
    pthread_t thread;
    void (*func)(int);
    int id;
};


