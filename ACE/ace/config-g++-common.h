// -*- C++ -*-
// This configuration file is designed to be included by another,
// specific configuration file.  It provides config information common
// to all g++ platforms, including egcs.

#ifndef ACE_GNUG_COMMON_H
#define ACE_GNUG_COMMON_H
#include /**/ "ace/pre.h"

#if !defined (ACE_CC_NAME)
# define ACE_CC_NAME ACE_TEXT ("g++")
#endif
#define ACE_CC_MAJOR_VERSION __GNUC__
#define ACE_CC_MINOR_VERSION __GNUC_MINOR__
#define ACE_CC_BETA_VERSION (0)

#define ACE_HAS_STANDARD_CPP_LIBRARY 1
#define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
#define ACE_TEMPLATES_REQUIRE_SOURCE
#define ACE_EXPLICIT_TEMPLATE_DESTRUCTOR_TAKES_ARGS
#define ACE_NEW_THROWS_EXCEPTIONS
#define ACE_HAS_NEW_NOTHROW

#if __cplusplus >= 201103L
# define ACE_HAS_CPP11
#endif
#if __cplusplus >= 201402L
# define ACE_HAS_CPP14
#endif
#if __cplusplus >= 201703L
# define ACE_HAS_CPP17
#endif
#if __cplusplus >= 202002L
# define ACE_HAS_CPP20
#endif

#if (defined (i386) || defined (__i386__)) && !defined (ACE_SIZEOF_LONG_DOUBLE)
# define ACE_SIZEOF_LONG_DOUBLE 12
#endif /* i386 */

#if !defined (__MINGW32__) && (defined (i386) || defined (__i386__))
  // If running an Intel, assume that it's a Pentium so that
  // ACE_OS::gethrtime () can use the RDTSC instruction.  If running a
  // 486 or lower, be sure to comment this out.  (If not running an
  // Intel CPU, this #define will not be seen because of the i386
  // protection, so it can be ignored.)
# define ACE_HAS_PENTIUM
#endif /* i386 */

#if (defined (ACE_HAS_PENTIUM) || defined (__amd64__) || defined (__x86_64__))
# define ACE_HAS_INTEL_ASSEMBLY
#endif

#if !defined (ACE_HAS_GCC_CONSTRUCTOR_ATTRIBUTE)
#define ACE_HAS_GCC_CONSTRUCTOR_ATTRIBUTE 1
#endif

#if !defined (ACE_HAS_GCC_DESTRUCTOR_ATTRIBUTE)
#define ACE_HAS_GCC_DESTRUCTOR_ATTRIBUTE 1
#endif

#if !defined (ACE_HAS_GCC_DEPRECATED_ATTRIBUTE)
#define ACE_HAS_GCC_DEPRECATED_ATTRIBUTE 1
#endif

#if !defined (ACE_HAS_GCC_FORMAT_ATTRIBUTE)
#define ACE_HAS_GCC_FORMAT_ATTRIBUTE 1
#endif

#if (ACE_HAS_GCC_CONSTRUCTOR_ATTRIBUTE == 1)
# define ACE_GCC_CONSTRUCTOR_ATTRIBUTE __attribute__ ((constructor))
#endif

#if (ACE_HAS_GCC_DESTRUCTOR_ATTRIBUTE == 1)
# define ACE_GCC_DESTRUCTOR_ATTRIBUTE __attribute__ ((destructor))
#endif

#if (ACE_HAS_GCC_DEPRECATED_ATTRIBUTE == 1)
#define ACE_DEPRECATED __attribute__ ((deprecated))
#endif

#if (ACE_HAS_GCC_FORMAT_ATTRIBUTE == 1)
# define ACE_GCC_FORMAT_ATTRIBUTE(TYPE, STR_INDEX, FIRST_INDEX) \
   __attribute__ ((format (TYPE, STR_INDEX, FIRST_INDEX)))
#endif

// Take advantage of g++ visibility attributes to generate
// improved shared library binaries.
#if !defined (__MINGW32__)

# if defined (ACE_HAS_CUSTOM_EXPORT_MACROS) && ACE_HAS_CUSTOM_EXPORT_MACROS == 0
#  undef ACE_HAS_CUSTOM_EXPORT_MACROS
#  if defined (ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS)
#    undef ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS
#  endif /* ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS */
#  define ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS 0
# else
#  ifndef ACE_HAS_CUSTOM_EXPORT_MACROS
#    define ACE_HAS_CUSTOM_EXPORT_MACROS
#  endif  /* !ACE_HAS_CUSTOM_EXPORT_MACROS */
#  ifndef ACE_Proper_Export_Flag
#    define ACE_Proper_Export_Flag __attribute__ ((visibility("default")))
#  endif /* !ACE_Proper_Export_Flag */
#  ifndef ACE_Proper_Import_Flag
#    define ACE_Proper_Import_Flag __attribute__ ((visibility("default")))
#  endif /* !ACE_Proper_Import_Flag */

#  ifndef ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS
#    define ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS 1
#  endif

#  if defined (ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS) && ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS == 1
#   define ACE_EXPORT_SINGLETON_DECLARATION(T) __extension__ extern template class ACE_Proper_Export_Flag T
#   define ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)  __extension__ extern template class ACE_Proper_Export_Flag SINGLETON_TYPE <CLASS, LOCK>;
#   define ACE_HAS_EXPLICIT_TEMPLATE_CLASS_INSTANTIATION
#  else  /* ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS */
#   define ACE_EXPORT_SINGLETON_DECLARATION(T)     \
        _Pragma ("GCC visibility push(default)")  \
        template class T                          \
        _Pragma ("GCC visibility pop")
#   define ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) \
        _Pragma ("GCC visibility push(default)")                    \
        template class SINGLETON_TYPE<CLASS, LOCK>;                 \
        _Pragma ("GCC visibility pop")
#  endif /* ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS */

// Note that the "__extension__" is needed to prevent g++ from issuing
// an error when using its "-pedantic" command line flag.
# define ACE_IMPORT_SINGLETON_DECLARATION(T) __extension__ extern template class T
# define ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) __extension__ extern template class SINGLETON_TYPE<CLASS, LOCK>;

# endif  /* ACE_HAS_CUSTOM_EXPORT_MACROS == 0 */
#endif  /* !__MINGW32__ */

#if defined (ACE_HAS_THREADS)
# if defined (__powerpc__)
// The builtins seem to be provided for all powerpc platforms
#   define ACE_HAS_GCC_ATOMIC_BUILTINS 1
# endif
# if defined (__ia64)
// The builtins seem to be provided for the IA64 platforms
#   define ACE_HAS_GCC_ATOMIC_BUILTINS 1
# endif
# if defined (__amd64__) || defined (__x86_64__)
// The builtin's are provided also for 64bit linux
#   define ACE_HAS_GCC_ATOMIC_BUILTINS 1
# endif
#endif /* ACE_HAS_THREADS */

#define ACE_GCC_NO_RETURN __attribute__ ((__noreturn__))

#include /**/ "ace/post.h"
#endif /* ACE_GNUG_COMMON_H */
