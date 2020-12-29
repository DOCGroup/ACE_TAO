#ifndef ACE_CONFIG_MACOSX_LION_H
#define ACE_CONFIG_MACOSX_LION_H

#include "ace/config-macosx-leopard.h"

#ifdef __clang__
# define ACE_ANY_OPS_USE_NAMESPACE
#endif /* __clang__ */

#if  defined (__x86_64__)
# define ACE_SSIZE_T_FORMAT_SPECIFIER_ASCII "%ld"
# define ACE_SIZE_T_FORMAT_SPECIFIER_ASCII "%lu"
#endif /* __x86_64__ */

#define ACE_LACKS_UCONTEXT_H

#endif // ACE_CONFIG_MACOSX_LION_H
