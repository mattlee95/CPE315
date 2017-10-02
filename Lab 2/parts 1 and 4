#include <stdio.h>


typedef struct _intfloat {
   int exponent;
   int fraction;
} INTFLOAT, *INTFLOAT_PTR;


unsigned int multiply(unsigned int a, unsigned int b)
{
   unsigned int product = 0;
   unsigned int lsb, reps = 0;
   
   while(1)
   {
      lsb = a & 1; /*determine if least significant bit is 0 or 1*/
      if(lsb == 1)
      {
       	 product += b; /*if 1, add multiplicand to product*/
      }

      b = b << 1; /*shift multiplicand left*/
      a = a >> 1; /*shift multiplier right*/
      
      reps++; /*increment count*/
      if(reps == 16)
      break;
   }
   return product;

}

void normalize(INTFLOAT_PTR xp)
{
   unsigned int signflag;
   
   signflag = (xp->fraction >> 1); /* sign bit, moved over 1*/
   
   if (xp->fraction == 0) return;	/* Escape clause: fraction is zero */
   
   while ( ((xp->fraction ^ signflag) & 0x40000000)==0) // As long as 0 (equal)
   // xor the sign with next bit, 0 means equal
   {
      xp->fraction <<= 1;			/* Shifts fraction left */
      xp->exponent--;			/* Decrement the exponent */
   }
}



int main(void)
{
   INTFLOAT x1, x2, x3, x4;
   x1.exponent =  0x00000001;
   x1.fraction =  0x40000000;
   
   x2.exponent =  0x00000000;
   x2.fraction =  0x00200000;
   
   x3.exponent =  0x0000000C;
   x3.fraction =  0xFFC00000;
   
   x4.exponent =  0xFFFFFFF8;
   x4.fraction =  0x20000000;
   
   long result1, result2, result3, result4, result5 = 0;
   result1 = multiply(0x0001, 0x0001);
   printf("0x0001 x 0x0001: %x\n", result1);
   
   result2 = multiply(0x0001, 0xFFFF);
   printf("0x0001 x 0xFFFF: %x\n", result2);
   
   result3 = multiply(0x8000, 0x0001);
   printf("0x8000 x 0x0001: %x\n", result3);
   
   result4 = multiply(0x4000, 0x4000);
   printf("0x4000 x 0x4000: %x\n", result4);
   
   result5 = multiply(0x8000, 0x8000);
   printf("0x8000 x 0x8000: %x\n", result5);
   
   normalize(&x1);
   normalize(&x2);
   normalize(&x3);
   normalize(&x4);
   printf("Normalized exponent: %x, fraction: %x\n", x1.exponent, x1.fraction);
   printf("Normalized exponent: %x, fraction: %x\n", x2.exponent, x2.fraction);
   printf("Normalized exponent: %x, fraction: %x\n", x3.exponent, x3.fraction);
   printf("Normalized exponent: %x, fraction: %x\n", x4.exponent, x4.fraction);

   
   return 0;
   
}
