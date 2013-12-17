// -*- C++ -*-

//=============================================================================
/**
 *  @file    PortableInterceptor.h
 *
 *  $Id$
 *
 *   A series of TAO_INTERCEPTOR* macros are defined in this file.
 *   These macros allows us to disable/enable support of interceptors
 *   easily.
 *
 *  @author  Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author  Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_PORTABLE_INTERCEPTOR_H
#define TAO_PORTABLE_INTERCEPTOR_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_INTERCEPTORS == 1)
# define TAO_INTERCEPTOR(X) X
#else
# define TAO_INTERCEPTOR(X)
#endif /* TAO_HAS_INTERCEPTORS */

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLE_INTERCEPTOR_H */
