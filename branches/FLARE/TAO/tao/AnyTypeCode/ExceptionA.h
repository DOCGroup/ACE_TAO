// -*- C++ -*-

//=============================================================================
/**
 *  @file    ExceptionA.h
 *
 *  $Id$
 *
 *   This file defines way in which CORBA exceptions are reported.
 *
 *  @author DOC Group at Vanderbilt U., Wash U, and UCI
 */
//=============================================================================

#ifndef TAO_EXCEPTIONA_H
#define TAO_EXCEPTIONA_H

#include /**/ "ace/pre.h"

// Do not try removing this. If you remove this for subsetting lots of
// things go wrong in TAO.
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"
#include "tao/Exception.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Any;
  typedef Any *Any_ptr;
}

TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, const CORBA::Exception &);
TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::Exception *);

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/"ace/post.h"

#endif /* TAO_EXCEPTIONA_H */
