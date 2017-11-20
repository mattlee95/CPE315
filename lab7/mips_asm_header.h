/*----------------------------------------------------------------------*
 *	mips_asm Binary header (.mb) file header format.  This header	*
 *	provides for future information on mips assembler files.	*
 *									*
 *	write_header - writes mips_asm header, along with test data	*
 *									*
 *	This shows how the mips_asm header is written, and provides	*
 *	a test case (testcase1.mb) to be used for lab projects.		*
 *----------------------------------------------------------------------*/

typedef struct _mb_hdr {
  char signature[4];		/* Signature = 0x7F, 'M', 'B", 0x00 */
  unsigned int size;		/* Size of assembler program portion, bytes */
  unsigned int entry;		/* Entry point offset of program if not zero */
  unsigned int filler1;		/* Unused ... reserved for future use. */
  unsigned char filler2[64-16];	/* Overall header is 64 bytes */
} MB_HDR, *MB_HDR_PTR;

/*ALL R instructions----------------------------------------------------------------------*/
void execute_R(char *F, unsigned int Rd, unsigned int Rs, unsigned int Rt, unsigned int Sh);
void R_add(unsigned int Rd, unsigned int Rs, unsigned int Rt);
void R_addu(unsigned int Rd, unsigned int Rs, unsigned int Rt);
void R_sub(unsigned int Rd, unsigned int Rs, unsigned int Rt);
void R_subu(unsigned int Rd, unsigned int Rs, unsigned int Rt);

void R_and(unsigned int Rd, unsigned int Rs, unsigned int Rt);
void R_nor(unsigned int Rd, unsigned int Rs, unsigned int Rt);
void R_or(unsigned int Rd, unsigned int Rs, unsigned int Rt);
void R_xor(unsigned int Rd, unsigned int Rs, unsigned int Rt);

void R_sll(unsigned int Rd, unsigned int Rt, unsigned int Sh);
void R_srl(unsigned int Rd, unsigned int Rt, unsigned int Sh);
void R_sra(unsigned int Rd, unsigned int Rt, unsigned int Sh);
void R_sllv(unsigned int Rd, unsigned int Rs, unsigned int Rt);
void R_srlv(unsigned int Rd, unsigned int Rs, unsigned int Rt);
void R_srav(unsigned int Rd, unsigned int Rs, unsigned int Rt);

void R_slt(unsigned int Rd, unsigned int Rs, unsigned int Rt);
void R_sltu(unsigned int Rd, unsigned int Rs, unsigned int Rt);

void R_jr(unsigned int Rs);
void R_jalr(unsigned int Rs);
/*--------------------------------------------------------------------------------------*/

/*ALL I instructions----------------------------------------------------------------------*/
void execute_I(char *F, unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_addi(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_addiu(unsigned int Rt, unsigned int Rs, unsigned short int Imm);

void I_andi(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_ori(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_xori(unsigned int Rt, unsigned int Rs, unsigned short int Imm);

void I_slti(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_sltiu(unsigned int Rt, unsigned int Rs, unsigned short int Imm);

void I_beq(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_bne(unsigned int Rt, unsigned int Rs, unsigned short int Imm);

void I_lb(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_lbu(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_lh(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_lhu(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_lui(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_lw(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_li(unsigned int Rt, unsigned int Rs, unsigned short int Imm);

void I_sb(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_sh(unsigned int Rt, unsigned int Rs, unsigned short int Imm);
void I_sw(unsigned int Rt, unsigned int Rs, unsigned short int Imm);

