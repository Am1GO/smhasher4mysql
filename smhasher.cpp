// SMHasher code is from SMHasher project, authored by Austin Appleby, et al.
// http://code.google.com/p/smhasher/

#include "MurmurHash3.h"

#ifdef STANDARD
// STANDARD is defined. Don't use any MySQL functions
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __WIN__
typedef unsigned __int64 ulonglong;     // Microsoft's 64 bit types
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif  //__WIN__

#else  // not STANDARD
#include <my_global.h>
#include <my_sys.h>
#include <m_string.h>

#if defined(MYSQL_SERVER)
#include <m_string.h>  // To get strcpy()
#else // when compiled as standalone
#include <string.h>
#endif

#endif  // STANDARD

#include <mysql.h>
#include <ctype.h>
//static pthread_mutex_t LOCK_hostname;

#ifdef HAVE_DLOPEN
//------------------------------------
extern "C"
{
    my_bool   my_hash64_init   (UDF_INIT *initid, UDF_ARGS *args, char *message);
    void      my_hash64_deinit (UDF_INIT *initid);
    ulonglong my_hash64        (UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
}
//------------------------------------
my_bool my_hash64_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (args->arg_count != 1)
    {
        strcpy(message, "my_hash64() requires exactly one argument");
        return 1;
    }

    if ( args->arg_type[0] != STRING_RESULT )
    {
        strcpy(message, "my_hash64() requires a string");
        return 1;
    }
    
    return 0;
}
//------------------------------------
void my_hash64_deinit(UDF_INIT *initid)
{
    ;// nothing to do here
}
//------------------------------------
ulonglong my_hash64(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
    // STRING_RESULT: char **args (args->args[i]), unsigned long *lenghts (args->lengths[i])
    
    *is_null = 0;
    
    unsigned char out[16];
    
    MurmurHash3_x64_128((void *)args->args[0], args->lengths[0], 0, &out);
    
    ulonglong result;
    
    result  = ((ulonglong) out[0]) << 56;
    result |= ((ulonglong) out[1]) << 48;
    result |= ((ulonglong) out[2]) << 40;
    result |= ((ulonglong) out[3]) << 32;
    result |= ((ulonglong) out[4]) << 24;
    result |= ((ulonglong) out[5]) << 16;
    result |= ((ulonglong) out[6]) << 8;
    result |= ((ulonglong) out[7]);
    
    return result;
}
//------------------------------------
#endif // HAVE_DLOPEN

