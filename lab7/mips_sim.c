/* MIPS pipelined simulator */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mips_asm_header.h"

unsigned int regs[32];
unsigned int PC;

int num_instructions;
int num_memory_accesses;
int num_clock_cycles;
int num_if, num_id, num_ex, num_mem, num_wb;

int haltflag;

/*For R instructions*/
unsigned int rs;
unsigned int rd;
unsigned int rt;
unsigned short int imm;
unsigned int shamt;

unsigned int memory[10000];

typedef unsigned int MIPS, *MIPS_PTR;

MB_HDR mb_hdr;    /* Header area */
MIPS mem[1024];      /* Room for 4K bytes */

void j(MIPS instruction);
void jal(MIPS instruction);
void syscall();

void load_instructions(char *filename);
void execute_instruction(MIPS instruction);

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

typedef struct {
   int active;
   MIPS instruction;
   unsigned int pc_plus_4;
} if_id;

typedef struct {
   int active;
   char type; //i, j ,r
   char *instr;
   unsigned int pc_plus_4;
   unsigned int rd;
   unsigned int rs;
   unsigned int rt;
   unsigned int imm;
   unsigned int shamt;
} id_ex;

typedef struct {
   int active;
   unsigned int alu_result;
} ex_me;

typedef struct {
   int active;
   unsigned int rd;
} me_wb;

void i_fetch(MIPS instruction, if_id *IFID);
void i_decode(if_id *IFID);
void ex(id_ex *IDEX);
void me(ex_me *EXME);
void wb(me_wb *MEWB);

int main(int argc, char *argv[])
{
    int i;
    char* filename = argv[1]; /* This is the filename to be loaded */
    load_instructions(filename);
    PC = mb_hdr.entry;
    num_instructions = num_clock_cycles = num_memory_accesses = 0; /*initialize all to 0*/
    
    /*sets register values to zero*/
    for (i=0; i < 32; i++)
    regs[i] = 0;

    if_id *IFID;
    id_ex *IDEX;
    ex_me *EXME;
    me_wb *MEWB;
	   
    for (haltflag = 0; haltflag; num_clock_cycles++) {
       IFID = (if_id *)malloc(sizeof(if_id));
       IDEX = (id_ex *)malloc(sizeof(id_ex));
       EXME = (ex_me *)malloc(sizeof(ex_me));
       MEWB = (me_wb *)malloc(sizeof(me_wb));

       i_fetch(mem[PC/4], IFID);
       // id, ex, me, wb

       free(IFID);
       free(IDEX);
       free(EXME);
       free(MEWB);
    }

    /*print statistics */
    for (i=0; i < 32; i++)
    {
	printf("%s: %x\n", reg_dic[i], regs[i]);
    }
    printf("Instructions Executed: %d\n", num_instructions);
    printf("Clock Cycles: %d\n", num_clock_cycles);
    printf("Program Counter: %d\n" , PC);
    printf("%d IFs, %d IDs, %d EXs, %d MEMs, %d WBs\n", num_if, num_id, num_ex, num_mem, num_wb);
    exit(0);
}

void i_fetch(MIPS instruction, if_id *IFID) {
   IFID->instruction = instruction;
   PC += 4;
   IFID->pc_plus_4 = PC;   
   num_if += 1;
   num_instructions += 1;
}

void i_decode(if_id *IFID) {


}

void ex(id_ex *IDEX) {


   //check for syscall
}

void me(ex_me *EXME) {

}

void wb(me_wb *MEWB) {

}

void syscall() {
    unsigned int opcode = (regs[2]) & 0x0000003F;
    if (opcode == 0x000000A) {
        haltflag = 1;
    }
    return;
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
    if (!(strcmp(mb_hdr.signature, "~MB")==0))
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
}

void execute_instruction(MIPS instruction)
{
    char *function;
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
        printf("Opcode = %02X, Function: %s\n", (instruction >> 26) & 0x0000003F, function = op_dic[opcode]);
    }
    else
    {
        printf("Opcode = %02X, Func Code: %02X, Function: %s\n", (instruction >> 26) & 0x0000003F, func, function = func_dic[func]);
    }
    if (instruct == 'r')
    {
        /*obtain index of registers used*/
        rs = (instruction >> 21) & 0x0000001F;
        rt = (instruction >> 16) & 0x0000001F;
        rd = (instruction >> 11) & 0x0000001F;
        shamt = (instruction >> 6) & 0x0000001F;
        
        printf("RS: %s, RT: %s, RD: %s, Shamt: %02X, Funct: %02X\n\n",
               reg_dic[rs],
               reg_dic[rt],
               reg_dic[rd],
               shamt,
               (instruction) & 0x0000003F);
        
        execute_R(function, rd, rs, rt, shamt);
    }
    else if (instruct == 'i')
    {
        rs = (instruction >> 21) & 0x0000001F;
        rt = (instruction >> 16) & 0x0000001F;
        imm = (instruction) & 0x0000FFFF;
        printf("RS: %s, RT: %s, Imm: %04X \n\n", reg_dic[rs], reg_dic[rt], imm);
        execute_I(function, rt, rs, imm);
    }
    else if (instruct == 'j')
    {
        printf("Word Index: %07X\n\n",
               (instruction) & 0x03FFFFFF);
    }
}

void execute_R(char *F, unsigned int Rd, unsigned int Rs, unsigned int Rt, unsigned int Sh)
{
    if (strcmp(F, "add") == 0) {R_add(Rd, Rs, Rt);}
    else if (strcmp(F, "addu") == 0) {R_addu(Rd, Rs, Rt);}
    else if (strcmp(F, "sub") == 0) {R_sub(Rd, Rs, Rt);}
    else if (strcmp(F, "subu") == 0) {R_subu(Rd, Rs, Rt);}
    else if (strcmp(F, "and") == 0) {R_and(Rd, Rs, Rt);}
    else if (strcmp(F, "nor") == 0) {R_nor(Rd, Rs, Rt);}
    else if (strcmp(F, "or") == 0) {R_or(Rd, Rs, Rt);}
    else if (strcmp(F, "xor") == 0) {R_xor(Rd, Rs, Rt);}
    
    else if (strcmp(F, "sll") == 0) {R_sll(Rd, Rt, Sh);}
    else if (strcmp(F, "srl") == 0) {R_srl(Rd, Rt, Sh);}
    else if (strcmp(F, "sra") == 0) {R_sra(Rd, Rt, Sh);}
    else if (strcmp(F, "sllv") == 0) {R_sllv(Rd, Rs, Rt);}
    else if (strcmp(F, "srlv") == 0) {R_srlv(Rd, Rs, Rt);}
    else if (strcmp(F, "srav") == 0) {R_srav(Rd, Rs, Rt);}
    
    else if (strcmp(F, "slt") == 0) {R_slt(Rd, Rs, Rt);}
    else if (strcmp(F, "sltu") == 0) {R_sltu(Rd, Rs, Rt);}
    
    else if (strcmp(F, "jr") == 0) {R_jr(Rs);}
    else if (strcmp(F, "jalr") == 0) {R_jalr(Rs);}
    
    
}
void R_add(unsigned int Rd, unsigned int Rs, unsigned int Rt) /*can throw overflow exceptions*/
{
    regs[Rd] = (signed int)regs[Rs] + (signed int)regs[Rt];
}

void R_addu(unsigned int Rd, unsigned int Rs, unsigned int Rt)
{
    regs[Rd] = regs[Rs] + regs[Rt];
}

void R_sub(unsigned int Rd, unsigned int Rs, unsigned int Rt) /*can throw overflow exceptions*/
{
    regs[Rd] = (signed int)regs[Rs] - (signed int)regs[Rt];
}
void R_subu(unsigned int Rd, unsigned int Rs, unsigned int Rt)
{
    regs[Rd] = regs[Rs] - regs[Rt];
}

void R_and(unsigned int Rd, unsigned int Rs, unsigned int Rt)
{
    regs[Rd] = regs[Rs] & regs[Rt];
}

void R_nor(unsigned int Rd, unsigned int Rs, unsigned int Rt)
{
    regs[Rd] = ~(regs[Rs] | regs[Rt]);
}

void R_or(unsigned int Rd, unsigned int Rs, unsigned int Rt)
{
    regs[Rd] = regs[Rs] | regs[Rt];
}

void R_xor(unsigned int Rd, unsigned int Rs, unsigned int Rt)
{
    regs[Rd] = regs[Rs] ^ regs[Rt];
}

void R_sll(unsigned int Rd, unsigned int Rt, unsigned int Sh)
{
    regs[Rd] = regs[Rt] << Sh;
}
void R_srl(unsigned int Rd, unsigned int Rt, unsigned int Sh)
{
    unsigned int temp = regs[Rt] >> Sh;
    regs[Rd] = temp;
}
void R_sra(unsigned int Rd, unsigned int Rt, unsigned int Sh)
{
    regs[Rd] = regs[Rt] >> Sh;
}


void R_sllv(unsigned int Rd, unsigned int Rs, unsigned int Rt)
{
    regs[Rd] = regs[Rt] << regs[Rs];
}

void R_srlv(unsigned int Rd, unsigned int Rs, unsigned int Rt)
{
    unsigned int temp = regs[Rt] >> regs[Rs];
    regs[Rd] = temp;
}

void R_srav(unsigned int Rd, unsigned int Rs, unsigned int Rt)
{
    regs[Rd] = regs[Rt] >> regs[Rs];
}

void R_slt(unsigned int Rd, unsigned int Rs, unsigned int Rt)
{
    if(regs[Rs] < regs[Rt])
    regs[Rd] = 1;
    else
    regs[Rd] = 0;
}

void R_sltu(unsigned int Rd, unsigned int Rs, unsigned int Rt)
{
    if((signed int)regs[Rs] < (signed int)regs[Rt])
    regs[Rd] = 1;
    else
    regs[Rd] = 0;
}

void R_jr(unsigned int Rs)
{
    PC = regs[rs];
}

void R_jalr(unsigned int Rs)
{
    PC = regs[rs];
    regs[31] = PC+4;
    
}

/*
 * Immediate Type Instructions
 */
void execute_I(char *F, unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    if (strcmp(F, "addi") == 0) {I_addi(Rt, Rs, Imm);}
    else if (strcmp(F, "addiu") == 0) {I_addiu(Rt, Rs, Imm);}
    
    else if (strcmp(F, "andi") == 0) {I_andi(Rt, Rs, Imm);}
    else if (strcmp(F, "ori") == 0) {I_ori(Rt, Rs, Imm);}
    else if (strcmp(F, "xori") == 0) {I_xori(Rt, Rs, Imm);}
    
    else if (strcmp(F, "slti") == 0) {I_slti(Rt, Rs, Imm);}
    else if (strcmp(F, "sltiu") == 0) {I_sltiu(Rt, Rs, Imm);}
    
    else if (strcmp(F, "beq") == 0) {I_beq(Rt, Rs, Imm);}
    else if (strcmp(F, "bne") == 0) {I_bne(Rt, Rs, Imm);}
    
    else if (strcmp(F, "lb") == 0) {I_lb(Rt, Rs, Imm);}
    else if (strcmp(F, "lbu") == 0) {I_lbu(Rt, Rs, Imm);}
    else if (strcmp(F, "lh") == 0) {I_lh(Rt, Rs, Imm);}
    else if (strcmp(F, "lhu") == 0) {I_lhu(Rt, Rs, Imm);}
    else if (strcmp(F, "lui") == 0) {I_lui(Rt, Rs, Imm);}
    else if (strcmp(F, "lw") == 0) {I_lw(Rt, Rs, Imm);}
    else if (strcmp(F, "li") == 0) {I_li(Rt, Rs, Imm);}
    
    else if (strcmp(F, "sb") == 0) {I_sb(Rt, Rs, Imm);}
    else if (strcmp(F, "sh") == 0) {I_sh(Rt, Rs, Imm);}
    else if (strcmp(F, "sw") == 0) {I_sw(Rt, Rs, Imm);}
}


void I_addi(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = *(signed int*)&regs[Rs] + *(signed short int*)&Imm;
}

void I_addiu(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = regs[Rs] + Imm;
}


void I_andi(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = regs[Rs] & Imm;
}

void I_ori(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = regs[Rs] | Imm;
}

void I_xori(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = regs[Rs] ^ Imm;
}


void I_slti(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    if (*(signed int*)&regs[Rs] < *(signed short int*)&Imm)
    {
        regs[Rt] = 1;
    }
    else
    {
        regs[Rt] = 0;
    }
}
void I_sltiu(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    if (regs[Rs] < Imm)
    {
        regs[Rt] = 1;
    }
    else
    {
        regs[Rt] = 0;
    }
}


void I_beq(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    if (regs[Rt] == regs[Rs])
    {
        PC += *(signed short int*)&Imm << 2;
    }
}
void I_bne(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    if (regs[Rt] != regs[Rs])
    {
        PC += *(signed short int*)&Imm << 2;
    }
}


void I_lb(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = mem[(*(signed int*)&regs[Rs] + *(signed short int*)&Imm)] & 0x000000FF;
}

void I_lbu(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = mem[(regs[Rs] + Imm)] & 0x000000FF;
}

void I_lh(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = mem[(*(signed int*)&regs[Rs] + *(signed short int*)&Imm)] & 0x0000FFFF;
}

void I_lhu(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = mem[regs[Rs] + Imm] & 0x0000FFFF;
}

void I_lui(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = (mem[regs[Rs] + Imm] >> 16) & 0x0000FFFF;
}

void I_lw(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = mem[(regs[Rs] + Imm)];
}

void I_li(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    regs[Rt] = Imm;
}


void I_sb(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    mem[regs[Rs] + Imm] = regs[Rt] & 0x000000FF;
}
void I_sh(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    mem[regs[Rs]] = regs[Rt] & 0x0000FFFF;
}
void I_sw(unsigned int Rt, unsigned int Rs, unsigned short int Imm)
{
    mem[regs[Rs]] = regs[Rt];
}


void j(MIPS instruction) {
    unsigned int jump_address = (instruction) & 0x03FFFFFF;
    jump_address = (jump_address << 2);
    unsigned int first_pc = (PC & 0xF0000000);
    jump_address = jump_address + first_pc - 4;
    PC = jump_address;
    
}

void jal(MIPS instruction) {
    regs[31] = PC + 4;
    unsigned int jump_address = (instruction) & 0x03FFFFFF;
    jump_address = (jump_address << 2);
    unsigned int first_pc = (PC & 0xF0000000);
    jump_address = jump_address + first_pc - 4;
    printf("jump_addr %d\n", jump_address);
    PC = jump_address;
}

