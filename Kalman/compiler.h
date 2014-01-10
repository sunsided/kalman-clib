#ifndef _COMPILER_H_
#define _COMPILER_H_

/**
* \def RESTRICT Marks a restricted pointer 
*/
#ifdef __GNUC__
#define RESTRICT restrict
#else
#define RESTRICT
#endif

#endif