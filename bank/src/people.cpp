#include people.h
#include <iostream>
using namespace std;

void people :: run(int q){
    while(q--){
            int jenisQ;
            cin >> jenisQ;

            switch(jenisQ){
            case 1:
                orang.panggil();
                break;
            
            case 2:{
                int ygmaju;
                cin >> ygmaju;
                orang.datang(ygmaju);
                break;
            }

            case 3:
                cout << orang.panggilLagi << endl;
                break;
            }
            
        }
}

peopleCalled :: peopleCalled(int n) : people(n){
    e1=0;
    for(int i=0;i<max;i++){
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