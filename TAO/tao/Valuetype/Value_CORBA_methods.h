// -*- C++ -*-

//=============================================================================
/**
 *  @file    Value_CORBA_methods.h
 *
 *  $Id$
 *
 *  Declarations of common ValueType methods in the CORBA namespace.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_VALUETYPE_CORBA_METHODS_H
#define TAO_VALUETYPE_CORBA_METHODS_H

#include /**/ "ace/pre.h"

#include "valuetype_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Valuetype_Export

namespace CORBA
{
  class ValueBase;

  extern TAO_Valuetype_Export void add_ref (ValueBase *);
  extern TAO_Valuetype_Export void remove_ref (ValueBase *);
}

#include /**/ "ace/post.h"

#endif  /* TAO_VALUETYPE_CORBA_METHODS_H */
