// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_intrin.h
 *
 *  Intrinsic methods
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_INTRIN_H
#define ACE_OS_INCLUDE_OS_INTRIN_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_INTRIN_H)
# include /**/ <intrin.h>
#endif /* !ACE_HAS_INTRIN_H */

#if defined (ACE_HAS_IA64INTRIN_H)
# include /**/ <ia64intrin.h>
#endif /* !ACE_HAS_IA64INTRIN_H */

#if defined (ACE_HAS_IA32INTRIN_H)
# include /**/ <ia32intrin.h>
#endif /* !ACE_HAS_IA32INTRIN_H */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_INTRIN_H */
