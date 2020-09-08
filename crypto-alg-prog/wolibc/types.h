#ifndef TYPES_H
#define TYPES_H

#if SIZE_MAX == UINT_MAX     // common 32 bit case 
typedef int ssize_t;        
typedef unsigned int size_t;
#elif SIZE_MAX == ULONG_MAX  // linux 64 bits
typedef long ssize_t;       
typedef unsigned long size_t;
#elif SIZE_MAX == ULLONG_MAX // windows 64 bits
typedef long long ssize_t;  
typedef unsigned long long size_t;
#elif SIZE_MAX == USHRT_MAX  // is this even possible?
typedef short ssize_t;      
typedef unsigned short size_t;
#else
#error Platform has exotic SIZE_MAX
#endif 

#endif // TYPES_H
