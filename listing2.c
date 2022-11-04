#include <stdlib.h>
#include <stdio.h>
#include "listing1.c"

/* 
 *
 * C operators:
 * https://www.programiz.com/c-programming/bitwise-operators#:~:text=To%20perform%20bit-level%20operations%20in%20C%20programming%2C%20bitwise,AND%20operation%20of%20two%20integers%2012%20and%2025.
 *
 * */

typedef unsigned char u8;
typedef long long i64;
typedef i64 field_elem[16];


int main(){



	field_elem a = {65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535};
	field_elem b = {65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535};

	fmul(a,a,b);
	
	for (int i=0;i<16;++i){
		printf("%lld, ",a[i]);
	}
	printf("\n");

	carry25519(a); carry25519(a); carry25519(a);
	for (int i=0;i<16;++i){
		printf("%lld, ",a[i]);
	
	}

	printf("\n");
}

static void finverse(field_elem out, const field_elem in)
{
	field_elem c;
	int i;
	for (i = 0; i < 16; ++i) c[i] = in[i];
	for (i = 253; i >= 0; i--) {
		fmul(c,c,c);
		if (i != 2 && i != 4) fmul(c,c,in);
	}
	for (i = 0; i < 16; ++i) out[i] = c[i];
}



static void swap25519(field_elem p, field_elem q, int bit)
{
	i64 t, i, c = ~(bit - 1);  /* ~ tilda je doplnok */ 
	for (i = 0; i < 16; ++i) {
		t = c & (p[i] ^ q[i]); /* ^ je or */
		p[i] ^= t;
		q[i] ^= t;
	}
}

static void pack25519(u8 *out, const field_elem in)
{
	int i, j, carry;
	field_elem m, t;
	for (i = 0; i < 16; ++i) t[i] = in[i];
	carry25519(t); carry25519(t); carry25519(t);
	for (j = 0; j < 2; ++j) {
		m[0] = t[0] - 0xffed;
		for(i = 1; i < 15; i++) {
			m[i] = t[i] - 0xffff - ((m[i-1] >> 16) & 1);
			m[i-1] &= 0xffff;
		}
		m[15] = t[15] - 0x7fff - ((m[14] >> 16) & 1);
		carry = (m[15] >> 16) & 1;
		m[14] &= 0xffff;
		swap25519(t, m, 1 - carry);
	}
	for (i = 0; i < 16; ++i) {
		out[2*i] = t[i] & 0xff;
		out[2*i + 1] = t[i] >> 8;
	}
}



