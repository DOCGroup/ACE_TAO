// -*- C++ -*-

//=============================================================================
/**
 *  @file    Daemon_Impl.h
 *
 *  $Id$
 *
 *  This file contains servant implementation for CIAO's daemon process control
 *  interface.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_NODEDAEMON_IMPL_H
#define CIAO_NODEDAEMON_IMPL_H
#include /**/ "ace/pre.h"

#include "NodeDaemonS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Functor.h"

namespace CIAO
{
  /**
   * @class NodeDaemon_Impl
   *
   * @brief Servant implementation CIAO's daemon process control interface
   *
   * This class implements the CIAO:NodeDaemon interface.
   *
   */
  class NodeDaemon_Impl
    : public virtual POA_CIAO::NodeDaemon,
      public virtual PortableServer::RefCountServantBase
  {
  public:

    /// Constructor
    NodeDaemon_Impl (const char *name,
		     CORBA::ORB_ptr o,
		     PortableServer::POA_ptr p);

    /// Destructor
    virtual ~NodeDaemon_Impl (void);

    /***************************************************
     *@@ Below are helper operations for the NodeDaemon which
     *   don't belong to the NodeManager interface.
     ***************************************************/

    /// Get the containing POA.  This operation does *not*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _default_POA (void);

    /// CIAO::Daemon defined attributes/operations.

    virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CORBA::Object_ptr get_service (const char * svc_name
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CIAO::NotFound));

    virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /**
     * @name CIAO::Daemon_Impl internal operations using the
     * canonical ACE_Map methods
     */
    //@{
    int bind (const char *orb_id, CORBA::Object_ptr obj);
    int unbind (const char *orb_id);
    //@}


    /***************************************************
     *@@ Below are operation from NodeManager Interface.
     *
     ***************************************************/
    virtual void joinDomain (
        const Deployment::Domain & domain,
        Deployment::TargetManager_ptr manager,
        Deployment::Logger_ptr log
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46

    virtual void leaveDomain (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46

    virtual ::Deployment::NodeApplicationManager_ptr preparePlan (
        const Deployment::DeploymentPlan & plan
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , Deployment::StartError
        , Deployment::PlanError
      ));

    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46

    virtual void destroyManager (
        Deployment::NodeApplicationManager_ptr appManager
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , Deployment::StopError
      ));

  protected:
    typedef ACE_Hash_Map_Manager_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, TAO_SYNCH_MUTEX> Table;
    typedef Table::iterator Iterator;

    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// My Canonical name.
    CORBA::String_var name_;

    /// Internal object hash table.
    Table table_;
  };
}

#if defined (__ACE_INLINE__)
# include "NodeDaemon_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_NODEDAEMON_IMPL_H */
