#include "SimonTypeDef.h" 

 /*
Simon 64-128 avx2  32 way
   
*/
  static const u32i ConstRK64[40]={
 0xfffffffd,0xfffffffd,0xfffffffc,0xfffffffd,0xfffffffd,0xfffffffc,0xfffffffd,0xfffffffd,
0xfffffffd,0xfffffffc,0xfffffffd,0xfffffffc,0xfffffffd,0xfffffffd,0xfffffffc,0xfffffffc,
0xfffffffc,0xfffffffd,0xfffffffd,0xfffffffc,0xfffffffc,0xfffffffd,0xfffffffc,0xfffffffd,
0xfffffffd,0xfffffffd,0xfffffffd,0xfffffffc,0xfffffffc,0xfffffffc,0xfffffffc,0xfffffffc,
0xfffffffc,0xfffffffd,0xfffffffc,0xfffffffc,0xfffffffd,0xfffffffc,0xfffffffc,0xfffffffc,
};   
static  const unsigned char _C_ALIGN32( Lbswap_maskD[32])   ={
 7, 0, 1,2,3, 4,5,6,  15, 8,9,10,11, 12, 13,14, 
 7, 0, 1,2,3, 4,5,6,  15, 8,9,10,11, 12, 13,14, 
}; 
static  const unsigned char _C_ALIGN32(Lbswap_maskE[32] ) ={
 6, 7, 0, 1,2,3, 4, 5, 14, 15, 8,9,10,11, 12, 13, 
 6,7, 0, 1,2,3,  4, 5, 14, 15, 8,9,10,11, 12, 13,
};  

static  const unsigned char _C_ALIGN32(Lbswap_maskF[32] ) ={
0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15,0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15,
};  

static  const unsigned char _C_ALIGN32(Lbswap_mask4[32] ) ={
 4,4,5,5,6,6,7,7,0,0,1,1,2,2,3,3,  4,4,5,5,6,6,7,7,0,0,1,1,2,2,3,3,
};    

  #define SimonBsRK_64AVX2(rk,m,x0)\
	A= _mm256_set1_epi32(x0);\
    A= _mm256_shuffle_epi8( A, MSK4) ;\
	*(rk+m) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK0, A) , MSK0); \
	*(rk+m+1) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK1, A) , MSK1); \
	*(rk+m+2) =_mm256_cmpeq_epi8( _mm256_and_si256(MSK2, A) , MSK2); \
	*(rk+m+3) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK3, A) , MSK3); 
 
                    
void SimonGenRK_64_128BSAVX2(__m256i * rk, const u8i *key )
{
	u32i subkeys[4] = {};
    u32i tmp1,tmp2;
  int i;
  
	__m256i A;
	const __m256i MSK0 =_mm256_set1_epi16(0x1001);
	const __m256i MSK1 =_mm256_set1_epi16(0x2002);
	const __m256i MSK2 =_mm256_set1_epi16(0x4004);
	const __m256i MSK3 =_mm256_set1_epi16(0x8008);   
	const __m256i MSK4 =_mm256_load_si256((__m256i*) Lbswap_mask4 );  
	memcpy(subkeys, key , 16); 
    SimonBsRK_64AVX2(rk, 0, *subkeys);
    for ( i = 0; i < RoundSimon64128-4; i+=4) { 
         SimonBsRK_64AVX2(rk, i*4+4, subkeys[1]);
         RKfun64Const(subkeys[0],subkeys[1],subkeys[3],i,ConstRK64); 
         SimonBsRK_64AVX2(rk, i*4+8, subkeys[2]);
         RKfun64Const(subkeys[1],subkeys[2],subkeys[0],i+1,ConstRK64);
         
         SimonBsRK_64AVX2(rk, i*4+12, subkeys[3]);
         RKfun64Const(subkeys[2],subkeys[3],subkeys[1],i+2,ConstRK64);         
         SimonBsRK_64AVX2(rk, i*4+16, subkeys[0]);
         RKfun64Const(subkeys[3],subkeys[0],subkeys[2],i+3,ConstRK64);   
    } 
        SimonBsRK_64AVX2(rk, i*4+4, subkeys[1]);  
        SimonBsRK_64AVX2(rk, i*4+8, subkeys[2]);          
         SimonBsRK_64AVX2(rk, i*4+12, subkeys[3]);   
} 
  
#define Simon64128InputData256(D0,D1,D2,D3,D4,D5,D6,D7,pt)\
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

 
#define Simon64128OutputData256(D0,D1,D2,D3,D4,D5,D6,D7,out) \
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
\
E0 = _mm256_shuffle_epi32( E0, 0xb1);\
E1 = _mm256_shuffle_epi32( E1, 0xb1);\
E2 = _mm256_shuffle_epi32( E2, 0xb1);\
E3 = _mm256_shuffle_epi32( E3, 0xb1);\
E4 = _mm256_shuffle_epi32( E4, 0xb1);\
E5 = _mm256_shuffle_epi32( E5, 0xb1);\
E6 = _mm256_shuffle_epi32( E6, 0xb1);\
E7 = _mm256_shuffle_epi32( E7, 0xb1);\
	_mm256_storeu_si256((__m256i*)(out  ), E0);\
	_mm256_storeu_si256((__m256i*)(out+32*1), E1);\
	_mm256_storeu_si256((__m256i*)(out+32*2), E2);\
	_mm256_storeu_si256((__m256i*)(out+32*3), E3);\
	_mm256_storeu_si256((__m256i*)(out+32*4), E4);\
	_mm256_storeu_si256((__m256i*)(out+32*5), E5);\
	_mm256_storeu_si256((__m256i*)(out+32*6), E6);\
	_mm256_storeu_si256((__m256i*)(out+32*7), E7); 	
  
  #define  SimonGfun32W(b0,b1,b2,b3,a0,  a1, a2, a3,p,a02,a12,a22,a32,a21,a31) \
  \
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
 
#define TwoRoundsSimon(D0,D1,D2,D3,D4,D5,D6,D7)\
	SimonGfun32W(D4,D5,D6,D7,D0,D1,D2,D3,p,E0,E1,E2,E3,E4,E5);  \
	SimonGfun32W(D0,D1,D2,D3,D4,D5,D6,D7,p+4,E0,E1,E2,E3,E4,E5);
 
 void SimonEnc_64_128BSAVX2(u8i *out,u8i *pt,   __m256i * rk)
 {
 	__m256i *p= (__m256i *)rk;
	__m256i *rkend=   p+ RoundSimon64128*4;      
	const __m256i MSKz= _mm256_load_si256((__m256i*) Lbswap_maskD );	
	const __m256i MSKx= _mm256_load_si256((__m256i*) Lbswap_maskE );	
	const __m256i MSKF= _mm256_load_si256((__m256i*) Lbswap_maskF );		
	__m256i D0,D1,D2,D3,D4,D5,D6,D7;  
	__m256i E0,E1,E2,E3,E4,E5,E6,E7;  
	
 	Simon64128InputData256(D0,D1,D2,D3,D4,D5,D6,D7,pt); 

	while(p< rkend)
	{  
	  TwoRoundsSimon(D0,D1,D2,D3,D4,D5,D6,D7);
	
	p+=8; 	
	} 
 
	Simon64128OutputData256(D0,D1,D2,D3,D4,D5,D6,D7,out) ;  
 } 
 
 
int crypto_stream_Simon64128ecb_avx2_32Way(
  unsigned char *out,
  unsigned char *in,
  unsigned long long inlen,
  const unsigned char *k
) 
{
 __m256i rk[RoundSimon64128*4];


  if (!inlen) return 0;

   SimonGenRK_64_128BSAVX2(rk, k); 

  while(inlen >= 256){
		 SimonEnc_64_128BSAVX2(out, in,rk);   
    inlen -= 256;
    in += 256;
    out += 256;
  }

  return 0;
}
 
 
void TestSimon64128BSAVX2_32Way()
{
	int i,j;
	uint8_t simon128_64_plain[] = {0x75, 0x6e, 0x64, 0x20, 0x6c, 0x69, 0x6b, 0x65};
    uint8_t simon128_64_cipher[] = {0x7a, 0xa0, 0xdf, 0xb9, 0x20, 0xfc, 0xc8, 0x44};
	unsigned char MK[16]={0x00, 0x01, 0x02, 0x03, 0x08, 0x09, 0x0A, 0x0B, 0x10, 0x11, 0x12, 0x13, 0x18, 0x19, 0x1A, 0x1B};

	unsigned char *pt;
	unsigned char *out;
	u64i inlen=TOTALLEN;
	  int cloop=ENCLOOPS;
	  u64i cyclea, cycleb;
	  u64i cp,cpb,cpc;	 
	  pt=_mm_malloc(inlen,32);
	  out=_mm_malloc(inlen,32);
		  memcpy(pt, simon128_64_plain,8); 
	   for(i=8;i<512;i++)
	    pt[i]=i%256;    
	 	 for(i=0;i<512;i++)
	 {
	      printf("%02x",pt[i]);
	      if((i+1)%LMAXCNT==0)  printf("\n");	
		  else if((i+1)%8==0) printf(" ");	 	
	 }
    printf("----------------------------------------\n"); 
	 crypto_stream_Simon64128ecb_avx2_32Way(out, pt,512,MK); 
	 for(i=0;i<512;i++)
	 {
	      printf("%02x",out[i]);
		   if((i+1)%LMAXCNT==0)  printf("\n");	
		  else if((i+1)%8==0) printf(" ");	 	
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
	   crypto_stream_Simon64128ecb_avx2_32Way(out, pt, inlen,MK); 
     	grdtscl(&cycleb); 
     	cp+= (cycleb-cyclea);
 } 	  
	cp=cp/inlen;
 	cpb= cp/(cloop);
	 cpc= (cp- cpb*cloop)*100/cloop;  
    printf("Simon 64-128 32-way Enc cpb: %ld.%02ld\n",cpb,cpc ); 
    _mm_free(pt);
    _mm_free(out);
} 

