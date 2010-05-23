// -*- C++ -*-
// $Id$

// TAO_IDL - Generated from 
// be/be_codegen.cpp:1579

#include "Locality_Manager_Impl.h"
#include "Logger/Log_Macros.h"


/*
 * CIAO Instance handlers
 * @TODO:  Needs to be replaced by dynamic provisioning system.
 */
#include "ciao/Deployment/Handlers/Container_Handler.h"
#include "ciao/Deployment/Handlers/Component_Handler.h"
#include "ciao/Deployment/Handlers/Home_Handler.h"
#include "ciao/Deployment/Handlers/Homed_Component_Handler.h"
#include "Deployment_Interceptors.h"
//

namespace DAnCE
{
  // Implementation skeleton constructor
  LocalityManager_i::LocalityManager_i (const ACE_TString &uuid,
                                        CORBA::ORB_ptr orb,
                                        PortableServer::POA_ptr poa)
    : uuid_ (uuid),
      orb_ (CORBA::ORB::_duplicate  (orb)),
      poa_ (PortableServer::POA::_duplicate (poa))
  {
    DANCE_TRACE ("LocalityManager_i::LocalityManager_i");
  }

  // Implementation skeleton destructor
  LocalityManager_i::~LocalityManager_i (void)
  {
    DANCE_TRACE ("LocalityManager_i::~LocalityManager_i");
  }
  
  void
  LocalityManager_i::init (Deployment::Properties *props)
  {
    DANCE_TRACE ("LocalityManager_i::init");
    
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
		     "LocalityManager_i::init - "
		     "Received %u properties from init\n",
		     props->length ()));

    this->props_ = props;
    
    // Initialize the CIAO instance handlers
    ::DAnCE::InstanceDeploymentHandler_ptr tmp;
    
    DAnCE::Utility::PROPERTY_MAP tmp_props;
    
    ACE_NEW_THROW_EX (tmp,
                      CIAO::Container_Handler_i (tmp_props,
                                                 this->orb_,
                                                 this->poa_),
                      CORBA::NO_MEMORY ());

    this->instance_handlers_[tmp->instance_type ()] = Handler (tmp);
    this->handler_order_.push_back (tmp->instance_type ());
    
    ACE_NEW_THROW_EX (tmp,
                      CIAO::Home_Handler_i,
                      CORBA::NO_MEMORY ());

    this->instance_handlers_[tmp->instance_type ()] = Handler (tmp);
    this->handler_order_.push_back (tmp->instance_type ());

    ACE_NEW_THROW_EX (tmp,
                      CIAO::Component_Handler_i (),
                      CORBA::NO_MEMORY ());

    this->instance_handlers_[tmp->instance_type ()] = Handler (tmp);                                     
    this->handler_order_.push_back (tmp->instance_type ());
    
    ACE_NEW_THROW_EX (tmp,
                      CIAO::Homed_Component_Handler_i (),
                      CORBA::NO_MEMORY ());
    
    this->instance_handlers_[tmp->instance_type ()] = Handler (tmp);
    this->handler_order_.push_back (tmp->instance_type ());
    
    ACE_NEW_THROW_EX (this->ii_interceptor_,
                      DAnCE_StoreReferences_i (this->orb_.in (),
                                               props),
                      CORBA::NO_MEMORY ());
  }

  ::Deployment::Properties *
  LocalityManager_i::configuration (void)
  {
    DANCE_TRACE ("LocalityManager_i::configuration");
    throw CORBA::NO_IMPLEMENT ();
  }

  ::Deployment::ApplicationManager_ptr
  LocalityManager_i::preparePlan (const ::Deployment::DeploymentPlan & plan,
                                  ::Deployment::ResourceCommitmentManager_ptr)
  {
    DANCE_TRACE ("LocalityManager_i::preparePlan");
    
    this->plan_ = plan;
    
    // populate the impl_type_table.
    for (CORBA::ULong i = 0;
         i < plan_.instance.length ();
         ++i)
      {
        CORBA::ULong implRef = plan.instance[i].implementationRef;

        const char  *instance_type = 
          Utility::get_instance_type (plan.implementation[implRef].execParameter);
        
        instance_handlers_[instance_type].instances_.push_back (i);
      }
    
    return this->_this ();
    // Add your implementation here
  }

  ::Deployment::Application_ptr
  LocalityManager_i::startLaunch (const ::Deployment::Properties &,
                                  ::Deployment::Connections_out providedReference)
  {
    DANCE_TRACE ("LocalityManager_i::startLaunch");
    
    
    for (HANDLER_ORDER::const_iterator i = this->handler_order_.begin ();
         i != this->handler_order_.end ();
         ++i)
      {
        DANCE_DEBUG (8, (LM_TRACE, DLINFO
                         ACE_TEXT ("LocalityManager_i::startLaunch - ")
                         ACE_TEXT ("Starting installation of %C type instances\n"),
                         i->c_str ()));

        ::DAnCE::InstanceDeploymentHandler_ptr handler = 
          this->instance_handlers_[*i].handler_;
        INSTANCE_LIST &inst_list = this->instance_handlers_[*i].instances_;

        for (INSTANCE_LIST::const_iterator j = inst_list.begin ();
             j != inst_list.end ();
             ++j)
          {
            DANCE_DEBUG (7, (LM_TRACE, DLINFO
                             ACE_TEXT ("LocalityManager_i::startLaunch - ")
                             ACE_TEXT ("Starting installation of instance %C\n"),
                             this->plan_.instance[*j].name.in ()));
                             
                             
            this->ii_interceptor_->instance_pre_install (this->plan_,
                                                         *j);

            CORBA::Any_var reference;
            handler->install_instance (this->plan_,
                                       *j,
                                       reference.out ());
            
            CORBA::Any exception;

            this->ii_interceptor_->instance_post_install (this->plan_,
                                                          *j,
                                                          reference.in (),
                                                          exception);
            this->instance_references_[*j] = reference._retn ();
            

            // @@ todo: post-interceptor
          }
      }

    ::Deployment::Connections *conn_cmp;
    
    ACE_NEW_THROW_EX (conn_cmp,
                      ::Deployment::Connections (this->plan_.connection.length ()),
                      CORBA::NO_MEMORY ());
    
    CORBA::ULong pos (0);

    for (CORBA::ULong i = 0;
         i < this->plan_.connection.length ();
         ++i)
      {
        const ::Deployment::PlanConnectionDescription &conn = 
          this->plan_.connection[i];
        for (CORBA::ULong j = 0;
             j != conn.internalEndpoint.length ();
             ++j)
          {
            if (conn.internalEndpoint[j].provider)
              {
                CORBA::ULong instRef =
                  conn.internalEndpoint[j].instanceRef;
                CORBA::ULong implRef = 
                  this->plan_.instance[instRef].implementationRef;
                
                const char  *inst_type = 
                  Utility::get_instance_type (this->plan_.implementation[implRef].execParameter);
                
                CORBA::Any_var ref;

                this->instance_handlers_[inst_type].handler_->
                  provide_endpoint_reference (this->plan_,
                                              i,
                                              ref.out ());
                
                CORBA::Object_var obj_ref;

                ref >>= CORBA::Any::to_object (obj_ref);
                conn_cmp->length (pos + 1);
                (*conn_cmp)[pos].name = conn.name.in ();
                (*conn_cmp)[pos].endpoint.length (1);
                (*conn_cmp)[pos].endpoint[0] = obj_ref;                
                ++pos;
              }
          }
      }
         
    
    providedReference = conn_cmp;

    return this->_this ();
  }

  void
  LocalityManager_i::finishLaunch (const ::Deployment::Connections & providedReference,
                                   ::CORBA::Boolean start)
  {
    DANCE_TRACE ("LocalityManager_i::finishLaunch");
    // Add your implementation here
    
    typedef std::map < std::string, CORBA::ULong > ConnMap;
    ConnMap conns;
    
    DANCE_DEBUG (6, (LM_TRACE, DLINFO 
                     ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                     ACE_TEXT ("Starting finsihLaunch, received %u references, ")
                     ACE_TEXT ("have %u connections\n"),
                     providedReference.length (),
                     this->plan_.connection.length ()
                     ));

    for (CORBA::ULong i = 0; i < this->plan_.connection.length (); ++i)
      {
        conns[this->plan_.connection[i].name.in ()] = i;
      }
    
    for (CORBA::ULong i = 0; i < providedReference.length (); ++i)
      {
        const char * name = providedReference[i].name.in ();
        ConnMap::const_iterator conn_ref = conns.find (name);
        
        if (conn_ref == conns.end ())
          continue;
        
        CORBA::ULong j (0);
        
        const ::Deployment::PlanConnectionDescription &conn = 
          this->plan_.connection[conn_ref->second];
        
        DANCE_DEBUG (9, (LM_TRACE, DLINFO
                         ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                         ACE_TEXT ("Connection <%C> has %u endpoints\n"),
                         conn.name.in (),
                         conn.internalEndpoint.length ()));

        if (conn.internalEndpoint.length () == 2)
          {
            if (!conn.internalEndpoint[1].provider)
              j = 1;
          }
        else if (conn.internalEndpoint[0].provider &&
                 conn.externalReference.length () == 0)
          {
            DANCE_DEBUG (9, (LM_TRACE, DLINFO
                             ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                             ACE_TEXT ("Skipping connection <%C>\n"),
                             conn.name.in ()));
            continue;
          }
          
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO 
                         ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                         ACE_TEXT ("Starting connection <%C>\n"),
                         name));
        
        CORBA::Any reference;
        
        reference <<= providedReference[i].endpoint[0];

        CORBA::ULong instRef =
          conn.internalEndpoint[j].instanceRef;
        CORBA::ULong implRef = 
          this->plan_.instance[instRef].implementationRef;
        
        const char  *inst_type = 
          Utility::get_instance_type (this->plan_.implementation[implRef].execParameter);
        
        // @@ placeholder for pre_connect interceptor
        this->instance_handlers_[inst_type].handler_->
          connect_instance (this->plan_,
                            conn_ref->second,
                            reference);
        // @@ placeholder for post_connect interceptor
      }

    
    for (HANDLER_ORDER::const_iterator i = this->handler_order_.begin ();
         i != this->handler_order_.end ();
         ++i)
      {
        ::DAnCE::InstanceDeploymentHandler_ptr handler = 
          this->instance_handlers_[*i].handler_;
        INSTANCE_LIST &inst_list = this->instance_handlers_[*i].instances_;
        
        for (INSTANCE_LIST::const_iterator i = inst_list.begin ();
             i != inst_list.end ();
             ++i)
          {
            CORBA::Any_var reference;
            handler->instance_configured (this->plan_,
                                          *i);
          }
      }

    if (start)
      this->start ();
  }

  void
  LocalityManager_i::start (void)
  {
    DANCE_TRACE ("LocalityManager_i::start");
    
    for (HANDLER_ORDER::const_iterator i = this->handler_order_.begin ();
         i != this->handler_order_.end ();
         ++i)
      {
        ::DAnCE::InstanceDeploymentHandler_ptr handler = 
          this->instance_handlers_[*i].handler_;
        INSTANCE_LIST &inst_list = this->instance_handlers_[*i].instances_;
        
        for (INSTANCE_LIST::const_iterator i = inst_list.begin ();
             i != inst_list.end ();
             ++i)
          {
            CORBA::Any_var reference;
            handler->activate_instance (this->plan_,
                                        *i,
                                        reference.in ());
          }
      }
  }

  void 
  LocalityManager_i::destroyApplication (::Deployment::Application_ptr)
  {
    DANCE_TRACE ("LocalityManager_i::destroyApplication");
    
    for (HANDLER_ORDER::const_reverse_iterator i = this->handler_order_.rbegin ();
         i != this->handler_order_.rend ();
         ++i)
      {
        ::DAnCE::InstanceDeploymentHandler_ptr handler = 
          this->instance_handlers_[*i].handler_;
        INSTANCE_LIST &inst_list = this->instance_handlers_[*i].instances_;
        
        for (INSTANCE_LIST::const_iterator i = inst_list.begin ();
             i != inst_list.end ();
             ++i)
          {
            CORBA::Any_var reference;
            handler->passivate_instance (this->plan_,
                                         *i,
                                         reference.in ());
          }
      }
    
    for (HANDLER_ORDER::const_reverse_iterator i = this->handler_order_.rbegin ();
           i != this->handler_order_.rend ();
         ++i)
      {
        ::DAnCE::InstanceDeploymentHandler_ptr handler = 
          this->instance_handlers_[*i].handler_;
        INSTANCE_LIST &inst_list = this->instance_handlers_[*i].instances_;
        
        for (INSTANCE_LIST::const_iterator i = inst_list.begin ();
             i != inst_list.end ();
             ++i)
          {
            CORBA::Any_var reference;
            handler->remove_instance (this->plan_,
                                         *i,
                                         reference.in ());
          }
      }
    // Add your implementation here
  }


  void 
  LocalityManager_i::destroyManager (::Deployment::ApplicationManager_ptr)
  {
    DANCE_TRACE ("LocalityManager_i::destroyManager");
    // Add your implementation here
  }
  
  
  void 
  LocalityManager_i::shutdown (void)
  {
    DANCE_TRACE ("LocalityManager_i::shutdown");
    
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT ("DAnCE LocalityManager shutdown request received for UUID <%C>\n"),
                     uuid_.c_str ()));
    
    this->orb_->shutdown ();
    // Add your implementation here
  }
}
