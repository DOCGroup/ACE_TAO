/*=======================================================================
 *
 * @file ExecutionManager_Impl.h
 *
 * $Id$
 *
 * @brief This file contains implementation for
 * Deployment::ExecutionManager interface.
 *
 * @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 * @auther Tao Lu <lu@dre.vanderbilt.edu>
 *
 *======================================================================*/

#ifndef EXECUTIONMANAGER_IMPL_H
#define EXECUTIONMANAGER_IMPL_H

#include "ExecutionManagerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/Hash_Map_Manager.h"
#include "DomainApplicationManager/DomainApplicationManager_Impl.h"

/**
 *
 * @class ExecutionManager_Impl
 *
 * @brief This class implements the ExecutionManger. ExecutionManager starts
 * the execution process after the planning stage.
 *
 * Serval assumptions I made now:
 *
 * 1. There is an ExecutionManager per application. => AssemblyFactory
 * 2. This interface will be brought up at or before the actual execution
 *    of an application starts.
 * 3. No XML parsing is be done in this interface, the requrest will be
 *    forwarded to the DomainApplicationManager.
 *
 */
namespace CIAO
{
  class ExecutionManager_Impl
    : public virtual POA_CIAO::ExecutionManagerDaemon,
    public virtual PortableServer::RefCountServantBase
    {

    public:
      /// Constructor.
      ExecutionManager_Impl (CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa,
                             const char * init_file
                             ACE_ENV_ARG_DECL);

      // @@ (OO) Since this class is reference counted, please make this
      //         destructor protected to enforce proper memory managment
      //         through the reference counting mechanism (i.e. to
      //         disallow calling operator delete() on an instance of
      //         this class.
      /// Destructor.
      ~ExecutionManager_Impl ();

      /// Generate an ApplicationManager factory.
      virtual Deployment::DomainApplicationManager_ptr
        preparePlan
        (
         const Deployment::DeploymentPlan & plan,
         CORBA::Boolean commitResources
         ACE_ENV_ARG_DECL_WITH_DEFAULTS
         )
        ACE_THROW_SPEC ((CORBA::SystemException,
                         Deployment::ResourceNotAvailable,
                         Deployment::PlanError,
                         Deployment::StartError
                         ));

      /// Return a set of the currently held DomainApplicationMnager.
      virtual Deployment::DomainApplicationManagers *
        getManagers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      /// Destroy a specific manager.
      virtual void
        destroyManager
        (
         Deployment::DomainApplicationManager_ptr manager
         ACE_ENV_ARG_DECL_WITH_DEFAULTS
         )
        ACE_THROW_SPEC ((
                         CORBA::SystemException,
                         Deployment::StopError
                         ));

      virtual void
        shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

    protected:
      /*typedef
      ACE_Hash_Map_Manager_Ex
      < ACE_CString,
        Deployment::DomainApplicationManager_var,
	ACE_Hash<ACE_CString>,
	ACE_Equal_To<ACE_CString>,
	TAO_SYNCH_MUTEX
      >
      Table;
      typedef Table::iterator Iterator;
      */

      /// Cached ORB pointer
      CORBA::ORB_var orb_;

      /// Cached POA pointer
      PortableServer::POA_var poa_;

      // Internal Hashtable
      // Table table_;

      /      // Path to the initialization file
      ACE_CString init_file_;

      Deployment::DomainApplicationManager_var dam_;

      // @@ (OO) You may want to consider caching this->dam_servant_
      //         into a TAO::Utils::Servant_Var<> to ease memory
      //         management, e.g. automatically decreasing reference
      //         counts, etc.
      /// Cached servant of the DomainApplicaitonmanager.
      CIAO::DomainApplicationManager_Impl * dam_servant_;
    };

};

#include /**/ "ace/post.h"
#endif /* EXECUTIONMANAGER_IMPL_H */
