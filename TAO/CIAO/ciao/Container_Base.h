// -*- C++ -*-

//=============================================================================
/**
 *  @file    Container_Base.h
 *
 *  $Id$
 *
 *  Header file for CIAO's container implementations
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_CONTAINER_BASE_H
#define CIAO_CONTAINER_BASE_H

#include "ace/pre.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "CIAO_CONTAINER_export.h"


namespace CIAO
{
  /**
   * @class Container
   *
   * @brief Common container interface definition.
   *
   * Perhaps we can use local interface to define these interfaces as
   * we will also get reference counting automatically.
   */
  class CIAO_CONTAINER_Export Container
  {
  public:
    Container (CORBA::ORB_ptr o);

    virtual ~Container (void) = 0;

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr the_POA (void);

    /// Initialize the container with a name.
    virtual int init (const char *name = 0
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  protected:
    CORBA::ORB_var orb_;

    PortableServer::POA_var poa_;
  };

  class CIAO_CONTAINER_Export Session_Container : public Container
  {
  public:
    Session_Container (CORBA::ORB_ptr o);

    virtual ~Session_Container (void);

    /// Initialize the container with a name.
    virtual int init (const char *name = 0
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Install a servant for component or home.
    virtual CORBA::Object_ptr install_servant (PortableServer::Servant p
                                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Uninstall a servant for component or home.
    virtual void uninstall_reference (CORBA::Object_ptr objref
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    long number_;

    static ACE_Atomic_Op <ACE_Thread_Mutex, long> serial_number_;
  };
};

#endif /* CIAO_CONTAINER_BASE_H */
