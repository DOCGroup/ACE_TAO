/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/config/config.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_CONFIG_CONFIG_H
#define PACE_CONFIG_CONFIG_H

#if !defined (PACE_HAS_POSIX_C_SOURCE)
# if defined (_POSIX_C_SOURCE)
#   define PACE_HAS_POSIX_C_SOURCE _POSIX_C_SOURCE
# endif /* _POSIX_C_SOURCE */
#endif /* ! PACE_HAS_POSIX_C_SOURCE */

#if !defined (PACE_HAS_EXTENSIONS)
# if defined (__EXTENSIONS__)
#   define PACE_HAS_EXTENSIONS __EXTENSIONS__
# endif /* __EXTENSIONS__ */
#endif /* ! PACE_HAS_EXTENSIONS */

#if !defined (PACE_HAS_REENTRANT)
# if defined (PACE_LINUX)
#   ifdef __USE_POSIX
#     define PACE_HAS_REENTRANT 1
#   else  /* ! __USE_POSIX */
#   endif /* ! __USE_POSIX */
# elif defined (PACE_LYNXOS)
/* LynxOS uses _POSIX_REENTRANT_FUNCTIONS, but not _REENTRANT */
# elif defined (PACE_SUNOS)
#   if defined (__EXTENSIONS__) || defined (_REENTRANT) || \
       (_POSIX_C_SOURCE - 0 >= 199506L)
#     define PACE_HAS_REENTRANT 1
#   else  /* ! __EXTENSIONS__ && ! _REENTRANT && _POSIX_C_SOURCE < 199506L */
#   endif /* ! __EXTENSIONS__ && ! _REENTRANT && _POSIX_C_SOURCE < 199506L */
# else  /* other POSIX */
#   define PACE_HAS_REENTRANT 1
# endif /* other POSIX */
#endif /* ! PACE_HAS_REENTRANT */


#if !defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
# if defined (_POSIX_PTHREAD_SEMANTICS)
#   define PACE_HAS_POSIX_PTHREAD_SEMANTICS _POSIX_PTHREAD_SEMANTICS
# endif /* _POSIX_PTHREAD_SEMANTICS */
#endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */


#if PACE_HAS_POSIX == PACE_LYNXOS
# define PACE_NONCONST_ARG_CAST(TYPE) (TYPE)
# define PACE_VOIDSTARTSTAR_ARG_CAST(TYPE) (TYPE)
# define PACE_AIO_SUSPEND_LIST_ARG_CAST (struct aiocb **)
# define PACE_THR_ENTRY_CAST
#elif PACE_HAS_POSIX == DIGITAL_UNIX
# define PACE_NONCONST_ARG_CAST(TYPE) (TYPE)
# define PACE_VOIDSTARTSTAR_ARG_CAST(TYPE)
# define PACE_AIO_SUSPEND_LIST_ARG_CAST (const struct aiocb **)
# define PACE_THR_ENTRY_CAST
#elif PACE_LACKS_POSIX == PACE_VXWORKS
# define PACE_NONCONST_ARG_CAST(TYPE) (TYPE)
# define PACE_VOIDSTARTSTAR_ARG_CAST(TYPE)
# define PACE_AIO_SUSPEND_LIST_ARG_CAST (const struct aiocb **)
# define PACE_THR_ENTRY_CAST(TYPE) (TYPE)
#else  /* ! PACE_LYNXOS */
# define PACE_NONCONST_ARG_CAST(TYPE)
# define PACE_VOIDSTARTSTAR_ARG_CAST(TYPE)
# define PACE_AIO_SUSPEND_LIST_ARG_CAST
# define PACE_THR_ENTRY_CAST
#endif /* ! PACE_LYNXOS */

/* Adding appropriate macros for the different POSIX units of
   functionality that PACE supports.
   These macros define which POSIX functions get included when
   PACE is built. The functions are grouped according to POSIX
   Units of Functionality per the POSIX document ?? pp. ??.
   The following macros correspond to the following POSIX
   Unit of Functionality:

   PACE_HAS_POSIX_SOCK_UOF     -> POSIX Socket UoF
   PACE_HAS_POSIX_SP_UOF       -> POSIX Single Process UoF
   PACE_HAS_POSIX_MP_UOF       -> POSIX Multiple Process UoF
   PACE_HAS_POSIX_SIG_UOF      -> POSIX Signals UoF
   PACE_HAS_POSIX_UG_UOF       -> POSIX User Groups UoF
   PACE_HAS_POSIX_FS_UOF       -> POSIX File System UoF
   PACE_HAS_POSIX_FA_UOF       -> POSIX File Attributes UoF
   PACE_HAS_POSIX_F_UOF        -> POSIX FIFO UoF
   PACE_HAS_POSIX_DI_UOF       -> POSIX Device I/O UoF
   PACE_HAS_POSIX_FM_UOF       -> POSIX File Descriptor Management UoF
   PACE_HAS_POSIX_P_UOF        -> POSIX Pipe UoF
   PACE_HAS_POSIX_DS_UOF       -> POSIX Device Specific UoF
   PACE_HAS_POSIX_SD_UOF       -> POSIX System Database UoF
   PACE_HAS_POSIX_CLS_UOF      -> POSIX C Language Support UoF
   PACE_HAS_POSIX_JC_UOF       -> POSIX Job Control UoF
   PACE_HAS_POSIX_UGR_UOF      -> POSIX User Groups (Reentrant) UoF
   PACE_HAS_POSIX_FL_UOF       -> POSIX File Locking UoF
   PACE_HAS_POSIX_CLSR_UOF     -> POSIX C Language Support (Reentrant) UoF
   PACE_HAS_POSIX_SDR_UOF      -> POSIX System Database (Reentrant) UoF
   PACE_HAS_POSIX_NONUOF_FUNCS -> Any POSIX function in PACE that is not in
                                  one of the above units of functionality.
 */
#if PACE_HAS_ALL_POSIX_FUNCS
# define PACE_HAS_POSIX_UOF          1
# define PACE_HAS_POSIX_SP_UOF       1
# define PACE_HAS_POSIX_MP_UOF       1
# define PACE_HAS_POSIX_SIG_UOF      1
# define PACE_HAS_POSIX_UG_UOF       1
# define PACE_HAS_POSIX_FS_UOF       1
# define PACE_HAS_POSIX_FA_UOF       1
# define PACE_HAS_POSIX_F_UOF        1
# define PACE_HAS_POSIX_DI_UOF       1
# define PACE_HAS_POSIX_FM_UOF       1
# define PACE_HAS_POSIX_P_UOF        1
# define PACE_HAS_POSIX_DS_UOF       1
# define PACE_HAS_POSIX_SD_UOF       1
# define PACE_HAS_POSIX_CLS_UOF      1
# define PACE_HAS_POSIX_JC_UOF       1
# define PACE_HAS_POSIX_UGR_UOF      1
# define PACE_HAS_POSIX_FL_UOF       1
# define PACE_HAS_POSIX_CLSR_UOF     1
# define PACE_HAS_POSIX_SDR_UOF      1
# define PACE_HAS_POSIX_NONUOF_FUNCS 1
#else
# if !defined PACE_HAS_POSIX_SOCK_UOF
#   define PACE_HAS_POSIX_SOCK_UOF 0
# endif /* ! PACE_HAS_POSIX_SOCK_UOF */
# if !defined PACE_HAS_POSIX_SP_UOF
#   define PACE_HAS_POSIX_SP_UOF 0
# endif /* ! PACE_HAS_POSIX_SP_UOF */
# if !defined PACE_HAS_POSIX_MP_UOF
#   define PACE_HAS_POSIX_MP_UOF 0
# endif /* ! PACE_HAS_POSIX_MP_UOF */
# if !defined PACE_HAS_POSIX_SIG_UOF
#   define PACE_HAS_POSIX_SIG_UOF 0
# endif /* ! PACE_HAS_POSIX_SIG_UOF */
# if !defined PACE_HAS_POSIX_UG_UOF
#   define PACE_HAS_POSIX_UG_UOF 0
# endif /* ! PACE_HAS_POSIX_UG_UOF */
# if !defined PACE_HAS_POSIX_FS_UOF
#   define PACE_HAS_POSIX_FS_UOF 0
# endif /* ! PACE_HAS_POSIX_FS_UOF */
# if !defined PACE_HAS_POSIX_FA_UOF
#   define PACE_HAS_POSIX_FA_UOF 0
# endif /* ! PACE_HAS_POSIX_FA_UOF */
# if !defined PACE_HAS_POSIX_F_UOF
#   define PACE_HAS_POSIX_F_UOF 0
# endif /* ! PACE_HAS_POSIX_F_UOF */
# if !defined PACE_HAS_POSIX_DI_UOF
#   define PACE_HAS_POSIX_DI_UOF 0
# endif /* ! PACE_HAS_POSIX_DI_UOF */
# if !defined PACE_HAS_POSIX_FM_UOF
#   define PACE_HAS_POSIX_FM_UOF 0
# endif /* ! PACE_HAS_POSIX_FM_UOF */
# if !defined PACE_HAS_POSIX_P_UOF
#   define PACE_HAS_POSIX_P_UOF 0
# endif /* ! PACE_HAS_POSIX_P_UOF */
# if !defined PACE_HAS_POSIX_DS_UOF
#   define PACE_HAS_POSIX_DS_UOF 0
# endif /* ! PACE_HAS_POSIX_DS_UOF */
# if !defined PACE_HAS_POSIX_SD_UOF
#   define PACE_HAS_POSIX_SD_UOF 0
# endif /* ! PACE_HAS_POSIX_SD_UOF */
# if !defined PACE_HAS_POSIX_CLS_UOF
#   define PACE_HAS_POSIX_CLS_UOF 0
# endif /* ! PACE_HAS_POSIX_CLS_UOF */
# if !defined PACE_HAS_POSIX_JC_UOF
#   define PACE_HAS_POSIX_JC_UOF 0
# endif /* ! PACE_HAS_POSIX_JC_UOF */
# if !defined PACE_HAS_POSIX_UGR_UOF
#   define PACE_HAS_POSIX_UGR_UOF 0
# endif /* ! PACE_HAS_POSIX_UGR_UOF */
# if !defined PACE_HAS_POSIX_FL_UOF
#   define PACE_HAS_POSIX_FL_UOF 0
# endif /* ! PACE_HAS_POSIX_FL_UOF */
# if !defined PACE_HAS_POSIX_CLSR_UOF
#   define PACE_HAS_POSIX_CLSR_UOF 0
# endif /* ! PACE_HAS_POSIX_CLSR_UOF */
# if !defined PACE_HAS_POSIX_SDR_UOF
#   define PACE_HAS_POSIX_SDR_UOF 0
# endif /* ! PACE_HAS_POSIX_SDR_UOF */
# if !defined PACE_HAS_POSIX_NONUOF_FUNCS
#   define PACE_HAS_POSIX_NONUOF_FUNCS 0
# endif /* ! PACE_HAS_POSIX_NONUOF_FUNCS */
#endif /* PACE_HAS_ALL_POSIX_FUNCS */

#if (PACE_WIN32)

# if defined (PACE_AS_STATIC_LIBS)
#   if !defined (PACE_HAS_DLL)
#     define PACE_HAS_DLL 0
#   endif /* ! PACE_HAS_DLL */
# else
#   if !defined (PACE_HAS_DLL)
#     define PACE_HAS_DLL 1
#   endif /* ! PACE_HAS_DLL */
# endif /* PACE_AS_STATIC_LIBS */

#define PACE_Proper_Export_Flag __declspec (dllexport)
#define PACE_Proper_Import_Flag __declspec (dllimport)

#endif /* PACE_WIN32 */

#if defined (PACE_HAS_DLL) && (PACE_HAS_DLL == 1)
#  if defined (PACE_BUILD_DLL)
#    define PACE_Export PACE_Proper_Export_Flag
#  else /* PACE_BUILD_DLL */
#    define PACE_Export PACE_Proper_Import_Flag
#  endif /* PACE_BUILD_DLL */
#else /* PACE_HAS_DLL != 1 */
#  define PACE_Export
#endif /* PACE_HAS_DLL != 1 */

#endif /* PACE_CONFIG_CONFIG_H */
