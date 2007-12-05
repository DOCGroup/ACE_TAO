//
// MATLAB Compiler: 4.7 (R2007b)
// Date: Wed Dec 05 14:13:02 2007
// Arguments: "-B" "macro_default" "-W" "cpplib:liblsq" "-T" "link:lib"
// "lsqlin_simple2.m" 
//

#ifndef __liblsq_h
#define __liblsq_h 1


#ifdef MX_COMPAT_32_OFF
#ifdef MX_COMPAT_32
#undef MX_COMPAT_32
#endif
#else
#ifndef MX_COMPAT_32
#define MX_COMPAT_32
#endif
#endif
#if defined(__cplusplus) && !defined(mclmcr_h) && defined(__linux__)
#  pragma implementation "mclmcr.h"
#endif
#include "mclmcr.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_liblsq
#define PUBLIC_liblsq_C_API __global
#else
#define PUBLIC_liblsq_C_API /* No import statement needed. */
#endif

#define LIB_liblsq_C_API PUBLIC_liblsq_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_liblsq
#define PUBLIC_liblsq_C_API __declspec(dllexport)
#else
#define PUBLIC_liblsq_C_API __declspec(dllimport)
#endif

#define LIB_liblsq_C_API PUBLIC_liblsq_C_API


#else

#define LIB_liblsq_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_liblsq_C_API 
#define LIB_liblsq_C_API /* No special import/export declaration */
#endif

extern LIB_liblsq_C_API 
bool MW_CALL_CONV liblsqInitializeWithHandlers(mclOutputHandlerFcn error_handler,
                                               mclOutputHandlerFcn print_handler);

extern LIB_liblsq_C_API 
bool MW_CALL_CONV liblsqInitialize(void);

extern LIB_liblsq_C_API 
void MW_CALL_CONV liblsqTerminate(void);



extern LIB_liblsq_C_API 
void MW_CALL_CONV liblsqPrintStackTrace(void);


extern LIB_liblsq_C_API 
bool MW_CALL_CONV mlxLsqlin_simple2(int nlhs, mxArray *plhs[],
                                    int nrhs, mxArray *prhs[]);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_liblsq
#define PUBLIC_liblsq_CPP_API __declspec(dllexport)
#else
#define PUBLIC_liblsq_CPP_API __declspec(dllimport)
#endif

#define LIB_liblsq_CPP_API PUBLIC_liblsq_CPP_API

#else

#if !defined(LIB_liblsq_CPP_API)
#if defined(LIB_liblsq_C_API)
#define LIB_liblsq_CPP_API LIB_liblsq_C_API
#else
#define LIB_liblsq_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_liblsq_CPP_API void MW_CALL_CONV lsqlin_simple2(int nargout
                                                           , mwArray& X
                                                           , const mwArray& C
                                                           , const mwArray& d
                                                           , const mwArray& A
                                                           , const mwArray& b);

#endif

#endif
