// -*- C++ -*-

//=============================================================================
/**
 *  @file     Servant_Activator.h
 *
 *   Defines a ServantActivator class, used with a POA having a
 *   RETAIN policy
 *
 *  @author  Irfan Pyarali  Michael Kircher
 */
//=============================================================================

#ifndef SERVANT_ACTIVATOR_H
#define SERVANT_ACTIVATOR_H

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/LocalObject.h"

class ServantActivator :
  public virtual PortableServer::ServantActivator,
  public virtual ::CORBA::LocalObject
{
  // = TITLE
  //   This class is used by a POA with USE_SERVANT_MANAGER and
  //   RETAIN policy.
public:
  ServantActivator (CORBA::ORB_ptr orb,
                    CORBA::Object_ptr forward_to);

  ServantActivator (ServantActivator const &) = delete;
  ServantActivator &operator= (ServantActivator const &) = delete;
  ServantActivator (ServantActivator &&) = delete;
  ServantActivator &operator= (ServantActivator &&) = delete;

  PortableServer::Servant incarnate (PortableServer::ObjectId const &oid,
                                     PortableServer::POA_ptr adapter) override;

  void etherealize (PortableServer::ObjectId const &oid,
                    PortableServer::POA_ptr adapter,
                    PortableServer::Servant serv,
                    CORBA::Boolean cleanup_in_progress,
                    CORBA::Boolean remaining_activations) override;

  /// Object should be forwarded from now on.
  void forward_requests ();

private:
  CORBA::ORB_var orb_;

  /// Location to forward to
  CORBA::Object_var forward_to_;
};

#endif /* SERVANT_ACTIVATOR_H */
