/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    stdstreams.h
//
// = AUTHOR
//    David L. Levine
//
// = DESCRIPTION
//    Wrapper for streams library #includes.  For use by application code
//    when they want to #include iostream, fstream, etc.
//
// ============================================================================

#if !defined (ACE_STDSTREAMS_H)
#define ACE_STDSTREAMS_H

#if defined (ACE_HAS_MINIMUM_STREAMH_INCLUSION)
# undef ACE_HAS_MINIMUM_STREAMH_INCLUSION
#endif /* ACE_HAS_MINIMUM_STREAMH_INCLUSION */

#include "ace/stdcpp.h"

#endif /* ACE_STDSTREAMS_H */
