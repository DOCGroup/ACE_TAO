// -*- C++ -*-

//=============================================================================
/**
 *  @file    Adapter_Factory.h
 *
 *  @author Carlos O'Ryan (coryan@uci.edu)
 */
//=============================================================================

#ifndef TAO_ADAPTER_FACTORY_H
#define TAO_ADAPTER_FACTORY_H

#include /**/ "ace/pre.h"

#include /**/ "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Versioned_Namespace.h"

#include "ace/Service_Object.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Adapter;
class TAO_ORB_Core;

class TAO_Export TAO_Adapter_Factory : public ACE_Service_Object
{
public:
  /// Destructor
  virtual ~TAO_Adapter_Factory ();

  /// Create a new adapter
  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ADAPTER_FACTORY_H */
