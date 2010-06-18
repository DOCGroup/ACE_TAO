// $Id$
/**========================================================
 *
 * @file   Domain_Application_Impl.h
 *
 * @brief  This file contains the implementation of
 *         the DomainApplication interface.
 *
 * @author Vinzenz Tornow <vt@prismtech.com>
 *========================================================*/

#ifndef DOMAIN_APPLICATION_H
#define DOMAIN_APPLICATION_H
#include /**/ "ace/pre.h"

#include "Domain_Application_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Deployment/Deployment_DomainApplicationS.h"
#include "Deployment/Deployment_NodeApplicationManagerC.h"
#include "Deployment/Deployment_NodeApplicationManagerS.h"
#include "Deployment/Deployment_NodeManagerC.h"
#include "Completion/CompletionHandler_T.h"
#include "Completion/Completion_Counter_Base.h"
#include "ace/Map_Manager.h"
#include "ace/Refcounted_Auto_Ptr.h"

namespace DAnCE
  {
  class DomainApplication_Impl;

  typedef CompletionHandler<DomainApplication_Impl>  DA_CompletionHandler;

  typedef ACE_Refcounted_Auto_Ptr<DA_CompletionHandler, ACE_Null_Mutex>  DA_CompletionHandler_AutoPtr;

  typedef PortableServer::Servant_var<DomainApplication_Impl> DomainApplication_Impl_var;

  class DA_NAM_ReplyHandlerImpl;
  class DA_NA_ReplyHandlerImpl;

  /**
   * @class DomainApplication_Impl
   *
   * @brief This class implements the DomainApplication interface.
   **/
  class Domain_Application_Export DomainApplication_Impl
        : public virtual POA_Deployment::AMH_DomainApplication
    {
    public:
      typedef ACE_Map_Manager < ::Deployment::NodeApplicationManager_var,
        ::Deployment::NodeManager_var,
        ACE_Null_Mutex > TNam2Nm;
      typedef ACE_Map_Manager<
        ::Deployment::NodeManager_var,
        ACE_CString,
        ACE_Null_Mutex> TNm2Id;
      typedef ACE_Map_Manager < ::Deployment::Application_var,
        ::Deployment::NodeApplicationManager_var,
        ACE_Null_Mutex > TApp2Mgr;
      typedef ACE_Map_Manager < ::Deployment::Application_var,
        ACE_CString,
        ACE_Null_Mutex > TApp2Id;

      DomainApplication_Impl (const char* plan_uuid,
                              PortableServer::POA_ptr poa,
                              TNam2Nm& nams,
                              TNm2Id& node_ids);

      virtual ~DomainApplication_Impl();

      void startLaunch (const ::Deployment::Properties & configProperty,
                        DA_CompletionHandler* dam_handler);

      void destroyApplication (const DA_CompletionHandler_AutoPtr& _ch_ptr);

      void getConnections (::Deployment::Connections& conn);

      const char* getPlanUUID (void);

      /**
      * The second step in launching an application in the
      * domain-level.  If the start parameter is true, the application
      * is started as well.  Raises the StartError exception if
      * launching or starting the application fails.
      */
      virtual void
      finishLaunch (::Deployment::AMH_ApplicationResponseHandler_ptr _tao_rh,
                    const ::Deployment::Connections & connections,
                    CORBA::Boolean start);
      /**
      * Starts the application. Raises the StartError exception if
      * starting the application fails.
      */
      virtual void start (::Deployment::AMH_ApplicationResponseHandler_ptr _tao_rh);

    private:
      friend class DA_NAM_ReplyHandlerImpl;
      friend class DA_NA_ReplyHandlerImpl;

      PortableServer::POA_var poa_;
      ACE_CString planUUID_;
      TApp2Mgr node_applications_;
      TApp2Id app_node_ids_;
      ::Deployment::Connections connections_;
      TNam2Nm & nams_;
      TNm2Id & node_ids_;
    };

  class DA_NAM_ReplyHandlerImpl
        : public POA_Deployment::AMI_NodeApplicationManagerHandler
    {
    public:
      class Counter
          : public Completion_Counter_Base<ACE_Null_Mutex>
        {
        public:
          Counter (unsigned int exec_count,
                   DomainApplication_Impl* da_servant,
                   const DA_CompletionHandler_AutoPtr& dam_completion_handler);

          protected:
            virtual void on_all_completed ();
            virtual void on_all_completed_with_failure ();

          private:
            DomainApplication_Impl_var da_servant_;
            DA_CompletionHandler_AutoPtr dam_ch_ptr_;
        };

      typedef ACE_Refcounted_Auto_Ptr<Counter, ACE_Null_Mutex> Counter_AutoPtr;

      DA_NAM_ReplyHandlerImpl (DomainApplication_Impl* da_servant,
                               ::Deployment::NodeApplicationManager_ptr nam,
                               const char* node_id,
                               const Counter_AutoPtr& counter);

      virtual void startLaunch (
        ::Deployment::Application_ptr ami_return_val,
        const ::Deployment::Connections & providedReference);
      virtual void startLaunch_excep (
        ::Messaging::ExceptionHolder * excep_holder);

      virtual void destroyApplication (void);
      virtual void destroyApplication_excep (
        ::Messaging::ExceptionHolder * excep_holder);

    private:
      DomainApplication_Impl_var da_servant_;
      ACE_CString node_id_;
      ::Deployment::NodeApplicationManager_var nam_;
      Counter_AutoPtr counter_;
    };

  class DA_NA_ReplyHandlerImpl
        : public POA_Deployment::AMI_ApplicationHandler
    {
    public:
      class Counter
          : public Completion_Counter_Base<ACE_Null_Mutex>
        {
        public:
          Counter (unsigned int exec_count,
                  DomainApplication_Impl* da_servant,
                  ::Deployment::AMH_ApplicationResponseHandler_ptr _tao_rh);

          protected:
            virtual void on_all_completed ();
            virtual void on_all_completed_with_failure ();

          private:
            DomainApplication_Impl_var da_servant_;
            ::Deployment::AMH_ApplicationResponseHandler_var da_rh_;
        };

      typedef ACE_Refcounted_Auto_Ptr<Counter, ACE_Null_Mutex> Counter_AutoPtr;

      DA_NA_ReplyHandlerImpl (DomainApplication_Impl* da_servant,
                              const char* node_id,
                              const Counter_AutoPtr& counter);

      virtual void finishLaunch ();
      virtual void finishLaunch_excep (
        ::Messaging::ExceptionHolder * excep_holder);

      virtual void start (void);
      virtual void start_excep (
        ::Messaging::ExceptionHolder * excep_holder);

    private:
      DomainApplication_Impl_var da_servant_;
      ACE_CString node_id_;
      Counter_AutoPtr counter_;
    };

} // DAnCE

#include /**/ "ace/post.h"
#endif // DOMAIN_APPLICATION_H


