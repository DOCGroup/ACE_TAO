// $Id$

#ifndef ACE_CONFIG_H_INCLUDED
#define ACE_CONFIG_H_INCLUDED
#ifdef __FreeBSD_kernel__
#include "config-kfreebsd.h"
#elif defined(__GNU__)
#include "config-hurd.h"
#else // assume linux
/*
 * Macros that were enabled in Debian are stored here.
 *
 * Rationale: those were captured in the generated libraries on
 * compilation; hence the same values must be used when including
 * ACE+TAO headers, to avoid unexpected results.
 */
#ifndef ACE_HAS_IPV6
#define ACE_HAS_IPV6 1
#endif

#ifndef ACE_USES_IPV4_IPV6_MIGRATION
#define ACE_USES_IPV4_IPV6_MIGRATION 1
#endif

#ifndef __ACE_INLINE__
#define __ACE_INLINE__
#endif

#include "config-linux.h"
#endif // __FreeBSD_version
#endif /* ACE_CONFIG_H_INCLUDED */
