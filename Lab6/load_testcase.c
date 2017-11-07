#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mips_asm_header.h"

unsigned int registers[32];
unsigned int PC;

int num_instructions;
int num_memory_accesses;
int num_clock_cycles;

unsigned int memory[10000];

typedef unsigned int MIPS, *MIPS_PTR;

MB_HDR mb_hdr;    /* Header area */
MIPS mem[1024];      /* Room for 4K bytes */

void load_instructions(char *filename);
void execute_instruction(MIPS instruction);
void j(MIPS instruction);
void jal(MIPS instruction);
int syscall();

char *func_dic[44] = {"sll", "", "srl", "sra", "sllv", "", "srlv", "srav", "jr",
                   "jalr", "", "", "", "", "", "", "", "", "", "", "", "", "",
                   "", "", "", "", "", "", "", "", "", "add", "addu", "sub",
                   "subu", "and", "or", "xor", "nor", "", "", "slt", "sltu"};
                   
char *op_dic[44] = {"R-type", "", "j", "jal", "beq", "bne", "", "", "addi", "addiu",
                 "slti", "sltiu", "andi", "ori", "xori", "lui", "", "", "", "",
                 "", "", "", "", "", "", "", "", "", "", "", "", "lb", "lh", "",
                 "lw", "lbu", "lhu", "", "", "sb", "sh", "", "sw"};

char *reg_dic[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0",
                     "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1",
                     "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0",
                     "$k1", "$gp", "$sp", "$fp", "$ra"};
                     
int main(int argc, char *argv[])
{
  char* filename = argv[1]; /* This is the filename to be loaded */
  load_instructions(filename);
  PC = 0; //set PC 
  char action = '\0';
  while (strcmp(&action, "exit") != 0) {
     if (strcmp(&action, "run") == 0) {
        while (!syscall()) {
           execute_instruction(mem[PC/4]);
        }
        // print statistics 
        exit(0);
     }
     if (strcmp(&action, "step") == 0) {
        execute_instruction(mem[PC/4]);
        if (syscall()) {
           //print statistics
           exit(0);
        }
        //print statistics
     }
     else {
        printf("run | step | exit\n");
     }
     scanf("%s", &action);
  }
  exit(0);
}

int syscall() {
   unsigned int opcode = (registers[2] << 26) & 0x0000003F;
   if (opcode == 0x000000A) {
      return 1;
   }
   return 0;
}

void load_instructions(char* filename) {
  FILE *fd;
  int n;
  int memp;
  int i;

/* format the MIPS Binary header */

  fd = fopen(filename, "rb");
  if (fd == NULL) { printf("\nCouldn't load test case - quitting.\n"); exit(99); }

  memp = 0;    /* This is the memory pointer, a byte offset */

/* read the header and verify it. */
  fread((void *) &mb_hdr, sizeof(mb_hdr), 1, fd);
  if (! strcmp(mb_hdr.signature, "~MB")==0)
    { printf("\nThis isn't really a mips_asm binary file - quitting.\n"); exit(98); }

  printf("\n%s Loaded ok, program size=%d bytes.\n\n", filename, mb_hdr.size);

/* read the binary code a word at a time */
  do {
    n = fread((void *) &mem[memp/4], 4, 1, fd); /* note div/4 to make word index */
    if (n)
      memp += 4;  /* Increment byte pointer by size of instr */
    else
      break;
    } while (memp < sizeof(mem)) ;

  fclose(fd);
/* ok, now dump out the instructions loaded: */

 /*
  for (i = 0; i<memp; i+=4)  //  i contains byte offset addresses 
     {
     printf("Instruction@%08X : %08X\n", i, mem[i/4]);
     execute_instruction(mem[i/4]);
     }
  printf("\n");
 */
}

void execute_instruction(MIPS instruction)
{
   char instruct = '0';
   unsigned int opcode = (instruction >> 26) & 0x0000003F;
   unsigned int func = (instruction) & 0x0000003F;
   if (opcode == 0)
   {
      printf("Instruction Type = R\n");
      instruct = 'r';
   }
   else if ((opcode < 0x04) && (opcode != 0x01))
   {
      printf("Instruction Type = J\n");
      instruct = 'j';
      if (opcode == 0x02) {
         j(instruction);
      }
      if (opcode == 0x03) {
         jal(instruction);
      }
   }
   else if ((opcode == 0x04) || (opcode == 0x05) || ((opcode > 0x07) && (opcode < 0x10))
      || ((opcode > 0x19) && (opcode < 0x2C) && (opcode != 0x22) && (opcode != 0x26) &&
      (opcode != 0x27) && (opcode != 0x2A)))
   {
      printf("Instruction Type = I\n");
      instruct = 'i';
   }
   else
   {
      printf("Invalid Instruction\n");
      exit(0);
   }
   if (opcode != 0)
   {
      printf("Opcode = %02X, Function: %s\n", (instruction >> 26) & 0x0000003F, op_dic[opcode]);
   }
   else
   {
      printf("Opcode = %02X, Func Code: %02X, Function: %s\n", (instruction >> 26) & 0x0000003F, func, func_dic[func]);
   }
   if (instruct == 'r')
   {
      printf("RS: %s, RT: %s, RD: %s, Shamt: %02X, Funct: %02X\n\n",
             reg_dic[(instruction >> 21) & 0x0000001F],
             reg_dic[(instruction >> 16) & 0x0000001F],
             reg_dic[(instruction >> 11) & 0x0000001F],
             (instruction >> 6) & 0x0000001F,
             (instruction) & 0x0000003F);
   }
   else if (instruct == 'i')
   {
      printf("RS: %s, RT: %s, Imm: %04X\n\n",
             reg_dic[(instruction >> 21) & 0x0000001F],
             reg_dic[(instruction >> 16) & 0x0000001F],
             (instruction) & 0x0000FFFF);
   }
   else if (instruct == 'j')
   {
      printf("Word Index: %07X\n\n",
            (instruction) & 0x03FFFFFF);
   }
}

void j(MIPS instruction) { //PC = JumpAddr
   unsigned int jump_address = (instruction) & 0x03FFFFFF; //get lower 26 bits
   jump_address = (jump_address << 2); //shift left twice 
   unsigned int first_pc = (PC & 0xF0000000); //get first 4 bits of PC
   jump_address = jump_address + first_pc; //substitute lower 28 bits of PC w/ new address
   PC = jump_address;

   num_instructions += 1;
   num_clock_cycles += 3;
}

void jal(MIPS instruction) { //$ra = PC + 4, PC = JumpAddr 
   registers[31] = PC + 4;
   unsigned int jump_address = (instruction) & 0x03FFFFFF; //get lower 26 bits
   jump_address = (jump_address << 2); //shift left twice 
   unsigned int first_pc = (PC & 0xF0000000); //get first 4 bits of PC
   jump_address = jump_address + first_pc; //substitute lower 28 bits of PC w/ new address
   PC = jump_address;

   num_instructions += 1;
   num_clock_cycles += 4;
}









                     
                     
                     
                     
                     
                     
                     
