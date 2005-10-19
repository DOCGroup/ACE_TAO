// -*- C++ -*-

//=============================================================================
/**
 *  @file    pre.h
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 *
 *  This file saves the original alignment rules and changes the alignment
 *  boundary to ACE's default, and opens a versioned namespace
 *  declaration (if enabled by the user).
 *
 */
//=============================================================================


// No header guard
#if defined (_MSC_VER)
# pragma warning (disable:4103)
# pragma pack (push, 8)
#elif defined (__BORLANDC__)
# pragma option push -a8 -b -Ve- -Vx- -w-rvl -w-rch -w-ccc -w-obs -w-aus -w-pia -w-inl -w-sig
# pragma nopushoptwarn
# pragma nopackwarning
#endif

// Versioned namespace (disabled by default)
#include "ace/Version.h"


#ifndef ACE_NAMESPACE_VERSION_ENTERED
#define ACE_NAMESPACE_VERSION_ENTERED
ACE_BEGIN_VERSIONED_NAMESPACE_DECL
#endif
