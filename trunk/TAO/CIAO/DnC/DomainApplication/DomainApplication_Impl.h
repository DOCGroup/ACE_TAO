//==================================================================
/**
 *  @file DomainApplication_Impl.h
 *
 *  $Id$
 *
 *  This file contains implementation for the servant of
 *  Deployment::DomainApplication.
 *
 *  @author Gan Deng <gan.deng@vanderbilt.edu>
 */
//=====================================================================

#ifndef CIAO_DOMAINAPPLICATION_IMPL_H
#define CIAO_DOMAINAPPLICATION_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "../DeploymentS.h"

#include "ciao/Object_Set_T.h" // Todo: do we need this?

namespace CIAO
{
  /**
   * @class DomainApplication_Impl
   *
   * @brief A servant for the Deployment::DomainApplication interface.
   * A ¡°global¡± DomainApplication has references to node-specific NodeApplication
   * elements as created by the <startLaunch> operation of the DomainApplicationManager.
   * The <finishLaunch> operation then calls <finishLaunch> on the node-specific 
   * NodeApplication instances, passing the relevant connections as determined by
   * the separation of the ¡°global¡± DeploymentPlan into node-specific plans.
   */
  class CIAO_DOMAINAPPLICATION_Export DomainApplication_Impl
    : public virtual POA_Deployment::DomainApplication,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /// Constructor
    DomainApplication_Impl (CORBA::ORB_ptr orb,
				                    PortableServer::POA_ptr poa);

    /// Destructor
    virtual ~DomainApplication_Impl (void);


    /*===========================================================
     * Below are operations from the DomainApplication
     *
     *============================================================*/

    /** 
     * The second step in launching an application. External references may be provided
     * to connect to the component¡¯s external user ports. If the start parameter is
     * true, the application is started as well. Raises the InvalidConnection if one of
     * the provided references is invalid. Raises the StartError exception if launching
     * or starting the application fails.
     */
    virtual void finishLaunch (const Deployment::Connections & providedReference,
                               CORBA::Boolean start
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Deployment::StartError,
                       Deployment::InvalidConnection));

    /**
     * Starts the application. Raises the StartError exception if starting the application
     * fails.
     */
    virtual void start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException, Deployment::StartError));

  protected:
    /// location of the Domainapplication
    CORBA::String_var domainapp_path_;

    /// Keep a pointer to the managing ORB serving this servant.
    CORBA::ORB_var orb_;

    /// Keep a pointer to the managing POA.
    PortableServer::POA_var poa_;

    /// Cache a object reference to this servant.
    Deployment::DomainApplicationManager_var objref_;

    /// Cache the ior of the previous reference
    CORBA::String_var ior_;

    /// Default svc.conf filename.
    ACE_CString default_svcconf_file_;

    /// Cached deployment plan for the particular domain.
    /// The plan will be initialized when init is called.
    Deployment::DeploymentPlan_var plan_;
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_DOMAINAPPLICATION_IMPL_H */
