// -*- C++ -*-

//=============================================================================
/**
 *  @file    Adapter_Activator.h
 */
//=============================================================================

#ifndef TAO_ADAPTER_ACTIVATOR_H
#define TAO_ADAPTER_ACTIVATOR_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/AdapterActivatorC.h"
#include "tao/PortableServer/POAManagerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)
#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

class Adapter_Activator
  : public PortableServer::AdapterActivator
{
public:
  explicit Adapter_Activator (PortableServer::POAManager_ptr poa_manager);

  Adapter_Activator (Adapter_Activator const &) = delete;
  Adapter_Activator &operator= (Adapter_Activator const &) = delete;
  Adapter_Activator (Adapter_Activator &&) = delete;
  Adapter_Activator &operator= (Adapter_Activator &&) = delete;

  CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
                                  char const *name) override;

protected:
  /// POA Manager
  PortableServer::POAManager_var poa_manager_;
};

#endif
#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_ADAPTER_ACTIVATOR_H */
