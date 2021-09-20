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

#define BLOCKSIZESAND64128 8 
#define BLOCKSIZESAND128128 16 


#define  RoundSAND64128  48 
#define  RoundSAND128128  54 
 
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

#define ROTL4(v, n) \
  (((v) << (n)) | ((v) >> (4 - (n)))) 
 
 #define ROTR4(v, n) \
  (((v) >> (n)) | ((v) << (4 - (n)))) 
  
#define ROTL8(v, n) \
  ((u8i)((v) << (n)) | ((v) >> (8 - (n)))) 
  
  
#define ROTL16(v, n) \
  ((u16i)((v) << (n)) | ((v) >> (16 - (n)))) 
  
#define ROTR16(v, n) \
  ((u16i)((v) >> (n)) | ((v) << (16 - (n))))   
  

#define ROTL32(v, n) \
  ((u32i)((v) << (n)) | ((v) >> (32 - (n))))
  
  
#define ROTL64(v, n) \
  ((u64i)((v) << (n)) | ((v) >> (64 - (n))))
  
 
  
#define SANDFunA_64B(M,T,m)\
	m=  ROTL32(M,20);\
	T= (  (M<<3)&0x80000000)^m;\
	t0=(  (m<<5)&0xEEE00000)^(  (m<<1)&0x11100000);\
	t1=(  (m<<10)&0xCC000000)^(  (m<<6)&0x33000000);\
	t2=((m<<3)&0x08880000)  ;\
	t3=(  (m<<4)&0x08800000);\
	T^=(   (m<<9)&0x08000000);\
	t0=t0^t1;\
	t2=t2^t3;\
 	T^=(  (m<<11)&0x70000000);\
 	T^=t0;\
 	T^=t2;
	
#define SANDFunA_128B(M,k,m)\
	  m=ROTL64(M,52);\
	  k=((M<<3 )&0x8000000000000000)^m;\
	  t0=( (m<<5)&0xEEE0000000000000)^( (m<<1)&0x1110000000000000);\
	  t1=( (m<<10)&0xCC00000000000000)^( (m<<6)&0x3300000000000000);\
	  t2=( (m<<3)&0x0888000000000000);\
	  t3=( (m<<4)&0x0880000000000000);\
	  k^=( (m<<9)&0x0800000000000000);\
	t0=t0^t1;\
	t2=t2^t3;\
 	  k^=( (m<<11)&0x7000000000000000);\
 	k^=t0;\
 	k^=t2;
 	
    #define XOR3(a,b,c)\
 _mm256_xor_si256(a, _mm256_xor_si256(b,c))
 	  
void grdtscl(u64i * ll);
void bitsliceAVX2(__m256i * x0, __m256i * x1, __m256i * x2, __m256i * x3,__m256i *  x4,__m256i *  x5, __m256i * x6, __m256i * x7 );
void TestSAND64128();
void TestSAND128128();
void TestSAND64128BSAVX2_32Way();
void TestSAND64128BSAVX2_64Way();
void TestSAND128128BSAVX2_16Way();
void TestSAND128128BSAVX2_32Way();

#endif
