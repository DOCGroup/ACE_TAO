// $Id$

// Transforms a string BUF into an ARGV-style vector of strings.

#include "ace/ARGV.h"

#if !defined (__ACE_INLINE__)
#include "ace/ARGV.inl"
#endif /* __ACE_INLINE__ */

// Open versioned namespace, if enabled by the user.
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_RCSID(ace, ARGV, "$Id$")

#if !defined (__ACE_INLINE__)
template class ACE_TARGV<ACE_TCHAR>;
#endif /* __ACE_INLINE__ */


// Close versioned namespace, if enabled by the user.
ACE_END_VERSIONED_NAMESPACE_DECL
