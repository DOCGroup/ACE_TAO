// -*- C++ -*-

//==========================================================================
/**
 *  @file   config-all.h
 *
 *  $Id$
 *
 *  @author (Originally in OS.h)Doug Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//==========================================================================

#ifndef ACE_CONFIG_ALL_H
#define ACE_CONFIG_ALL_H

#include /**/ "ace/pre.h"

#include "ace/config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// By default we perform no tracing on the OS layer, otherwise the
// coupling between the OS layer and Log_Msg is too tight.  But the
// application can override the default if they wish to.
# if !defined(ACE_OS_TRACE)
#  define ACE_OS_TRACE(X)
# endif /* ACE_OS_TRACE */

#define ACE_BITS_PER_ULONG (8 * sizeof (u_long))

#if !defined (ACE_OSTREAM_TYPE)
# if defined (ACE_LACKS_IOSTREAM_TOTALLY)
#   define ACE_OSTREAM_TYPE FILE
# else  /* ! ACE_LACKS_IOSTREAM_TOTALLY */
#   define ACE_OSTREAM_TYPE ostream
# endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */
#endif /* ! ACE_OSTREAM_TYPE */

#if !defined (ACE_DEFAULT_LOG_STREAM)
# if defined (ACE_LACKS_IOSTREAM_TOTALLY)
#   define ACE_DEFAULT_LOG_STREAM 0
# else  /* ! ACE_LACKS_IOSTREAM_TOTALLY */
#   define ACE_DEFAULT_LOG_STREAM (&cerr)
# endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */
#endif /* ! ACE_DEFAULT_LOG_STREAM */

// These two are only for backward compatibility. You should avoid
// using them if not necessary.
# define ACE_SYNCH_1 ACE_SYNCH_DECL
# define ACE_SYNCH_2 ACE_SYNCH_USE

// For Win32 compatibility...
# if !defined (ACE_WSOCK_VERSION)
#   define ACE_WSOCK_VERSION 0, 0
# endif /* ACE_WSOCK_VERSION */

# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#   define ACE_MT(X) X
#   if !defined (_REENTRANT)
#     define _REENTRANT
#   endif /* _REENTRANT */
# else
#   define ACE_MT(X)
# endif /* ACE_MT_SAFE */

# if defined (ACE_HAS_USING_KEYWORD)
#   define ACE_USING using
# else
#   define ACE_USING
# endif /* ACE_HAS_USING_KEYWORD */

# if defined (ACE_HAS_TYPENAME_KEYWORD)
#   define ACE_TYPENAME typename
# else
#   define ACE_TYPENAME
# endif /* ACE_HAS_TYPENAME_KEYWORD */

# if defined (ACE_HAS_STD_TEMPLATE_SPECIALIZATION)
#   define ACE_TEMPLATE_SPECIALIZATION template<>
# else
#   define ACE_TEMPLATE_SPECIALIZATION
# endif /* ACE_HAS_STD_TEMPLATE_SPECIALIZATION */

# if defined (ACE_HAS_STD_TEMPLATE_METHOD_SPECIALIZATION)
#   define ACE_TEMPLATE_METHOD_SPECIALIZATION template<>
# else
#   define ACE_TEMPLATE_METHOD_SPECIALIZATION
# endif /* ACE_HAS_STD_TEMPLATE_SPECIALIZATION */

# if defined (ACE_HAS_STD_TEMPLATE_CLASS_MEMBER_SPECIALIZATION)
#   define ACE_TEMPLATE_CLASS_MEMBER_SPECIALIZATION
# else
#   define ACE_TEMPLATE_CLASS_MEMBER_SPECIALIZATION template<>
# endif /* ACE_HAS_STD_TEMPLATE_CLASS_MEMBER_SPECIALIZATION */

// =========================================================================
// Perfect Multicast filting refers to RFC 3376, where a socket is only
// delivered dgrams for groups joined even if it didn't bind the group
// address.  We turn this option off by default, although most OS's
// except for Windows and Solaris probably lack perfect filtering.
// =========================================================================

# if !defined (ACE_LACKS_PERFECT_MULTICAST_FILTERING)
#   define ACE_LACKS_PERFECT_MULTICAST_FILTERING 0
# endif /* ACE_LACKS_PERFECT_MULTICAST_FILTERING */

// =========================================================================
// Enable/Disable Features By Default
// =========================================================================

# if !defined (ACE_HAS_POSITION_INDEPENDENT_POINTERS)
#   define ACE_HAS_POSITION_INDEPENDENT_POINTERS 1
# endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS */

// =========================================================================
// RCSID Macros
// =========================================================================

// By default, DO include RCS Id strings in object code.
#if ! defined (ACE_USE_RCSID)
#  define ACE_USE_RCSID 1
#endif /* #if ! defined (ACE_USE_RCSID) */

#if (defined (ACE_USE_RCSID) && (ACE_USE_RCSID != 0))
#  if ! defined (ACE_RCSID)

   // This hack has the following purposes:
   // 1. To define the RCS id string variable as a static char*, so
   //    that there won't be any duplicate extern symbols at link
   //    time.
   // 2. To have a RCS id string variable with a unique name for each
   //    file.
   // 3. To avoid warnings of the type "variable declared and never
   //    used".

#    define ACE_RCSID(path, file, id) \
      static inline const char* get_rcsid_ ## path ## _ ## file (const char*) \
      { \
        return id ; \
      } \
      static const char* rcsid_ ## path ## _ ## file = \
        get_rcsid_ ## path ## _ ## file ( rcsid_ ## path ## _ ## file ) ;

#  endif /* #if ! defined (ACE_RCSID) */
#else

   // RCS id strings are not wanted.
#  if defined (ACE_RCSID)
#    undef ACE_RCSID
#  endif /* #if defined (ACE_RCSID) */
#  define ACE_RCSID(path, file, id) /* noop */
#endif /* #if (defined (ACE_USE_RCSID) && (ACE_USE_RCSID != 0)) */

// =========================================================================
// INLINE macros
//
// These macros handle all the inlining of code via the .i or .inl files
// =========================================================================

#if defined (ACE_LACKS_INLINE_FUNCTIONS) && !defined (ACE_NO_INLINE)
#  define ACE_NO_INLINE
#endif /* defined (ACE_LACKS_INLINE_FUNCTIONS) && !defined (ACE_NO_INLINE) */

// ACE inlining has been explicitly disabled.  Implement
// internally within ACE by undefining __ACE_INLINE__.
#if defined (ACE_NO_INLINE)
#  undef __ACE_INLINE__
#endif /* ! ACE_NO_INLINE */

#if defined (__ACE_INLINE__)
#  define ACE_INLINE inline
#  if !defined (ACE_HAS_INLINED_OSCALLS)
#    define ACE_HAS_INLINED_OSCALLS
#  endif /* !ACE_HAS_INLINED_OSCALLS */
#else
#  define ACE_INLINE
#endif /* __ACE_INLINE__ */

# if !defined (ACE_HAS_GNUC_BROKEN_TEMPLATE_INLINE_FUNCTIONS)
#   define ACE_INLINE_FOR_GNUC ACE_INLINE
# else
#   define ACE_INLINE_FOR_GNUC
# endif /* ACE_HAS_GNUC_BROKEN_TEMPLATE_INLINE_FUNCTIONS */

// Some ACE classes always use inline functions to maintain high
// performance, but some platforms have buggy inline function support.
// In this case, we don't use inline with them.
# if defined (ACE_LACKS_INLINE_FUNCTIONS)
#   if defined (ASYS_INLINE)
#     undef ASYS_INLINE
#   endif /* ASYS_INLINE */
#   define ASYS_INLINE
#   if defined (ACE_HAS_INLINED_OSCALLS)
#     undef ACE_HAS_INLINED_OSCALLS
#   endif /* ACE_HAS_INLINED_OSCALLS */
# else
#   define ASYS_INLINE inline
# endif /* ACE_LACKS_INLINE_FUNCTIONS */

// =========================================================================
// EXPLICIT macro
// =========================================================================

# if defined (ACE_HAS_EXPLICIT_KEYWORD)
#   define ACE_EXPLICIT explicit
# else  /* ! ACE_HAS_EXPLICIT_KEYWORD */
#   define ACE_EXPLICIT
# endif /* ! ACE_HAS_EXPLICIT_KEYWORD */

// =========================================================================
// MUTABLE macro
// =========================================================================

# if defined (ACE_HAS_MUTABLE_KEYWORD)
#   define ACE_MUTABLE mutable
#   define ACE_CONST_WHEN_MUTABLE const // Addition #1
# else  /* ! ACE_HAS_MUTABLE_KEYWORD */
#   define ACE_MUTABLE
#   define ACE_CONST_WHEN_MUTABLE       // Addition #2
# endif /* ! ACE_HAS_MUTABLE_KEYWORD */

// ============================================================================
// EXPORT macros
//
// Since Win32 DLL's do not export all symbols by default, they must be
// explicitly exported (which is done by *_Export macros).
// ============================================================================

// Win32 should have already defined the macros in config-win32-common.h
#if !defined (ACE_HAS_CUSTOM_EXPORT_MACROS)
#  define ACE_Proper_Export_Flag
#  define ACE_Proper_Import_Flag
#  define ACE_EXPORT_SINGLETON_DECLARATION(T)
#  define ACE_IMPORT_SINGLETON_DECLARATION(T)
#  define ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  define ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ACE_HAS_CUSTOM_EXPORT_MACROS */

// This is a whim of mine -- that instead of annotating a class with
// ACE_Export in its declaration, we make the declaration near the TOP
// of the file with ACE_DECLARE_EXPORT.
// TS = type specifier (e.g., class, struct, int, etc.)
// ID = identifier
// So, how do you use it?  Most of the time, just use ...
// ACE_DECLARE_EXPORT(class, someobject);
// If there are global functions to be exported, then use ...
// ACE_DECLARE_EXPORT(void, globalfunction) (int, ...);
// Someday, when template libraries are supported, we made need ...
// ACE_DECLARE_EXPORT(template class, sometemplate) <class TYPE, class LOCK>;
# define ACE_DECLARE_EXPORT(TS,ID) TS ACE_Export ID

// ============================================================================
// Cast macros
//
// These macros are used to choose between the old cast style and the new
// *_cast<> operators
// ============================================================================

# if defined (ACE_HAS_ANSI_CASTS)

#   define ACE_sap_any_cast(TYPE)                                      reinterpret_cast<TYPE> (const_cast<ACE_Addr &> (ACE_Addr::sap_any))

#   define ACE_static_cast(TYPE, EXPR)                                 static_cast<TYPE> (EXPR)
#   define ACE_static_cast_1_ptr(TYPE, T1, EXPR)                       static_cast<TYPE<T1> *> (EXPR)
#   define ACE_static_cast_2_ptr(TYPE, T1, T2, EXPR)                   static_cast<TYPE<T1, T2> *> (EXPR)
#   define ACE_static_cast_3_ptr(TYPE, T1, T2, T3, EXPR)               static_cast<TYPE<T1, T2, T3> *> (EXPR)
#   define ACE_static_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)           static_cast<TYPE<T1, T2, T3, T4> *> (EXPR)
#   define ACE_static_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)       static_cast<TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#   define ACE_static_cast_1_ref(TYPE, T1, EXPR)                       static_cast<TYPE<T1> &> (EXPR)
#   define ACE_static_cast_2_ref(TYPE, T1, T2, EXPR)                   static_cast<TYPE<T1, T2> &> (EXPR)
#   define ACE_static_cast_3_ref(TYPE, T1, T2, T3, EXPR)               static_cast<TYPE<T1, T2, T3> &> (EXPR)
#   define ACE_static_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)           static_cast<TYPE<T1, T2, T3, T4> &> (EXPR)
#   define ACE_static_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)       static_cast<TYPE<T1, T2, T3, T4, T5> &> (EXPR)

#   define ACE_const_cast(TYPE, EXPR)                                  const_cast<TYPE> (EXPR)
#   define ACE_const_cast_1_ptr(TYPE, T1, EXPR)                        const_cast<TYPE<T1> *> (EXPR)
#   define ACE_const_cast_2_ptr(TYPE, T1, T2, EXPR)                    const_cast<TYPE<T1, T2> *> (EXPR)
#   define ACE_const_cast_3_ptr(TYPE, T1, T2, T3, EXPR)                const_cast<TYPE<T1, T2, T3> *> (EXPR)
#   define ACE_const_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)            const_cast<TYPE<T1, T2, T3, T4> *> (EXPR)
#   define ACE_const_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)        const_cast<TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#   define ACE_const_cast_1_ref(TYPE, T1, EXPR)                        const_cast<TYPE<T1> &> (EXPR)
#   define ACE_const_cast_2_ref(TYPE, T1, T2, EXPR)                    const_cast<TYPE<T1, T2> &> (EXPR)
#   define ACE_const_cast_3_ref(TYPE, T1, T2, T3, EXPR)                const_cast<TYPE<T1, T2, T3> &> (EXPR)
#   define ACE_const_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)            const_cast<TYPE<T1, T2, T3, T4> &> (EXPR)
#   define ACE_const_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)        const_cast<TYPE<T1, T2, T3, T4, T5> &> (EXPR)

#   define ACE_reinterpret_cast(TYPE, EXPR)                            reinterpret_cast<TYPE> (EXPR)
#   define ACE_reinterpret_cast_1_ptr(TYPE, T1, EXPR)                  reinterpret_cast<TYPE<T1> *> (EXPR)
#   define ACE_reinterpret_cast_2_ptr(TYPE, T1, T2, EXPR)              reinterpret_cast<TYPE<T1, T2> *> (EXPR)
#   define ACE_reinterpret_cast_3_ptr(TYPE, T1, T2, T3, EXPR)          reinterpret_cast<TYPE<T1, T2, T3> *> (EXPR)
#   define ACE_reinterpret_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)      reinterpret_cast<TYPE<T1, T2, T3, T4> *> (EXPR)
#   define ACE_reinterpret_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)  reinterpret_cast<TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#   define ACE_reinterpret_cast_1_ref(TYPE, T1, EXPR)                  reinterpret_cast<TYPE<T1> &> (EXPR)
#   define ACE_reinterpret_cast_2_ref(TYPE, T1, T2, EXPR)              reinterpret_cast<TYPE<T1, T2> &> (EXPR)
#   define ACE_reinterpret_cast_3_ref(TYPE, T1, T2, T3, EXPR)          reinterpret_cast<TYPE<T1, T2, T3> &> (EXPR)
#   define ACE_reinterpret_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)      reinterpret_cast<TYPE<T1, T2, T3, T4> &> (EXPR)
#   define ACE_reinterpret_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)  reinterpret_cast<TYPE<T1, T2, T3, T4, T5> &> (EXPR)

#   if defined (ACE_LACKS_RTTI)
#     define ACE_dynamic_cast(TYPE, EXPR)                              static_cast<TYPE> (EXPR)
#     define ACE_dynamic_cast_1_ptr(TYPE, T1, EXPR)                    static_cast<TYPE<T1> *> (EXPR)
#     define ACE_dynamic_cast_2_ptr(TYPE, T1, T2, EXPR)                static_cast<TYPE<T1, T2> *> (EXPR)
#     define ACE_dynamic_cast_3_ptr(TYPE, T1, T2, T3, EXPR)            static_cast<TYPE<T1, T2, T3> *> (EXPR)
#     define ACE_dynamic_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)        static_cast<TYPE<T1, T2, T3, T4> *> (EXPR)
#     define ACE_dynamic_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)    static_cast<TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#     define ACE_dynamic_cast_1_ref(TYPE, T1, EXPR)                    static_cast<TYPE<T1> &> (EXPR)
#     define ACE_dynamic_cast_2_ref(TYPE, T1, T2, EXPR)                static_cast<TYPE<T1, T2> &> (EXPR)
#     define ACE_dynamic_cast_3_ref(TYPE, T1, T2, T3, EXPR)            static_cast<TYPE<T1, T2, T3> &> (EXPR)
#     define ACE_dynamic_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)        static_cast<TYPE<T1, T2, T3, T4> &> (EXPR)
#     define ACE_dynamic_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)    static_cast<TYPE<T1, T2, T3, T4, T5> &> (EXPR)
#   else  /* ! ACE_LACKS_RTTI */
#     define ACE_dynamic_cast(TYPE, EXPR)                              dynamic_cast<TYPE> (EXPR)
#     define ACE_dynamic_cast_1_ptr(TYPE, T1, EXPR)                    dynamic_cast<TYPE<T1> *> (EXPR)
#     define ACE_dynamic_cast_2_ptr(TYPE, T1, T2, EXPR)                dynamic_cast<TYPE<T1, T2> *> (EXPR)
#     define ACE_dynamic_cast_3_ptr(TYPE, T1, T2, T3, EXPR)            dynamic_cast<TYPE<T1, T2, T3> *> (EXPR)
#     define ACE_dynamic_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)        dynamic_cast<TYPE<T1, T2, T3, T4> *> (EXPR)
#     define ACE_dynamic_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)    dynamic_cast<TYPE<T1, T2, T3, T4, T5> *> (EXPR)
#     define ACE_dynamic_cast_1_ref(TYPE, T1, EXPR)                    dynamic_cast<TYPE<T1> &> (EXPR)
#     define ACE_dynamic_cast_2_ref(TYPE, T1, T2, EXPR)                dynamic_cast<TYPE<T1, T2> &> (EXPR)
#     define ACE_dynamic_cast_3_ref(TYPE, T1, T2, T3, EXPR)            dynamic_cast<TYPE<T1, T2, T3> &> (EXPR)
#     define ACE_dynamic_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)        dynamic_cast<TYPE<T1, T2, T3, T4> &> (EXPR)
#     define ACE_dynamic_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)    dynamic_cast<TYPE<T1, T2, T3, T4, T5> &> (EXPR)
#   endif /* ! ACE_LACKS_RTTI */

# else

#   define ACE_sap_any_cast(TYPE)                                      ((TYPE) (ACE_Addr::sap_any))

#   define ACE_static_cast(TYPE, EXPR)                                 ((TYPE) (EXPR))
#   define ACE_static_cast_1_ptr(TYPE, T1, EXPR)                       ((TYPE<T1> *) (EXPR))
#   define ACE_static_cast_2_ptr(TYPE, T1, T2, EXPR)                   ((TYPE<T1, T2> *) (EXPR))
#   define ACE_static_cast_3_ptr(TYPE, T1, T2, T3, EXPR)               ((TYPE<T1, T2, T3> *) (EXPR))
#   define ACE_static_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)           ((TYPE<T1, T2, T3, T4> *) (EXPR))
#   define ACE_static_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)       ((TYPE<T1, T2, T3, T4, T5> *) (EXPR))
#   define ACE_static_cast_1_ref(TYPE, T1, EXPR)                       ((TYPE<T1> &) (EXPR))
#   define ACE_static_cast_2_ref(TYPE, T1, T2, EXPR)                   ((TYPE<T1, T2> &) (EXPR))
#   define ACE_static_cast_3_ref(TYPE, T1, T2, T3, EXPR)               ((TYPE<T1, T2, T3> &) (EXPR))
#   define ACE_static_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)           ((TYPE<T1, T2, T3, T4> &) (EXPR))
#   define ACE_static_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)       ((TYPE<T1, T2, T3, T4, T5> &) (EXPR))

#   define ACE_const_cast(TYPE, EXPR)                                  ((TYPE) (EXPR))
#   define ACE_const_cast_1_ptr(TYPE, T1, EXPR)                        ((TYPE<T1> *) (EXPR))
#   define ACE_const_cast_2_ptr(TYPE, T1, T2, EXPR)                    ((TYPE<T1, T2> *) (EXPR))
#   define ACE_const_cast_3_ptr(TYPE, T1, T2, T3, EXPR)                ((TYPE<T1, T2, T3> *) (EXPR))
#   define ACE_const_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)            ((TYPE<T1, T2, T3, T4> *) (EXPR))
#   define ACE_const_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)        ((TYPE<T1, T2, T3, T4, T5> *) (EXPR))
#   define ACE_const_cast_1_ref(TYPE, T1, EXPR)                        ((TYPE<T1> &) (EXPR))
#   define ACE_const_cast_2_ref(TYPE, T1, T2, EXPR)                    ((TYPE<T1, T2> &) (EXPR))
#   define ACE_const_cast_3_ref(TYPE, T1, T2, T3, EXPR)                ((TYPE<T1, T2, T3> &) (EXPR))
#   define ACE_const_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)            ((TYPE<T1, T2, T3, T4> &) (EXPR))
#   define ACE_const_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)        ((TYPE<T1, T2, T3, T4, T5> &) (EXPR))

#   define ACE_reinterpret_cast(TYPE, EXPR)                            ((TYPE) (EXPR))
#   define ACE_reinterpret_cast_1_ptr(TYPE, T1, EXPR)                  ((TYPE<T1> *) (EXPR))
#   define ACE_reinterpret_cast_2_ptr(TYPE, T1, T2, EXPR)              ((TYPE<T1, T2> *) (EXPR))
#   define ACE_reinterpret_cast_3_ptr(TYPE, T1, T2, T3, EXPR)          ((TYPE<T1, T2, T3> *) (EXPR))
#   define ACE_reinterpret_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)      ((TYPE<T1, T2, T3, T4> *) (EXPR))
#   define ACE_reinterpret_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)  ((TYPE<T1, T2, T3, T4, T5> *) (EXPR))
#   define ACE_reinterpret_cast_1_ref(TYPE, T1, EXPR)                  ((TYPE<T1> &) (EXPR))
#   define ACE_reinterpret_cast_2_ref(TYPE, T1, T2, EXPR)              ((TYPE<T1, T2> &) (EXPR))
#   define ACE_reinterpret_cast_3_ref(TYPE, T1, T2, T3, EXPR)          ((TYPE<T1, T2, T3> &) (EXPR))
#   define ACE_reinterpret_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)      ((TYPE<T1, T2, T3, T4> &) (EXPR))
#   define ACE_reinterpret_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)  ((TYPE<T1, T2, T3, T4, T5> &) (EXPR))

#   define ACE_dynamic_cast(TYPE, EXPR)                                ((TYPE) (EXPR))
#   define ACE_dynamic_cast_1_ptr(TYPE, T1, EXPR)                      ((TYPE<T1> *) (EXPR))
#   define ACE_dynamic_cast_2_ptr(TYPE, T1, T2, EXPR)                  ((TYPE<T1, T2> *) (EXPR))
#   define ACE_dynamic_cast_3_ptr(TYPE, T1, T2, T3, EXPR)              ((TYPE<T1, T2, T3> *) (EXPR))
#   define ACE_dynamic_cast_4_ptr(TYPE, T1, T2, T3, T4, EXPR)          ((TYPE<T1, T2, T3, T4> *) (EXPR))
#   define ACE_dynamic_cast_5_ptr(TYPE, T1, T2, T3, T4, T5, EXPR)      ((TYPE<T1, T2, T3, T4, T5> *) (EXPR))
#   define ACE_dynamic_cast_1_ref(TYPE, T1, EXPR)                      ((TYPE<T1> &) (EXPR))
#   define ACE_dynamic_cast_2_ref(TYPE, T1, T2, EXPR)                  ((TYPE<T1, T2> &) (EXPR))
#   define ACE_dynamic_cast_3_ref(TYPE, T1, T2, T3, EXPR)              ((TYPE<T1, T2, T3> &) (EXPR))
#   define ACE_dynamic_cast_4_ref(TYPE, T1, T2, T3, T4, EXPR)          ((TYPE<T1, T2, T3, T4> &) (EXPR))
#   define ACE_dynamic_cast_5_ref(TYPE, T1, T2, T3, T4, T5, EXPR)      ((TYPE<T1, T2, T3, T4, T5> &) (EXPR))
# endif /* ACE_HAS_ANSI_CASTS */

# if !defined (ACE_CAST_CONST)
    // Sun CC 4.2, for example, requires const in reinterpret casts of
    // data members in const member functions.  But, other compilers
    // complain about the useless const.  This keeps everyone happy.
#   if defined (__SUNPRO_CC)
#     define ACE_CAST_CONST const
#   else  /* ! __SUNPRO_CC */
#     define ACE_CAST_CONST
#   endif /* ! __SUNPRO_CC */
# endif /* ! ACE_CAST_CONST */

// ============================================================================
// UNICODE macros (to be added later)
// ============================================================================

// Get the unicode (i.e. ACE_TCHAR) defines
# include "ace/ace_wchar.h"


// ============================================================================
// Compiler Silencing macros
//
// Some compilers complain about parameters that are not used.  This macro
// should keep them quiet.
// ============================================================================

#if defined (ghs) || defined (__GNUC__) || defined (__hpux) || defined (__sgi) || defined (__DECCXX) || defined (__KCC) || defined (__rational__) || defined (__USLC__) || defined (ACE_RM544)
// Some compilers complain about "statement with no effect" with (a).
// This eliminates the warnings, and no code is generated for the null
// conditional statement.  NOTE: that may only be true if -O is enabled,
// such as with GreenHills (ghs) 1.8.8.
# define ACE_UNUSED_ARG(a) do {/* null */} while (&a == 0)
#else /* ghs || __GNUC__ || ..... */
# define ACE_UNUSED_ARG(a) (a)
#endif /* ghs || __GNUC__ || ..... */

#if defined (__sgi) || defined (ghs) || defined (__DECCXX) || defined(__BORLANDC__) || defined (__KCC) || defined (ACE_RM544) || defined (__USLC__)
# define ACE_NOTREACHED(a)
#else  /* __sgi || ghs || ..... */
# define ACE_NOTREACHED(a) a
#endif /* __sgi || ghs || ..... */

// ============================================================================
// ACE_ALLOC_HOOK* macros
//
// Macros to declare and define class-specific allocation operators.
// ============================================================================

# if defined (ACE_HAS_ALLOC_HOOKS)
#   define ACE_ALLOC_HOOK_DECLARE \
  void *operator new (size_t bytes); \
  void operator delete (void *ptr);

  // Note that these are just place holders for now.  Some day they
  // may be be replaced by <ACE_Malloc>.
#   define ACE_ALLOC_HOOK_DEFINE(CLASS) \
  void *CLASS::operator new (size_t bytes) { return ::new char[bytes]; } \
  void CLASS::operator delete (void *ptr) { delete [] ((char *) ptr); }
# else
#   define ACE_ALLOC_HOOK_DECLARE struct __Ace {} /* Just need a dummy... */
#   define ACE_ALLOC_HOOK_DEFINE(CLASS)
# endif /* ACE_HAS_ALLOC_HOOKS */

// ============================================================================
// ACE_OSCALL_* macros
//
// The following two macros ensure that system calls are properly
// restarted (if necessary) when interrupts occur.
// ============================================================================

#if defined (ACE_HAS_SIGNAL_SAFE_OS_CALLS)
#   define ACE_OSCALL(OP,TYPE,FAILVALUE,RESULT) \
  do \
    RESULT = (TYPE) OP; \
  while (RESULT == FAILVALUE && errno == EINTR)
#   define ACE_OSCALL_RETURN(OP,TYPE,FAILVALUE) \
  do { \
    TYPE ace_result_; \
    do \
      ace_result_ = (TYPE) OP; \
    while (ace_result_ == FAILVALUE && errno == EINTR); \
    return ace_result_; \
  } while (0)
# elif defined (ACE_WIN32)
#   define ACE_OSCALL_RETURN(X,TYPE,FAILVALUE) \
  do \
    return (TYPE) X; \
  while (0)
#   define ACE_OSCALL(X,TYPE,FAILVALUE,RESULT) \
  do \
    RESULT = (TYPE) X; \
  while (0)
#   if defined (__BORLANDC__) && (__BORLANDC__ <= 0x550)
#   define ACE_WIN32CALL_RETURN(X,TYPE,FAILVALUE) \
  do { \
    TYPE ace_result_; \
    TYPE ace_local_result_ = (TYPE) X; \
    ace_result_ = ace_local_result_; \
    if (ace_result_ == FAILVALUE) \
      ACE_OS::set_errno_to_last_error (); \
    return ace_result_; \
  } while (0)
#   else
#     define ACE_WIN32CALL_RETURN(X,TYPE,FAILVALUE) \
  do { \
    TYPE ace_result_; \
    ace_result_ = (TYPE) X; \
    if (ace_result_ == FAILVALUE) \
      ACE_OS::set_errno_to_last_error (); \
    return ace_result_; \
  } while (0)
#   endif /* defined (__BORLANDC__) && (__BORLANDC__ <= 0x550) */
#   define ACE_WIN32CALL(X,TYPE,FAILVALUE,RESULT) \
  do { \
    RESULT = (TYPE) X; \
    if (RESULT == FAILVALUE) \
      ACE_OS::set_errno_to_last_error (); \
  } while (0)
#else /* ACE_HAS_SIGNAL_SAFE_OS_CALLS */
#   define ACE_OSCALL_RETURN(OP,TYPE,FAILVALUE) do { TYPE ace_result_ = FAILVALUE; ace_result_ = ace_result_; return OP; } while (0)
#   define ACE_OSCALL(OP,TYPE,FAILVALUE,RESULT) do { RESULT = (TYPE) OP; } while (0)
#endif /* ACE_HAS_SIGNAL_SAFE_OS_CALLS */

// ============================================================================
// at_exit declarations
// ============================================================================

// Marker for cleanup, used by ACE_Exit_Info.
extern int ace_exit_hook_marker;

// For use by <ACE_OS::exit>.
extern "C"
{
  typedef void (*ACE_EXIT_HOOK) (void);
}

// Signature for registering a cleanup function that is used by the
// <ACE_Object_Manager> and the <ACE_Thread_Manager>.
# if defined (ACE_HAS_SIG_C_FUNC)
extern "C" {
# endif /* ACE_HAS_SIG_C_FUNC */
typedef void (*ACE_CLEANUP_FUNC)(void *object, void *param) /* throw () */;
# if defined (ACE_HAS_SIG_C_FUNC)
}
# endif /* ACE_HAS_SIG_C_FUNC */

// ============================================================================
// log_msg declarations
// ============================================================================

# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
typedef int (*ACE_SEH_EXCEPT_HANDLER)(void *);
// Prototype of win32 structured exception handler functions.
// They are used to get the exception handling expression or
// as exception handlers.
# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */

class ACE_OS_Thread_Descriptor;
class ACE_OS_Log_Msg_Attributes;
typedef void (*ACE_INIT_LOG_MSG_HOOK) (ACE_OS_Log_Msg_Attributes &attr
# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
                                       , ACE_SEH_EXCEPT_HANDLER selector
                                       , ACE_SEH_EXCEPT_HANDLER handler
# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
                                       );
typedef void (*ACE_INHERIT_LOG_MSG_HOOK) (ACE_OS_Thread_Descriptor*,
                                          ACE_OS_Log_Msg_Attributes &);

typedef void (*ACE_CLOSE_LOG_MSG_HOOK) (void);

typedef void (*ACE_SYNC_LOG_MSG_HOOK) (const ACE_TCHAR *prog_name);

typedef ACE_OS_Thread_Descriptor *(*ACE_THR_DESC_LOG_MSG_HOOK) (void);

// ============================================================================
// Fundamental types
// ============================================================================

#if defined (ACE_WIN32)

typedef HANDLE ACE_HANDLE;
typedef SOCKET ACE_SOCKET;
# define ACE_INVALID_HANDLE INVALID_HANDLE_VALUE

#else /* ! ACE_WIN32 */

typedef int ACE_HANDLE;
typedef ACE_HANDLE ACE_SOCKET;
# define ACE_INVALID_HANDLE -1

#endif /* ACE_WIN32 */

// Define the type that's returned from the platform's native thread
// functions. ACE_THR_FUNC_RETURN is the type defined as the thread
// function's return type, except when the thread function doesn't return
// anything (pSoS). The ACE_THR_FUNC_NO_RETURN_VAL macro is used to
// indicate that the actual thread function doesn't return anything. The
// rest of ACE uses a real type so there's no a ton of conditional code
// everywhere to deal with the possibility of no return type.
# if defined (VXWORKS)
//typedef FUNCPTR ACE_THR_FUNC;  // where typedef int (*FUNCPTR) (...)
typedef int ACE_THR_FUNC_RETURN;
# elif defined (ACE_PSOS)
typedef int ACE_THR_FUNC_RETURN;
// pSOS task functions don't really return anything... this just makes it
// easier to deal with declaring variables internally in ACE.
# elif defined (ACE_WIN32)
typedef DWORD ACE_THR_FUNC_RETURN;
# else
typedef void* ACE_THR_FUNC_RETURN;
# endif /* VXWORKS */
typedef ACE_THR_FUNC_RETURN (*ACE_THR_FUNC)(void *);

extern "C" {
typedef void (*ACE_THR_C_DEST)(void *);
}
typedef void (*ACE_THR_DEST)(void *);

# if defined (VXWORKS)
#   if defined (ghs)
    // GreenHills 1.8.8 needs the stdarg.h #include before the #include of
    // vxWorks.h.
    // Also, be sure that these #includes come _after_ the key_t typedef, and
    // before the #include of time.h.
#     include "ace/os_include/os_stdarg.h"
#   endif /* ghs */

#   include /**/ <vxWorks.h>
# endif /* VXWORKS */

// Now some platforms have special requirements...
# if defined (VXWORKS)
typedef FUNCPTR ACE_THR_FUNC_INTERNAL;  // where typedef int (*FUNCPTR) (...)
# elif defined (ACE_PSOS)
typedef void (*ACE_THR_FUNC_INTERNAL)(void *);
# else
typedef ACE_THR_FUNC ACE_THR_FUNC_INTERNAL;
# endif /* VXWORKS */

extern "C"
{
# if defined (VXWORKS)
typedef FUNCPTR ACE_THR_C_FUNC;  // where typedef int (*FUNCPTR) (...)
# elif defined (ACE_PSOS)
// needed to handle task entry point type inconsistencies in pSOS+
typedef void (*PSOS_TASK_ENTRY_POINT)();
typedef void (*ACE_THR_C_FUNC)(void *);
# else
typedef ACE_THR_FUNC_RETURN (*ACE_THR_C_FUNC)(void *);
# endif /* VXWORKS */
}

// ============================================================================
// Macros for controlling the lifetimes of dlls loaded by ACE_DLL--including
// all dlls loaded via the ACE Service Config framework.
//
// Please don't change these values or add new ones wantonly, since we use
// the ACE_BIT_ENABLED, etc..., macros to test them.
// ============================================================================

// Per-process policy that unloads dlls eagerly.
#define ACE_DLL_UNLOAD_POLICY_PER_PROCESS 0
// Apply policy on a per-dll basis.  If the dll doesn't use one of the macros
// below, the current per-process policy will be used.
#define ACE_DLL_UNLOAD_POLICY_PER_DLL 1
// Don't unload dll when refcount reaches zero, i.e., wait for either an
// explicit unload request or program exit.
#define ACE_DLL_UNLOAD_POLICY_LAZY 2
// Default policy allows dlls to control their own destinies, but will
// unload those that don't make a choice eagerly.
#define ACE_DLL_UNLOAD_POLICY_DEFAULT ACE_DLL_UNLOAD_POLICY_PER_DLL

// Add this macro you one of your cpp file in your dll.  X should
// be either ACE_DLL_UNLOAD_POLICY_DEFAULT or ACE_DLL_UNLOAD_POLICY_LAZY.
#define ACE_DLL_UNLOAD_POLICY(CLS,X) \
extern "C" u_long CLS##_Export _get_dll_unload_policy (void) \
  { return X;}

// ============================================================================
// ACE_USES_CLASSIC_SVC_CONF macro
// ============================================================================

// For now, default is to use the classic svc.conf format.
#if !defined (ACE_USES_CLASSIC_SVC_CONF)
# if defined (ACE_HAS_CLASSIC_SVC_CONF) && defined (ACE_HAS_XML_SVC_CONF)
#   error You can only use either CLASSIC or XML svc.conf, not both.
# endif
// Change the ACE_HAS_XML_SVC_CONF to ACE_HAS_CLASSIC_SVC_CONF when
// we switch ACE to use XML svc.conf as default format.
# if defined (ACE_HAS_XML_SVC_CONF)
#   define ACE_USES_CLASSIC_SVC_CONF 0
# else
#   define ACE_USES_CLASSIC_SVC_CONF 1
# endif /* ACE_HAS_XML_SVC_CONF */
#endif /* ACE_USES_CLASSIC_SVC_CONF */

// ============================================================================
// Default svc.conf file extension.
// ============================================================================
#if defined (ACE_USES_CLASSIC_SVC_CONF) && (ACE_USES_CLASSIC_SVC_CONF == 1)
# define ACE_DEFAULT_SVC_CONF_EXT   ".conf"
#else
# define ACE_DEFAULT_SVC_CONF_EXT   ".conf.xml"
#endif /* ACE_USES_CLASSIC_SVC_CONF && ACE_USES_CLASSIC_SVC_CONF == 1 */

// ============================================================================
// Miscellaneous macros
// ============================================================================

// This is used to indicate that a platform doesn't support a
// particular feature.
#if defined ACE_HAS_VERBOSE_NOTSUP
  // Print a console message with the file and line number of the
  // unsupported function.
# if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
#   include /**/ <cstdio>
# else
#   include "ace/os_include/os_stdio.h"
# endif
# define ACE_NOTSUP_RETURN(FAILVALUE) do { errno = ENOTSUP; fprintf (stderr, ACE_LIB_TEXT ("ACE_NOTSUP: %s, line %d\n"), __FILE__, __LINE__); return FAILVALUE; } while (0)
# define ACE_NOTSUP do { errno = ENOTSUP; fprintf (stderr, ACE_LIB_TEXT ("ACE_NOTSUP: %s, line %d\n"), __FILE__, __LINE__); return; } while (0)
#else /* ! ACE_HAS_VERBOSE_NOTSUP */
# define ACE_NOTSUP_RETURN(FAILVALUE) do { errno = ENOTSUP ; return FAILVALUE; } while (0)
# define ACE_NOTSUP do { errno = ENOTSUP; return; } while (0)
#endif /* ! ACE_HAS_VERBOSE_NOTSUP */

#if defined (ACE_USES_EXPLICIT_STD_NAMESPACE)
#  define ACE_STD_NAMESPACE std
#else
#  define ACE_STD_NAMESPACE
#endif

// empty ACE_OS namespace to help identify compiler errors more easily.
namespace ACE_OS {}
#if !defined (ACE_OS_String)
#  define ACE_OS_String ACE_OS
#endif /* ACE_OS_String */
#if !defined (ACE_OS_Memory)
#  define ACE_OS_Memory ACE_OS
#endif /* ACE_OS_Memory */
#if !defined (ACE_OS_Dirent)
#  define ACE_OS_Dirent ACE_OS
#endif /* ACE_OS_Dirent */
#if !defined (ACE_OS_TLI)
#  define ACE_OS_TLI ACE_OS
#endif /* ACE_OS_TLI */

// These includes are here to avoid circular dependencies.
#ifndef ACE_OS_MAIN_H
// Keep this at the bottom of the file.  It contains the main macros.
#  include "ace/OS_main.h"
#endif /* ACE_OS_MAIN_h */

#include /**/ "ace/post.h"

#endif /* ACE_CONFIG_ALL_H */
