// $Id$

//=============================================================================
/**
 * @file Object_Factory_i.h
 *
 * $Id$
 *
 * Implementation header for the "Gateway" IDL interface for the
 * ORT example.
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 * @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef OBJECT_FACTORY_I_H
#define OBJECT_FACTORY_I_H

#include "GatewayS.h"

#include "tao/PortableServer/PortableServerC.h"
#include "tao/ORB.h"

class Object_Factory_i : public virtual POA_Gateway::Object_Factory
{
 public:

  /// Constructor
  Object_Factory_i (CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr gateway_poa);

  CORBA::Object_ptr
    create_object (const char *interface_repository_id,
                   CORBA::Object_ptr gatewayed_object
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

 private:

  CORBA::ORB_ptr orb_;
  PortableServer::POA_ptr gateway_poa_;
};

#endif /* OBJECT_FACTORY_I_H */
