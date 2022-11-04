#include <stdlib.h>
#include <stdio.h>
typedef unsigned char u8;
typedef long long i64;
typedef i64 field_elem[16];

static void unpack25519(field_elem out, const u8 *in)
{
	int i;
	for (i = 0; i < 16; ++i){
		printf("%lu + %lu\n",in[2*i],in[2*i+1]);
	       	out[i] = in[2*i] + ((i64) in[2*i + 1] << 8);
		printf("out[%d]: %lu\n",i,out[i]);
	}
	out[15] &= 0x7fff;
}

static void carry25519(field_elem elem){

	int i;
	i64 carry;
	for (i=0; i<16;++i){
		carry = elem[i] >> 16;
		elem[i] -= carry <<16;
		//printf("CARRY:\n");
		//print(elem);
		if(i <15) elem[i+1]+=carry;else elem[0]+= carry*38;
		//print(elem);
		//printf("END CARRY\n");
	}

}

static void fadd(field_elem out, const field_elem a, const field_elem b){
	int i;
	for (i=0;i<16;++i){
		out[i] = a[i]+b[i];
	}
}

static void fsub(field_elem out, const field_elem a, const field_elem b){
	int i;
	for (i=0;i<16;++i){
		out[i] = a[i]-b[i];
	}
}



static void fmul(field_elem out, const field_elem a, const field_elem b){
	i64 i,j, product[31];
	for (i = 0; i<31;++i) product[i] = 0;
	for (i=0;i<16;++i){
		for (j=0;j<16;++j){
			product[i+j] = a[i] * b[j];
		}
	}
	for (i=0; i<15;++i) product[i]+=38*product[i+16];
	for (i=0; i<16;++i) out[i] = product[i];

	carry25519(out);
	carry25519(out);
}

void print(field_elem o){

//	printf("PRINT: ");
	for (int i=0;i<16;i++){
		printf("%lld ",o[i]);
		
	}
	printf("\n");
}


/*
int main(){
field_elem out = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const u8 in[32] = {1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15,16,16};






unpack25519(out,in);


field_elem a = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
field_elem b = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

fmul(a,a,b);


printf("SKUKSA SCINANIA 2 8bit CISEL\n");

i64 cislo_a = 65535;
i64 cislo_b = 65535;

printf("%u + %u = %u\n",cislo_a,cislo_b,cislo_a+cislo_b);



return 0;
}

*/
