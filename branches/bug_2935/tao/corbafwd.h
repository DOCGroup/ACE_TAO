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

#include "tao/CORBA_methods.h"

#include /**/ "ace/post.h"

#endif /* TAO_CORBAFWD_H */
