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

/**
* \def INLINE Marks a function as to be inlined
*/
#ifdef _MSC_VER
#define INLINE __inline
#else
#define INLINE inline
#endif

/**
* \def EXTERN_INLINE Marks a function as to be inlined, but also externally defined
*/
#define EXTERN_INLINE extern INLINE

#endif