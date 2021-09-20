#ifndef _H_SDUCRYTYPE_
#define _H_SDUCRYTYPE_


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <immintrin.h>
#include <avx2intrin.h>

#define t0_128 3
#define t1_128 1

#define t0_64 3
#define t1_64 1

#define BLOCKSIZESimon64128 8 
#define BLOCKSIZESimon128128 16  
 
#define  RoundSimon64128  44 
#define  RoundSimon128128  68 


#define TOTALLEN  (512*4000) 
#define ENCLOOPS  2000 

#define LMAXCNT 64 

#ifndef u8i
 #ifdef  __GNUC__
   #define u8i uint8_t
 #elif _MSC_VER 
   #define u8i unsigned char
 #endif
#endif



#ifndef u16i
 #ifdef  __GNUC__
   #define u16i uint16_t
 #elif _MSC_VER 
   #define u16i unsigned short
 #endif
#endif


#ifndef u32i
 #ifdef  __GNUC__
   #define u32i uint32_t
 #elif _MSC_VER 
   #define u32i unsigned long 
 #endif
#endif

#ifndef u64i
 #ifdef  __GNUC__
   #define u64i uint64_t
 #elif _MSC_VER 
   #define u64i unsigned long long
 #endif
#endif





#ifdef  __GNUC__
   #define _C_ALIGN(n, A)  A __attribute__ ((aligned (n)))
#elif _MSC_VER 
   #define _C_ALIGN(n, A)  __declspec( align(n) ) A
#endif

#define _C_ALIGN16(A) _C_ALIGN(16,A)
#define _C_ALIGN32(A) _C_ALIGN(32,A)


#define ROTL32(v, n) \
  ((u32i)((v) << (n)) | ((v) >> (32 - (n))))
  
#define ROTR32(v, n) \
  ((u32i)((v) >> (n)) | ((v) << (32 - (n))))
    
  
#define ROTL64(v, n) \
  ((u64i)((v) << (n)) | ((v) >> (64 - (n)))) 
 
  
#define ROTR64(v, n) \
  ((u64i)((v) >> (n)) | ((v) << (64 - (n)))) 
 
 	
    #define XOR3(a,b,c)\
 _mm256_xor_si256(c,_mm256_and_si256(a,b)   )
 
 
      #define XOR4(a,b,c,d)\
      _mm256_xor_si256( _mm256_xor_si256(a,d), _mm256_and_si256(b,c) )
    
    #define RKfun64Const(subk0,subk1,subk3,i,ConstRK)\
        tmp1 = ROTR32(subk3,3); \
        tmp1 ^= subk1;  \
        tmp1 ^=ROTR32(tmp1,1)^ subk0; \
        subk0=tmp1^ConstRK[i]; 
		
         
     #define RKFun128Const( sub0, sub1,i, ConstRK)\
        tmp1 = ROTR64(sub1,3);  \
        tmp1 ^= ROTR64(tmp1,1)^sub0;\
        sub0=tmp1^ConstRK[i];
		   
			  
 void grdtscl(u64i *ll); 
void bitsliceAVX2(__m256i * x0, __m256i * x1, __m256i * x2, __m256i * x3,__m256i *  x4,__m256i *  x5, __m256i * x6, __m256i * x7 );

void TestSimon64128BSAVX2_32Way();
void TestSimon64128BSAVX2_64Way();
void TestSimon128128BSAVX2_16Way();
void TestSimon128128BSAVX2_32Way();

#endif
