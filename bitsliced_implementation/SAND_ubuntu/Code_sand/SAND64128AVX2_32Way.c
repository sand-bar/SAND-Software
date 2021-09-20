#include "SANDTypeDef.h" 

/*
SAND 64-128 avx2  32 way

*/

static  const unsigned char _C_ALIGN32(Lbswap_maskD[32]) = {
5,2,7,4,1,6,3,0, 13,10,15,12,9,14,11,8, 5,2,7,4,1,6,3,0, 13,10,15,12,9,14,11,8,
};
static  const unsigned char _C_ALIGN32(Lbswap_maskE[32]) = {
4,1,6,3,0,5,2,7, 12,9,14,11,8,13,10,15,4,1,6,3,0,5,2,7, 12,9,14,11,8,13,10,15,
};
static  const unsigned char _C_ALIGN32(Lbswap_maskF[32]) = {
0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15,0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15,
};

static  const unsigned char _C_ALIGN32(Lbswap_mask4[32]) = {
 4,4,5,5,6,6,7,7,0,0,1,1,2,2,3,3,  4,4,5,5,6,6,7,7,0,0,1,1,2,2,3,3,
};

#define SANDBsRK_64AVX2(rk,m,x0)\
	A= _mm256_set1_epi32(x0);\
    A= _mm256_shuffle_epi8( A, MSK4) ;\
	*(rk+m) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK0, A) , MSK0); \
	*(rk+m+1) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK1, A) , MSK1); \
	*(rk+m+2) =_mm256_cmpeq_epi8( _mm256_and_si256(MSK2, A) , MSK2); \
	*(rk+m+3) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK3, A) , MSK3); 


void SANDGenRK_64_128BSAVX2(__m256i * rk, const u8i *MK)
{
	u32i x0;
	u32i x1;
	u32i x2;
	u32i x3;
	u32i t0, t1, t2, t3;
	u32i  k, m;
	u32i i;
	__m256i A;
	const __m256i MSK0 = _mm256_set1_epi16(0x1001);
	const __m256i MSK1 = _mm256_set1_epi16(0x2002);
	const __m256i MSK2 = _mm256_set1_epi16(0x4004);
	const __m256i MSK3 = _mm256_set1_epi16(0x8008);
	const __m256i MSK4 = _mm256_load_si256((__m256i*) Lbswap_mask4);
	x3 = (*((u32i*)(MK + 12)));
	x2 = (*((u32i*)(MK + 8)));
	x1 = (*((u32i*)(MK + 4)));
	x0 = (*((u32i*)(MK)));
	for (i = 0; i < RoundSAND64128 - 4; i += 4)
	{


		SANDBsRK_64AVX2(rk, i * 4, (x0));	x0 ^= (i + 1);
		SANDFunA_64B(x3, k, m);  x0 ^= k;

		SANDBsRK_64AVX2(rk, i * 4 + 4, (x1)); x1 ^= (i + 2);
		SANDFunA_64B(x0, k, m);  x1 ^= k;

		SANDBsRK_64AVX2(rk, i * 4 + 8, (x2));  x2 ^= (i + 3);
		SANDFunA_64B(x1, k, m);  x2 ^= k;

		SANDBsRK_64AVX2(rk, i * 4 + 12, (x3));  x3 ^= (i + 4);
		SANDFunA_64B(x2, k, m); x3 ^= k;
	}
	SANDBsRK_64AVX2(rk, i * 4, (x0));
	i++;
	SANDBsRK_64AVX2(rk, i * 4, (x1));
	i++;
	SANDBsRK_64AVX2(rk, i * 4, (x2));
	i++;
	SANDBsRK_64AVX2(rk, i * 4, (x3));
}

#define SAND64128InputData256(D0,D1,D2,D3,D4,D5,D6,D7,pt)\
	D0 = _mm256_loadu_si256((__m256i*)(pt));\
	D1 = _mm256_loadu_si256((__m256i*)(pt+32));\
	D2 = _mm256_loadu_si256((__m256i*)(pt+32*2));\
	D3 = _mm256_loadu_si256((__m256i*)(pt+32*3));\
	D4 = _mm256_loadu_si256((__m256i*)(pt+32*4));\
	D5 = _mm256_loadu_si256((__m256i*)(pt+32*5));\
	D6 = _mm256_loadu_si256((__m256i*)(pt+32*6));\
	D7 = _mm256_loadu_si256((__m256i*)(pt+32*7));\
	bitsliceAVX2(&D0,&D1,&D2,&D3,&D4,&D5,&D6,&D7);\
	E0=  _mm256_unpacklo_epi8(D0, D4);\
	E4=  _mm256_unpackhi_epi8(D0, D4);\
	E1=  _mm256_unpacklo_epi8(D1, D5);\
	E5=  _mm256_unpackhi_epi8(D1, D5);\
	E2=  _mm256_unpacklo_epi8(D2, D6);\
	E6=  _mm256_unpackhi_epi8(D2, D6);\
	E3=  _mm256_unpacklo_epi8(D3, D7);\
	E7=  _mm256_unpackhi_epi8(D3, D7); \
	\
	D4=  _mm256_unpacklo_epi64(E0, E4);\
	D0=  _mm256_unpackhi_epi64(E0, E4);\
	D5=  _mm256_unpacklo_epi64(E1, E5);\
	D1=  _mm256_unpackhi_epi64(E1, E5);\
	D6=  _mm256_unpacklo_epi64(E2, E6);\
	D2=  _mm256_unpackhi_epi64(E2, E6);\
	D7=  _mm256_unpacklo_epi64(E3, E7);\
	D3=  _mm256_unpackhi_epi64(E3, E7); 


#define SAND64128OutputData256(D0,D1,D2,D3,D4,D5,D6,D7,out) \
	D0 = _mm256_shuffle_epi8( D0, MSKF);\
	D1 = _mm256_shuffle_epi8( D1, MSKF);\
	D2 = _mm256_shuffle_epi8( D2, MSKF);\
	D3 = _mm256_shuffle_epi8( D3, MSKF);\
	D4 = _mm256_shuffle_epi8( D4 ,MSKF);\
	D5 = _mm256_shuffle_epi8( D5, MSKF);\
	D6 = _mm256_shuffle_epi8( D6, MSKF);\
	D7 = _mm256_shuffle_epi8( D7, MSKF);\
	\
	E0=  _mm256_unpacklo_epi32(D0, D4);\
	E4=  _mm256_unpackhi_epi32(D0, D4);\
	E1=  _mm256_unpacklo_epi32(D1, D5);\
	E5=  _mm256_unpackhi_epi32(D1, D5);\
	E2=  _mm256_unpacklo_epi32(D2, D6);\
	E6=  _mm256_unpackhi_epi32(D2, D6);\
	E3=  _mm256_unpacklo_epi32(D3, D7);\
	E7=  _mm256_unpackhi_epi32(D3, D7); \
	\
	bitsliceAVX2(&E0,&E1,&E2,&E3,&E4,&E5,&E6,&E7);\
	_mm256_storeu_si256((__m256i*)(out  ), E0);\
	_mm256_storeu_si256((__m256i*)(out+32*1), E1);\
	_mm256_storeu_si256((__m256i*)(out+32*2), E2);\
	_mm256_storeu_si256((__m256i*)(out+32*3), E3);\
	_mm256_storeu_si256((__m256i*)(out+32*4), E4);\
	_mm256_storeu_si256((__m256i*)(out+32*5), E5);\
	_mm256_storeu_si256((__m256i*)(out+32*6), E6);\
	_mm256_storeu_si256((__m256i*)(out+32*7), E7); 	

 
#define  SANDGfun32W(b0,b1,b2,b3,a0,  a1, a2, a3,p,a00,a03,a12,a11) \
a00= _mm256_and_si256(a2,a3);\
a00= _mm256_xor_si256(a0,a00);\
a03= _mm256_and_si256(a1,a00);\
a03= _mm256_xor_si256(a3,a03);\
a12= _mm256_and_si256(a1,a3);\
a12= _mm256_xor_si256(a2,a12);\
a11= _mm256_and_si256(a0,a12);\
a11= _mm256_xor_si256(a1,a11);\
b0= _mm256_xor_si256(b0,*(p));\
b1= _mm256_xor_si256(b1,*(p+1));\
b2= _mm256_xor_si256(b2,*(p+2));\
b3= _mm256_xor_si256(b3,*(p+3)); \
b0= _mm256_xor_si256(b0, _mm256_shuffle_epi8( a0, MSKx) );\
b1= _mm256_xor_si256(b1, _mm256_shuffle_epi8( a1, MSKz) );\
b2= _mm256_xor_si256(b2, _mm256_shuffle_epi8( a2, MSKz)  );\
b3= _mm256_xor_si256(b3,_mm256_shuffle_epi8( a3, MSKx));\
b0= _mm256_xor_si256(  _mm256_shuffle_epi8( a00, MSKz), b0 );\
b1= _mm256_xor_si256(  _mm256_shuffle_epi8( a11, MSKx), b1 );\
b2= _mm256_xor_si256(  _mm256_shuffle_epi8( a12, MSKx), b2);\
b3= _mm256_xor_si256( _mm256_shuffle_epi8( a03, MSKz) , b3 );

#define TwoRoundsSAND(D0,D1,D2,D3,D4,D5,D6,D7)\
	SANDGfun32W(D4,D5,D6,D7,D0,D1,D2,D3,p,E0,E1,E2,E3);  \
	SANDGfun32W(D0,D1,D2,D3,D4,D5,D6,D7,p+4,E0,E1,E2,E3);

void SANDEnc_64_128BSAVX2(u8i *out, u8i *pt, __m256i * rk)
{
	__m256i *p = (__m256i *)rk;
	__m256i *rkend = p + RoundSAND64128 * 4;
	const __m256i MSKz = _mm256_load_si256((__m256i*) Lbswap_maskD);
	const __m256i MSKx = _mm256_load_si256((__m256i*) Lbswap_maskE);
	const __m256i MSKF = _mm256_load_si256((__m256i*) Lbswap_maskF);
	__m256i D0, D1, D2, D3, D4, D5, D6, D7;
	__m256i E0, E1, E2, E3, E4, E5, E6, E7;

	SAND64128InputData256(D0, D1, D2, D3, D4, D5, D6, D7, pt);

	while (p < rkend)
	{
		TwoRoundsSAND(D0, D1, D2, D3, D4, D5, D6, D7);

		p += 8;
	}

	SAND64128OutputData256(D0, D1, D2, D3, D4, D5, D6, D7, out);
}


int crypto_stream_SAND64128ecb_avx2_32Way(
	unsigned char *out,
	unsigned char *in,
	unsigned long long inlen,
	const unsigned char *k
)
{
	__m256i rk[RoundSAND64128 * 4];


	if (!inlen) return 0;

	SANDGenRK_64_128BSAVX2(rk, k);

	while (inlen >= 256) {
		SANDEnc_64_128BSAVX2(out, in, rk);
		inlen -= 256;
		in += 256;
		out += 256;
	}

	return 0;
}


void TestSAND64128BSAVX2_32Way()
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
		MK[i] = 0x10 * (15 - i) + 0x0f;
	for (i = 0; i < 8; i++)
		pt[i] = 0x0f + 0x10 * (7 - i);
	for (i = 8; i < 512; i++)
		pt[i] = i % 256;
	for (i = 0; i < 512; i++)
	{
		printf("%02x", pt[i]);
		if ((i + 1) % LMAXCNT == 0)  printf("\n");
		else if ((i + 1) % 8 == 0) printf(" ");
	}
	printf("----------------------------------------\n");
	crypto_stream_SAND64128ecb_avx2_32Way(out, pt, 512, MK);
	for (i = 0; i < 512; i++)
	{
		printf("%02x", out[i]);
		if ((i + 1) % LMAXCNT == 0)  printf("\n");
		else if ((i + 1) % 8 == 0) printf(" ");
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
		crypto_stream_SAND64128ecb_avx2_32Way(out, pt, inlen, MK);
		grdtscl(&cycleb);
		cp += (cycleb - cyclea);
	}
	cp = cp / inlen;
	cpb = cp / (cloop);
	cpc = (cp - cpb * cloop) * 100 / cloop;
	printf("SAND 64-128 32-way Enc cpb: %ld.%02ld\n", cpb, cpc);
	_mm_free(pt);
	_mm_free(out);
}


