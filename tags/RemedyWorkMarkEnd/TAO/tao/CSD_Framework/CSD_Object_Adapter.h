// -*- C++ -*-

//=============================================================================
/**
 *  @file CSD_Object_Adapter.h
 *
 *  $Id$
 *
 *  @author Yan Dai (dai_y@ociweb.com)
 */
//=============================================================================


#ifndef TAO_CSD_OBJECT_ADAPTER_H
#define TAO_CSD_OBJECT_ADAPTER_H
#include /**/ "ace/pre.h"

#include "tao/CSD_Framework/CSD_FW_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Object_Adapter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_CSD_Object_Adapter
 *
 * @brief Defines the CSD version Object Adapter which overrides
 *        default dispatch implementation.
 *
 * This class will be used as a facade for the CSD POAs in a server
 */
class TAO_CSD_FW_Export TAO_CSD_Object_Adapter : public TAO_Object_Adapter
{
public:

  /// Constructor
  TAO_CSD_Object_Adapter (const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters,
                          TAO_ORB_Core &orb_core);

  /// Destructor
  virtual ~TAO_CSD_Object_Adapter (void);

  /// Hand the request to the Service_Dispatching_Strategy_Proxy for
  /// dispatching.
  virtual void do_dispatch (TAO_ServerRequest& req,
                            TAO::Portable_Server::Servant_Upcall& upcall);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_CSD_OBJECT_ADAPTER_H */
