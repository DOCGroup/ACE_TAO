/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    compiler.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_CONFIG_COMPILER_H
#define PACE_CONFIG_COMPILER_H


#include <errno.h>
int errno;


# if defined (_POSIX_C_SOURCE)
# define PACE_POSIX_C_SOURCE _POSIX_C_SOURCE
# endif /* _POSIX_C_SOURCE */

# if defined (__cplusplus)
# define PACE_INLINE inline
# define PACE_HAS_INLINE
# define PACE_HAS_CPLUSPLUS
# else  /* ! __cplusplus */
# define PACE_INLINE static
# endif /* ! __cplusplus */

# define PACE_RETURN_NO_SUPPORT(X) \
do {                               \
  errno = ENOTSUP;                 \
  return -1;                       \
} while (0)

# define PACE_SET_ERRNO(X)         \
errno=X















/* From ACE_OS ------------------------------------------------------------- */

# if defined (PACE_LACKS_INLINE_FUNCTIONS) && !defined (PACE_NO_INLINE)
#   define PACE_NO_INLINE
# endif /* defined (PACE_LACKS_INLINE_FUNCTIONS) && !defined (PACE_NO_INLINE) */

# if defined (PACE_NO_INLINE)
/* PACE inlining has been explicitly disabled.  Implement
 * internally within PACE by undefining __PACE_INLINE__.
 */
#   undef __PACE_INLINE__
# endif /* ! PACE_NO_INLINE */

# if !defined (PACE_NOT_POSIX)
# if !defined (PACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* PACE_LACKS_PRAGMA_ONCE */
# endif /* ! PACE_NOT_POSIX */

# if !defined (PACE_HAS_POSITION_INDEPENDENT_POINTERS)
#   define PACE_HAS_POSITION_INDEPENDENT_POINTERS 1
# endif /* PACE_HAS_POSITION_INDEPENDENT_POINTERS */

# if defined (PACE_HAS_ANSI_CASTS)

#   define PACE_sap_any_cast(TYPE)                                      reinterpret_cast<TYPE> (const_cast<PACE_Addr &> (PACE_Addr::sap_any))

#   define PACE_static_cast(TYPE, EXPR)                                 static_cast<TYPE> (EXPR)
#   define PACE_static_cast_1_ptr(TYPE, T1, EXPR)                       static_cast<TYPE<T1> *> (EXPR)
#   define PACE_static_cast_2_ptr(TYPE, T1, T2, EXPR)                   static_cast<TYPE<T1, T2> *> (EXPR)
#   define PACE_static_cast_3_ptr(TYPE, T1, T2, T3, EXPR)               static_cast<TYPE<T1, T2, T3> *> (EXPR)
#   define PACE_static_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)           static_cast<TYPE<T1, T2, T3, T4> *> (EXPR)
#   define PACE_static_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)       static_cast<TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#   define PACE_static_cast_1_ref(TYPE, T1, EXPR)                       static_cast<TYPE<T1> &> (EXPR)
#   define PACE_static_cast_2_ref(TYPE, T1, T2, EXPR)                   static_cast<TYPE<T1, T2> &> (EXPR)
#   define PACE_static_cast_3_ref(TYPE, T1, T2, T3, EXPR)               static_cast<TYPE<T1, T2, T3> &> (EXPR)
#   define PACE_static_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)           static_cast<TYPE<T1, T2, T3, T4> &> (EXPR)
#   define PACE_static_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)       static_cast<TYPE<T1, T2, T3, T4, T5> &> (EXPR)

#   define PACE_const_cast(TYPE, EXPR)                                  const_cast<TYPE> (EXPR)
#   define PACE_const_cast_1_ptr(TYPE, T1, EXPR)                        const_cast<TYPE<T1> *> (EXPR)
#   define PACE_const_cast_2_ptr(TYPE, T1, T2, EXPR)                    const_cast<TYPE<T1, T2> *> (EXPR)
#   define PACE_const_cast_3_ptr(TYPE, T1, T2, T3, EXPR)                const_cast<TYPE<T1, T2, T3> *> (EXPR)
#   define PACE_const_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)            const_cast<TYPE<T1, T2, T3, T4> *> (EXPR)
#   define PACE_const_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)        const_cast<TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#   define PACE_const_cast_1_ref(TYPE, T1, EXPR)                        const_cast<TYPE<T1> &> (EXPR)
#   define PACE_const_cast_2_ref(TYPE, T1, T2, EXPR)                    const_cast<TYPE<T1, T2> &> (EXPR)
#   define PACE_const_cast_3_ref(TYPE, T1, T2, T3, EXPR)                const_cast<TYPE<T1, T2, T3> &> (EXPR)
#   define PACE_const_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)            const_cast<TYPE<T1, T2, T3, T4> &> (EXPR)
#   define PACE_const_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)        const_cast<TYPE<T1, T2, T3, T4, T5> &> (EXPR)

#   define PACE_reinterpret_cast(TYPE, EXPR)                            reinterpret_cast<TYPE> (EXPR)
#   define PACE_reinterpret_cast_1_ptr(TYPE, T1, EXPR)                  reinterpret_cast<TYPE<T1> *> (EXPR)
#   define PACE_reinterpret_cast_2_ptr(TYPE, T1, T2, EXPR)              reinterpret_cast<TYPE<T1, T2> *> (EXPR)
#   define PACE_reinterpret_cast_3_ptr(TYPE, T1, T2, T3, EXPR)          reinterpret_cast<TYPE<T1, T2, T3> *> (EXPR)
#   define PACE_reinterpret_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)      reinterpret_cast<TYPE<T1, T2, T3, T4> *> (EXPR)
#   define PACE_reinterpret_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)  reinterpret_cast<TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#   define PACE_reinterpret_cast_1_ref(TYPE, T1, EXPR)                  reinterpret_cast<TYPE<T1> &> (EXPR)
#   define PACE_reinterpret_cast_2_ref(TYPE, T1, T2, EXPR)              reinterpret_cast<TYPE<T1, T2> &> (EXPR)
#   define PACE_reinterpret_cast_3_ref(TYPE, T1, T2, T3, EXPR)          reinterpret_cast<TYPE<T1, T2, T3> &> (EXPR)
#   define PACE_reinterpret_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)      reinterpret_cast<TYPE<T1, T2, T3, T4> &> (EXPR)
#   define PACE_reinterpret_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)  reinterpret_cast<TYPE<T1, T2, T3, T4, T5> &> (EXPR)

#   if defined (PACE_LACKS_RTTI)
#     define PACE_dynamic_cast(TYPE, EXPR)                              static_cast<TYPE> (EXPR)
#     define PACE_dynamic_cast_1_ptr(TYPE, T1, EXPR)                    static_cast<TYPE<T1> *> (EXPR)
#     define PACE_dynamic_cast_2_ptr(TYPE, T1, T2, EXPR)                static_cast<TYPE<T1, T2> *> (EXPR)
#     define PACE_dynamic_cast_3_ptr(TYPE, T1, T2, T3, EXPR)            static_cast<TYPE<T1, T2, T3> *> (EXPR)
#     define PACE_dynamic_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)        static_cast<TYPE<T1, T2, T3, T4> *> (EXPR)
#     define PACE_dynamic_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)    static_cast<TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#     define PACE_dynamic_cast_1_ref(TYPE, T1, EXPR)                    static_cast<TYPE<T1> &> (EXPR)
#     define PACE_dynamic_cast_2_ref(TYPE, T1, T2, EXPR)                static_cast<TYPE<T1, T2> &> (EXPR)
#     define PACE_dynamic_cast_3_ref(TYPE, T1, T2, T3, EXPR)            static_cast<TYPE<T1, T2, T3> &> (EXPR)
#     define PACE_dynamic_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)        static_cast<TYPE<T1, T2, T3, T4> &> (EXPR)
#     define PACE_dynamic_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)    static_cast<TYPE<T1, T2, T3, T4, T5> &> (EXPR)
#   else  /* ! PACE_LACKS_RTTI */
#     define PACE_dynamic_cast(TYPE, EXPR)                              dynamic_cast<TYPE> (EXPR)
#     define PACE_dynamic_cast_1_ptr(TYPE, T1, EXPR)                    dynamic_cast<TYPE<T1> *> (EXPR)
#     define PACE_dynamic_cast_2_ptr(TYPE, T1, T2, EXPR)                dynamic_cast<TYPE<T1, T2> *> (EXPR)
#     define PACE_dynamic_cast_3_ptr(TYPE, T1, T2, T3, EXPR)            dynamic_cast<TYPE<T1, T2, T3> *> (EXPR)
#     define PACE_dynamic_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)        dynamic_cast<TYPE<T1, T2, T3, T4> *> (EXPR)
#     define PACE_dynamic_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)    dynamic_cast<TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#     define PACE_dynamic_cast_1_ref(TYPE, T1, EXPR)                    dynamic_cast<TYPE<T1> &> (EXPR)
#     define PACE_dynamic_cast_2_ref(TYPE, T1, T2, EXPR)                dynamic_cast<TYPE<T1, T2> &> (EXPR)
#     define PACE_dynamic_cast_3_ref(TYPE, T1, T2, T3, EXPR)            dynamic_cast<TYPE<T1, T2, T3> &> (EXPR)
#     define PACE_dynamic_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)        dynamic_cast<TYPE<T1, T2, T3, T4> &> (EXPR)
#     define PACE_dynamic_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)    dynamic_cast<TYPE<T1, T2, T3, T4, T5> &> (EXPR)
#   endif /* ! PACE_LACKS_RTTI */

# else

#   define PACE_sap_any_cast(TYPE)                                      ((TYPE) (PACE_Addr::sap_any))

#   define PACE_static_cast(TYPE, EXPR)                                 ((TYPE) (EXPR))
#   define PACE_static_cast_1_ptr(TYPE, T1, EXPR)                       ((TYPE<T1> *) (EXPR))
#   define PACE_static_cast_2_ptr(TYPE, T1, T2, EXPR)                   ((TYPE<T1, T2> *) (EXPR))
#   define PACE_static_cast_3_ptr(TYPE, T1, T2, T3, EXPR)               ((TYPE<T1, T2, T3> *) (EXPR))
#   define PACE_static_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)           ((TYPE<T1, T2, T3, T4> *) (EXPR))
#   define PACE_static_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)       ((TYPE<T1, T2, T3, T4, T5> *) (EXPR))
#   define PACE_static_cast_1_ref(TYPE, T1, EXPR)                       ((TYPE<T1> &) (EXPR))
#   define PACE_static_cast_2_ref(TYPE, T1, T2, EXPR)                   ((TYPE<T1, T2> &) (EXPR))
#   define PACE_static_cast_3_ref(TYPE, T1, T2, T3, EXPR)               ((TYPE<T1, T2, T3> &) (EXPR))
#   define PACE_static_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)           ((TYPE<T1, T2, T3, T4> &) (EXPR))
#   define PACE_static_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)       ((TYPE<T1, T2, T3, T4, T5> &) (EXPR))

#   define PACE_const_cast(TYPE, EXPR)                                  ((TYPE) (EXPR))
#   define PACE_const_cast_1_ptr(TYPE, T1, EXPR)                        ((TYPE<T1> *) (EXPR))
#   define PACE_const_cast_2_ptr(TYPE, T1, T2, EXPR)                    ((TYPE<T1, T2> *) (EXPR))
#   define PACE_const_cast_3_ptr(TYPE, T1, T2, T3, EXPR)                ((TYPE<T1, T2, T3> *) (EXPR))
#   define PACE_const_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)            ((TYPE<T1, T2, T3, T4> *) (EXPR))
#   define PACE_const_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)        ((TYPE<T1, T2, T3, T4, T5> *) (EXPR))
#   define PACE_const_cast_1_ref(TYPE, T1, EXPR)                        ((TYPE<T1> &) (EXPR))
#   define PACE_const_cast_2_ref(TYPE, T1, T2, EXPR)                    ((TYPE<T1, T2> &) (EXPR))
#   define PACE_const_cast_3_ref(TYPE, T1, T2, T3, EXPR)                ((TYPE<T1, T2, T3> &) (EXPR))
#   define PACE_const_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)            ((TYPE<T1, T2, T3, T4> &) (EXPR))
#   define PACE_const_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)        ((TYPE<T1, T2, T3, T4, T5> &) (EXPR))

#   define PACE_reinterpret_cast(TYPE, EXPR)                            ((TYPE) (EXPR))
#   define PACE_reinterpret_cast_1_ptr(TYPE, T1, EXPR)                  ((TYPE<T1> *) (EXPR))
#   define PACE_reinterpret_cast_2_ptr(TYPE, T1, T2, EXPR)              ((TYPE<T1, T2> *) (EXPR))
#   define PACE_reinterpret_cast_3_ptr(TYPE, T1, T2, T3, EXPR)          ((TYPE<T1, T2, T3> *) (EXPR))
#   define PACE_reinterpret_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)      ((TYPE<T1, T2, T3, T4> *) (EXPR))
#   define PACE_reinterpret_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)  ((TYPE<T1, T2, T3, T4, T5> *) (EXPR))
#   define PACE_reinterpret_cast_1_ref(TYPE, T1, EXPR)                  ((TYPE<T1> &) (EXPR))
#   define PACE_reinterpret_cast_2_ref(TYPE, T1, T2, EXPR)              ((TYPE<T1, T2> &) (EXPR))
#   define PACE_reinterpret_cast_3_ref(TYPE, T1, T2, T3, EXPR)          ((TYPE<T1, T2, T3> &) (EXPR))
#   define PACE_reinterpret_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)      ((TYPE<T1, T2, T3, T4> &) (EXPR))
#   define PACE_reinterpret_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)  ((TYPE<T1, T2, T3, T4, T5> &) (EXPR))

#   define PACE_dynamic_cast(TYPE, EXPR)                                ((TYPE) (EXPR))
#   define PACE_dynamic_cast_1_ptr(TYPE, T1, EXPR)                      ((TYPE<T1> *) (EXPR))
#   define PACE_dynamic_cast_2_ptr(TYPE, T1, T2, EXPR)                  ((TYPE<T1, T2> *) (EXPR))
#   define PACE_dynamic_cast_3_ptr(TYPE, T1, T2, T3, EXPR)              ((TYPE<T1, T2, T3> *) (EXPR))
#   define PACE_dynamic_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)          ((TYPE<T1, T2, T3, T4> *) (EXPR))
#   define PACE_dynamic_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)      ((TYPE<T1, T2, T3, T4, T5> *) (EXPR))
#   define PACE_dynamic_cast_1_ref(TYPE, T1, EXPR)                      ((TYPE<T1> &) (EXPR))
#   define PACE_dynamic_cast_2_ref(TYPE, T1, T2, EXPR)                  ((TYPE<T1, T2> &) (EXPR))
#   define PACE_dynamic_cast_3_ref(TYPE, T1, T2, T3, EXPR)              ((TYPE<T1, T2, T3> &) (EXPR))
#   define PACE_dynamic_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)          ((TYPE<T1, T2, T3, T4> &) (EXPR))
#   define PACE_dynamic_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)      ((TYPE<T1, T2, T3, T4, T5> &) (EXPR))
# endif /* PACE_HAS_ANSI_CASTS */

# if !defined (PACE_CAST_CONST)
/* Sun CC 4.2, for example, requires const in reinterpret casts of
 * data members in const member functions.  But, other compilers
 * complain about the useless const.  This keeps everyone happy.
 */
#   if defined (__SUNPRO_CC)
#     define PACE_CAST_CONST const
#   else  /* ! __SUNPRO_CC */
#     define PACE_CAST_CONST
#   endif /* ! __SUNPRO_CC */
# endif /* ! PACE_CAST_CONST */

/* Deal with MSVC++ insanity for CORBA... */
# if defined (PACE_HAS_BROKEN_NAMESPPACES)
#   define PACE_CORBA_1(NAME) CORBA_##NAME
#   define PACE_CORBA_2(TYPE, NAME) CORBA_##TYPE##_##NAME
#   define PACE_CORBA_3(TYPE, NAME) CORBA_##TYPE::NAME
#   define PACE_NESTED_CLASS(TYPE, NAME) NAME
# else  /* ! PACE_HAS_BROKEN_NAMESPPACES */
#   define PACE_CORBA_1(NAME) CORBA::NAME
#   define PACE_CORBA_2(TYPE, NAME) CORBA::TYPE::NAME
#   define PACE_CORBA_3(TYPE, NAME) CORBA::TYPE::NAME
#   define PACE_NESTED_CLASS(TYPE, NAME) TYPE::NAME
# endif /* ! PACE_HAS_BROKEN_NAMESPPACES */


#endif /* PACE_CONFIG_COMPILER_H */
