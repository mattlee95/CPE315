typedef struct _mb_hdr
{
   char signature[4];     /* Signature = 0x7F, 'M', 'B", 0x00 */
   unsigned int size;      /* Size of assembler program portion, bytes */    
   unsigned int entry;      /* Entry point offset of program if not zero */
   unsigned int filler1;     /* Unused ... reserved for future use. */
   unsigned char filler2[64-16]; /* Overall header is 64 bytes */
} MB_HDR, *MB_HDR_PTR;
