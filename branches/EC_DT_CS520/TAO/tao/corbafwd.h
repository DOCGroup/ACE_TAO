// -*- C++ -*-

//=============================================================================
/**
 *  @file   corbafwd.h
 *
 *  $Id$
 *
 * Forward declare the basic types used in the ORB
 * implementation. Regular users of the ORB should only include
 * tao/corba.h (generally through the stub/skeleton headers), but the
 * implementation of the ORB itself should include this file.  That
 * reduces interdependency and speeds up compilations and
 * recompilations.
 *
 *  @author Carlos O'Ryan
 *  @author Ossama Othman
 *  @author Chris Cleeland
 *  @author Douglas C. Schmidt
 */
//=============================================================================

#ifndef TAO_CORBAFWD_H
#define TAO_CORBAFWD_H

#include /**/ "ace/pre.h"

#include "tao/ORB_Constants.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TC_Constants_Forward.h"
#include "tao/OBV_Constants.h"
#include "tao/CORBA_methods.h"



#if defined (__ACE_INLINE__)
# include "tao/corbafwd.i"
#endif /* ! __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_CORBAFWD_H */
