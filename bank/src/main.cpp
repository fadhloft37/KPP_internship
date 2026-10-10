    #include <iostream>
    #include "people.h"
    using namespace std;

int main(){
    int jumlah_orang, banyak_event;
    cin >> jumlah_orang >> banyak_event;

    peopleCalled orang(jumlah_orang);
    orang.run(banyak_event);

    return 0;
}