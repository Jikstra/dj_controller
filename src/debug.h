#pragma once

//#define DEBUG // Uncomment to enable debugging
 
#ifdef DEBUG 
  #define IFDEBUG(x) x
  #define IFNDEBUG(x)
#else 
  #define IFDEBUG(x)
  #define IFNDEBUG(x) x
#endif
