#include <stdio.h>

int main(){
    int n,q; scanf("%d %d", &n,&q);

    int event, e1=0, queue[n], visit, e3[q], count3=0;
    while(q--){
        scanf("%d", &event);
        if(event==1){
            e1++;
            queue[e1-1] = e1;
        }else if(event==2){
            scanf("%d", &visit);
            for(int i=0;i<n;i++){
                if(queue[i]==visit) queue[i]=0;
                else continue;
            }
        }else if(event==3){
            for(int i=0;i<n;i++){
                if(queue[i]==0) continue;
                else{
                    e3[count3++]=queue[i];
                    break;
                }
            }
        }
    }

    for(int i=0; i<count3; i++){
        printf("%d\n", e3[i]);
    }

    return 0;
}