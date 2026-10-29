//=============================================================================
/**
 * @file Object_Factory_i.h
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
  Object_Factory_i (Object_Factory_i const &) = delete;
  Object_Factory_i (Object_Factory_i &&) = delete;
  Object_Factory_i &operator= (Object_Factory_i const &) = delete;
  Object_Factory_i &operator= (Object_Factory_i &&) = delete;

  CORBA::Object_ptr
    create_object (char const *interface_repository_id,
                   CORBA::Object_ptr gatewayed_object) override;

 private:
  CORBA::ORB_ptr orb_;
  PortableServer::POA_ptr gateway_poa_;
};

#endif /* OBJECT_FACTORY_I_H */
