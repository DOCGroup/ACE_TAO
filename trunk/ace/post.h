// -*- C++ -*-

//=============================================================================
/**
 *  @file    post.h
 *
 *  $Id$
 *
 *  @author Christopher Kohlhoff <chris@kohlhoff.com>
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 *
 *  This file restores the original alignment rules, and closes
 *  the versioned namespace (if enabled by the user).
 *
 */
//=============================================================================


// No need to include header defining this macro.  It should have
// already been pulled in by ace/pre.h.
ACE_END_VERSIONED_NAMESPACE_DECL


// No header guard
#if defined (_MSC_VER)
# pragma pack (pop)
#elif defined (__BORLANDC__)
# pragma option pop
# pragma nopushoptwarn
# pragma nopackwarning
#endif
