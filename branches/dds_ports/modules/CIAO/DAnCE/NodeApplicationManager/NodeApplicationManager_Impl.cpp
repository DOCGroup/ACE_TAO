// $Id$

#include "ace/Log_Msg.h"
#include "ace/streams.h"
#include "DAnCE/Deployment/DAnCE_PropertiesC.h"
#include "DAnCE/Logger/Log_Macros.h"

#include "NodeApplicationManager_Impl.h"
#include "NodeApplication/NodeApplication_Impl.h"
#include "NodeManager/NodeManager_Impl.h"

using namespace DAnCE;

NodeApplicationManager_Impl::NodeApplicationManager_Impl (CORBA::ORB_ptr orb,
                                                          PortableServer::POA_ptr poa,
                                                          const Deployment::DeploymentPlan& plan,
                                                          RedirectionService & redirection,
                                                          const ACE_CString& node_name,
                                                          const PROPERTY_MAP &properties)
    : plan_ (plan),
      orb_ (CORBA::ORB::_duplicate (orb)),
      poa_ (PortableServer::POA::_duplicate (poa)),
      application_ (0),
      redirection_ (redirection),
      node_name_ (node_name),
      properties_ ()
{
  DANCE_TRACE (DLINFO "NodeApplicationManager_Impl::NodeApplicationManager_Impl");

  DANCE_DEBUG((LM_DEBUG, DLINFO " NodeApplicationManager_Impl::NodeApplicationManager_Impl - "
               "Initializing for node '%s' and plan '%s' starting...\n",
               node_name.c_str(),
               plan_.UUID.in()));
  this->register_plan();

  PROPERTY_MAP::const_iterator i = properties.begin ();
  while (!i.done ())
    {
      DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplicationManager_Impl::NodeApplicationManager_Impl - "
                    "Binding value for property %s\n", i->key ().c_str ()));
      this->properties_.bind (i->key (), i->item ());
      i.advance ();
    }
}

NodeApplicationManager_Impl::~NodeApplicationManager_Impl()
{
  DANCE_TRACE (DLINFO "NodeApplicationManager_Impl::~NodeApplicationManager_Impl");

  try
    {
      if (this->application_ != 0)
        {
          CORBA::Object_var app = this->poa_->servant_to_reference (this->application_);
          PortableServer::ObjectId_var id = this->poa_->reference_to_id (app);
          this->poa_->deactivate_object (id);

          delete this->application_;
          this->application_ = 0;
        }
    }
  catch (...)
    {
      DANCE_ERROR ((LM_WARNING, DLINFO "NodeApplicationManager_Impl::~NodeApplicationManager_Impl - "
                    "Caught exception in NodeApplicationManager destructor\n"));
    }
}

Deployment::Application_ptr
NodeApplicationManager_Impl::startLaunch (const Deployment::Properties &,
                                          Deployment::Connections_out providedReference)
{
  DANCE_TRACE (DLINFO "NodeApplicationManager_Impl::startLaunch");

  // Creating NodeApplication object
  DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplicationManager_impl::startLaunch - "
               "Initializing NodeApplication\n"));
  ACE_NEW_THROW_EX (this->application_,
                    NodeApplication_Impl (this->orb_.in(),
                                          this->poa_.in(),
                                          this->plan_,
                                          this->redirection_,
                                          this->node_name_,
                                          this->properties_),
                    CORBA::NO_MEMORY ());

  DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplicationManager_impl::startLaunch - "
               "Instructing NodeApplication to initialize components.\n"));
  this->application_->init_components();

  DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplicationManager_impl::startLaunch - "
               "Collecting connection references\n"));
  providedReference = this->application_->getAllConnections();
  //this->parent_.registerConnections(this->plan_.UUID.in(), *providedReference);

  DANCE_DEBUG((LM_DEBUG, DLINFO "NodeApplicationManager_impl::startLaunch - "
               "Activating NodeApplication servant\n"));
  PortableServer::ObjectId_var as_id =
    this->poa_->activate_object (this->application_);

  CORBA::Object_var as_obj = this->poa_->id_to_reference (as_id.in ());
  Deployment::Application_var app = Deployment::Application::_narrow (as_obj.in ());

  return app._retn ();
}

void
NodeApplicationManager_Impl::destroyApplication (Deployment::Application_ptr application)
{
  DANCE_TRACE (DLINFO "NodeApplicationManager_Impl::destroyApplication");

  try
  {
    if (!application->_is_equivalent (this->poa_->servant_to_reference (this->application_)))
      {
        DANCE_ERROR((LM_ERROR, DLINFO "NodeApplicationManager_Impl::destroyApplication - "
                     "application is equivalent to current application\n"));
        throw ::Deployment::StopError();
      }
    this->redirection_.unregister (this->node_name_, this->plan_.UUID.in());

    CORBA::Any val;

    if (this->properties_.find (DAnCE::STANDALONE_NM,
                                val) == 0)
      {
        DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicationManager_Impl::destroyApplication - "
                      "Found STANDALONE_NM property\n"));

        CORBA::Boolean standalone (false);

        val >>= CORBA::Any::to_boolean (standalone);
        if (standalone)
          {
            DANCE_DEBUG ((LM_DEBUG, DLINFO "NodeApplicationManager_Impl::destroyApplication - "
                          "Acting as a standalone NM, passivating and removing installed components\n"));

            this->application_->passivate_components ();
            this->application_->remove_components ();
          }
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
    DANCE_ERROR((LM_ERROR, DLINFO "NodeApplicationManager_Impl::destroyApplication failed with UserException %s(%s) \"%s\"\n",
                 e._name(), e._rep_id(), e._info().c_str()));
    throw Deployment::StopError(e._name(), e._info().c_str());
  }
  catch (...)
  {
    DANCE_ERROR((LM_ERROR, DLINFO "NodeApplicationManager_Impl::destroyApplication failed with unknown exception.\n"));
    throw Deployment::StopError();
  }
}

void
NodeApplicationManager_Impl::register_plan(void)
{
  DANCE_TRACE(DLINFO "NodeApplicationManager_Impl::register_plan()");

  this->redirection_.registration_start (this->node_name_, this->plan_.UUID.in());
  DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplicationManager_Impl::register_plan - registering objects...\n"));
  for (unsigned int i = 0; i < this->plan_.instance.length(); i++)
    {
      this->redirection_.registration (this->node_name_,
                                       this->plan_.UUID.in(),
                                       this->plan_.instance[i].name.in(),
                                       CORBA::Object::_nil());
    }

  DANCE_DEBUG((LM_TRACE, DLINFO "NodeApplicationManager_Impl::register_plan - registering endpoints...\n"));
  for (unsigned int i = 0; i < this->plan_.connection.length(); i++)
    {
      for (unsigned int j = 0; j < this->plan_.connection[i].internalEndpoint.length(); j++)
        {
          if (this->plan_.connection[i].internalEndpoint[j].provider)
            {
              DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicationManager_Impl::register_plan - ",
                            "Registering Port '%s' on instance '%s' on node '%s'\n",
                            this->plan_.connection[i].internalEndpoint[j].portName.in(),
                            this->plan_.instance[this->plan_.connection[i].internalEndpoint[j].instanceRef].name.in(),
                            this->node_name_.c_str ()));
              this->redirection_.registration (this->node_name_,
                                               this->plan_.UUID.in(),
                                               this->plan_.instance[this->plan_.connection[i].internalEndpoint[j].instanceRef].name.in(),
                                               this->plan_.connection[i].internalEndpoint[j].portName.in(),
                                               CORBA::Object::_nil());
            }
        }
    }
  DANCE_DEBUG ((LM_TRACE, DLINFO "NodeApplicationManager_Impl::register_plan - "
                "Finishing registration\n"));
  this->redirection_.registration_finish (this->node_name_, this->plan_.UUID.in());
}
