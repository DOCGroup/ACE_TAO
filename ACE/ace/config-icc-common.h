// -*- C++ -*-
#ifndef ACE_LINUX_ICC_COMMON_H
#define ACE_LINUX_ICC_COMMON_H
#include /**/ "ace/pre.h"

# define ACE_HAS_STANDARD_CPP_LIBRARY 1
# define ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB 1
# define ACE_HAS_STRING_CLASS

#if defined (ACE_HAS_CUSTOM_EXPORT_MACROS) && ACE_HAS_CUSTOM_EXPORT_MACROS == 0
#  undef ACE_HAS_CUSTOM_EXPORT_MACROS
#  if defined (ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS)
#    undef ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS
#  endif /* ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS */
#  define ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS 0
#else
#  ifndef ACE_HAS_CUSTOM_EXPORT_MACROS
#    define ACE_HAS_CUSTOM_EXPORT_MACROS
#  endif  /* !ACE_HAS_CUSTOM_EXPORT_MACROS */
#  define ACE_Proper_Export_Flag __attribute__ ((visibility("default")))
#  define ACE_Proper_Import_Flag __attribute__ ((visibility("default")))

#  ifndef ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS
#    define ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS 1
#  endif

#  if defined (ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS) && ACE_GCC_HAS_TEMPLATE_INSTANTIATION_VISIBILITY_ATTRS == 1
#   define ACE_EXPORT_SINGLETON_DECLARATION(T) template class ACE_Proper_Export_Flag T
#   define ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) template class ACE_Proper_Export_Flag SINGLETON_TYPE <CLASS, LOCK>;
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
#endif  /* ACE_HAS_CUSTOM_EXPORT_MACROS == 0 */

#define ACE_NEW_THROWS_EXCEPTIONS

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

#define ACE_TEMPLATES_REQUIRE_SOURCE

#if (__INTEL_COMPILER >= 910)
# define ACE_EXPLICIT_TEMPLATE_DESTRUCTOR_TAKES_ARGS
#endif

#if defined (__ia64)
# define ACE_HAS_IA64INTRIN_H
# define ACE_HAS_INTRINSIC_INTERLOCKED
#else
# define ACE_HAS_IA32INTRIN_H
#endif

// We assume that Intel C++ 15 and higher do have correct C++11 support when
// it runs with GCC 4.7 or higher emulation mode
#if (__INTEL_COMPILER > 1400) && defined (__INTEL_CXX11_MODE__)
#  define ACE_HAS_CPP11
#endif

#include /**/ "ace/post.h"
#endif /* ACE_LINUX_ICC_COMMON_H */
