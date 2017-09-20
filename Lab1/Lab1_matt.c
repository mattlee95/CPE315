#include <stdio.h>

unsigned char add_bytes(unsigned char byte1, unsigned char byte2);
unsigned char find_carry(unsigned char byte1, unsigned char byte2);
unsigned char add_bytes_signed(unsigned char byte1, unsigned char byte2);
unsigned char find_overflow(unsigned char byte1, unsigned char byte2);
unsigned int arbitrary_byte_add (unsigned char *result, unsigned char *a1, unsigned char *a2, int size, unsigned int carry_in);


int main()
{
   //========== PART 1 ==========
   printf("========== PART 1 ==========\n");
   
   int integer;
   short int short_int;
   char byte;
   long int long_int;

   printf("size of int %d\n", sizeof(integer));
   printf("size of short int %d\n", sizeof(short_int));
   printf("size of byte %d\n", sizeof(byte));
   printf("size of long int %d\n", sizeof(long_int));
   
   //========== PART 2 ==========
   printf("\n========== PART 2 ==========\n");

   int i;

   unsigned char bytestring[] = {0x41, 0x33, 0x54, 0x80, 0xFF, 0x99, 0x01, 0x78, 0x55, 0x20, 0xFE, 0xEE, 0x00, 0x00, 0x00, 0x00}; 
   unsigned char bytestring_swap_int[] = {0x80, 0x54, 0x33, 0x41, 0x78, 0x01, 0x99, 0xFF, 0xEE, 0xFE, 0x20, 0x55, 0x00, 0x00, 0x00, 0x00};
   unsigned char bytestring_swap_short[] = {0x33, 0x41, 0x80, 0x54, 0x99, 0xFF, 0x78, 0x01, 0x20, 0x55, 0xFE, 0xEE, 0x00, 0x00, 0x00, 0x00};
   unsigned char bytestring_swap_long[] = {0x78, 0x01, 0x99, 0xFF, 0x80, 0x54, 0x33, 0x41, 0x00, 0x00, 0x00, 0x00, 0xEE, 0xFE, 0x20, 0x55};

   long int *long_int_cast = (long int*)&bytestring;
   long int *long_int_cast_swap = (long int*)&bytestring_swap_long;
   printf("cast to long int %lx\n", *long_int_cast);
   printf("cast to long int (endian swap)");
   for (i = 0; i < sizeof(*bytestring_swap_long)/sizeof(long int); i++)
   {
      printf(" %lx", long_int_cast_swap[i]);
   }
   printf("\n");

   int *int_cast = (int*)&bytestring;
   int *int_cast_swap = (int*)&bytestring_swap_int;
   printf("cast to int %x\n", *int_cast);
   printf("cast to int (endian swap) %x\n", *int_cast_swap);

   short int *short_int_cast = (short int*)&bytestring;
   short int *short_int_cast_swap = (short int*)&bytestring_swap_short;
   printf("cast to short int");
   for (i = 0; i < sizeof(*bytestring_swap_short)/sizeof(short int); i++)
   {
      printf(" %x", short_int_cast[i]);
   }
   printf("\n");
   printf("cast to short int (endian swap) %x\n", *short_int_cast_swap); 

   //========== PART 3 ==========
   printf("\n========== PART 3 ==========\n");

   printf("3a. 0x20 + 0x35 = 0x%x\n", add_bytes(0x20, 0x35));
   printf("3b. 0x80 + 0x7F = 0x%x\n", add_bytes(0x80, 0x7F));
   printf("3c. 0x80 + 0xFF = 0x%x\n", add_bytes(0x80, 0xFF));
   printf("3d. 0xFF + 0x01 = 0x%x\n", add_bytes(0xFF, 0x01));

   //========== PART 4 ==========
   printf("\n========== PART 4 ==========\n");

   printf("4.1 0x20 + 0x35 = carry: %i\n", find_carry(0x20, 0x35));
   printf("4.2 0x80 + 0x7F = carry: %i\n", find_carry(0x80, 0x7F));
   printf("4.3 0x80 + 0xFF = carry: %i\n", find_carry(0x80, 0xFF));
   printf("4.4 0xFF + 0x01 = carry: %i\n", find_carry(0xFF, 0x01));

   //========== PART 5 ==========
   printf("\n======== Part 5 ==========\n");

   printf("5.1 0x20 + 0x35 = 0x%x, overflow: %i\n", add_bytes_signed(0x20, 0x35), find_overflow(0x20, 0x35));
   printf("5.2 0x80 + 0x7F = 0x%x, overflow: %i\n", add_bytes_signed(0x80, 0x7F), find_overflow(0x80, 0x7F));
   printf("5.3 0x80 + 0xFF = 0x%x, overflow: %i\n", add_bytes_signed(0x80, 0xFF), find_overflow(0x80, 0xFF));
   printf("5.4 0xFF + 0x01 = 0x%x, overflow: %i\n", add_bytes_signed(0xFF, 0x01), find_overflow(0xFF, 0x01));

   //========== PART 6 ==========
   printf("\n======== Part 6 ==========\n");

   unsigned char result[] = {0x00, 0x00, 0x00, 0x00, 0x00};
   unsigned char a1[] = {0x44, 0x00, 0x00, 0x00, 0x01};
   unsigned char a2[] = {0x30, 0xFF, 0xFF, 0x00, 0x00};
   unsigned int carry_in = 0;
   unsigned int carry = arbitrary_byte_add(result, a1, a2, 5, carry_in);
   printf("6.a 0x4400000001 + 0x30FFFF0000 = 0x%x ", result[0]);
   printf("0x%x ", result[1]);
   printf("0x%x ", result[2]);
   printf("0x%x ", result[3]);
   printf("0x%x, ", result[4]);
   printf("carry = %d\n", carry);

   unsigned char a21[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   unsigned char a22[] = {0x00, 0x00, 0x00, 0x00, 0x01};
   unsigned char result2[] =  {0x00, 0x00, 0x00, 0x00, 0x00};
   unsigned int carry_in2 = 0;
   unsigned int carry2 = arbitrary_byte_add(result2, a21, a22, 5, carry_in2);
   printf("6. 0xFFFFFFFFFF + 0x0000000001 = 0x%x ", result2[0]);
   printf("0x%x ", result2[1]);
   printf("0x%x ", result2[2]);
   printf("0x%x ", result2[3]);
   printf("0x%x, ", result2[4]);
   printf("carry = %d\n", carry2);
}

unsigned char add_bytes(unsigned char byte1, unsigned char byte2)
{
   unsigned char result = byte1 + byte2;
   return result;
}

unsigned char find_carry(unsigned char byte1, unsigned char byte2)
{
   unsigned char result = 0;
   unsigned char sum = byte1 + byte2;
   if (sum < byte1)
   {
      result = 1;
   }
   return result;
}

unsigned char add_bytes_signed(unsigned char byte1, unsigned char byte2)
{
   unsigned char res = byte1 + byte2;
   return res;
}

unsigned char find_overflow(unsigned char byte1, unsigned char byte2)
{
   unsigned char res = 0;
   unsigned char sum = byte1 + byte2;
   //idk
   return res;
}

unsigned int arbitrary_byte_add(unsigned char *result, unsigned char *a1, unsigned char *a2, int size, unsigned int carry_in)
{
   unsigned int carry = carry_in;
   int n;
   unsigned char check;
   result = result + (size-1) * (int)sizeof(unsigned char);
   a1 = a1 + (size-1) * (int)sizeof(unsigned char);
   a2 = a2 + (size-1) * (int)sizeof(unsigned char);
   for (n = 0; n < size;n++)
   {
      *result = *a1 + *a2;
      if (carry == 1)
      {
         *result = *result + 0x01;
      }
      if (*result < *a1)
      {
         carry = 1;
      }
      else
      {
         carry = 0;
      }
      result = result - (int)sizeof(unsigned char);
      a1 = a1 - (int)sizeof(unsigned char);
      a2 = a2 - (int)sizeof(unsigned char);
   }
   return carry;
}

