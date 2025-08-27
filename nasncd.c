#include <stdio.h>
#include <stdbool.h>
#include <windows.h> // Sleep için

void print_binary8(unsigned char n);
void move(unsigned char *x); // state machine için uygun değil çünkü tek metod
void goleft(unsigned char *x);
void goright(unsigned char *x);
void limitcontrol(unsigned char x);


typedef enum {
   leftgoing,
   rightgoing
} State;

State currentState = leftgoing;
int main() {
    unsigned char x = 1;
    
    

    while (1) {

        
       switch (currentState) {
            case leftgoing:
                goleft(&x);
                break;
            case rightgoing:
                goright(&x);
                break;
        }
        print_binary8(x);
        Sleep(200); // 100 milisaniye bekle
        limitcontrol(x);               
        
    }
    return 0;
}





void goleft(unsigned char *x) {
    *x = (*x << 1);
}

void goright(unsigned char *x) {
    *x = (*x >> 1);
}

void limitcontrol(unsigned char x) {
    if (x == 128) {
        currentState = rightgoing;  // En sola ulaştıysa sağa dön
    } else if (x == 1) {
        currentState = leftgoing;   // En sağa ulaştıysa sola dön
    }
}
              
            


void print_binary8(unsigned char n) {
    for (int i = 7; i >= 0; i--) {  // Soldan sağa doğru yazdırmak için 7'den 0'a
        printf("%u", (n >> i) & 1);
    }
    printf("\r"); // Satır başına dön
    
}


void move(unsigned char *x) {
    
         *x = (currentState == leftgoing) ? (*x << 1) : (*x >> 1);
}
