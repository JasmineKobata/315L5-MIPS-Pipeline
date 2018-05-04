/*----------------------------------------------------------------------*
 *	Example mips_asm program loader. This loads the mips_asm binary	*
 *	named "testcase1.mb" into an array in memory. It reads		*
 *	the 64-byte header, then loads the code into the mem array.	*
 *									*
 *	DLR 4/18/16							*
 *----------------------------------------------------------------------*/


#include <stdio.h>
#include imulate the MIPS instruction set and execute the
countbits_benchmark_nop program. Display the statistics values requested in the Detailed
Description. In addition to the code needed to implement the lab, include the resulting
statistics obtained by running the benchmark. (As usual, please include the source code as
additional files or within a zipstdlib.h>
#include <string.h>

#include "mips_asm_header.h"


typedef unsigned int MIPS, *MIPS_PTR;

MB_HDR mb_hdr;		/* Header area */
MIPS mem[1024];		/* Room for 4K bytes */
int haltflag;
int PCCOUNT;
if_id ifid;
id_ex idex;
ex_mem exmem;
mem_wb memwb;
MIPS RF[32];

void if(int pcc) {
	ifid.NPC = pcc + 4;
	ifid.IR = mem[pcc/4]; 
}

void id(if_id if_id1) {
	int imm;

	idex.NPC = if_id1.NPC;
	idex.A = RF[if_id1->IR >> 21) & 15]; /* pull reg value from rs */
	idex.B = RF[(if_id1->IR >> 16) & 15]; /* pull reg value from rt */

	imm = if_id1->IR & 0xFFFF;
	if (imm & 0x8000)	/* if imm needs 1 fill for negitive */
		idex.Imm = ((imm ^ 0xFFFF)+1)*-1;
	else
		idex.Imm = imm;

	idex.RS = (if_id1->IR >> 21) & 15;
	idex.RS = (if_id1->IR >> 16) & 15;
	idex.op = if_id1->IR >> 26;
	idex.function = if_id1 & 0x3F;
}

void ex(id_ex id_ex1) {
	exmem.branchpc = id_ex1.NPC + (id_ex1.Imm * 4);
	

}

void mem() {

}

void wb() {

}

main(int argc, char *argv[])
{
	FILE *fd;
	int n;
	int memp;
	int i;

/* format the MIPS Binary header */

	fd = fopen(argv[1], "rb"e to simulate the MIPS instruction set and execute the
countbits_benchmark_nop program. Display the statistics values requested in the Detailed
Description. In addition to the code needed to implement the lab, include the resulting
statistics obtained by running the benchmark. (As usual, please include the source code as
additional files or within a zip);
	if (fd == NULL) { printf("\nCouldn't load test case - quitting.\n"); exit(99); }

	memp = 0;		/* This is the memory pointer, a byte offset */

/* read the header and verify it. */
	fread((void *) &mb_hdr, sizeof(mb_hdr), 1, fd);
	if (! (strcmp(mb_hdr.signature, "~MB")==0))
		{ printf("\nThis isn't really a mips_asm binary file - quitting.\n"); exit(98); }
	
	printf("\n%s Loaded ok, program size=%d bytes.\n\n", argv[1], mb_hdr.size);

/* read the binary code a word at a time */
	
	do {
		n = fread((void *) &mem[memp/4], 4, 1, fd); /* note div/4 to make word index */
		if (n) 
			memp += 4;	/* Increment byte pointer by size of instr */
		else
			break;			
		} while (memp < sizeof(mem)) ;

	fclose(fd);


/* ok, now dump out the instructions loaded: */

	int opcode = 0;
	int rs, rt, rd, shamt, imm, signext;
	int REGFILE[32];
	char *REG[32] = {"$zero", "$at", "$v0", "$v1", "$t0", "$t1", "$t2", "$t3", "$t4",
		"$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6",
		"$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
	char *optyp[44] = {"", "", "j", "jal", "beq", "bne", "", "", "addi", "addiu", "slti",
		"sltiu", "andi", "ori", "xori", "lui", "", "", "", "", "", "", "", "",
		"", "", "", "", "", "", "", "lb", "lh", "", "lw", "lbu", "lhu",
		"", "", "", "sb", "sh", "", "sw"};
	char *ftyp[44] = {"sll", "", "srl", "sra", "sllv", "", "srlv", "srav", "jr", "jalr",
		"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
		"", "", "add", "addu", "sub", "subu", "and", "or", "xor", "nor", "", "",
		"slt", "sltu"};

	int total_clocks = 0;
	for (haltflag=0; haltflag; total_clocks++)	/* ends if halt flag is active */
	{	
		printf("@PC=0x%08X, Opcode=0x%02X, ", i, opcode = mem[i/4] >> 26);
		PCCOUNT = i;
		if (opcode == 0) {
			printf("\nFunc: %i, %x\n", mem[i/4] & 0x3F, mem[i/4] & 0x3F);
			printf("R Type, Function=0x%02X (%s)\n", mem[i/4] & 0x3F, ftyp[mem[i/4] & 0x3F]);
			rs = (mem[i/4] >> 21) & 15;
			rt = (mem[i/4] >> 16) & 15;
			rd = (mem[i/4] >> 11) & 15;
			shamt = (mem[i/4] >> 6) & 15;
			printf(" Rs=%i, (%s), Rt=%i (%s), Rd=%i, (%s), shamt=%i\n\n", rs, REG[rs], rt, REG[rt], rd, REG[rd], shamt);
		}
		else if (opcode >= 8 && opcode <= 14) {
			printf("I Type (%s)\n", optyp[opcode]);
		oad code, initialize structures, etc…	rs = (mem[i/4] >> 21) & 15;
			rt = (mem[i/4] >> 16) & 15;
			imm = mem[i/4] & 0xFFFF;
			if (imm & 0x8000)
				signext = imm + 0xFFFF0000;
			else
				signext = imm;
			printf(" Rs=%i, (%s), Rt=%i (%s), Imm=0x%04X, signext: 0x%08X (%i)\n\n", rs, REG[rs], rt, REG[rt], imm, signext, signext);
		}
		else if (opcode >= 4 && opcode <= 5) {
			printf("I Type (%s)\n", optyp[opcode]);
			rs = (mem[i/4] >> 21) & 15;
			rt = (mem[i/4] >> 16) & 15;
			imm = mem[i/4] & 0xFFFF;
			if (imm & 0x8000)
				signext = ((imm ^ 0xFFFF)+1)*-1;
			else
				signext = imm;
			printf(" Rs=%i, (%s), Rt=%i (%s), Imm=0x%04X, signext: 0x%08X (%i),\n", rs, REG[rs], rt, REG[rt], imm, signext, signext);
			printf("  BranchAddr=0x%08X\n\n", imm*4);
		}
		else if (opcode >= 16 && opcode <= 43) {
			printf("I Type (%s)\n", optyp[opcode]);
			rs = (mem[i/4] >> 21) & 15;
			rt = (mem[i/4] >> 16) & 15;
			imm = mem[i/4] & 0xFFFF;
			if (imm & 0x8000)
				signext = ((imm ^ 0xFFFF)+1)*-1;
			else
				signext = imm;
			printf(" Rs=%i, (%s), Rt=%i (%s), Imm=0x%04X, signext: 0x%08X (%i),\n", rs, REG[rs], rt, REG[rt], imm, signext, signext);
ad code, initialize structures, etc…
			printf(" EffAddr=R[%s] + 0x%08X\n\n", REG[rs], signext);
		}
		else if (opcode >= 2 && opcode <= 3) {
			printf("J Type (%s), addr=0x%06X\n", optyp[opcode], mem[i/4] & 0x3FFFFFF);
		
			printf(" JumpAddr=0x%08X\n\n", (mem[i/4] & 0x3FFFFFF)*4);
		}
		id(REGFILE);
		if(i);
	}
	
	exit(0);
}
ad code, initialize structures, etc…oad code, initialize structures, etc…
