#include "SimonTypeDef.h" 
 /*
Simon 64-128 avx2  64 way  
 
*/ 
 
static const unsigned char _C_ALIGN32(Lbswap_maskA[32] ) ={
3, 0, 1,2, 7,4,5,6,  11, 8,9,10,15, 12, 13,14,  
3, 0, 1,2, 7,4,5,6,  11, 8,9,10,15, 12, 13,14, 
 
};   

 static const u32i ConstRK64[40]={
 0xfffffffd,0xfffffffd,0xfffffffc,0xfffffffd,0xfffffffd,0xfffffffc,0xfffffffd,0xfffffffd,
0xfffffffd,0xfffffffc,0xfffffffd,0xfffffffc,0xfffffffd,0xfffffffd,0xfffffffc,0xfffffffc,
0xfffffffc,0xfffffffd,0xfffffffd,0xfffffffc,0xfffffffc,0xfffffffd,0xfffffffc,0xfffffffd,
0xfffffffd,0xfffffffd,0xfffffffd,0xfffffffc,0xfffffffc,0xfffffffc,0xfffffffc,0xfffffffc,
0xfffffffc,0xfffffffd,0xfffffffc,0xfffffffc,0xfffffffd,0xfffffffc,0xfffffffc,0xfffffffc,
};

  #define SimonBsRK_64AVX2A(rk,m,x0)\
	A= _mm256_set1_epi32(x0);\
	*(rk+m) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK0, A) , MSK0); \
	*(rk+m+1) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK1, A) , MSK1); \
	*(rk+m+2) =_mm256_cmpeq_epi8( _mm256_and_si256(MSK2, A) , MSK2); \
	*(rk+m+3) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK3, A) , MSK3); \
	*(rk+m+4) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK4, A) , MSK4); \
	*(rk+m+5) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK5, A) , MSK5); \
	*(rk+m+6) =_mm256_cmpeq_epi8( _mm256_and_si256(MSK6, A) , MSK6); \
	*(rk+m+7) = _mm256_cmpeq_epi8( _mm256_and_si256(MSK7, A) , MSK7);

                 
void SimonGenRK_64_128BSAVX2_64Way(__m256i * rk,const u8i *key )
{
	u32i subkeys[4] = {};
    u32i  tmp1,tmp2; 
  int i ; 
	__m256i A;
	const __m256i MSK0 =_mm256_set1_epi8(0x01);
	const __m256i MSK1 =_mm256_set1_epi8(0x02);
	const __m256i MSK2 =_mm256_set1_epi8(0x04);
	const __m256i MSK3 =_mm256_set1_epi8(0x08);  
	const __m256i MSK4 =_mm256_set1_epi8(0x10);
	const __m256i MSK5 =_mm256_set1_epi8(0x20);
	const __m256i MSK6 =_mm256_set1_epi8(0x40);
	const __m256i MSK7 =_mm256_set1_epi8(0x80); 
	 memcpy(subkeys, key , 16);  
          SimonBsRK_64AVX2A(rk, 0, *((u32i *)subkeys));
      for ( i = 0; i <  RoundSimon64128-4; i+=4) { 
         SimonBsRK_64AVX2A(rk, i*8+8, subkeys[1]);
         RKfun64Const(subkeys[0],subkeys[1],subkeys[3],i,ConstRK64);
          
         SimonBsRK_64AVX2A(rk, i*8+16, subkeys[2]);
         RKfun64Const(subkeys[1],subkeys[2],subkeys[0],i+1,ConstRK64);
         
         SimonBsRK_64AVX2A(rk, i*8+24, subkeys[3]);
         RKfun64Const(subkeys[2],subkeys[3],subkeys[1],i+2,ConstRK64);     
		     
         SimonBsRK_64AVX2A(rk, i*8+32, subkeys[0]);
         RKfun64Const(subkeys[3],subkeys[0],subkeys[2],i+3,ConstRK64);   
    } 
        SimonBsRK_64AVX2A(rk, i*8+8, subkeys[1]);  
        SimonBsRK_64AVX2A(rk, i*8+16, subkeys[2]);          
         SimonBsRK_64AVX2A(rk, i*8+24, subkeys[3]);   
 
} 
 
#define Simon64128InputData256A(D0,D1,D2,D3,D4,D5,D6,D7,pt)\
	D0 = _mm256_loadu_si256((__m256i*)(pt));\
	D1 = _mm256_loadu_si256((__m256i*)(pt+32));\
	D2 = _mm256_loadu_si256((__m256i*)(pt+32*2));\
	D3 = _mm256_loadu_si256((__m256i*)(pt+32*3));\
	D4 = _mm256_loadu_si256((__m256i*)(pt+32*4));\
	D5 = _mm256_loadu_si256((__m256i*)(pt+32*5));\
	D6 = _mm256_loadu_si256((__m256i*)(pt+32*6));\
	D7 = _mm256_loadu_si256((__m256i*)(pt+32*7));\
	bitsliceAVX2(&D0,&D1,&D2,&D3,&D4,&D5,&D6,&D7);   
	
#define Simon64128OutputData256A(E0,E1,E2,E3,E4,E5,E6,E7,out) \
	bitsliceAVX2(&E0,&E1,&E2,&E3,&E4,&E5,&E6,&E7);\
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
 
 

#define 	SimonGfun64W(a0,a1,a2,a3,a4,a5,a6,a7,        a8,a9,a10,a11,a12,a13,a14,a15,p )\
\
a8= _mm256_xor_si256(a8,*(p));\
a9= _mm256_xor_si256(a9,*(p+1));\
a10= _mm256_xor_si256(a10,*(p+2));\
a11= _mm256_xor_si256(a11,*(p+3));\
a12= _mm256_xor_si256(a12,*(p+4));\
a13= _mm256_xor_si256(a13,*(p+5));\
a14= _mm256_xor_si256(a14,*(p+6));\
a15= _mm256_xor_si256(a15,*(p+7));\
E6= _mm256_shuffle_epi8(a6,MSKA);\
E7= _mm256_shuffle_epi8(a7,MSKA);\
a14=  _mm256_xor_si256(a14,XOR3(a5,E6,a4));\
a15=  _mm256_xor_si256(a15,XOR3(a6,E7,a5)); \
E0 = _mm256_shuffle_epi8( a0 ,MSKA);\
E1 = _mm256_shuffle_epi8( a1, MSKA);\
\
a8=  _mm256_xor_si256(a8,XOR3(E7,E0,E6));\
a9=  _mm256_xor_si256(a9,XOR3(a0,E1,E7));\
E2 = _mm256_shuffle_epi8( a2, MSKA);\
E3 = _mm256_shuffle_epi8( a3, MSKA);\
E4= _mm256_shuffle_epi8(a4,MSKA);\
E5= _mm256_shuffle_epi8(a5,MSKA);\
a10=  _mm256_xor_si256(a10,XOR3(a1,E2,a0));\
a11=  _mm256_xor_si256(a11,XOR3(a2,E3,a1));\
a12=  _mm256_xor_si256(a12,XOR3(a3,E4,a2));\
a13=  _mm256_xor_si256(a13,XOR3(a4,E5,a3));
	
#define TwoRoundsSimon64128_64Way(D0,D1,D2,D3,D4,D5,D6,D7, D8,D9,D10,D11,D12,D13,D14,D15)\
	SimonGfun64W(D0,D1,D2,D3,D4,D5,D6,D7,        D8,D9,D10,D11,D12,D13,D14,D15,p );  \
	SimonGfun64W(D8,D9,D10,D11,D12,D13,D14,D15,  D0,D1,D2,D3,D4,D5,D6,D7      , p+8 );
	
		
 void SimonEnc_64_128BSAVX2_64Way(u8i *out,u8i *pt,   __m256i * rk)
 {
 	__m256i *p= (__m256i *)rk;
	__m256i *rkend=   p+ RoundSimon64128*8;      
	const __m256i MSKA= _mm256_load_si256((__m256i*) Lbswap_maskA ); 
		 	
	__m256i D0,D1,D2,D3,D4,D5,D6,D7, D8,D9,D10,D11,D12,D13,D14,D15;  
	__m256i E0,E1,E2,E3,E4,E5,E6,E7, E8,E9,E10,E11,E12,E13,E14,E15;  
	
 	Simon64128InputData256A(D0,D1,D2,D3,D4,D5,D6,D7,pt); //bit slicing
  	Simon64128InputData256A(D8,D9,D10,D11,D12,D13,D14,D15,pt+256); //bit slicing  
  	
 	E0=  _mm256_unpacklo_epi32(D0, D8);	    E8 =  _mm256_unpackhi_epi32(D0, D8);
	E1=  _mm256_unpacklo_epi32(D1, D9);	    E9 =  _mm256_unpackhi_epi32(D1, D9);
	E2=  _mm256_unpacklo_epi32(D2, D10);	E10=  _mm256_unpackhi_epi32(D2, D10);
	E3=  _mm256_unpacklo_epi32(D3, D11);	E11=  _mm256_unpackhi_epi32(D3, D11);
 	E4=  _mm256_unpacklo_epi32(D4, D12);	E12=  _mm256_unpackhi_epi32(D4, D12);
	E5=  _mm256_unpacklo_epi32(D5, D13);	E13=  _mm256_unpackhi_epi32(D5, D13);
	E6=  _mm256_unpacklo_epi32(D6, D14);	E14=  _mm256_unpackhi_epi32(D6, D14);
	E7=  _mm256_unpacklo_epi32(D7, D15);	E15=  _mm256_unpackhi_epi32(D7, D15);   
	
    D0=  _mm256_unpackhi_epi32(E0, E8);	    D8 =  _mm256_unpacklo_epi32(E0, E8);
	D1=  _mm256_unpackhi_epi32(E1, E9);	    D9 =  _mm256_unpacklo_epi32(E1, E9);
	D2=  _mm256_unpackhi_epi32(E2, E10);	D10=  _mm256_unpacklo_epi32(E2, E10);
	D3=  _mm256_unpackhi_epi32(E3, E11);	D11=  _mm256_unpacklo_epi32(E3, E11);
 	D4=  _mm256_unpackhi_epi32(E4, E12);	D12=  _mm256_unpacklo_epi32(E4, E12);
	D5=  _mm256_unpackhi_epi32(E5, E13);	D13=  _mm256_unpacklo_epi32(E5, E13);
	D6=  _mm256_unpackhi_epi32(E6, E14);	D14=  _mm256_unpacklo_epi32(E6, E14);
	D7=  _mm256_unpackhi_epi32(E7, E15);	D15=  _mm256_unpacklo_epi32(E7, E15); 	 
	//d0 ~d7   left  d8 ~ d15  right 

	while(p< rkend)
	{ 
	    TwoRoundsSimon64128_64Way(   D0,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10,D11,D12,D13,D14,D15  );

	    p+=16; 	
	}  
    E0=  _mm256_unpacklo_epi32(D0, D8);	E8=  _mm256_unpackhi_epi32(D0, D8);
	E1=  _mm256_unpacklo_epi32(D1, D9);	E9=  _mm256_unpackhi_epi32(D1, D9);
	E2=  _mm256_unpacklo_epi32(D2, D10);	E10=  _mm256_unpackhi_epi32(D2, D10);
	E3=  _mm256_unpacklo_epi32(D3, D11);	E11=  _mm256_unpackhi_epi32(D3, D11);
 	E4=  _mm256_unpacklo_epi32(D4, D12);	E12=  _mm256_unpackhi_epi32(D4, D12);
	E5=  _mm256_unpacklo_epi32(D5, D13);	E13=  _mm256_unpackhi_epi32(D5, D13);
	E6=  _mm256_unpacklo_epi32(D6, D14);	E14=  _mm256_unpackhi_epi32(D6, D14);
	E7=  _mm256_unpacklo_epi32(D7, D15);	E15=  _mm256_unpackhi_epi32(D7, D15);
    Simon64128OutputData256A(E0,E1,E2,E3,E4,E5,E6,E7,out );
	Simon64128OutputData256A( E8,E9,E10,E11,E12,E13,E14,E15, out+256) ;  
 } 
 
int crypto_stream_Simon64128ecb_avx2_64Way(
  unsigned char *out,
  unsigned char *in,
  unsigned long long inlen,
  const unsigned char *k
) 
{
 __m256i rk[RoundSimon64128*8]; 
int i;
  if (!inlen) return 0; 
  SimonGenRK_64_128BSAVX2_64Way(rk, k);   
  while(inlen >= 512){
		 SimonEnc_64_128BSAVX2_64Way(out, in,rk);   
    inlen -= 512;
    in += 512;
    out += 512;
  } 
  return 0;
} 

void TestSimon64128BSAVX2_64Way()
{	  
u64i cyclea, cycleb;
	  u64i cp,cpb,cpc;	     
    uint8_t simon128_64_plain[] = {0x75, 0x6e, 0x64, 0x20, 0x6c, 0x69, 0x6b, 0x65};
    uint8_t simon128_64_cipher[] = {0x7a, 0xa0, 0xdf, 0xb9, 0x20, 0xfc, 0xc8, 0x44};
	unsigned char MK[16]={0x00, 0x01, 0x02, 0x03, 0x08, 0x09, 0x0A, 0x0B, 0x10, 0x11, 0x12, 0x13, 0x18, 0x19, 0x1A, 0x1B};
	unsigned char *pt; 
	unsigned char *out; 
	u64i inlen=TOTALLEN;
	  int cloop=ENCLOOPS; 
	int i,j; 	  
	  pt= _mm_malloc(inlen,32);
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
	
	 crypto_stream_Simon64128ecb_avx2_64Way(out, pt,512, MK); 
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
	   crypto_stream_Simon64128ecb_avx2_64Way(out, pt, inlen,MK); 
     	grdtscl(&cycleb); 
     	cp+= (cycleb-cyclea);
 } 	   
	cp=cp/inlen;	 
 	cpb= cp/(cloop);
	 cpc= (cp- cpb*cloop)*100/cloop;  
    printf("Simon 64-128 64-Way Enc cpb: %ld.%02ld\n",cpb,cpc ); 
        _mm_free(pt);
    _mm_free(out);
} 
