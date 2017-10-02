#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
   unsigned int exponent;
   unsigned int sign;
   unsigned int fraction;
}INTFLOAT;

float fmul(float a, float b);
float fadd(float a, float b);
float fsub(float a, float b);
float struct_to_float(INTFLOAT *intFltPtr);
void float_to_struct(INTFLOAT *intFltPtr, float a);

int main()
{
   printf("\n========== PART5 ==========\n");
   
   float f1 = -1.0f; //0xBF800000;
   float f2 = 1.0f; //0x3F800000;
   float res1 = fadd(f1, f2);
   unsigned int int1 = (unsigned int) * (unsigned int *)&res1;
   printf("5a. 0xBF800000 and 0x3F800000 (-1 and +1)\n  Sum:%08X, %g\n", int1, res1);

   float f3 = 1.0f; //0x3F800000;
   float f4 = .25f; //0x3E800000;
   float res2 = fadd(f3, f4);
   unsigned int int2 = (unsigned int) * (unsigned int *)&res2;
   printf("5b. 0x3F800000 and 0x3E800000 (1 + .25)\n  Sum:%08X, %g\n", int2, res2);

   float f5 = 4.0f; //0x40800000;
   float f6 = .125f; //0xBE000000;
   float res3 = fadd(f5, f6);
   unsigned int int3 = (unsigned int) * (unsigned int *)&res3;
   printf("5c. 0x40800000 and 0xBE000000 (4.0 + (-.125) )\n  Sum:%08X, %g\n", int3, res3); 

   printf("===========================\n");

   printf("\n========== PART6 ==========\n");
   float f7 = 3.0f;
   float f8 = 1.0f;
   float res4 = fsub(f7, f8);
   unsigned int int4 = (unsigned int) * (unsigned int *)&res4;
   printf("6a. 0x40400000 and 0x3F800000 (3 – 1)\n  Diff:%08X, %g\n", int4, res4);

   float f9 = 3.0f;
   float f10 = -1.0f;
   float res5 = fsub(f9, f10);
   unsigned int int5 = (unsigned int) * (unsigned int *)&res5;
   printf("6b. 0x40400000 and 0xBF800000 (3 – (-1) )\n  Diff:%08X, %g\n", int5, res5);

   float f11 = 2.0f;
   float f12 = 2.0f;
   float res6 = fsub(f11, f12);
   unsigned int int6 = (unsigned int) * (unsigned int *)&res6;
   printf("6c. 0x40000000 and 0x40000000\n  Diff:%08X, %g\n", int6, res6);

   printf("===========================\n");

   printf("\n========== PART7 ==========\n");

   float f13 = 2.5f;
   float f14 = 2.5f;
   float res7 = fmul(f13, f14);
   unsigned int int7 = (unsigned int) * (unsigned int *)&res7;
   printf("7a. 0x40200000 and 0x40200000 (2.5 x 2.5)\n  Product:%08X, %g\n", int7, res7);

   float f15 = -15.0f;
   float f16 = 4096.0f;
   float res8 = fmul(f15, f16);
   unsigned int int8 = (unsigned int) * (unsigned int *)&res8;
   printf("7b. 0xc1700000 and 0x45800000 (-15 x 4096)\n  Product:%08X, %g\n", int8, res8);

   printf("===========================\n");
   
   return 1;
}

float fmul(float a, float b)
{
   INTFLOAT fltA, fltB, sum;
   float_to_struct(&fltA, a);
   float_to_struct(&fltB, b);

   sum.exponent = fltA.exponent + fltB.exponent - 127;
   sum.fraction = (((fltA.fraction + 0x00800000)>>11) * ((fltB.fraction + 0x00800000)>>11) >> 1) - 0x00800000;
   if (fltA.sign == fltB.sign)
   {
      sum.sign = 1;
   }
   else
   {
      sum.sign = -1;
   }
   float ret = struct_to_float(&sum);
   return ret;
}

float fadd(float a, float b)
{
   INTFLOAT fltA, fltB, sum;
   float_to_struct(&fltA, a);
   float_to_struct(&fltB, b);

   if (fltA.exponent >= fltB.exponent)
   {
      int diff = fltA.exponent - fltB.exponent;
      fltB.fraction += 0x00800000;
      fltA.fraction += 0x00800000;
      fltB.fraction = fltB.fraction >> diff;
      sum.exponent = fltA.exponent;
   }
   else
   {
      int diff = fltB.exponent - fltA.exponent;
      fltA.fraction += 0x00800000;
      fltB.fraction += 0x00800000;
      fltA.fraction = fltA.fraction >> diff;
      sum.exponent = fltB.exponent;
   }
   if ((fltA.sign == 1) && (fltB.sign == 1))
   {
      sum.fraction = (fltA.fraction + fltB.fraction) & 0x007fffff;
      sum.sign = 1;
   }
   if ((fltA.sign == 1) && (fltB.sign == -1))
   {
      sum.fraction = (fltA.fraction - fltB.fraction) & 0x007fffff;
      if (fltA.fraction == fltB.fraction)
      {
         sum.exponent = 0x00000000;
      }
      else if (fltA.fraction > fltB.fraction)
      {
         sum.sign = 1;
      }
      else
      {
         sum.sign = -1;
      }
   }
   if ((fltA.sign == -1) && (fltB.sign == 1))
   {
      sum.fraction = (fltB.fraction - fltA.fraction) & 0x007fffff;
      if (fltA.fraction == fltB.fraction)
      {
         sum.exponent = 0x00000000;
      }
      else if (fltB.fraction > fltA.fraction)
      {
         sum.sign = 1;
      }
      else
      {
         sum.sign = -1;
      }
   }
   if ((fltA.sign == -1) && (fltB.sign == -1))
   {
      sum.fraction = (fltA.fraction + fltB.fraction) & 0x007fffff;
      sum.sign = -1;
   }
   float retVal = struct_to_float(&sum);
   return retVal;
}


float fsub(float a, float b)
{
   INTFLOAT fltA, fltB, sum;
   float_to_struct(&fltA, a);
   float_to_struct(&fltB, b);

   if (fltA.exponent >= fltB.exponent)
   {
      int diff = fltA.exponent - fltB.exponent;
      fltB.fraction += 0x00800000;
      fltA.fraction += 0x00800000;
      fltB.fraction = fltB.fraction >> diff;
      sum.exponent = fltA.exponent;
   }
   else
   {
      int diff = fltB.exponent - fltA.exponent;
      fltA.fraction += 0x00800000;
      fltB.fraction += 0x00800000;
      fltA.fraction = fltA.fraction >> diff;
      sum.exponent = fltB.exponent;
   }
   if ((fltA.sign == -1) && (fltB.sign == -1))
   {
      sum.fraction = (fltA.fraction + fltB.fraction) & 0x007fffff;
      sum.sign = -1;
      if (fltA.fraction == fltB.fraction)
      {
         sum.exponent = 0x00000000;
      }
   }
   if ((fltA.sign == 1) && (fltB.sign == -1))
   {
      sum.fraction = (fltB.fraction - fltA.fraction) & 0x007fffff;
      if (fltA.fraction > fltB.fraction)
      {
         sum.sign = -1;
      }
      else
      {
         sum.sign = 1;
      }
   }
   if ((fltA.sign == -1) && (fltB.sign == 1))
   {
      sum.fraction = (fltA.fraction - fltB.fraction) & 0x007fffff;
      if (fltB.fraction > fltA.fraction)
      {
         sum.sign = -1;
      }
      else
      {
         sum.sign = 1;
      }
   }
   if ((fltA.sign == 1) && (fltB.sign == 1))
   {
      sum.fraction = (fltA.fraction + fltB.fraction) & 0x007fffff;
      sum.sign = 1;
      if (fltA.fraction == fltB.fraction)
      {
         sum.exponent = 0x00000000;
      }
   }
   float retVal = struct_to_float(&sum);
   return retVal;
}
void float_to_struct(INTFLOAT *intFltPtr, float a)
{
   unsigned int b = (unsigned int) * (unsigned int *)&a;
   intFltPtr->sign = b & 0x80000000;
   if (intFltPtr->sign != 0)
   {
      intFltPtr->sign = -1;
   }
   else
   {
      intFltPtr->sign = 1;
   }
   intFltPtr->exponent = ((b & 0x7f800000) >> 23);
   intFltPtr->fraction = b & 0x007fffff;
}

float struct_to_float(INTFLOAT *intFltPtr)
{
   unsigned int ret = 0x00000000;
   ret |= intFltPtr->fraction;
   ret |= ((intFltPtr->exponent) << 23) & 0x7f800000;
   if (intFltPtr->sign == -1)
   {
      ret |= 0x80000000; 
   }
   float float_ret = (float) * (float *)&ret;
   return float_ret;
}
