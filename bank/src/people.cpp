#include people.h

peopleCalled :: peopleCalled(int n) : people(n){
    e1=0;
    for(int i=0;i<maax;i++){
        queue[i]=0;
    }
}

void peopleCalled :: panggil(){
    e1++;
    queue[e1-1]=e1;
}

void peopleCalled :: datang(int x){
    for(int i=0;i<total;i++){
        if(queue[i]==x){
            queue[i]=0;
        }
    }
}

int peopleCalled :: panggilLagi(){
    for(int i=0;i<total;i++){
        if(queue[i]!=0){
            return queue[i];
        }
    }
    return 0;
}

peopleServed :: peopleServed(int n) : people(n){
    next=1;
    first=1;
    for(int i=0;i<max;i++){
        finish[i]=0
    }
}

void peopleServed :: panggil(){
    next++;
}

void peopleServed :: datang(int x){
    finish[x] = 1;
}

int peopleServed :: panggilLagi(){
    while(finish[first]){
        first++;
    }
    return first;

    return 0;
}