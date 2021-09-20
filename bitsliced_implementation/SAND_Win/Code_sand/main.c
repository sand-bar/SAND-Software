#include "BATTypeDef.h"  

int main()
{ 
 //TestBAT64128();
  TestBAT64128BSAVX2_32Way();
 TestBAT64128BSAVX2_64Way(); 
 //TestBAT128128();
 TestBAT128128BSAVX2_16Way();
 TestBAT128128BSAVX2_32Way();
return 1; 
 
} 
