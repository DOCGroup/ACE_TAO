// -*- C++ -*-

//=============================================================================
/**
 *  @file    Adapter_Factory.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@uci.edu)
 */
//=============================================================================

#ifndef TAO_ADAPTER_FACTORY_H
#define TAO_ADAPTER_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Adapter;
class TAO_ORB_Core;

class TAO_Export TAO_Adapter_Factory : public ACE_Service_Object
{
public:
  /// Destructor
  virtual ~TAO_Adapter_Factory (void);

  /// Create a new adapter
  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core) = 0;
};

#include /**/ "ace/post.h"

#endif /* TAO_ADAPTER_FACTORY_H */
