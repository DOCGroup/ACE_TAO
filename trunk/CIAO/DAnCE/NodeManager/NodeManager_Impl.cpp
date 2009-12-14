// $Id$

#include "NodeManager_Impl.h"


#include "ace/streams.h"

#include "DAnCE/Logger/Log_Macros.h"

#include "ace/OS_Memory.h"

#ifdef GEN_OSTREAM_OPS
#include <iostream>
#include <sstream>
#endif /* GEN_OSTREAM_OPS */

namespace DAnCE
{
  NodeManager_Impl::NodeManager_Impl(CORBA::ORB_ptr orb,
                                     PortableServer::POA_ptr poa,
                                     const char* name,
                                     const PROPERTY_MAP &properties)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      poa_ (PortableServer::POA::_duplicate (poa)),
      name_ (name),
      properties_ (properties.current_size ())
  {
    DANCE_TRACE ( "NodeManager_Impl::NodeManager_Impl");
    DANCE_DEBUG ((LM_INFO, DLINFO ACE_TEXT("NodeManager_impl::NodeManager_impl has been created\n")));

    PROPERTY_MAP::const_iterator i = properties.begin ();
    while (!i.done ())
      {
        DANCE_DEBUG ((LM_TRACE, DLINFO ACE_TEXT("NodeManager_Impl::NodeManager_Impl - ")
                      ACE_TEXT("Binding property %C provided by caller.\n"), i->key ().c_str ()));
        this->properties_.bind (i->key (), i->item ());
        i.advance ();
      }
  }

  NodeManager_Impl::~NodeManager_Impl()
  {
    DANCE_TRACE ( "NodeManager_Impl::~NodeManager_Impl()");
    for (TManagers::iterator iter = this->managers_.begin();
         iter != this->managers_.end();
         ++iter)
      {
        try
          {
            PortableServer::ObjectId_var id = this->poa_->servant_to_id ( (*iter).int_id_);
            DANCE_DEBUG ((LM_TRACE, DLINFO
                          ACE_TEXT("NodeManager_impl::~NodeManager_impl - Deactivating NodeApplicationManager %C\n"),
                          (*iter).ext_id_.c_str ()));
            this->poa_->deactivate_object (id.in());
            DANCE_DEBUG ((LM_TRACE, DLINFO
                          ACE_TEXT("NodeManager_impl::~NodeManager_impl - deleting NodeApplicationManager\n")));
            delete (*iter).int_id_;
            DANCE_DEBUG ((LM_DEBUG, DLINFO
                          ACE_TEXT("NodeManager_impl::~NodeManager_impl - NodeApplicationManager deleted\n")));
          }
        catch (...)
          {
            DANCE_ERROR ((LM_WARNING, DLINFO
                          ACE_TEXT("NodeManager_impl::~NodeManager_impl - Caught exception while removing ")
                          ACE_TEXT("NodeApplicationManager %C\n"), (*iter).ext_id_.c_str ()));
          }
      }
  }

  void
  NodeManager_Impl::joinDomain (const ::Deployment::Domain & /*theDomain*/,
                                ::Deployment::TargetManager_ptr /*manager*/,
                                ::Deployment::Logger_ptr /*log*/,
                                ::CORBA::Long /*updateInterval*/)
  {
    DANCE_TRACE ( "NodeManager_Impl::joinDomain");
    DANCE_DEBUG ((LM_ERROR, DLINFO ACE_TEXT("NodeManager_Impl::joinDomain - ")
                ACE_TEXT("joinDomain not implemented\n")));
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  NodeManager_Impl::leaveDomain ()
  {
    DANCE_TRACE ( "NodeManager_Impl::leaveDomain");
    DANCE_DEBUG ((LM_ERROR, DLINFO ACE_TEXT("NodeManager_Impl::leaveDomain - ")
                ACE_TEXT("leave not implemented\n")));
    throw CORBA::NO_IMPLEMENT ();
  }

  ::Deployment::NodeApplicationManager_ptr
  NodeManager_Impl::preparePlan (const ::Deployment::DeploymentPlan & plan,
                                 ::Deployment::ResourceCommitmentManager_ptr /*resourceCommitment*/)
  {
    DANCE_TRACE ( "NodeManager_Impl::preparePlan");

    DANCE_DEBUG ((LM_DEBUG, DLINFO ACE_TEXT("NodeManager_impl::preparePlan - ")
                  ACE_TEXT("started for node %C and plan %C\n"),
                  this->name_.c_str(), plan.UUID.in()));

#ifdef GEN_OSTREAM_OPS
    {
      std::ostringstream plan_stream;
      plan_stream << plan << std::endl;
      DANCE_DEBUG ((LM_TRACE, DLINFO "NodeManager_impl::preparePlan - %C",
                    plan_stream.str ().c_str ()));
    }
#endif /* GEN_OSTREAM_OPS */

    // resourceCommitment will be used on next development stage
    if (0 == this->managers_.find (plan.UUID.in()))
      {
        // What should we do here if we already have application for this plan?
        // Probably it is mistake because we should previously call destroyApplication
        // before performe relaunching of application
        DANCE_ERROR ((LM_ERROR, DLINFO ACE_TEXT("NodeManager_impl::preparePlan - ")
                      ACE_TEXT("ApplicationManager for UUID %C already exists\n"),
                      plan.UUID.in ()));
        throw ::Deployment::PlanError();
      }
    DANCE_DEBUG ((LM_TRACE, DLINFO ACE_TEXT("NodeManager_impl::preparePlan - creating NodeApplicationManager...\n")));
    NodeApplicationManager_Impl* manager = 0;
    ACE_NEW_THROW_EX (manager,
                      NodeApplicationManager_Impl (this->orb_.in(),
                                                   this->poa_.in(),
                                                   plan,
                                                   this->name_,
                                                   this->properties_),
                      CORBA::NO_MEMORY());
    DANCE_DEBUG ((LM_TRACE, DLINFO ACE_TEXT("NodeManager_impl::preparePlan - activating NodeApplicationManager...\n")));
    PortableServer::ObjectId_var id = this->poa_->activate_object (manager);
    CORBA::Object_var nam = this->poa_->id_to_reference (id.in());

    // There is an idea to check if plan.UUID really exists
    this->managers_.bind (plan.UUID.in(), manager);
    // narrow should return a nil reference if it fails.
    DANCE_DEBUG ((LM_INFO, DLINFO ACE_TEXT("NodeManager_impl::preparePlan - NodeApplicationManager for plan %C completed\n"),
                  plan.UUID.in ()));
    return Deployment::NodeApplicationManager::_narrow (nam.in ());
  }

  void
  NodeManager_Impl::destroyManager (::Deployment::NodeApplicationManager_ptr appManager)
  {
    DANCE_TRACE ( "NodeManager_Impl::destroyManager");

    for (TManagers::iterator iter = this->managers_.begin();
         iter != this->managers_.end();
         ++iter)
      {
        if (appManager->_is_equivalent (this->poa_->servant_to_reference ( (*iter).int_id_)))
          {
            PortableServer::ObjectId_var id = this->poa_->reference_to_id (appManager);
            this->poa_->deactivate_object (id.in());
            DANCE_DEBUG ((LM_TRACE, DLINFO ACE_TEXT("NodeManager_impl::destroyManager - deleting NodeApplicationManager\n")));
            delete (*iter).int_id_;
            DANCE_DEBUG ((LM_TRACE, DLINFO ACE_TEXT("NodeManager_impl::destroyManager - NodeApplicationManager deleted\n")));
            this->managers_.unbind ( (*iter).ext_id_);
            DANCE_DEBUG ((LM_TRACE, DLINFO ACE_TEXT("NodeManager_impl::destroyManager - finished\n")));
            return;
          }
      }

    DANCE_ERROR((LM_ERROR, DLINFO ACE_TEXT("NodeManager_impl::destroyManager - ")
                 ACE_TEXT("correponding NodeApplicationManager cannot be found\n")));
    throw ::Deployment::InvalidReference();
  }

  ::Deployment::Resources *
  NodeManager_Impl::getDynamicResources ()
  {
    DANCE_TRACE ( "NodeManager_Impl::getDynamicResources ()");
    DANCE_DEBUG ((LM_ERROR, DLINFO ACE_TEXT("NodeManager_Impl::getDynamicResources - ")
                ACE_TEXT("getDynamicResources not implemented\n")));
      throw CORBA::NO_IMPLEMENT ();
    return 0;
  }
} //DAnCE

