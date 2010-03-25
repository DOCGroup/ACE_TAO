// -*- C++ -*-

//=============================================================================
/**
 *  @file    DomainApplicationManager_Impl.h
 *
 *  $Id$
 *
 * @brief  Implementation of POA_Deployment::DomainApplicationManager
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 */
//=============================================================================

#ifndef DOMAINAPPLICATIONMANAGER_IMPL_H_
#define DOMAINAPPLICATIONMANAGER_IMPL_H_

#include "ace/Vector_T.h"
#include "ace/SStringfwd.h"
#include "ace/Map_Manager.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "DomainApplicationManager_Export.h"
#include "Node_Locator.h"
#include "Deployment/Deployment_NodeManagerC.h"
#include "Deployment/Deployment_NodeManagerS.h"
#include "DomainApplication/Domain_Application_Impl.h"
#include "Completion/CompletionHandler_T.h"
#include "Completion/Completion_Counter_Base.h"
#include "Deployment/Deployment_DomainApplicationManagerS.h"
#include "Split_Plan/Split_Plan_impl.h"

namespace DAnCE
  {
  class DomainApplicationManager_Impl;

  typedef PortableServer::Servant_var<DomainApplicationManager_Impl> DomainApplicationManager_Impl_var;

  typedef CompletionHandler<DomainApplicationManager_Impl>  DAM_CompletionHandler;

  typedef ACE_Refcounted_Auto_Ptr<DAM_CompletionHandler, ACE_Null_Mutex>  DAM_CompletionHandler_AutoPtr;

  class DAM_NM_ReplyHandlerImpl;

  class DomainApplicationManager_Export DomainApplicationManager_Impl
        : public virtual POA_Deployment::AMH_DomainApplicationManager
    {
    public:
      DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
                                     PortableServer::POA_ptr poa,
                                     const ::Deployment::DeploymentPlan& plan,
                                     Node_Locator &nodes);

      virtual ~DomainApplicationManager_Impl();

      virtual void startLaunch (
        ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh,
        const ::Deployment::Properties & configProperty);

      virtual void destroyApplication (
        ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh,
        ::Deployment::Application_ptr);

      virtual void getApplications (
        ::Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh);

      virtual void getPlan (
        ::Deployment::AMH_DomainApplicationManagerResponseHandler_ptr _tao_rh);

      const char* getPlanUUID (void);

      void preparePlan (DAM_CompletionHandler* em_handler);

      void destroyManager (DAM_CompletionHandler* em_handler);

      class StartLaunchCompletionHandler
            : public CompletionHandler<DomainApplication_Impl>
        {
        public:
          StartLaunchCompletionHandler (
            DomainApplicationManager_Impl *dam_servant,
            ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh);
          ~StartLaunchCompletionHandler ();

          virtual void handle_completion (DomainApplication_Impl *da_servant);
          virtual void handle_exception (CORBA::Exception* local_ex);

        private:
          DomainApplicationManager_Impl_var dam_servant_;
          ::Deployment::AMH_ApplicationManagerResponseHandler_var dam_rh_;
        };

      class DestroyManagerCompletionHandler
            : public CompletionHandler<DomainApplication_Impl>,
              public Completion_Counter_Base<ACE_Null_Mutex>
        {
        public:
          DestroyManagerCompletionHandler (
            DomainApplicationManager_Impl *dam_servant,
            const DAM_CompletionHandler_AutoPtr& ch_ptr,
            unsigned int exec_count);

          virtual void handle_completion (DomainApplication_Impl *da_servant);
          virtual void handle_exception (CORBA::Exception* local_ex);

        protected:
          virtual void on_all_completed ();
          virtual void on_all_completed_with_failure ();

        private:
          DomainApplicationManager_Impl_var dam_servant_;
          DAM_CompletionHandler_AutoPtr dam_ch_;
        };

      class DestroyApplicationCompletionHandler
            : public CompletionHandler<DomainApplication_Impl>
        {
        public:
          DestroyApplicationCompletionHandler (
            DomainApplicationManager_Impl *dam_servant,
            ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh,
            DomainApplication_Impl *da_servant);

          virtual void handle_completion (DomainApplication_Impl *da_servant);
          virtual void handle_exception (CORBA::Exception* local_ex);

        private:
          DomainApplicationManager_Impl_var dam_servant_;
          ::Deployment::AMH_ApplicationManagerResponseHandler_var dam_rh_;
          DomainApplication_Impl_var da_servant_;
        };

    private:
      void finish_startLaunch (
        ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh,
        DomainApplication_Impl *da_servant,
        const ::Deployment::Connections & providedReference);
      void fail_startLaunch (
        ::Deployment::AMH_ApplicationManagerResponseHandler_ptr _tao_rh,
        CORBA::Exception* local_ex);

      void finishDestroyManager (const DAM_CompletionHandler_AutoPtr& ch_ptr);

      friend class StartLaunchCompletionHandler;
      friend class DestroyManagerCompletionHandler;

      friend class DAM_NM_ReplyHandlerImpl;

      CORBA::ORB_var orb_;
      PortableServer::POA_var poa_;
      const Deployment::DeploymentPlan plan_;
      DomainApplication_Impl::TNam2Nm sub_app_mgr_;
      DomainApplication_Impl::TNm2Id node_ids_;
      Node_Locator &nodes_;
      typedef ACE_Vector<DomainApplication_Impl*> TApplications;
      TApplications running_app_;
    };

  class DAM_NM_ReplyHandlerImpl
      : public POA_Deployment::AMI_NodeManagerHandler
  {
  public:
    class Counter
        : public Completion_Counter_Base<ACE_Null_Mutex>
    {
    public:
      Counter (unsigned int exec_count,
               DomainApplicationManager_Impl* dam_servant,
               const DAM_CompletionHandler_AutoPtr& em_completion_handler);

    protected:
      virtual void on_all_completed ();
      virtual void on_all_completed_with_failure ();

    private:
      DomainApplicationManager_Impl_var dam_servant_;
      DAM_CompletionHandler_AutoPtr em_ch_ptr_;
    };

    typedef ACE_Refcounted_Auto_Ptr<Counter, ACE_Null_Mutex> Counter_AutoPtr;

    DAM_NM_ReplyHandlerImpl (DomainApplicationManager_Impl* dam_servant,
                             Deployment::NodeManager_ptr nm,
                             const char* node_id,
                             const Counter_AutoPtr& counter);
    virtual ~DAM_NM_ReplyHandlerImpl ();

    virtual void joinDomain (void);
    virtual void joinDomain_excep (
      ::Messaging::ExceptionHolder * excep_holder);

    virtual void leaveDomain (void);
    virtual void leaveDomain_excep (
      ::Messaging::ExceptionHolder * excep_holder);

    virtual void preparePlan (
      ::Deployment::NodeApplicationManager_ptr ami_return_val);
    virtual void preparePlan_excep (
      ::Messaging::ExceptionHolder * excep_holder);

    virtual void destroyManager (void);
    virtual void destroyManager_excep (
      ::Messaging::ExceptionHolder * excep_holder);

    virtual void getDynamicResources (
      const ::Deployment::Resources & ami_return_val);
    virtual void getDynamicResources_excep (
      ::Messaging::ExceptionHolder * excep_holder);

  private:
    DomainApplicationManager_Impl_var dam_servant_;
    Deployment::NodeManager_var nm_;
    ACE_CString node_id_;
    Counter_AutoPtr counter_;
  };

  }//DAnCE

#endif /*DOMAINAPPLICATIONMANAGER_IMPL_H_*/
