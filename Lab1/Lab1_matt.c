#include <stdio.h>

unsigned char add_bytes(unsigned char byte1, unsigned char byte2);
unsigned char find_carry(unsigned char byte1, unsigned char byte2);
char add_bytes_signed(char byte1, char byte2);
char find_overflow(char byte1, char byte2);

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

   unsigned char bytestring[] = {0x41, 0x33, 0x54, 0x80, 0xFF, 0x99, 0x01, 0x78, 0x55, 0x20, 0xFE, 0xEE, 0x00, 0x00, 0x00, 0x00}; 
   unsigned char bytestring_swap[] = {0x00, 0x00, 0x00, 0x00, 0xEE, 0xFE, 0x20, 0x55, 0x78, 0x01, 0x99, 0xFF, 0x80, 0x54, 0x33, 0x41};

   long int *long_int_cast = (long int*)bytestring;
   long int *long_int_cast_swap = (long int*)bytestring_swap;
   printf("cast to long int %x\n", *long_int_cast);
   printf("cast to long int (endian swap) %x\n", *long_int_cast_swap);

   int *int_cast = (int*)bytestring;
   int *int_cast_swap = (int*)bytestring_swap;
   printf("cast to int %x\n", *int_cast);
   printf("cast to int (endian swap) %x\n", *int_cast_swap);

   short int *short_int_cast = (short int*)bytestring;
   short int *short_int_cast_swap = (short int*)bytestring_swap;
   printf("cast to short int %x\n", *short_int_cast);
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

char add_bytes_signed(char byte1, char byte2)
{
   char result = byte1 + byte2;
   return result;
}

char find_overflow(char byte1, char byte2)
{
   char result = 0;
   char sum = byte1 + byte2;
   //idk
   return result;
}

