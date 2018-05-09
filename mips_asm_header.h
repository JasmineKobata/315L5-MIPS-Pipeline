/*----------------------------------------------------------------------*
 *	mips_asm Binary header (.mb) file header format.  This header	*
 *	provides for future information on mips assembler files.	*
 *									*
 *	write_header - writes mips_asm header, along with test data	*
 *									*
 *	This shows how the mips_asm header is written, and provides	*
 *	a test case (testcase1.mb) to be used for lab projects.		*
 *----------------------------------------------------------------------*/

typedef struct _mb_hdr
  {
  char signature[4];		/* Signature = 0x7F, 'M', 'B", 0x00 */
  unsigned int size;		/* Size of assembler program portion, bytes */
  unsigned int entry;		/* Entry point offset of program if not zero */
  unsigned int filler1;		/* Unused ... reserved for future use. */
  unsigned char filler2[64-16];	/* Overall header is 64 bytes */
  } MB_HDR, *MB_HDR_PTR;
struct if_id
  {
	int NPC; /* next pc */
	int IR; /* instruction */
  };
struct id_ex
  {
	int NPC; /* next pc */
	int A; /* ALU A in */
	int B; /* ALU B in */
	int Imm; /* sign extended immediate value */
	int shamt; /* shift amount */
	int op; /* op code */
	int function; /* function code */
	int RegWrite;
	int MemtoReg;
  };
struct ex_mem
  {
	int branch_pc; /* potentail pc for a branch */
	int cond; /* zero execption for breq */
	int AO; /* ALU out */
	int B; /* B value potentail value being writen to memory */
	int dest; /* destination of reg wright back */
	int op; /* op code */
	int RegWrite;
	int MemtoReg;
  };
struct mem_wb
  {
	int LDM; /* Memory output */ 
	int AO; /* ALU out */
	int dest; /* write back reg dest */
	int op; /* op code */
	int RegWrite;
	int MemtoReg;
  };
