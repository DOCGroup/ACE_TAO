/* -*- C++ -*- */
// $Id$

// This configuration file is designed to build only the minimal
// ACE_OS adaptation layer.

#ifndef ACE_CONFIG_MINIMAL_H
#define ACE_CONFIG_MINIMAL_H

#define ACE_HAS_MINIMAL_ACE_OS

#if defined (ACE_HAS_TSS_EMULATION)
# undef ACE_HAS_TSS_EMULATION
#endif /* ACE_HAS_TSS_EMULATION */

#if defined (ACE_ASSERT)
# undef ACE_ASSERT
#endif /* ACE_ASSERT */
#define ACE_ASSERT(x)

#if defined (ACE_DEBUG)
# undef ACE_DEBUG
#endif /* ACE_DEBUG */
#define ACE_DEBUG(x)

#if defined (ACE_ERROR)
# undef ACE_ERROR
#endif /* ACE_ERROR */
#define ACE_ERROR(x)

#endif /* ACE_CONFIG_MINIMAL_H */
