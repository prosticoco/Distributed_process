#include "layerzero.h"
#include "pending.h"


int test_serial(){
    printf("---- TEST SERIALISATION ---- \n");
    msg_t msg0;
    unsigned int clock0[] = {1,2,3,4,5};
    msg0.ackid = 0;
    msg0.mid = 10;
    msg0.mtype = 20;
    msg0.sender = 30;
    msg0.urb_msg.mid = 40;
    msg0.urb_msg.seen_id = 50;
    int error = alloc_vector(&(msg0.urb_msg.lcb_msg.vec_clock),5,clock0);
    if(error){
        printf("alloc vector error \n");
        return error;
    }
    error = print_message(msg0);
    char* buffer;
    error = serialize(&msg0,&buffer,5);
    if(error){
        printf("error in serialize \n");
        return error;
    }
    msg_t msg1;
    error = deserialize(&msg1,buffer,5);
    if(error){
        printf("error in deserialize \n");
        return error;
    }
    print_message(msg1);
    return 0;
}

int main(){

    int error = test_serial();
    if(error){
        printf("error in test_serial \n");
        return error;
    }
    return 0;
}