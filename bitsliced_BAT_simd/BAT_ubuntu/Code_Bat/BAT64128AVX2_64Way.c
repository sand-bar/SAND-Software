#include "BATTypeDef.h" 
/*
BAT 64-128 avx2  64 way

*/

static const unsigned char _C_ALIGN32(Lbswap_maskA[32]) = {
2,3,0,1, 6,7, 4,5, 10,11, 8,9, 14,15, 12,13, 2,3,0,1, 6,7, 4,5, 10,11, 8,9, 14,15, 12,13,
};

static const unsigned char _C_ALIGN32(Lbswap_maskB[32]) = {
1,2,3,0, 5,6,7, 4, 9,10,11, 8, 13,14,15, 12,  1,2,3,0, 5,6,7, 4, 9,10,11, 8, 13,14,15, 12,
};


#define BATBsRK_64AVX2A(rk,m,x0)\
	A= _mm256_set1_epi32(x0);\
	*(rk+m) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK0, A) , MSK0); \
	*(rk+m+1) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK1, A) , MSK1); \
	*(rk+m+2) =_mm256_cmpeq_epi8( _mm256_and_si256(MSK2, A) , MSK2); \
	*(rk+m+3) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK3, A) , MSK3); \
	*(rk+m+4) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK4, A) , MSK4); \
	*(rk+m+5) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK5, A) , MSK5); \
	*(rk+m+6) =_mm256_cmpeq_epi8( _mm256_and_si256(MSK6, A) , MSK6); \
	*(rk+m+7) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK7, A) , MSK7);


void BATGenRK_64_128BSAVX2_64Way(__m256i * rk, const u8i *MK)
{
	u32i x0;
	u32i x1;
	u32i x2;
	u32i x3;
	u32i t, k, m;
	u32i t0, t1, t2, t3;
	u32i i;
	__m256i A;
	const __m256i MSK0 = _mm256_set1_epi8(0x01);
	const __m256i MSK1 = _mm256_set1_epi8(0x02);
	const __m256i MSK2 = _mm256_set1_epi8(0x04);
	const __m256i MSK3 = _mm256_set1_epi8(0x08);
	const __m256i MSK4 = _mm256_set1_epi8(0x10);
	const __m256i MSK5 = _mm256_set1_epi8(0x20);
	const __m256i MSK6 = _mm256_set1_epi8(0x40);
	const __m256i MSK7 = _mm256_set1_epi8(0x80);
	x3 = (*((u32i*)(MK + 12)));
	x2 = (*((u32i*)(MK + 8)));
	x1 = (*((u32i*)(MK + 4)));
	x0 = (*((u32i*)(MK)));
	for (i = 0; i < RoundBAT64128 - 4; i += 4)
	{
		BATBsRK_64AVX2A(rk, i * 8, (x0));	x0 ^= (i + 1);
		BATFunA_64B(x3, k, m);  x0 ^= k;

		BATBsRK_64AVX2A(rk, i * 8 + 8, (x1)); x1 ^= (i + 2);
		BATFunA_64B(x0, k, m);  x1 ^= k;

		BATBsRK_64AVX2A(rk, i * 8 + 16, (x2));  x2 ^= (i + 3);
		BATFunA_64B(x1, k, m);  x2 ^= k;

		BATBsRK_64AVX2A(rk, i * 8 + 24, (x3));  x3 ^= (i + 4);
		BATFunA_64B(x2, k, m); x3 ^= k;
	}
	BATBsRK_64AVX2A(rk, i * 8, (x0));
	i++;
	BATBsRK_64AVX2A(rk, i * 8, (x1));
	i++;
	BATBsRK_64AVX2A(rk, i * 8, (x2));
	i++;
	BATBsRK_64AVX2A(rk, i * 8, (x3));
}


#define BAT64128InputData256A(D0,D1,D2,D3,D4,D5,D6,D7,pt)\
	D0 = _mm256_loadu_si256((__m256i*)(pt));\
	D1 = _mm256_loadu_si256((__m256i*)(pt+32));\
	D2 = _mm256_loadu_si256((__m256i*)(pt+32*2));\
	D3 = _mm256_loadu_si256((__m256i*)(pt+32*3));\
	D4 = _mm256_loadu_si256((__m256i*)(pt+32*4));\
	D5 = _mm256_loadu_si256((__m256i*)(pt+32*5));\
	D6 = _mm256_loadu_si256((__m256i*)(pt+32*6));\
	D7 = _mm256_loadu_si256((__m256i*)(pt+32*7));\
	bitsliceAVX2(&D0,&D1,&D2,&D3,&D4,&D5,&D6,&D7);   

#define BAT64128OutputData256A(E0,E1,E2,E3,E4,E5,E6,E7,out) \
	bitsliceAVX2(&E0,&E1,&E2,&E3,&E4,&E5,&E6,&E7);\
	_mm256_storeu_si256((__m256i*)(out  ), E0);\
	_mm256_storeu_si256((__m256i*)(out+32*1), E1);\
	_mm256_storeu_si256((__m256i*)(out+32*2), E2);\
	_mm256_storeu_si256((__m256i*)(out+32*3), E3);\
	_mm256_storeu_si256((__m256i*)(out+32*4), E4);\
	_mm256_storeu_si256((__m256i*)(out+32*5), E5);\
	_mm256_storeu_si256((__m256i*)(out+32*6), E6);\
	_mm256_storeu_si256((__m256i*)(out+32*7), E7);  


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
 
#define 	BATGfun64W(a0,a1,a2,a3,a4,a5,a6,a7,        a8,a9,a10,a11,a12,a13,a14,a15,p )\
a8= _mm256_xor_si256(a8,*(p));\
a9= _mm256_xor_si256(a9,*(p+1));\
a10= _mm256_xor_si256(a10,*(p+2));\
a11= _mm256_xor_si256(a11,*(p+3));\
a12= _mm256_xor_si256(a12,*(p+4));\
a13= _mm256_xor_si256(a13,*(p+5));\
a14= _mm256_xor_si256(a14,*(p+6));\
a15= _mm256_xor_si256(a15,*(p+7));\
FunG0(E7,E4,a6,a7,a4, a5 );\
E4= _mm256_xor_si256(E4,a0);\
E7= _mm256_xor_si256(E7,a3);\
FunG1(E1,E2,a1,a3,a2,a0);\
E2= _mm256_xor_si256(E2,a6);\
E1= _mm256_xor_si256(E1,a5);\
	E4 = _mm256_shuffle_epi8( E4, MSKA);\
	E1 = _mm256_shuffle_epi8( E1, MSKA);\
	E2 = _mm256_shuffle_epi8( E2, MSKA);\
	E7 = _mm256_shuffle_epi8( E7, MSKA);\
	a8=  _mm256_xor_si256(a8,E4);\
	a9=  _mm256_xor_si256(a9,E1);\
	a10=  _mm256_xor_si256(a10,E2);\
	a11=  _mm256_xor_si256(a11,E7);\
FunG0(E3,E0,a2,a3,a0, a1 );\
FunG1(E5,E6,a5,a7,a6,a4);\
	a12=  _mm256_xor_si256(a12,a4);\
	a13=  _mm256_xor_si256(a13,E5);\
	a14=  _mm256_xor_si256(a14,E6);\
	a15=  _mm256_xor_si256(a15,a7);\
	E0 = _mm256_shuffle_epi8( E0 ,MSKB);\
	E1 = _mm256_shuffle_epi8( a1, MSKB);\
	E2 = _mm256_shuffle_epi8( a2, MSKB);\
	E3 = _mm256_shuffle_epi8( E3, MSKB);\
	a12=  _mm256_xor_si256(a12,E0);\
	a13=  _mm256_xor_si256(a13,E1);\
	a14=  _mm256_xor_si256(a14,E2);\
	a15=  _mm256_xor_si256(a15,E3); 



#define TwoRoundsBAT64128_64Way(D0,D1,D2,D3,D4,D5,D6,D7, D8,D9,D10,D11,D12,D13,D14,D15)\
	BATGfun64W(D0,D1,D2,D3,D4,D5,D6,D7,        D8,D9,D10,D11,D12,D13,D14,D15,p );  \
	BATGfun64W(D8,D9,D10,D11,D12,D13,D14,D15,  D0,D1,D2,D3,D4,D5,D6,D7      , p+8 );


void BATEnc_64_128BSAVX2_64Way(u8i *out, u8i *pt, __m256i * rk)
{
	__m256i *p = (__m256i *)rk;
	__m256i *rkend = p + RoundBAT64128 * 8;
	const __m256i MSKA = _mm256_load_si256((__m256i*) Lbswap_maskA);
	const __m256i MSKB = _mm256_load_si256((__m256i*) Lbswap_maskB);

	__m256i D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15;
	__m256i E0, E1, E2, E3, E4, E5, E6, E7, E8, E9, E10, E11, E12, E13, E14, E15;

	BAT64128InputData256A(D0, D1, D2, D3, D4, D5, D6, D7, pt); //bit slicing
	BAT64128InputData256A(D8, D9, D10, D11, D12, D13, D14, D15, pt + 256); //bit slicing  

	E0 = _mm256_unpacklo_epi32(D0, D8);	    E8 = _mm256_unpackhi_epi32(D0, D8);
	E1 = _mm256_unpacklo_epi32(D1, D9);	    E9 = _mm256_unpackhi_epi32(D1, D9);
	E2 = _mm256_unpacklo_epi32(D2, D10);	E10 = _mm256_unpackhi_epi32(D2, D10);
	E3 = _mm256_unpacklo_epi32(D3, D11);	E11 = _mm256_unpackhi_epi32(D3, D11);
	E4 = _mm256_unpacklo_epi32(D4, D12);	E12 = _mm256_unpackhi_epi32(D4, D12);
	E5 = _mm256_unpacklo_epi32(D5, D13);	E13 = _mm256_unpackhi_epi32(D5, D13);
	E6 = _mm256_unpacklo_epi32(D6, D14);	E14 = _mm256_unpackhi_epi32(D6, D14);
	E7 = _mm256_unpacklo_epi32(D7, D15);	E15 = _mm256_unpackhi_epi32(D7, D15);

	D0 = _mm256_unpackhi_epi32(E0, E8);	    D8 = _mm256_unpacklo_epi32(E0, E8);
	D1 = _mm256_unpackhi_epi32(E1, E9);	    D9 = _mm256_unpacklo_epi32(E1, E9);
	D2 = _mm256_unpackhi_epi32(E2, E10);	D10 = _mm256_unpacklo_epi32(E2, E10);
	D3 = _mm256_unpackhi_epi32(E3, E11);	D11 = _mm256_unpacklo_epi32(E3, E11);
	D4 = _mm256_unpackhi_epi32(E4, E12);	D12 = _mm256_unpacklo_epi32(E4, E12);
	D5 = _mm256_unpackhi_epi32(E5, E13);	D13 = _mm256_unpacklo_epi32(E5, E13);
	D6 = _mm256_unpackhi_epi32(E6, E14);	D14 = _mm256_unpacklo_epi32(E6, E14);
	D7 = _mm256_unpackhi_epi32(E7, E15);	D15 = _mm256_unpacklo_epi32(E7, E15);
	//d0 ~d7   left  d8 ~ d15  right  
	while (p < rkend)
	{
		TwoRoundsBAT64128_64Way(D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15);

		p += 16;
	}

	E0 = _mm256_unpacklo_epi32(D0, D8);	E8 = _mm256_unpackhi_epi32(D0, D8);
	E1 = _mm256_unpacklo_epi32(D1, D9);	E9 = _mm256_unpackhi_epi32(D1, D9);
	E2 = _mm256_unpacklo_epi32(D2, D10);	E10 = _mm256_unpackhi_epi32(D2, D10);
	E3 = _mm256_unpacklo_epi32(D3, D11);	E11 = _mm256_unpackhi_epi32(D3, D11);
	E4 = _mm256_unpacklo_epi32(D4, D12);	E12 = _mm256_unpackhi_epi32(D4, D12);
	E5 = _mm256_unpacklo_epi32(D5, D13);	E13 = _mm256_unpackhi_epi32(D5, D13);
	E6 = _mm256_unpacklo_epi32(D6, D14);	E14 = _mm256_unpackhi_epi32(D6, D14);
	E7 = _mm256_unpacklo_epi32(D7, D15);	E15 = _mm256_unpackhi_epi32(D7, D15);
	BAT64128OutputData256A(E0, E1, E2, E3, E4, E5, E6, E7, out);
	BAT64128OutputData256A(E8, E9, E10, E11, E12, E13, E14, E15, out + 256);
}

int crypto_stream_BAT64128ecb_avx2_64Way(
	unsigned char *out,
	unsigned char *in,
	unsigned long long inlen,
	const unsigned char *k
)
{
	__m256i rk[RoundBAT64128 * 8];


	if (!inlen) return 0;

	BATGenRK_64_128BSAVX2_64Way(rk, k);

	while (inlen >= 512) {
		BATEnc_64_128BSAVX2_64Way(out, in, rk);
		inlen -= 512;
		in += 512;
		out += 512;
	}

	return 0;
}


void TestBAT64128BSAVX2_64Way()
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

	crypto_stream_BAT64128ecb_avx2_64Way(out, pt, 512, MK);
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
		crypto_stream_BAT64128ecb_avx2_64Way(out, pt, inlen, MK);
		grdtscl(&cycleb);
		cp += (cycleb - cyclea);
	}
	cp = cp / inlen;
	cpb = cp / (cloop);
	cpc = (cp - cpb * cloop) * 100 / cloop;
	printf("BAT 64-128 64-Way Enc cpb: %ld.%02ld\n", cpb, cpc);
	_mm_free(pt);
	_mm_free(out);
}

