/* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for the SGI
// Indigo2EX running Irix 6.5 platform using the KAI C++ compiler.

#ifndef ACE_CONFIG_H
#define ACE_CONFIG_H
#include /**/ "ace/pre.h"

#include "ace/config-kcc-common.h"
#include "ace/config-irix6.x-common.h"

// Denotes that GNU has cstring.h as standard
// which redefines memchr()
#define ACE_HAS_GNU_CSTRING_H

// Compiler/platform supports SVR4 signal typedef.
#define ACE_HAS_IRIX_53_SIGNALS

#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES

// Compiler/platform supports sys_siglist array.
//#define ACE_HAS_SYS_SIGLIST

// This gets defined in config-irix6.x-common.h, but kcc does support this.
#undef ACE_LACKS_PLACEMENT_OPERATOR_DELETE

// Shift this --- otherwise, it lands on top of libKCC.
#define ACE_DEFAULT_BASE_ADDR ((char*) 0x0500000)

// kcc 4.0d miscompiles memchr_emulation on irix at +K3 optimization.
// So just use the library version instead.
#define ACE_HAS_MEMCHR

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_H */
