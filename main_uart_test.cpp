#include "mbed.h"
 
Serial pc(USBTX, USBRX); // tx, rx
DigitalOut myled(LED1);
 
int main() {
    pc.printf("Hello World!");
    myled =1;
    while(1) {
        myled = !myled;
        pc.putc('1');
        wait_ms(100);
    }
}