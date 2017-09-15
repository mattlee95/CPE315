#include <stdio.h>
#include <stdlib.h>

int main(void) {
   short int *shortint;
   int *regint;
   long int *longint;
   unsigned char bytestring[] = { 0x41, 0x33, 0x54, 0x80, 0xFF, 0x99, 0x01, 0x78, 0x55, 0x20, 0xFE, 0xEE, 0x00, 0x00, 0x00, 0x00 };
   shortint = (short int*) &bytestring;
   printf("short int: %hu\n", *shortint); //??

//   unsigned char integer = sizeof(int);
 //  printf("int: %d\n", integer);
   return 0;
}
