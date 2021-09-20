#include "SANDTypeDef.h"  

int main()
{ 
 //TestSAND64128();
  TestSAND64128BSAVX2_32Way();
 TestSAND64128BSAVX2_64Way(); 
 //TestSAND128128();
 TestSAND128128BSAVX2_16Way();
 TestSAND128128BSAVX2_32Way();
return 1; 
 
} 
