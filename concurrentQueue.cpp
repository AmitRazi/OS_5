//
// Created by 97250 on 28/05/2023.
//


#include <queue>
#include <pthread.h>

class concurrentQueue {
private:
    std::queue<int> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

public:
    concurrentQueue() {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond,NULL);
    }

    ~concurrentQueue(){
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mutex);

    }
    void enqueue(int num) {
        pthread_mutex_lock(&mutex);
        queue.push(num);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    int dequeue() {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME,&ts);

        ts.tv_sec +=1;
        int rc = 0;
        pthread_mutex_lock(&mutex);
        if (queue.empty()) {
            rc = pthread_cond_timedwait(&cond,&mutex,&ts);
        }

        if(rc == ETIMEDOUT){
            return 0;
        }
        int res = queue.front();
        queue.pop();
        pthread_mutex_unlock(&mutex);
        return res;
    }
};

