// -*- C++ -*-
// $Id$

// ================================================================
//
// = LIBRARY
//     TAO/examples/POA/Forwarding
//
// = FILENAME
//     Servant_Activator.h
//
// = DESCRIPTION
//     Defines a ServantActivator class, used with a POA having a
//     RETAIN policy
//
// = AUTHOR
//     Irfan Pyarali
//     Michael Kircher
//
// ================================================================

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

  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId & oid,
                                             PortableServer::POA_ptr adapter);

  virtual void etherealize (const PortableServer::ObjectId & oid,
                            PortableServer::POA_ptr adapter,
                            PortableServer::Servant serv,
                            CORBA::Boolean cleanup_in_progress,
                            CORBA::Boolean remaining_activations);

  void forward_requests (void);
  // Object should be forwarded from now on.

private:
  CORBA::ORB_var orb_;

  CORBA::Object_var forward_to_;
  // Location to forward to
};

#endif /* SERVANT_ACTIVATOR_H */
