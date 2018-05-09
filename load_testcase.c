/*----------------------------------------------------------------------*
 *	Example mips_asm program loader. This loads the mips_asm binary	*
 *	named "testcase1.mb" into an array in memory. It reads		*
 *	the 64-byte header, then loads the code into the mem array.	*
 *									*
 *	DLR 4/18/16							*
 *----------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mips_asm_header.h"


typedef unsigned int MIPS, *MIPS_PTR;

MB_HDR mb_hdr;		/* Header area */
MIPS mem[1024];		/* Room for 4K bytes */
MIPS datareg[2048];
int haltflag;
struct if_id ifid;
struct id_ex idex;
struct ex_mem exmem;
struct mem_wb memwb;
MIPS RF[32];

void fetch() {
	ifid.IR = mem[ifid.NPC/4];
	ifid.NPC = ifid.NPC + 4;
	printf("IR: 0x%08X\n", ifid.IR);
	ifid.ready = 1;
}

void id() {
	int imm;

	idex.NPC = ifid.NPC;
	idex.A = RF[(ifid.IR >> 21) & 15]; /* pull reg value from rs */
	idex.B = RF[(ifid.IR >> 16) & 15]; /* pull reg value from rt */

	imm = ifid.IR & 0xFFFF;
	if (imm & 0x8000)	/* if imm needs 1 fill for negitive */
		idex.Imm = ((imm ^ 0xFFFF)+1)*-1;
	else
		idex.Imm = imm;

	idex.op = ifid.IR >> 26;
	idex.function = ifid.IR & 0x3F;
 	idex.RegWrite = 1;
	idex.MemtoReg = 0;
	idex.RD = (ifid.IR >> 11) & 15;
	idex.RT = (ifid.IR >> 16) & 15;
	idex.RS = (ifid.IR >> 21) & 15;
	idex.jumpword = ifid.IR & 0x3FFFFFF;
	if ((idex.op == 0) && (idex.function == 0x08)) {
	    idex.RegWrite = 0;
	 
	} else {

  		if (idex.op == 0x02 || idex.op == 0x02 || idex.op == 0x02 
			|| idex.op == 0x02 || idex.op == 0x02 || idex.op == 0x02) {
			idex.RegWrite = 0;
		} 
	}
	
	if (idex.op <= 0x25 && idex.op >= 0x0F)
		idex.MemtoReg = 1;
	ifid.ready = 1;
}

void ex() {
    exmem.branch_pc = idex.NPC + (idex.Imm * 4);
    exmem.op = idex.op;
    exmem.RegWrite = idex.RegWrite;
    exmem.MemtoReg = idex.MemtoReg;
	exmem.B = idex.B;
	exmem.jumpword = idex.jumpword;
	exmem.function = idex.function;
	exmem.RS = idex.RS;
    if (idex.op == 0) {
		exmem.dest = idex.RD;
        switch (idex.function) {
            case 0x20 : //add
                exmem.AO = idex.A + idex.B;
                break;
            case 0x21 : //add unsigned
                exmem.AO = (unsigned int)idex.A + (unsigned int)idex.B;
                break;
            case 0x22 : //sub
                exmem.AO = idex.A - idex.B;
                break;
            case 0x23 : //sub unsigned
                exmem.AO = (unsigned int)idex.A - (unsigned int)idex.B;
                break;
            case 0x24 : //and
                exmem.AO = idex.A & idex.B;
                break;
            case 0x27 : //nor
                exmem.AO = ~(idex.A | idex.B);
                break;
            case 0x25 : //or
                exmem.AO = idex.A | idex.B;
                break;
            case 0x26 : //xor
                exmem.AO = idex.A ^ idex.B;
                break;
            case 0x00 : //shift left logical
                exmem.AO = idex.A << idex.shamt;
                break;
            case 0x02 : //shift right logical
                exmem.AO = idex.A >> idex.shamt;
                break;
            case 0x03 : //shift right arithmetic
                exmem.AO = (signed int)idex.A >> idex.shamt;
                break;
            case 0x04 : //shift left logical variable
                exmem.AO = idex.A << idex.B;            
                break;
            case 0x06 : //shift right logical variable
                exmem.AO = idex.A >> idex.B;
                break;
            case 0x07 : //shift right arithmetic variable
                exmem.AO = (signed int)idex.A >> idex.B;
                break;
            case 0x2A : //set less than
                if (idex.A < idex.B)
                    exmem.AO = 1;
                else 
                    exmem.AO = 0;
                break;
            case 0x2B : //set less than unsigned
                if ((unsigned int)idex.A < (unsigned int)idex.B)
                    exmem.AO = 1;
                else 
                    exmem.AO = 0;
        }
    } else {
		exmem.dest = idex.RT;
        switch (idex.op) {

	    	case 0x20 : //load byte
            case 0x24 : //load byte unsigned
            case 0x21 : //load halfword
            case 0x25 : //load halfword unsigned
            case 0x0F : //load upper immediate
            case 0x23 : //load word
            case 0x28 : //store Byte
            case 0x29 : //store halfword
            case 0x2B : //store word
            case 0x08 : //add immediate
                exmem.AO = idex.A + idex.Imm;
                break;
            case 0x09 : //add unsigned immediate
                exmem.AO = (unsigned int)idex.A + (unsigned int)idex.Imm;
                break;
            case 0x0C : //and immediate
				exmem.AO = idex.A & idex.Imm;
                break;
            case 0x0D : //or immediate
                exmem.AO = idex.A | idex.Imm;
                break;
            case 0x0E : //xor immediate
				exmem.AO = idex.A ^ idex.Imm;
                break;
            case 0x0A : //set less than immediate
				if (idex.A < idex.Imm)
                    exmem.AO = 1;
                else 
                    exmem.AO = 0;
                break;
            case 0x0B : //set less than immediate unsigned
				if ((unsigned int)idex.A < (unsigned int)idex.Imm)
                    exmem.AO = 1;
                else 
                    exmem.AO = 0;
                break;

        }            
    }
	if (exmem.AO == 0)
		exmem.cond = 1;
	else
		exmem.cond = 0;
	exmem.ready = 1;
}

void clear() {
	ifid.ready = 0;
	exmem.ready = 0;
	idex.ready = 0;
	memwb.ready = 0;
}

void memory() {
	memwb.RegWrite = exmem.RegWrite;
	memwb.MemtoReg = exmem.MemtoReg;
	if (exmem.op == 0x02) { //Jump
		ifid.NPC = (exmem.jumpword * 4);
		clear();
	}
	else if (exmem.op == 0x03) {
		RF[31] = ifid.NPC; //JAL
		ifid.NPC = (exmem.jumpword *  4);
		clear();
	}
	else if (exmem.op == 0x00 && exmem.function == 0x08) { //JR
		ifid.NPC = RF[exmem.RS];
		clear();
	}
	else if (exmem.op == 0x00 && exmem.function == 0x09) { //JALR
		RF[31] = ifid.NPC;
		ifid.NPC = RF[exmem.RS];
		clear();
	}
	else if ((exmem.op == 0x04 && exmem.cond == 0x01) ||
		(exmem.op == 0x05 && exmem.cond == 0x00)) {
		ifid.NPC = exmem.branch_pc;
		clear();
	}
	else if (exmem.op == 0x00 && exmem.cond == 0x0C && RF[2] == 0)
		haltflag = 1;
		

	if (exmem.op >= 0x28 && exmem.op <= 0x2B) {
		datareg[exmem.B] = exmem.AO;
		clear();
	}

	memwb.LDM = datareg[exmem.B];
	memwb.dest = exmem.dest;
	memwb.AO = exmem.AO;
	memwb.op = exmem.op;
	memwb.ready = 1;
}

void wb() {
  if (memwb.RegWrite) {
    if (memwb.MemtoReg )
      RF[memwb.dest] = memwb.LDM;
    else
      RF[memwb.dest] = memwb.AO;
  }
}

int main(int argc, char *argv[])
{
	FILE *fd;
	int n;
	int memp;

/* format the MIPS Binary header */

	fd = fopen(argv[1], "rb");
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

	clear();
	int total_clocks = 0;
	int tmp_pc;
	ifid.NPC = 0;
	for (haltflag=0; haltflag != 1; total_clocks++)	/* ends if halt flag is active */
	{	
		if (memwb.ready == 1)
			wb();
		if (exmem.ready == 1)
			memory();
		if (idex.ready == 1)
			ex();
		if (ifid.ready == 1)
			id();
		tmp_pc = ifid.NPC;
		fetch();
		if (tmp_pc == ifid.NPC)
			break;

	}
	printf("%i\n", total_clocks);
	exit(0);
}
