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

#include "orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_INTERCEPTORS == 1)
# define TAO_INTERCEPTOR(X) X
# define TAO_INTERCEPTOR_CHECK ACE_TRY_CHECK
# define TAO_INTERCEPTOR_CHECK_RETURN(X) ACE_TRY_CHECK
# define TAO_INTERCEPTOR_THROW(X) ACE_TRY_THROW(X)
# define TAO_INTERCEPTOR_THROW_RETURN(X,Y) ACE_TRY_THROW(X)
#else
# define TAO_INTERCEPTOR(X)
# define TAO_INTERCEPTOR_CHECK ACE_CHECK
# define TAO_INTERCEPTOR_CHECK_RETURN(X) ACE_CHECK_RETURN (X)
# define TAO_INTERCEPTOR_THROW(X) ACE_THROW(X)
# define TAO_INTERCEPTOR_THROW_RETURN(X,Y) ACE_THROW_RETURN(X,Y)
#endif /* TAO_HAS_INTERCEPTORS */

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLE_INTERCEPTOR_H */
