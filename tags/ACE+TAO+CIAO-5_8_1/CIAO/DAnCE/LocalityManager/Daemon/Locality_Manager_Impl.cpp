// -*- C++ -*-
// $Id$

#include "Locality_Manager_Impl.h"
#include "Logger/Log_Macros.h"

#include "DAnCE/DAnCE_Utility.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "LocalityManager/Scheduler/Plugin_Manager.h"
#include "LocalityManager/Scheduler/Deployment_Completion.h"
#include "LocalityManager/Scheduler/Events/Install.h"
#include "LocalityManager/Scheduler/Events/Connect.h"
#include "LocalityManager/Scheduler/Events/Endpoint.h"
#include "LocalityManager/Scheduler/Events/Start.h"
#include "LocalityManager/Scheduler/Events/Configured.h"
#include "LocalityManager/Scheduler/Events/Remove.h"
#include "LocalityManager/Scheduler/Events/Passivate.h"

using DAnCE::Utility::extract_and_throw_exception;

namespace DAnCE
{
  // Implementation skeleton constructor
  LocalityManager_i::LocalityManager_i (const ACE_TString &uuid,
                                        CORBA::ORB_ptr orb,
                                        PortableServer::POA_ptr poa)
    : uuid_ (uuid),
      orb_ (CORBA::ORB::_duplicate  (orb)),
      poa_ (PortableServer::POA::_duplicate (poa)),
      spawn_delay_ (30)
  {
    DANCE_TRACE ("LocalityManager_i::LocalityManager_i");
    this->scheduler_.activate_scheduler (0);
  }

  // Implementation skeleton destructor
  LocalityManager_i::~LocalityManager_i (void)
  {
    DANCE_TRACE ("LocalityManager_i::~LocalityManager_i");

    this->scheduler_.terminate_scheduler ();
  }

  void
  LocalityManager_i::init (Deployment::Properties *props)
  {
    DANCE_TRACE ("LocalityManager_i::init");

    if (props)
      {
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT ("LocalityManager_i::init - ")
                     ACE_TEXT ("Received %u properties from init\n"),
                     props->length ()));
        this->props_ = props;

        DAnCE::Utility::update_property_value (ENTITY_POA,
                                               *this->props_,
                                               this->poa_);

        PLUGIN_MANAGER::instance ()->set_configuration (*props);
      }

    CORBA::String_var type =
      PLUGIN_MANAGER::instance ()->register_installation_handler (
        ACE_TEXT_CHAR_TO_TCHAR ("CIAO_Deployment_Handlers"),
        ACE_TEXT_CHAR_TO_TCHAR ("create_Container_Handler"));

    if (type.in ())
      {
        this->handler_order_.push_back (type.in ());
        this->instance_handlers_[type.in ()] = INSTANCE_LIST ();
      }

    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT ("LocalityManager_i::init - ")
                     ACE_TEXT ("Registered handler for <%C>\n"),
                     type.in ()));

    type =
      PLUGIN_MANAGER::instance ()->register_installation_handler (
        ACE_TEXT_CHAR_TO_TCHAR ("CIAO_Deployment_Handlers"),
        ACE_TEXT_CHAR_TO_TCHAR ("create_Home_Handler"));

    if (type.in ())
      {
        this->handler_order_.push_back (type.in ());
        this->instance_handlers_[type.in ()] = INSTANCE_LIST ();
      }

    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT ("LocalityManager_i::init - ")
                     ACE_TEXT ("Registered handler for <%C>\n"),
                     type.in ()));

    type =
      PLUGIN_MANAGER::instance ()->register_installation_handler (
        ACE_TEXT_CHAR_TO_TCHAR ("CIAO_Deployment_Handlers"),
        ACE_TEXT_CHAR_TO_TCHAR ("create_Homed_Component_Handler"));
    if (type.in ())
      {
        this->handler_order_.push_back (type.in ());
        this->instance_handlers_[type.in ()] = INSTANCE_LIST ();
      }

    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT ("LocalityManager_i::init - ")
                     ACE_TEXT ("Registered handler for <%C>\n"),
                     type.in ()));

    type =
      PLUGIN_MANAGER::instance ()->register_installation_handler (
        ACE_TEXT_CHAR_TO_TCHAR ("CIAO_Deployment_Handlers"),
        ACE_TEXT_CHAR_TO_TCHAR ("create_Component_Handler"));
    if (type.in ())
      {
        this->handler_order_.push_back (type.in ());
        this->instance_handlers_[type.in ()] = INSTANCE_LIST ();
      }

    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT ("LocalityManager_i::init - ")
                     ACE_TEXT ("Registered handler for <%C>\n"),
                     type.in ()));

    PLUGIN_MANAGER::instance ()->register_interceptor (
      ACE_TEXT_CHAR_TO_TCHAR ("CIAO_Deployment_Interceptors"),
      ACE_TEXT_CHAR_TO_TCHAR ("create_CIAO_StoreReferences"));
    
    PLUGIN_MANAGER::instance ()->register_interceptor (
         ACE_TEXT_CHAR_TO_TCHAR ("DAnCE_SHS_Interceptors"),
         ACE_TEXT_CHAR_TO_TCHAR ("create_DAnCE_SHS_Interceptor"));
    
    bool tmp;
    if (props &&
        DAnCE::Utility::get_property_value (DAnCE::LOCALITY_BESTEFFORT,
                                            *props,
                                            tmp))
      {
        PLUGIN_MANAGER::instance ()->register_interceptor (
          ACE_TEXT_CHAR_TO_TCHAR ("DAnCE_Error_Interceptors"),
          ACE_TEXT_CHAR_TO_TCHAR ("create_DAnCE_Best_Effort"));
      }
    else
      {
        PLUGIN_MANAGER::instance ()->register_interceptor (
          ACE_TEXT_CHAR_TO_TCHAR ("DAnCE_Error_Interceptors"),
          ACE_TEXT_CHAR_TO_TCHAR ("create_DAnCE_Standard_Error"));
      }
    
    PLUGIN_MANAGER::instance ()->register_configuration_plugin (
               ACE_TEXT_CHAR_TO_TCHAR ("DAnCE_LM_Config_Plugins"),
               ACE_TEXT_CHAR_TO_TCHAR ("create_Process_Name"));

    PLUGIN_MANAGER::instance ()->register_configuration_plugin (
               ACE_TEXT_CHAR_TO_TCHAR ("DAnCE_LM_Config_Plugins"),
               ACE_TEXT_CHAR_TO_TCHAR ("create_CPU_Affinity"));

    if (this->props_)
      {
        if (DAnCE::Utility::get_property_value (DAnCE::LOCALITY_TIMEOUT,
                                                *this->props_,
                                                this->spawn_delay_))
          {
            DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                             ACE_TEXT ("LocalityManager_i::configure - ")
                             ACE_TEXT ("Using provided spawn delay %u\n"),
                             this->spawn_delay_));
          }
        
        for (CORBA::ULong i = 0; i < this->props_->length (); ++i)
          {
            DANCE_DEBUG (8, (LM_DEBUG, DLINFO
                             ACE_TEXT ("LocalityManager_i::init - ")
                             ACE_TEXT ("Looking up configuration handler for <%C>\n"),
                             this->props_[i].name.in ()));

            ::DAnCE::LocalityConfiguration_var config = 
              PLUGIN_MANAGER::instance ()->get_configuration_handler (this->props_[i].name.in ());
            
            if (config.in ())
              {
                DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                                 ACE_TEXT ("LocalityManager_i::init - ")
                                 ACE_TEXT ("Invoking configuration handler for <%C>\n"),
                                 this->props_[i].name.in ()));
                config->configure (this->props_[i]);
              }
          }
      }
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

        instance_handlers_[instance_type].push_back (i);
      }

    return this->_this ();
  }

  ::Deployment::Application_ptr
  LocalityManager_i::startLaunch (const ::Deployment::Properties &prop,
                                  ::Deployment::Connections_out providedReference)
  {
    DANCE_TRACE ("LocalityManager_i::startLaunch");

    this->install_instances (prop);

    this->collect_references (providedReference);

    return this->_this ();
  }

  void
  LocalityManager_i::install_instances (const ::Deployment::Properties &)
  {
    DANCE_TRACE ("LocalityManager_i::install_instances");

    CORBA::ULong dispatched (0);

    Deployment_Completion completion (this->scheduler_);

    for (HANDLER_ORDER::const_iterator i = this->handler_order_.begin ();
         i != this->handler_order_.end ();
         ++i)
      {
        DANCE_DEBUG (8, (LM_TRACE, DLINFO
                         ACE_TEXT ("LocalityManager_i::startLaunch - ")
                         ACE_TEXT ("Starting installation of %C type instances\n"),
                         i->c_str ()));

        INSTANCE_LIST &inst_list = this->instance_handlers_[*i];

        for (INSTANCE_LIST::const_iterator j = inst_list.begin ();
             j != inst_list.end ();
             ++j)
          {
            DANCE_DEBUG (7, (LM_TRACE, DLINFO
                             ACE_TEXT ("LocalityManager_i::startLaunch - ")
                             ACE_TEXT ("Starting installation of instance %C\n"),
                             this->plan_.instance[*j].name.in ()));

            Install_Instance *event (0);
            Event_Future result;
            completion.accept (result);

            ACE_NEW_THROW_EX (event,
                              Install_Instance (this->plan_,
                                                *j,
                                                i->c_str (),
                                                result),
                              CORBA::NO_MEMORY ());

            this->scheduler_.schedule_event (event);
            ++dispatched;
          }
      }

    ACE_Time_Value tv (ACE_OS::gettimeofday () + ACE_Time_Value (this->spawn_delay_));

    if (!completion.wait_on_completion (&tv))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("LocalityManager_i::startLaunch - ")
                         ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    Event_List completed_events;
    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (2, (LM_WARNING, DLINFO
                         ACE_TEXT ("LocalityManager_i::startLaunch - ")
                         ACE_TEXT ("Received only %u completed events, expected %u\n"),
                         dispatched,
                         completed_events.size ()));
      }

    for (Event_List::iterator i = completed_events.begin ();
         i != completed_events.end ();
         ++i)
      {
        Event_Result event;
        if (i->get (event, &tv) != 0)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::startLaunch - ")
                             ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(extract_and_throw_exception < Deployment::StartError >
              (event.contents_.in ()) ||
              extract_and_throw_exception < Deployment::InvalidProperty >
              (event.contents_.in ()) ||
              extract_and_throw_exception < Deployment::InvalidNodeExecParameter >
              (event.contents_.in ()) ||
              extract_and_throw_exception < Deployment::InvalidComponentExecParameter >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::startLaunch - ")
                             ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StartError (event.id_.c_str (),
                                            "Unknown exception");
          }

        DANCE_DEBUG (5, (LM_INFO, DLINFO
                         ACE_TEXT ("LocalityManager_i::startLaunch - ")
                         ACE_TEXT ("Instance <%C> successfully deployed\n"),
                         event.id_.c_str ()));

        this->instance_references_[event.id_] = event.contents_;
      }
  }

  void
  LocalityManager_i::collect_references (::Deployment::Connections_out &providedReference)
  {
    DANCE_TRACE ("LocalityManager_i::collect_references");

    CORBA::ULong dispatched (0);
    Deployment_Completion completion (this->scheduler_);

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


                Endpoint_Reference *event (0);
                Event_Future result;
                completion.accept (result);

                ACE_NEW_THROW_EX (event,
                                  Endpoint_Reference (this->plan_,
                                                      i,
                                                      inst_type,
                                                      result),
                                  CORBA::NO_MEMORY ());

                this->scheduler_.schedule_event (event);
                ++dispatched;
              }
          }
      }

    ACE_Time_Value tv (ACE_OS::gettimeofday () + ACE_Time_Value (this->spawn_delay_));

    if (!completion.wait_on_completion (&tv))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("LocalityManager_i::startLaunch - ")
                         ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    Event_List completed_events;
    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (2, (LM_WARNING, DLINFO
                         ACE_TEXT ("LocalityManager_i::startLaunch - ")
                         ACE_TEXT ("Received only %u completed events, expected %u\n"),
                         dispatched,
                         completed_events.size ()));
      }

    ::Deployment::Connections *conn_cmp = 0;
    ACE_NEW_THROW_EX (conn_cmp,
                      ::Deployment::Connections (this->plan_.connection.length ()),
                      CORBA::NO_MEMORY ());

    CORBA::ULong pos (0);
    for (Event_List::iterator i = completed_events.begin ();
         i != completed_events.end ();
         ++i)
      {
        Event_Result event;
        if (i->get (event, &tv) != 0)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::startLaunch - ")
                             ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(extract_and_throw_exception < Deployment::StartError >
              (event.contents_.in ()) ||
              extract_and_throw_exception < Deployment::InvalidProperty >
              (event.contents_.in ())))
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::startLaunch - ")
                             ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StartError (event.id_.c_str (),
                                            "Unknown exception");
          }

        CORBA::Object_var obj_ref;

        if (event.contents_.ptr () != 0)
          event.contents_ >>= CORBA::Any::to_object (obj_ref);
        else
          {
            DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                             ACE_TEXT ("LocalityManager_i::startLaunch - ")
                             ACE_TEXT ("No reference returned for connection <%C>\n"),
                             event.id_.c_str ()));
          }

        conn_cmp->length (pos + 1);
        (*conn_cmp)[pos].name = event.id_.c_str ();
        (*conn_cmp)[pos].endpoint.length (1);
        (*conn_cmp)[pos].endpoint[0] = obj_ref;
        ++pos;
      }

    providedReference = conn_cmp;
  }

  void
  LocalityManager_i::finishLaunch (const ::Deployment::Connections & providedReference,
                                   ::CORBA::Boolean start)
  {
    DANCE_TRACE ("LocalityManager_i::finishLaunch");
    // Add your implementation here

    typedef std::map < std::string, CORBA::ULong > ConnMap;
    ConnMap conns;

    Deployment_Completion completion (this->scheduler_);

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

    CORBA::ULong dispatched (0);

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

        Connect_Instance *event (0);
        Event_Future result;
        completion.accept (result);

        ACE_NEW_THROW_EX (event,
                          Connect_Instance (this->plan_,
                                            conn_ref->second,
                                            reference,
                                            inst_type,
                                            result),
                          CORBA::NO_MEMORY ());

        this->scheduler_.schedule_event (event);
        ++dispatched;
      }

    ACE_Time_Value tv (ACE_OS::gettimeofday () + ACE_Time_Value (this->spawn_delay_));

    if (!completion.wait_on_completion (&tv))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                         ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    Event_List completed_events;
    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (2, (LM_WARNING, DLINFO
                         ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                         ACE_TEXT ("Received only %u completed events, expected %u\n"),
                         dispatched,
                         completed_events.size ()));
      }

    for (Event_List::iterator i = completed_events.begin ();
         i != completed_events.end ();
         ++i)
      {
        Event_Result event;
        if (i->get (event, &tv) != 0)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                             ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(extract_and_throw_exception < Deployment::StartError >
              (event.contents_.in ()) ||
              extract_and_throw_exception < Deployment::InvalidConnection >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                             ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StartError (event.id_.c_str (),
                                            "Unknown exception");
          }
      }

    dispatched = 0;

    for (HANDLER_ORDER::const_iterator i = this->handler_order_.begin ();
         i != this->handler_order_.end ();
         ++i)
      {
        INSTANCE_LIST &inst_list = this->instance_handlers_[*i];

        for (INSTANCE_LIST::const_iterator j = inst_list.begin ();
             j != inst_list.end ();
             ++j)
          {
            Instance_Configured *event (0);
            Event_Future result;
            completion.accept (result);

            ACE_NEW_THROW_EX (event,
                              Instance_Configured (this->plan_,
                                                   *j,
                                                   i->c_str (),
                                                   result),
                              CORBA::NO_MEMORY ());

            this->scheduler_.schedule_event (event);
            ++dispatched;
          }
      }

    tv = ACE_OS::gettimeofday () + ACE_Time_Value (this->spawn_delay_);

    if (!completion.wait_on_completion (&tv))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                         ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (2, (LM_WARNING, DLINFO
                         ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                         ACE_TEXT ("Received only %u completed events, expected %u\n"),
                         dispatched,
                         completed_events.size ()));
      }

    for (Event_List::iterator i = completed_events.begin ();
         i != completed_events.end ();
         ++i)
      {
        Event_Result event;
        if (i->get (event, &tv) != 0)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                             ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(extract_and_throw_exception < Deployment::StartError >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                             ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StartError (event.id_.c_str (),
                                            "Unknown exception from instance_configured");
          }
      }

    if (start)
      this->start ();
  }

  void
  LocalityManager_i::start (void)
  {
    DANCE_TRACE ("LocalityManager_i::start");

    Deployment_Completion completion (this->scheduler_);
    CORBA::ULong dispatched (0);

    for (HANDLER_ORDER::const_iterator i = this->handler_order_.begin ();
         i != this->handler_order_.end ();
         ++i)
      {
        INSTANCE_LIST &inst_list = this->instance_handlers_[*i];

        for (INSTANCE_LIST::const_iterator j = inst_list.begin ();
             j != inst_list.end ();
             ++j)
          {
            Start_Instance *event (0);
            Event_Future result;
            completion.accept (result);

            const char *name = this->plan_.instance[*j].name.in ();

            ACE_NEW_THROW_EX (event,
                              Start_Instance (this->plan_,
                                              *j,
                                              this->instance_references_[name].in (),
                                              i->c_str (),
                                              result),
                              CORBA::NO_MEMORY ());

            this->scheduler_.schedule_event (event);
            ++dispatched;
          }
      }

    ACE_Time_Value tv (ACE_OS::gettimeofday () + ACE_Time_Value (this->spawn_delay_));

    if (!completion.wait_on_completion (&tv))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("LocalityManager_i::start - ")
                         ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    Event_List completed_events;
    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (2, (LM_WARNING, DLINFO
                         ACE_TEXT ("LocalityManager_i::start - ")
                         ACE_TEXT ("Received only %u completed events, expected %u\n"),
                         dispatched,
                         completed_events.size ()));
      }

    for (Event_List::iterator i = completed_events.begin ();
         i != completed_events.end ();
         ++i)
      {
        Event_Result event;
        if (i->get (event, &tv) != 0)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::start - ")
                             ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(extract_and_throw_exception < Deployment::StartError >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::start - ")
                             ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StartError (event.id_.c_str (),
                                            "Unknown exception");
          }

        DANCE_DEBUG (5, (LM_INFO, DLINFO
                         ACE_TEXT ("LocalityManager_i::start - ")
                         ACE_TEXT ("Instance <%C> successfully activated\n"),
                         event.id_.c_str ()));
      }
  }

  void
  LocalityManager_i::destroyApplication (::Deployment::Application_ptr)
  {
    DANCE_TRACE ("LocalityManager_i::destroyApplication");

    Deployment_Completion completion (this->scheduler_);
    CORBA::ULong dispatched (0);

    for (size_t i = this->handler_order_.size ();
         i > 0;
         --i)
      {
        INSTANCE_LIST &inst_list = this->instance_handlers_[this->handler_order_[i-1]];

        for (INSTANCE_LIST::const_iterator j = inst_list.begin ();
             j != inst_list.end ();
             ++j)
          {
            Passivate_Instance *event (0);
            Event_Future result;
            completion.accept (result);

            const char *name = this->plan_.instance[*j].name.in ();

            ACE_NEW_THROW_EX (event,
                              Passivate_Instance (this->plan_,
                                                  *j,
                                                  this->instance_references_[name].in (),
                                                  this->handler_order_[i-1].c_str (),
                                                  result),
                              CORBA::NO_MEMORY ());

            this->scheduler_.schedule_event (event);
            ++dispatched;
          }
      }

    ACE_Time_Value tv (ACE_OS::gettimeofday () + ACE_Time_Value (this->spawn_delay_));

    if (!completion.wait_on_completion (&tv))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                         ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    Event_List completed_events;
    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (2, (LM_WARNING, DLINFO
                         ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                         ACE_TEXT ("Received only %u completed events, expected %u\n"),
                         dispatched,
                         completed_events.size ()));
      }

    dispatched = 0;

    for (Event_List::iterator i = completed_events.begin ();
         i != completed_events.end ();
         ++i)
      {
        Event_Result event;
        if (i->get (event, &tv) != 0)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                             ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(extract_and_throw_exception < Deployment::StopError >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                             ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StopError (event.id_.c_str (),
                                           "Unknown exception");
          }

        DANCE_DEBUG (5, (LM_INFO, DLINFO
                         ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                         ACE_TEXT ("Instance <%C> successfully passivated\n"),
                         event.id_.c_str ()));
      }

    for (size_t i = this->handler_order_.size ();
         i > 0;
         --i)
      {
        INSTANCE_LIST &inst_list = this->instance_handlers_[this->handler_order_[i-1]];

        for (INSTANCE_LIST::const_iterator j = inst_list.begin ();
             j != inst_list.end ();
             ++j)
          {
            Remove_Instance *event (0);
            Event_Future result;
            completion.accept (result);

            const char *name = this->plan_.instance[*j].name.in ();

            REFERENCE_MAP::iterator ref = this->instance_references_.find (name);

            ACE_NEW_THROW_EX (event,
                              Remove_Instance (this->plan_,
                                               *j,
                                               ref->second.in (),
                                               this->handler_order_[i-1].c_str (),
                                               result),
                              CORBA::NO_MEMORY ());

            this->instance_references_.erase (ref);

            this->scheduler_.schedule_event (event);
            ++dispatched;
          }
      }

    tv = ACE_OS::gettimeofday () + ACE_Time_Value (this->spawn_delay_);

    if (!completion.wait_on_completion (&tv))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                         ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (2, (LM_WARNING, DLINFO
                         ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                         ACE_TEXT ("Received only %u completed events, expected %u\n"),
                         dispatched,
                         completed_events.size ()));
      }

    dispatched = 0;

    for (Event_List::iterator i = completed_events.begin ();
         i != completed_events.end ();
         ++i)
      {
        Event_Result event;
        if (i->get (event, &tv) != 0)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                             ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(extract_and_throw_exception < Deployment::StopError >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                             ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StopError (event.id_.c_str (),
                                           "Unknown exception");
          }

        DANCE_DEBUG (5, (LM_INFO, DLINFO
                         ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                         ACE_TEXT ("Instance <%C> successfully removed\n"),
                         event.id_.c_str ()));
      }
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
    
    // Explicitly close the plugin manager to release memory.
    PLUGIN_MANAGER::close ();
  }
}
