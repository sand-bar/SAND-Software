#include "SimonTypeDef.h"


 /*
Simon 128-128 avx2  16 way 
*/ 
   
static  const unsigned char _C_ALIGN32( Lbswap_maskD[32])   ={
  15,0, 1,2,3, 4,5,6, 7,  8,9,10,11, 12, 13,14, 
 15,0, 1,2,3, 4,5,6, 7,  8,9,10,11, 12, 13,14, 
}; 
static  const unsigned char _C_ALIGN32(Lbswap_maskE[32] ) ={
 14, 15,0, 1,2,3, 4, 5, 6, 7,  8,9,10,11, 12, 13, 
 14, 15,0, 1,2,3, 4, 5, 6, 7,  8,9,10,11, 12, 13, 
};   
static  const unsigned char _C_ALIGN32(Lbswap_maskF[32] ) ={
0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15,0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15,
};  

static  const unsigned char _C_ALIGN32(Lbswap_mask4[32] ) ={
   0,0,1,1,2,2,3,3, 4,4,5,5,6,6,7,7,  0,0,1,1,2,2,3,3, 4,4,5,5,6,6,7,7,  
 
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
#define Simon128128InputData256(D0,D1,D2,D3,D4,D5,D6,D7,pt)\
D0 = _mm256_loadu_si256((__m256i*)(pt));\
D1 = _mm256_loadu_si256((__m256i*)(pt+32));\
D2 = _mm256_loadu_si256((__m256i*)(pt+32*2));\
D3 = _mm256_loadu_si256((__m256i*)(pt+32*3));\
D4 = _mm256_loadu_si256((__m256i*)(pt+32*4));\
D5 = _mm256_loadu_si256((__m256i*)(pt+32*5));\
D6 = _mm256_loadu_si256((__m256i*)(pt+32*6));\
D7 = _mm256_loadu_si256((__m256i*)(pt+32*7));\
E0 = _mm256_permute4x64_epi64( D0, 0xb1);\
E1 = _mm256_permute4x64_epi64( D1, 0xb1);\
E2 = _mm256_permute4x64_epi64( D2, 0xb1);\
E3 = _mm256_permute4x64_epi64( D3, 0xb1);\
E4 = _mm256_permute4x64_epi64( D4, 0xb1);\
E5 = _mm256_permute4x64_epi64( D5, 0xb1);\
E6 = _mm256_permute4x64_epi64( D6, 0xb1);\
E7 = _mm256_permute4x64_epi64( D7, 0xb1);\
bitsliceAVX2(&E0,&E1,&E2,&E3,&E4,&E5,&E6,&E7);\
\
D0=  _mm256_unpacklo_epi8(E0, E4);\
D4=  _mm256_unpackhi_epi8(E0, E4);\
D1=  _mm256_unpacklo_epi8(E1, E5);\
D5=  _mm256_unpackhi_epi8(E1, E5);\
D2=  _mm256_unpacklo_epi8(E2, E6);\
D6=  _mm256_unpackhi_epi8(E2, E6);\
D3=  _mm256_unpacklo_epi8(E3, E7);\
D7=  _mm256_unpackhi_epi8(E3, E7); 

 
#define Simon128128OutputData256(D0,D1,D2,D3,D4,D5,D6,D7,out) \
D0 = _mm256_shuffle_epi8( D0, MSKF);\
D1 = _mm256_shuffle_epi8( D1, MSKF);\
D2 = _mm256_shuffle_epi8( D2, MSKF);\
D3 = _mm256_shuffle_epi8( D3, MSKF);\
D4 = _mm256_shuffle_epi8( D4 ,MSKF);\
D5 = _mm256_shuffle_epi8( D5, MSKF);\
D6 = _mm256_shuffle_epi8( D6, MSKF);\
D7 = _mm256_shuffle_epi8( D7, MSKF);\
E0=  _mm256_unpacklo_epi64(D0, D4);\
E4=  _mm256_unpackhi_epi64(D0, D4);\
E1=  _mm256_unpacklo_epi64(D1, D5);\
E5=  _mm256_unpackhi_epi64(D1, D5);\
E2=  _mm256_unpacklo_epi64(D2, D6);\
E6=  _mm256_unpackhi_epi64(D2, D6);\
E3=  _mm256_unpacklo_epi64(D3, D7);\
E7=  _mm256_unpackhi_epi64(D3, D7); \
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
 
 
  #define  SimonGfun16W(b0,b1,b2,b3,a0,  a1, a2, a3,p,a02,a12,a22,a32,a21,a31) \
a02= _mm256_shuffle_epi8( a0, MSKx);\
a12= _mm256_shuffle_epi8( a1, MSKx);\
a22= _mm256_shuffle_epi8( a2, MSKx);\
a32= _mm256_shuffle_epi8( a3, MSKx);\
a21= _mm256_shuffle_epi8( a2, MSKz);\
a31= _mm256_shuffle_epi8( a3, MSKz);\
b0= _mm256_xor_si256(b0,*(p));\
b1= _mm256_xor_si256(b1,*(p+1));\
b2= _mm256_xor_si256(b2,*(p+2));\
b3= _mm256_xor_si256(b3,*(p+3)); \
b0= _mm256_xor_si256(b0,XOR3(a31,a02,a21));\
b1= _mm256_xor_si256(b1,XOR3(a0,a12,a31));\
b2= _mm256_xor_si256(b2,XOR3(a1,a22,a0));\
b3= _mm256_xor_si256(b3,XOR3(a2,a32,a1));


#define TwoRoundsSimon128(D0,D1,D2,D3,D4,D5,D6,D7)\
	SimonGfun16W(D4,D5,D6,D7,D0,D1,D2,D3,p,E0,E1,E2,E3,E4,E5);  \
	SimonGfun16W(D0,D1,D2,D3,D4,D5,D6,D7,p+4,E0,E1,E2,E3,E4,E5);  

  #define SimonBsRK_128BSAVX2(rk,m,x0)\
	A= _mm256_set1_epi64x(x0);\
    A=_mm256_shuffle_epi8( A, MSK4) ;\
	*(rk+m) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK0, A) , MSK0); \
	*(rk+m+1) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK1, A) , MSK1); \
	*(rk+m+2) =_mm256_cmpeq_epi8( _mm256_and_si256(MSK2, A) , MSK2); \
	*(rk+m+3) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK3, A) , MSK3); 
	  
void SimonGenRK_128_128BSAVX2( __m256i* rk, const unsigned char *key )
{

	u64i subkey0, subkey1;
	u64i  i ;
	u64i  tmp1,tmp2; 
    __m256i A;
	const __m256i MSK0 =_mm256_set1_epi16(0x1001);
	const __m256i MSK1 =_mm256_set1_epi16(0x2002);
	const __m256i MSK2 =_mm256_set1_epi16(0x4004);
	const __m256i MSK3 =_mm256_set1_epi16(0x8008);   
	const __m256i MSK4 =_mm256_load_si256((__m256i*) Lbswap_mask4 ); 
   memcpy( &subkey0 , key , 8);
	 memcpy( &subkey1 , key+8 , 8);
          SimonBsRK_128BSAVX2(rk, 0, subkey0);
    for ( i = 0; i < RoundSimon128128 - 2; i+=2) {	
          SimonBsRK_128BSAVX2(rk,(i+1)*4, subkey1); 
    RKFun128Const( subkey0, subkey1,i,ConstRK128);
         SimonBsRK_128BSAVX2(rk,(i+2)*4, subkey0); 
    RKFun128Const( subkey1, subkey0,i+1,ConstRK128);
    }  	
    SimonBsRK_128BSAVX2(rk,(i+1)*4, subkey1);  


} 


 void SimonEnc_128_128BSAVX2_16Way(u8i *out,u8i *pt,   __m256i * rk)
 {
 	__m256i *p= (__m256i *)rk;
	__m256i *rkend=   p+ RoundSimon128128*4;      
	const __m256i MSKz= _mm256_load_si256((__m256i*) Lbswap_maskD );	
	const __m256i MSKx= _mm256_load_si256((__m256i*) Lbswap_maskE );	
	const __m256i MSKF= _mm256_load_si256((__m256i*) Lbswap_maskF );		
	__m256i D0,D1,D2,D3,D4,D5,D6,D7;  
	__m256i E0,E1,E2,E3,E4,E5,E6,E7;  
	
 	Simon128128InputData256(D0,D1,D2,D3,D4,D5,D6,D7,pt); 

	while(p< rkend)
	{ 
	  
      TwoRoundsSimon128(D0,D1,D2,D3,D4,D5,D6,D7); 
	  p+=8; 	
	}  
	Simon128128OutputData256(D0,D1,D2,D3,D4,D5,D6,D7,out) ;  
 }  

 
 
int crypto_stream_Simon128128ecb_avx2_16Way(
  unsigned char *out,
  unsigned char *in,
  unsigned long long inlen,
  const unsigned char *k
) 
{
 __m256i rk[RoundSimon128128*4];

  if (!inlen) return 0;
    	SimonGenRK_128_128BSAVX2(rk, k); 

  while(inlen >= 256){
		 SimonEnc_128_128BSAVX2_16Way(out, in,rk);   
    inlen -= 256;
    in += 256;
    out += 256;
  }

  return 0;
}



void TestSimon128128BSAVX2_16Way()
{
	int i,j;
uint8_t simon128_128_plain[] = {0x20, 0x74, 0x72, 0x61, 0x76, 0x65, 0x6c, 0x6c, 0x65, 0x72, 0x73, 0x20, 0x64, 0x65, 
0x73, 0x63};
uint8_t simon128_128_cipher[] = {0xbc, 0x0b, 0x4e, 0xf8, 0x2a, 0x83, 0xaa, 0x65, 0x3f, 0xfe, 0x54, 0x1e, 0x1e, 0x1b, 
0x68, 0x49}; 
	unsigned char MK[16]={0x00, 0x01, 0x02, 0x03,0x04, 0x05, 0x06,0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F 
};

	unsigned char *pt;//[512*1000];
	unsigned char *out;//[512*1000];
	u64i inlen=TOTALLEN;
	  int cloop=ENCLOOPS;
	  u64i cyclea, cycleb;
	  u64i cp,cpb,cpc;	 
	  pt= _mm_malloc(inlen,32);
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
	crypto_stream_Simon128128ecb_avx2_16Way(out, pt,512, MK); 
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
	   crypto_stream_Simon128128ecb_avx2_16Way(out, pt, inlen,MK); 
     	grdtscl(&cycleb); 
     	cp+= (cycleb-cyclea);
 } 	
 cp=cp/inlen;
 	cpb= cp/(cloop);
	 cpc= (cp- cpb*cloop)*100/cloop;  
    printf("Simon 128-128 16-way Enc cpb: %ld.%02ld\n",cpb,cpc );
_mm_free(pt);
    _mm_free(out);
}
 
