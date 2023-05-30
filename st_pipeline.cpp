#include <iostream>
#include <cmath>
#include <array>
#include <unistd.h>
#include "ActiveObject.cpp"


int seed = -1;
int ActiveObjectIndex = 0;
int expectedElements =0;
std::array<ActiveObject*,4> pipeline;

int isPrime(unsigned int num){

    if(num == 2){
        return 1;
    }

    if(num % 2 == 0){
        return 0;
    }

    for(int i = 3 ; i < sqrt(num) ; i+=2){
        if(num % i == 0){
            return 0;
        }
    }

    return 1;
}

void* ActiveObjectLoop(void *arg){
    ActiveObject* AO = (ActiveObject*)arg;
    int task = 0;

    if(AO->id == 0){
        seed == -1 ? srand(time(NULL)) : srand(seed);
        int i = 0;
        while(i < expectedElements){
            int num;
            num = (int)rand();
            pipeline[1]->queue->enqueue(num);
            usleep(1000);
            i++;
        }
    }

    while((task = AO->queue->dequeue())){
        AO->func(task);
    }


}

concurrentQueue* getQueue(ActiveObject* AO){
    return AO->queue;
}

void CreateActiveObject(void (*func)(int)){
    concurrentQueue *queue = new concurrentQueue;
    ActiveObject *AO = new ActiveObject();
    AO->queue = queue;
    AO->func = func;
    AO->id = ActiveObjectIndex;
    pipeline[ActiveObjectIndex++] = AO;
    pthread_create(&AO->thread,NULL,ActiveObjectLoop,AO);
}

void AOfunc2(int num){
        if(isPrime(num)){
            std::cout<<"true\n"<<num<<"\n";
        } else{
            std::cout<<"false\n"<<num<<"\n";
        }
        num+=11;
        pipeline[2]->queue->enqueue(num);
}

void AOfunc3(int num){
        if(isPrime(num)){
            std::cout<<"true\n"<<num<<"\n";
        } else{
            std::cout<<"false\n"<<num<<"\n";
        }
        num-=13;
        pipeline[3]->queue->enqueue(num);
}

void AOfunc4(int num){
        std::cout<<num<<"\n";
        num+=2;
        std::cout<<num<<"\n";

}


int main(int argc,char* argv[]) {
    if(argc < 2){
        fprintf(stderr,"Usage: ./st_pipeline N S, N - Numbers, S - random seed");
        return 1;
    }

    if(argc == 3){
        expectedElements = atoi(argv[1]);
        seed = atoi(argv[2]);
    } else{
        expectedElements = atoi(argv[1]);
    }

    std::fill(pipeline.begin(), pipeline.end(), nullptr);
    CreateActiveObject(NULL);
    CreateActiveObject(AOfunc2);
    CreateActiveObject(AOfunc3);
    CreateActiveObject(AOfunc4);
    pthread_join(pipeline[3]->thread, NULL);

    for (auto& AO : pipeline) {
        if (AO != nullptr) {
            delete AO->queue;
            delete AO;
        }
    }

    return 0;
}
