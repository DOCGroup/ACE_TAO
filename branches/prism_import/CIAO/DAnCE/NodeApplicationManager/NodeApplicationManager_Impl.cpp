// $Id$

#include "ace/Log_Msg.h"
#include "ace/streams.h"

#include "NodeApplicationManager_Impl.h"
#include "NodeApplication/NodeApplication_Impl.h"
#include "NodeManager/NodeManager_Impl.h"

using namespace DAnCE;

NodeApplicationManager_Impl::NodeApplicationManager_Impl (CORBA::ORB_ptr orb
                                                          , PortableServer::POA_ptr poa
                                                          , const Deployment::DeploymentPlan& plan
                                                          , RedirectionService & redirection
                                                          , const ACE_CString& node_name)
    : plan_ (plan)
    , orb_ (CORBA::ORB::_duplicate (orb))
    , poa_ (PortableServer::POA::_duplicate (poa))
    , application_ (0)
    , redirection_ (redirection)
    , node_name_ (node_name)
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_Impl constructor for node %s and plan %s starting...\n", node_name.c_str(), plan_.UUID.in()));
  this->register_plan();
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_Impl constructor for node %s and plan %s completed.\n", node_name.c_str(), plan_.UUID.in()));
}

NodeApplicationManager_Impl::~NodeApplicationManager_Impl()
{
  if (this->application_ != 0)
    {
      CORBA::Object_var app = this->poa_->servant_to_reference (this->application_);
      PortableServer::ObjectId_var id = this->poa_->reference_to_id (app);
      this->poa_->deactivate_object (id);

      delete this->application_;
      this->application_ = 0;
    }
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_impl destructor\n"));
}

Deployment::Application_ptr
NodeApplicationManager_Impl::startLaunch (const Deployment::Properties &,
                                          Deployment::Connections_out providedReference)
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_impl::startLaunch - started\n"));

  // Creating NodeApplication object
  ACE_NEW_THROW_EX (this->application_,
                    NodeApplication_Impl (this->orb_.in(),
                                          this->poa_.in(),
                                          this->plan_,
                                          this->redirection_,
                                          this->node_name_),
                    CORBA::NO_MEMORY ());

  this->application_->init();
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_impl::startLaunch - ComponentInstallation and ServerActivator objects have been created\n"));
  this->application_->initComponents();
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_impl::startLaunch - ComponentInitialization have been finished\n"));
  providedReference = this->application_->getAllConnections();
  //this->parent_.registerConnections(this->plan_.UUID.in(), *providedReference);
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_impl::startLaunch - providedConnection have been setted\n"));
  PortableServer::ObjectId_var as_id =
    this->poa_->activate_object (this->application_);

  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_impl::startLaunch - applicationServant object created\n"));
  CORBA::Object_var as_obj = this->poa_->id_to_reference (as_id.in ());
  Deployment::Application_var app = Deployment::Application::_narrow (as_obj.in ());
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_impl::startLaunch - finished\n"));
  return app._retn ();
}

void
NodeApplicationManager_Impl::destroyApplication (Deployment::Application_ptr application)
{
  ACE_DEBUG ( (LM_TRACE, "[%M] NodeApplicationManager_Impl::destroyApplication - started\n"));

  try
  {
    this->redirection_.unregister (this->node_name_, this->plan_.UUID.in());

    if (!application->_is_equivalent (this->poa_->servant_to_reference (this->application_)))
      {
        ACE_ERROR ( (LM_ERROR, "[%M] NodeApplicationManager_Impl::destroyApplication - application is equivalent to current application \n"));
        throw ::Deployment::StopError();
      }
    PortableServer::ObjectId_var id = this->poa_->reference_to_id (application);
    this->poa_->deactivate_object (id);

    delete this->application_;
    this->application_ = 0;
  }
  catch (CORBA::SystemException &)
  {
    throw;
  }
  catch (Deployment::StopError &)
  {
    throw;
  }
  catch (CORBA::UserException &e)
  {
    ACE_ERROR((LM_ERROR, "[%M] NodeApplicationManager_Impl::destroyApplication failed with UserException %s(%s) \"%s\"\n"
        , e._name(), e._rep_id(), e._info().c_str()));
    throw Deployment::StopError(e._name(), e._info().c_str());
  }
  catch (...)
  {
    ACE_ERROR((LM_ERROR, "[%M] NodeApplicationManager_Impl::destroyApplication failed with undefined exception.\n"));
    throw Deployment::StopError();
  }
  ACE_DEBUG ( (LM_TRACE, "[%M] NodeApplicationManager_Impl::destroyApplication - finished\n"));
}

void NodeApplicationManager_Impl::register_plan()
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_Impl::register_plan starting.\n"));
  this->redirection_.registration_start (this->node_name_, this->plan_.UUID.in());
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_Impl::register_plan - registering objects...\n"));
  for (unsigned int i = 0; i < this->plan_.instance.length(); i++)
    {
      this->redirection_.registration (this->node_name_,
                                       this->plan_.UUID.in(),
                                       this->plan_.instance[i].name.in(),
                                       CORBA::Object::_nil());
    }
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_Impl::register_plan - registering endpoints...\n"));
  for (unsigned int i = 0; i < this->plan_.connection.length(); i++)
    {
      for (unsigned int j = 0; j < this->plan_.connection[i].internalEndpoint.length(); j++)
        {
          if (this->plan_.connection[i].internalEndpoint[j].provider)
            {
              this->redirection_.registration (this->node_name_,
                                               this->plan_.UUID.in(),
                                               this->plan_.instance[this->plan_.connection[i].internalEndpoint[j].instanceRef].name.in(),
                                               this->plan_.connection[i].internalEndpoint[j].portName.in(),
                                               CORBA::Object::_nil());
            }
        }
    }
  this->redirection_.registration_finish (this->node_name_, this->plan_.UUID.in());
  ACE_DEBUG ( (LM_DEBUG, "[%M] NodeApplicationManager_Impl::register_plan completed.\n"));
}
