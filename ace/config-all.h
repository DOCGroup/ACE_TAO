// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   ace
//
// = FILENAME
//   config-all.h
//
// = AUTHOR
//   (Originally in OS.h)
//   Doug Schmidt <schmidt@cs.wustl.edu>, Jesper S. M|ller
//   <stophph@diku.dk>, and a cast of thousands...
//
// ============================================================================

#ifndef ACE_CONFIG_ALL_H
#define ACE_CONFIG_ALL_H
#include "ace/pre.h"

#include "ace/config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// ============================================================================
// RCSID Macros
// ============================================================================

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
      inline const char* get_rcsid_ ## path ## _ ## file (const char*) \
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

// ============================================================================
// INLINE macros
//
// These macros handle all the inlining of code via the .i or .inl files
// ============================================================================

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

# if defined (ACE_HAS_GNUC_BROKEN_TEMPLATE_INLINE_FUNCTIONS)
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

// ============================================================================
// UNICODE macros (to be added later)
// ============================================================================

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
#endif /* ACE_HAS_CUSTOM_EXPORT_MACROS */

#include "ace/ACE_export.h"
#include "ace/svc_export.h"

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
// Cast macros
// ============================================================================

#include "ace/post.h"
#endif /* ACE_CONFIG_ALL_H */
