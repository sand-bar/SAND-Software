#include "BATTypeDef.h" 
/*
BAT  128-128 avx2  32 way

*/
static const unsigned char _C_ALIGN32(Lbswap_maskA[32]) = {
5,2,7,4,1,6,3,0, 13,10,15,12,9,14,11,8, 5,2,7,4,1,6,3,0, 13,10,15,12,9,14,11,8,
};

static const unsigned char _C_ALIGN32(Lbswap_maskB[32]) = {
4,1,6,3,0,5, 2,7, 12,9,14,11,8,13, 10, 15,4,1,6,3,0,5, 2,7, 12,9,14,11,8,13, 10, 15,
};

#define BAT128128InputData256A(D0,D1,D2,D3,D4,D5,D6,D7,pt)\
D0 = _mm256_loadu_si256((__m256i*)(pt));\
D1 = _mm256_loadu_si256((__m256i*)(pt+32));\
D2 = _mm256_loadu_si256((__m256i*)(pt+32*2));\
D3 = _mm256_loadu_si256((__m256i*)(pt+32*3));\
D4 = _mm256_loadu_si256((__m256i*)(pt+32*4));\
D5 = _mm256_loadu_si256((__m256i*)(pt+32*5));\
D6 = _mm256_loadu_si256((__m256i*)(pt+32*6));\
D7 = _mm256_loadu_si256((__m256i*)(pt+32*7));\
D0 = _mm256_permute4x64_epi64( D0, 0xb1);\
D1 = _mm256_permute4x64_epi64( D1, 0xb1);\
D2 = _mm256_permute4x64_epi64( D2, 0xb1);\
D3 = _mm256_permute4x64_epi64( D3, 0xb1);\
D4 = _mm256_permute4x64_epi64( D4, 0xb1);\
D5 = _mm256_permute4x64_epi64( D5, 0xb1);\
D6 = _mm256_permute4x64_epi64( D6, 0xb1);\
D7 = _mm256_permute4x64_epi64( D7, 0xb1);\
bitsliceAVX2(&D0,&D1,&D2,&D3,&D4,&D5,&D6,&D7);

#define BAT128128OutputData256A(E0,E1,E2,E3,E4,E5,E6,E7,out) \
bitsliceAVX2(&E0,&E1,&E2,&E3,&E4,&E5,&E6,&E7);\
_mm256_storeu_si256((__m256i*)(out  ), E0);\
_mm256_storeu_si256((__m256i*)(out+32*1), E1);\
_mm256_storeu_si256((__m256i*)(out+32*2), E2);\
_mm256_storeu_si256((__m256i*)(out+32*3), E3);\
_mm256_storeu_si256((__m256i*)(out+32*4), E4);\
_mm256_storeu_si256((__m256i*)(out+32*5), E5);\
_mm256_storeu_si256((__m256i*)(out+32*6), E6);\
_mm256_storeu_si256((__m256i*)(out+32*7), E7); 


#define BATBsRK_128BSAVX2A(rk,m,x0)\
	A= _mm256_set1_epi64x(x0);\
	*(rk+m) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK0, A) , MSK0); \
	*(rk+m+1) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK1, A) , MSK1); \
	*(rk+m+2) =_mm256_cmpeq_epi8( _mm256_and_si256(MSK2, A) , MSK2); \
	*(rk+m+3) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK3, A) , MSK3); \
	*(rk+m+4) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK4, A) , MSK4); \
	*(rk+m+5) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK5, A) , MSK5); \
	*(rk+m+6) =_mm256_cmpeq_epi8( _mm256_and_si256(MSK6, A) , MSK6); \
	*(rk+m+7) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK7, A) , MSK7); 


void BATGenRK_128_128BSAVX2_32Way(__m256i* rk, const unsigned char *MK)
{
	u64i x0;
	u64i x1;
	u64i k, t, m;
	u64i t0, t1, t2, t3;
	u64i i;
	__m256i A;
	const __m256i MSK0 = _mm256_set1_epi8(0x01);
	const __m256i MSK1 = _mm256_set1_epi8(0x02);
	const __m256i MSK2 = _mm256_set1_epi8(0x04);
	const __m256i MSK3 = _mm256_set1_epi8(0x08);
	const __m256i MSK4 = _mm256_set1_epi8(0x10);
	const __m256i MSK5 = _mm256_set1_epi8(0x20);
	const __m256i MSK6 = _mm256_set1_epi8(0x40);
	const __m256i MSK7 = _mm256_set1_epi8(0x80);

	x1 = (*((u64i*)(MK + 8)));
	x0 = (*((u64i*)(MK)));
	for (i = 0; i < RoundBAT128128 - 2; i += 2)
	{
		BATBsRK_128BSAVX2A(rk, i * 8, (x0));	 x0 ^= (i + 1);
		BATFunA_128B(x1, k, m);		x0 ^= k;
		BATBsRK_128BSAVX2A(rk, i * 8 + 8, (x1)); x1 ^= (i + 2);
		BATFunA_128B(x0, k, m);		x1 ^= k;

	}
	BATBsRK_128BSAVX2A(rk, i * 8, (x0));
	i++;
	BATBsRK_128BSAVX2A(rk, i * 8, (x1));
}

#define FunG0(a03,a00,a2,a3,a0, a1 )\
a00= _mm256_and_si256(a2,a3);\
a00= _mm256_xor_si256(a00,a0);\
a03=  _mm256_and_si256(a00,a1);\
a03= _mm256_xor_si256(a03,a3);


#define FunG1(a11,a12,a1,a3,a2, a0 )\
 a12= _mm256_and_si256(a1,a3);\
a12= _mm256_xor_si256(a12,a2);\
a11=  _mm256_and_si256(a12,a0);\
a11= _mm256_xor_si256(a11,a1);


#define 	BATGfun32WC(a0,a1,a2,a3,a4,a5,a6,a7,        a8,a9,a10,a11,a12,a13,a14,a15,p )\
a8= _mm256_xor_si256(a8,*(p));\
a9= _mm256_xor_si256(a9,*(p+1));\
a10= _mm256_xor_si256(a10,*(p+2));\
a11= _mm256_xor_si256(a11,*(p+3));\
a12= _mm256_xor_si256(a12,*(p+4));\
a13= _mm256_xor_si256(a13,*(p+5));\
a14= _mm256_xor_si256(a14,*(p+6));\
a15= _mm256_xor_si256(a15,*(p+7)); \
FunG0(D3,D0,a2,a3,a0, a1 );\
FunG0(D7,D4,a6,a7,a4, a5 );\
D4= _mm256_xor_si256(a0,D4);\
D7= _mm256_xor_si256(a3,D7);\
FunG1(D1,D2,a1,a3,a2,a0);\
D2= _mm256_xor_si256(a6,D2);\
D1= _mm256_xor_si256(a5,D1);\
FunG1(D5,D6,a5,a7,a6,a4);\
	D4 = _mm256_shuffle_epi8( D4, MSKA);\
	D7 = _mm256_shuffle_epi8( D7, MSKA);\
	D2 = _mm256_shuffle_epi8( D2, MSKA);\
	D1 = _mm256_shuffle_epi8( D1, MSKA);\
	a12=  _mm256_xor_si256(a12,D4);\
	a13=  _mm256_xor_si256(a13,D1);\
	a14=  _mm256_xor_si256(a14,D2);\
	a15=  _mm256_xor_si256(a15,D7);\
	D4 = _mm256_shuffle_epi8( a4 ,MSKB);\
	D1 = _mm256_shuffle_epi8( a1, MSKA);\
	D2 = _mm256_shuffle_epi8( a2, MSKA);\
	D7 = _mm256_shuffle_epi8( a7, MSKB);\
	D0 = _mm256_shuffle_epi8( D0, MSKA);\
	D5 = _mm256_shuffle_epi8( D5, MSKB);\
	D6 = _mm256_shuffle_epi8( D6, MSKB);\
	D3 = _mm256_shuffle_epi8( D3, MSKA);\
	a8=  _mm256_xor_si256(a8,D4);\
	a9=  _mm256_xor_si256(a9,D1);\
	a10=  _mm256_xor_si256(a10,D2);\
	a11=  _mm256_xor_si256(a11,D7);	\
	a8=  _mm256_xor_si256(a8,D0);\
	a9=  _mm256_xor_si256(a9,D5);\
	a10=  _mm256_xor_si256(a10,D6);\
	a11=  _mm256_xor_si256(a11,D3);	


#define TwoRoundsBAT128128_64Way(E0,E1,E2,E3,E4,E5,E6,E7, E8,E9,E10,E11,E12,E13,E14,E15)\
	BATGfun32WC(E0,E1,E2,E3,E4,E5,E6,E7,        E8,E9,E10,E11,E12,E13,E14,E15,p );  \
	BATGfun32WC(E8,E9,E10,E11,E12,E13,E14,E15,  E0,E1,E2,E3,E4,E5,E6,E7      , p+8 );

void BATEnc_128_128BSAVX2_32Way(u8i *out, u8i *pt, __m256i * rk)
{
	__m256i *p = (__m256i *)rk;
	__m256i *rkend = p + RoundBAT128128 * 8;
	const __m256i MSKA = _mm256_load_si256((__m256i*) Lbswap_maskA);
	const __m256i MSKB = _mm256_load_si256((__m256i*) Lbswap_maskB);
	__m256i D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15;
	__m256i E0, E1, E2, E3, E4, E5, E6, E7, E8, E9, E10, E11, E12, E13, E14, E15;
	BAT128128InputData256A(D0, D1, D2, D3, D4, D5, D6, D7, pt); //bit slicing
	BAT128128InputData256A(D8, D9, D10, D11, D12, D13, D14, D15, pt + 256); //bit slicing  
	E0 = _mm256_unpacklo_epi64(D0, D8);	    E8 = _mm256_unpackhi_epi64(D0, D8);
	E1 = _mm256_unpacklo_epi64(D1, D9);	    E9 = _mm256_unpackhi_epi64(D1, D9);
	E2 = _mm256_unpacklo_epi64(D2, D10);	E10 = _mm256_unpackhi_epi64(D2, D10);
	E3 = _mm256_unpacklo_epi64(D3, D11);	E11 = _mm256_unpackhi_epi64(D3, D11);
	E4 = _mm256_unpacklo_epi64(D4, D12);	E12 = _mm256_unpackhi_epi64(D4, D12);
	E5 = _mm256_unpacklo_epi64(D5, D13);	E13 = _mm256_unpackhi_epi64(D5, D13);
	E6 = _mm256_unpacklo_epi64(D6, D14);	E14 = _mm256_unpackhi_epi64(D6, D14);
	E7 = _mm256_unpacklo_epi64(D7, D15);	E15 = _mm256_unpackhi_epi64(D7, D15);

	while (p < rkend)
	{
		TwoRoundsBAT128128_64Way(E0, E1, E2, E3, E4, E5, E6, E7, E8, E9, E10, E11, E12, E13, E14, E15);
		p += 16;
	}

	D0 = _mm256_unpacklo_epi64(E0, E8);	    D8 = _mm256_unpackhi_epi64(E0, E8);
	D1 = _mm256_unpacklo_epi64(E1, E9);	    D9 = _mm256_unpackhi_epi64(E1, E9);
	D2 = _mm256_unpacklo_epi64(E2, E10);	D10 = _mm256_unpackhi_epi64(E2, E10);
	D3 = _mm256_unpacklo_epi64(E3, E11);	D11 = _mm256_unpackhi_epi64(E3, E11);
	D4 = _mm256_unpacklo_epi64(E4, E12);	D12 = _mm256_unpackhi_epi64(E4, E12);
	D5 = _mm256_unpacklo_epi64(E5, E13);	D13 = _mm256_unpackhi_epi64(E5, E13);
	D6 = _mm256_unpacklo_epi64(E6, E14);	D14 = _mm256_unpackhi_epi64(E6, E14);
	D7 = _mm256_unpacklo_epi64(E7, E15);	D15 = _mm256_unpackhi_epi64(E7, E15);
	BAT128128OutputData256A(D0, D1, D2, D3, D4, D5, D6, D7, out);
	BAT128128OutputData256A(D8, D9, D10, D11, D12, D13, D14, D15, out + 256);

}

int crypto_stream_BAT128128ecb_avx2_32Way(
	unsigned char *out,
	unsigned char *in,
	unsigned long long inlen,
	const unsigned char *k
)
{
	__m256i rk[RoundBAT128128 * 8];

	if (!inlen) return 0;

	BATGenRK_128_128BSAVX2_32Way(rk, k);

	while (inlen >= 512) {
		BATEnc_128_128BSAVX2_32Way(out, in, rk);
		inlen -= 512;
		in += 512;
		out += 512;
	}

	return 0;
}

void TestBAT128128BSAVX2_32Way()
{
	int i, j;
	unsigned char MK[16] = { 0 };
	unsigned char *pt;
	unsigned char *out;
	u64i inlen = TOTALLEN;
	int cloop = ENCLOOPS;
	u64i cyclea, cycleb;
	u64i cp, cpb, cpc;
	pt = _mm_malloc(inlen,32);
	out = _mm_malloc(inlen,32);
	for (i = 0; i < 16; i++)
		MK[i] = 0x10 + i;
	for (i = 0; i < 16; i++)
		pt[i] = 0x0f + 0x10 * (i);
	for (i = 16; i < 512; i++)
		pt[i] = i % 256;


	for (i = 0; i < 512; i++)
	{
		printf("%02x", pt[i]);
		if ((i + 1) % LMAXCNT == 0)  printf("\n");
		else if ((i + 1) % 16 == 0) printf(" ");
	}
	printf("----------------------------------------\n");

	crypto_stream_BAT128128ecb_avx2_32Way(out, pt, 512, MK);
	for (i = 0; i < 512; i++)
	{
		printf("%02x", out[i]);
		if ((i + 1) % LMAXCNT == 0)  printf("\n");
		else if ((i + 1) % 16 == 0) printf(" ");
	}
	printf("\n");
	cp = 0;
	for (i = 0; i < cloop; i++)
	{
		srand((unsigned)time(NULL));
		for (j = 0; j < 16; j++)
			MK[j] = rand() % 256;
		for (j = 0; j < inlen; j++)
			pt[j] = rand() % 256;
		grdtscl(&cyclea);
		crypto_stream_BAT128128ecb_avx2_32Way(out, pt, inlen, MK);
		grdtscl(&cycleb);
		cp += (cycleb - cyclea);
	}
	cp = cp / inlen;
	cpb = cp / (cloop);
	cpc = (cp - cpb * cloop) * 100 / cloop;
	printf("BAT 128-128 32-way Enc cpb: %ld.%02ld\n", cpb, cpc);
	_mm_free(pt);
	_mm_free(out);
}


