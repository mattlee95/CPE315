#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct infloat {
   int exponent;
   int fraction;
   unsigned int sign;
};

struct infloat* extract_float(struct infloat *xp, float f);
void pack_float(struct infloat *xp);


int main(int argc, char *argv[]) {
   struct infloat *xp1 = (struct infloat*) malloc(sizeof(struct infloat));
   struct infloat *xp2 = (struct infloat*) malloc(sizeof(struct infloat));
   struct infloat *xp3 = (struct infloat*) malloc(sizeof(struct infloat));
   struct infloat *xp4 = (struct infloat*) malloc(sizeof(struct infloat));
   unsigned int x = 0x40C80000;
   unsigned int x2 = 0xC3000000;
   unsigned int x3 = 0x3E000000;
   unsigned int x4 = 0x3EAAAAAB;
   float f = *(float *) &x;
   float f2 = *(float *) &x2;
   float f3 = *(float *) &x3;
   float f4 = *(float *) &x4;
   printf("=========Part 2==========\n");
   struct infloat *uf1 = (struct infloat*) malloc(sizeof(struct infloat));
   struct infloat *uf2 = (struct infloat*) malloc(sizeof(struct infloat));
   struct infloat *uf3 = (struct infloat*) malloc(sizeof(struct infloat));
   struct infloat *uf4 = (struct infloat*) malloc(sizeof(struct infloat));
   printf("2a. Test case: 0x40C80000\n");
   uf1 = extract_float(xp1, f);
   printf("2b. Test case: 0xc3000000\n");
   uf2 = extract_float(xp2, f2);
   printf("2c. Test case: 0x3e000000\n");
   uf3 = extract_float(xp3, f3);
   printf("2d. Test case: 0x3EAAAAAB\n");
   uf4 = extract_float(xp4, f4);
   printf("=========================\n");
   printf("=========Part 3==========\n");
   printf("3a. Test case: 0x40C80000\n");
   pack_float(uf1);
   printf("3b. Test case: 0xc3000000\n");
   pack_float(uf2);
   printf("3c. Test case: 0x3e000000\n");
   pack_float(uf3);
   printf("3d. Test case: 0x3EAAAAAB\n");
   pack_float(uf4);
   printf("=========================\n");
   return 0;
}

struct infloat* extract_float(struct infloat *xp, float f) {
   unsigned floatbits;
   floatbits = *(unsigned int *) &f; //extract binary float #
   //get sign as boolean - true if negative
   xp->sign = (floatbits & 0x80000000); //high-order bit = 1 
   //get exponent from floatbits
   xp->exponent = (floatbits >> 23) & 0x000000FF; //8-bit exponent
   xp->exponent -= 127;    // minus bias of 127
   // get fraction as 2's complement, binary point is to the right of the hidden 1
   xp->fraction = (floatbits << 7) & 0x3FFFFF80;  //23 bits of fraction
   xp->fraction |= 0x40000000; //force hidden '1' (0 1 [.] b-1 b-2 b-3 .... )
   if (xp->sign) xp->fraction = -xp->fraction; //negate 2's comp if negative
   printf("  Float: %.8f\n", f);
   printf("  Exponent: %d\n", xp->exponent);
   printf("  Fraction: 0x%08X\n\n", xp->fraction);
   return xp;
}

// Part 3 - Pack IEEE 754 Value 
void pack_float(struct infloat *xp) {
   int nbits = 32;
   char *s = malloc(nbits+1);
   s[nbits] = '\0';
   unsigned int u = *(unsigned int*)&xp->fraction;
   unsigned int mask = 1 << (nbits - 1); //fill in values right->left
   // create char array of bits
   for (int i=0; i<nbits; i++, mask >>= 1) {
      s[i] = ((u & mask) != 0) + '0';
   }
   // obtain fractional value
   float fraction = 0;
   for (int i=2; i<nbits; i++) {
      if (s[i] == '1') {
         fraction += pow(2, (2 - i - 1));
      }
   }
   float f = pow(-1, xp->sign) * pow(2, xp->exponent) * (1 + fraction);
   printf("  Float: %f\n\n", f);
}
