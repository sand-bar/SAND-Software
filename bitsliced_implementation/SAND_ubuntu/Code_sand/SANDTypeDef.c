#include "SANDTypeDef.h"

 
static void swapmove(__m256i * a, __m256i * b, int n, __m256i m) 
{
	__m256i t;
	t= _mm256_srli_epi32(*b, n);
	t= _mm256_xor_si256(*a, t); 
	t= _mm256_and_si256( m,t);	
	*a= _mm256_xor_si256(*a, t); 
	t= _mm256_slli_epi32(t, n);
	*b= _mm256_xor_si256(*b, t); 	
} 

 void bitsliceAVX2(__m256i * x0, __m256i * x1, __m256i * x2, __m256i * x3,__m256i *  x4,__m256i *  x5, __m256i * x6, __m256i * x7 )
{
	__m256i t0= _mm256_set1_epi32(0x55555555)	;
	swapmove(x7, x6, 1, t0 );  
	swapmove(x5, x4, 1, t0 );  
	swapmove(x3, x2, 1, t0 ); 
	swapmove(x1, x0, 1, t0 );  
	t0= _mm256_set1_epi32(0x33333333)	; 
	swapmove(x7, x5, 2, t0 ); 
	swapmove(x6, x4, 2, t0 );  
	swapmove(x3, x1, 2, t0 );  
	swapmove(x2, x0, 2, t0 );  
	t0= _mm256_set1_epi32(0x0f0f0f0f)	; 
	swapmove(x7, x3, 4, t0 );  
	swapmove(x6, x2, 4, t0 );  
	swapmove(x5, x1, 4, t0 ); 
	swapmove(x4, x0, 4, t0 );	
}   

#ifdef  __GNUC__
void grdtscl(u64i *ll)
{ 
u32i lo,hi;

        __asm__ __volatile__
        (
         "rdtsc":"=a"(lo),"=d"(hi)
        );
        *ll= (u64i)hi<<32|lo;                      
} 
#elif _MSC_VER 
 #pragma comment(lib,"grdtscl.lib")

 #endif
 

