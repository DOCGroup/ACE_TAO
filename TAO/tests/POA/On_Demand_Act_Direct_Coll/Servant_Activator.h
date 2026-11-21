// -*- C++ -*-

//=============================================================================
/**
 *  @file     Servant_Activator.h
 *
 *   Defines a ServantActivator class.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/LocalObject.h"

#include "tao/ORB.h"

class ServantActivator :
  public virtual PortableServer::ServantActivator,
  public virtual ::CORBA::LocalObject
{
public:
  ServantActivator (CORBA::ORB_ptr orb, ACE_thread_t thrid);
  ServantActivator (ServantActivator const &) = delete;
  ServantActivator (ServantActivator &&) = delete;
  ServantActivator &operator= (ServantActivator const &) = delete;
  ServantActivator &operator= (ServantActivator &&) = delete;

  /**
   * This method is invoked by a POA with USE_SERVANT_MANAGER and
   * RETAIN policies, whenever it receives a request for a test object
   * that is not currently active.
   */
  PortableServer::Servant incarnate (PortableServer::ObjectId const &oid,
                                     PortableServer::POA_ptr poa) override;

  /// This method is invoked whenever a test object is deactivated.
  void etherealize (PortableServer::ObjectId const &oid,
                    PortableServer::POA_ptr adapter,
                    PortableServer::Servant servant,
                    CORBA::Boolean cleanup_in_progress,
                    CORBA::Boolean remaining_activations) override;

private:
  /// A reference to the ORB.
  CORBA::ORB_var orb_;
  ACE_thread_t thr_id_;
};
