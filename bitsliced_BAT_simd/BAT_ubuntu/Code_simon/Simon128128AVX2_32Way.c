#include "SimonTypeDef.h" 
/*
Simon  128-128 avx2  32 way 
 
*/
static const unsigned char _C_ALIGN32(Lbswap_maskA[32] ) ={
7, 0,1,2,3,4, 5,6,  15, 8,9,10,11, 12, 13,14,  
7, 0,1,2,3,4, 5,6,  15, 8,9,10,11, 12, 13,14,   
};  
 
 static const u64i ConstRK128[66]={
0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,
0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,
0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,
0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,
0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,
0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,
0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,
0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFC,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,
0xFFFFFFFFFFFFFFFD,0xFFFFFFFFFFFFFFFC,
}; 
#define Simon128128InputData256A(D0,D1,D2,D3,D4,D5,D6,D7,pt)\
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

#define Simon128128OutputData256A(E0,E1,E2,E3,E4,E5,E6,E7,out) \
bitsliceAVX2(&E0,&E1,&E2,&E3,&E4,&E5,&E6,&E7);\
E0 = _mm256_permute4x64_epi64( E0, 0xb1);\
E1 = _mm256_permute4x64_epi64( E1, 0xb1);\
E2 = _mm256_permute4x64_epi64( E2, 0xb1);\
E3 = _mm256_permute4x64_epi64( E3, 0xb1);\
E4 = _mm256_permute4x64_epi64( E4, 0xb1);\
E5 = _mm256_permute4x64_epi64( E5, 0xb1);\
E6 = _mm256_permute4x64_epi64( E6, 0xb1);\
E7 = _mm256_permute4x64_epi64( E7, 0xb1);\
_mm256_storeu_si256((__m256i*)(out  ), E0);\
_mm256_storeu_si256((__m256i*)(out+32*1), E1);\
_mm256_storeu_si256((__m256i*)(out+32*2), E2);\
_mm256_storeu_si256((__m256i*)(out+32*3), E3);\
_mm256_storeu_si256((__m256i*)(out+32*4), E4);\
_mm256_storeu_si256((__m256i*)(out+32*5), E5);\
_mm256_storeu_si256((__m256i*)(out+32*6), E6);\
_mm256_storeu_si256((__m256i*)(out+32*7), E7); 
 

  #define SimonBsRK_128BSAVX2A(rk,m,x0)\
	A= _mm256_set1_epi64x(x0);\
	*(rk+m) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK0, A) , MSK0); \
	*(rk+m+1) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK1, A) , MSK1); \
	*(rk+m+2) =_mm256_cmpeq_epi8( _mm256_and_si256(MSK2, A) , MSK2); \
	*(rk+m+3) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK3, A) , MSK3); \
	*(rk+m+4) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK4, A) , MSK4); \
	*(rk+m+5) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK5, A) , MSK5); \
	*(rk+m+6) =_mm256_cmpeq_epi8( _mm256_and_si256(MSK6, A) , MSK6); \
	*(rk+m+7) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK7, A) , MSK7); 
 
 

void SimonGenRK_128_128BSAVX2_32Way( __m256i* rk, const unsigned char *key )
{
	u64i subkey0, subkey1;
	u64i  i ; 
	u64i  tmp1,tmp2; 
    __m256i A;
    
	const __m256i MSK0 =_mm256_set1_epi8(0x01);
	const __m256i MSK1 =_mm256_set1_epi8(0x02);
	const __m256i MSK2 =_mm256_set1_epi8(0x04);
	const __m256i MSK3 =_mm256_set1_epi8(0x08);  
	const __m256i MSK4 =_mm256_set1_epi8(0x10);
	const __m256i MSK5 =_mm256_set1_epi8(0x20);
	const __m256i MSK6 =_mm256_set1_epi8(0x40);
	const __m256i MSK7 =_mm256_set1_epi8(0x80);  
     memcpy( &subkey0 , key , 8);
	 memcpy( &subkey1 , key+8 , 8);
          SimonBsRK_128BSAVX2A(rk, 0, subkey0);
    for ( i = 0; i < RoundSimon128128 - 2; i+=2) {	
          SimonBsRK_128BSAVX2A(rk,(i+1)*8, subkey1); 
    RKFun128Const( subkey0, subkey1,i,ConstRK128);
         SimonBsRK_128BSAVX2A(rk,(i+2)*8, subkey0); 
    RKFun128Const( subkey1, subkey0,i+1,ConstRK128);
    }  	
    SimonBsRK_128BSAVX2A(rk,(i+1)*8, subkey1);  
} 	 
	  

  #define 	SimonGfun32WC(a0,a1,a2,a3,a4,a5,a6,a7,        a8,a9,a10,a11,a12,a13,a14,a15,p )\
D6= _mm256_shuffle_epi8(a6,MSKA);\
D7= _mm256_shuffle_epi8(a7,MSKA);\
D0 = _mm256_shuffle_epi8( a0 ,MSKA);\
	a8=   XOR4(a8,D7,D0,D6);\
	a14=  XOR4(a14,a5,D6,a4);\
	a15=  XOR4(a15,a6,D7,a5);\
	D1 = _mm256_shuffle_epi8( a1, MSKA);\
	D2 = _mm256_shuffle_epi8( a2, MSKA);\
	D3 = _mm256_shuffle_epi8( a3, MSKA);\
	a9=  XOR4(a9,a0,D1,D7);\
	a10=  XOR4(a10,a1,D2,a0);\
	a11=  XOR4(a11,a2,D3,a1);\
D4= _mm256_shuffle_epi8(a4,MSKA);\
D5= _mm256_shuffle_epi8(a5,MSKA);\
	a12=  XOR4(a12,a3,D4,a2);\
	a13=  XOR4(a13,a4,D5,a3);\
a8= _mm256_xor_si256(a8,*(p));\
a9= _mm256_xor_si256(a9,*(p+1));\
a10= _mm256_xor_si256(a10,*(p+2));\
a11= _mm256_xor_si256(a11,*(p+3));\
a12= _mm256_xor_si256(a12,*(p+4));\
a13= _mm256_xor_si256(a13,*(p+5));\
a14= _mm256_xor_si256(a14,*(p+6));\
a15= _mm256_xor_si256(a15,*(p+7));
  	
#define TwoRoundsSimon128128_64Way(E0,E1,E2,E3,E4,E5,E6,E7, E8,E9,E10,E11,E12,E13,E14,E15)\
	SimonGfun32WC(E0,E1,E2,E3,E4,E5,E6,E7,        E8,E9,E10,E11,E12,E13,E14,E15,p );  \
	SimonGfun32WC(E8,E9,E10,E11,E12,E13,E14,E15,  E0,E1,E2,E3,E4,E5,E6,E7      , p+8 );
	
 void SimonEnc_128_128BSAVX2_32Way(u8i *out,u8i *pt,   __m256i * rk)
 {
 	__m256i *p= (__m256i *)rk;
	__m256i *rkend=   p+ RoundSimon128128*8;      
	const __m256i MSKA= _mm256_load_si256((__m256i*) Lbswap_maskA ); 		
	__m256i D0,D1,D2,D3,D4,D5,D6,D7, D8,D9,D10,D11,D12,D13,D14,D15;  
	__m256i E0,E1,E2,E3,E4,E5,E6,E7, E8,E9,E10,E11,E12,E13,E14,E15;  
 	Simon128128InputData256A(D0,D1,D2,D3,D4,D5,D6,D7,pt); //bit slicing
  	Simon128128InputData256A(D8,D9,D10,D11,D12,D13,D14,D15,pt+256); //bit slicing  
   	E0=  _mm256_unpacklo_epi64(D0, D8);	    E8 =  _mm256_unpackhi_epi64(D0, D8);
	E1=  _mm256_unpacklo_epi64(D1, D9);	    E9 =  _mm256_unpackhi_epi64(D1, D9);
	E2=  _mm256_unpacklo_epi64(D2, D10);	E10=  _mm256_unpackhi_epi64(D2, D10);
	E3=  _mm256_unpacklo_epi64(D3, D11);	E11=  _mm256_unpackhi_epi64(D3, D11);
 	E4=  _mm256_unpacklo_epi64(D4, D12);	E12=  _mm256_unpackhi_epi64(D4, D12);
	E5=  _mm256_unpacklo_epi64(D5, D13);	E13=  _mm256_unpackhi_epi64(D5, D13);
	E6=  _mm256_unpacklo_epi64(D6, D14);	E14=  _mm256_unpackhi_epi64(D6, D14);
	E7=  _mm256_unpacklo_epi64(D7, D15);	E15=  _mm256_unpackhi_epi64(D7, D15);   

	while(p< rkend)
	{  
	    TwoRoundsSimon128128_64Way(E0,E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15); 
	    p+=16; 	
	} 
 
    D0=  _mm256_unpacklo_epi64(E0, E8);	    D8 =  _mm256_unpackhi_epi64(E0, E8);
	D1=  _mm256_unpacklo_epi64(E1, E9);	    D9 =  _mm256_unpackhi_epi64(E1, E9);
	D2=  _mm256_unpacklo_epi64(E2, E10);	D10=  _mm256_unpackhi_epi64(E2, E10);
	D3=  _mm256_unpacklo_epi64(E3, E11);	D11=  _mm256_unpackhi_epi64(E3, E11);
 	D4=  _mm256_unpacklo_epi64(E4, E12);	D12=  _mm256_unpackhi_epi64(E4, E12);
	D5=  _mm256_unpacklo_epi64(E5, E13);	D13=  _mm256_unpackhi_epi64(E5, E13);
	D6=  _mm256_unpacklo_epi64(E6, E14);	D14=  _mm256_unpackhi_epi64(E6, E14);
	D7=  _mm256_unpacklo_epi64(E7, E15);	D15=  _mm256_unpackhi_epi64(E7, E15); 	
	Simon128128OutputData256A(D0,D1,D2,D3,D4,D5,D6,D7,out) ; 
	Simon128128OutputData256A(D8,D9,D10,D11,D12,D13,D14,D15,out+256) ;	
	 
 }  

int crypto_stream_Simon128128ecb_avx2_32Way(
  unsigned char *out,
  unsigned char *in,
  unsigned long long inlen,
  const unsigned char *k
) 
{
 __m256i rk[RoundSimon128128*8];
int i;
  if (!inlen) return 0;

    	SimonGenRK_128_128BSAVX2_32Way(rk, k); 
 
  while(inlen >= 512){
		 SimonEnc_128_128BSAVX2_32Way(out, in,rk);   
    inlen -= 512;
    in += 512;
    out += 512;
  }

  return 0;
}
 
void TestSimon128128BSAVX2_32Way()
{
	int i,j;

uint8_t simon128_128_plain[] = {0x20, 0x74, 0x72, 0x61, 0x76, 0x65, 0x6c, 0x6c, 0x65, 0x72, 0x73, 0x20, 0x64, 0x65, 
0x73, 0x63};
uint8_t simon128_128_cipher[] = {0xbc, 0x0b, 0x4e, 0xf8, 0x2a, 0x83, 0xaa, 0x65, 0x3f, 0xfe, 0x54, 0x1e, 0x1e, 0x1b, 
0x68, 0x49}; 
	unsigned char MK[16]={0x00, 0x01, 0x02, 0x03,0x04, 0x05, 0x06,0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F 
};
	unsigned char *pt; 
	unsigned char *out; 
	u64i inlen=TOTALLEN;
	  int cloop=ENCLOOPS;
	  u64i cyclea, cycleb;
	  u64i cp,cpb,cpc;	 
	  pt=_mm_malloc(inlen,32);
	  out=_mm_malloc(inlen,32);
	  memcpy(pt, simon128_128_plain,16);
	     for(i=16;i<512;i++)
	    pt[i]=i%256;  
    	 

	 for(i=0;i<512;i++)
	 {
	      printf("%02x",pt[i]);
	      if((i+1)%LMAXCNT==0)  printf("\n");	
		  else if((i+1)%16==0) printf(" ");	 	
	 }
    printf("----------------------------------------\n");
	
	 crypto_stream_Simon128128ecb_avx2_32Way(out, pt,512,MK); 
	 for(i=0;i<512;i++)
	 {
	      printf("%02x",out[i]);
	      if((i+1)%LMAXCNT==0)  printf("\n");	
		  else if((i+1)%16==0) printf(" ");	 	
	 } 
cp=0;	
		for(   i=0;i<cloop;i++) 
 {  
	 srand((unsigned)time(NULL));
	for (j = 0; j < 16; j++)
		MK[j] = rand() % 256;
	for (j = 0; j < inlen; j++)
		pt[j] = rand() % 256;
	   grdtscl(&cyclea);   
	   crypto_stream_Simon128128ecb_avx2_32Way(out, pt, inlen,MK); 
     	grdtscl(&cycleb); 
     	cp+= (cycleb-cyclea);
 } 
	cp=cp/inlen;
 	cpb= cp/(cloop);
	 cpc= (cp- cpb*cloop)*100/cloop;  
    printf("Simon 128-128 32-way Enc cpb: %ld.%02ld\n",cpb,cpc ); 
    _mm_free(pt);
    _mm_free(out);	
}
 
