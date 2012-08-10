// -*- C++ -*-
// $Id$

#include "Locality_Manager_Impl.h"
#include "dance/Logger/Log_Macros.h"

#include "dance/DAnCE_Utility.h"
#include "dance/DAnCE_PropertiesC.h"
#include "dance/LocalityManager/Handler/Plugin_Conf.h"
#include "dance/LocalityManager/Scheduler/Plugin_Manager.h"
#include "dance/LocalityManager/Scheduler/Deployment_Completion.h"
#include "dance/LocalityManager/Scheduler/Events/Install.h"
#include "dance/LocalityManager/Scheduler/Events/Connect.h"
#include "dance/LocalityManager/Scheduler/Events/Endpoint.h"
#include "dance/LocalityManager/Scheduler/Events/Start.h"
#include "dance/LocalityManager/Scheduler/Events/Configured.h"
#include "dance/LocalityManager/Scheduler/Events/Remove.h"
#include "dance/LocalityManager/Scheduler/Events/Passivate.h"
#include "dance/LocalityManager/Scheduler/Events/Disconnect.h"
#include "ace/Auto_Ptr.h"

namespace DAnCE
{
  // Implementation skeleton constructor
  LocalityManager_i::LocalityManager_i (const ACE_TString &uuid,
                                        std::list < std::string > plugin_config,
                                        CORBA::ORB_ptr orb,
                                        PortableServer::POA_ptr poa)
    : uuid_ (uuid),
      plugin_config_files_ (plugin_config),
      orb_ (CORBA::ORB::_duplicate  (orb)),
      poa_ (PortableServer::POA::_duplicate (poa)),
      spawn_delay_ (30)
  {
    DANCE_TRACE ("LocalityManager_i::LocalityManager_i");
    this->scheduler_.activate_scheduler (0);
  }

  LocalityManager_i::~LocalityManager_i (void)
  {
    DANCE_TRACE ("LocalityManager_i::~LocalityManager_i");

    this->scheduler_.terminate_scheduler ();

    PLUGIN_MANAGER::instance ()->set_orb (::CORBA::ORB::_nil ());
  }

  void
  LocalityManager_i::init (const Deployment::Properties &props)
  {
    DANCE_TRACE ("LocalityManager_i::init");

    DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                 (LM_DEBUG, DLINFO
                  ACE_TEXT ("LocalityManager_i::init - ")
                  ACE_TEXT ("Received %u properties from init\n"),
                  props.length ()));

    PLUGIN_MANAGER::instance ()->set_configuration (props);

    PLUGIN_MANAGER::instance ()->set_orb (this->orb_.in ());

    Plugin_Configurator config;

    DANCE_DEBUG (DANCE_LOG_DETAILED_TRACE,
                 (LM_DEBUG, DLINFO
                  ACE_TEXT ("LocalityManager_i::init - ")
                  ACE_TEXT ("Loading %u plugin configuration files\n"),
                  this->plugin_config_files_.size ()));

    for (std::list < std::string >::const_iterator i =
           this->plugin_config_files_.begin ();
         i != this->plugin_config_files_.end (); ++i)
      {
        DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                     (LM_DEBUG, DLINFO
                      ACE_TEXT ("LocalityManager_i::init - ")
                      ACE_TEXT ("Loading plugin file <%C>\n"),
                      i->c_str ()));
        config.load_from_text_file (ACE_TEXT_CHAR_TO_TCHAR (i->c_str ()));
      }

    if (props.length () != 0)
      {
        if (DAnCE::Utility::get_property_value (DAnCE::LOCALITY_TIMEOUT,
                                                props,
                                                this->spawn_delay_))
          {
            DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_DEBUG, DLINFO
                          ACE_TEXT ("LocalityManager_i::init - ")
                          ACE_TEXT ("Using provided spawn delay %u\n"),
                          this->spawn_delay_));
          }

        DANCE_DEBUG (DANCE_LOG_TRACE,
                     (LM_DEBUG, DLINFO
                      ACE_TEXT ("LocalityManager_i::init - ")
                      ACE_TEXT ("Number of LM configuration properties: %u\n"),
                      props.length ()));

        for (CORBA::ULong i = 0; i < props.length (); ++i)
          {
            DANCE_DEBUG (DANCE_LOG_TRACE,
                         (LM_DEBUG, DLINFO
                          ACE_TEXT ("LocalityManager_i::init - ")
                          ACE_TEXT ("Looking up configuration handler for <%C>\n"),
                          props[i].name.in ()));

            ::DAnCE::LocalityConfiguration_var config =
              PLUGIN_MANAGER::instance ()->get_configuration_handler (props[i].name.in ());

            if (config.in ())
              {
                DANCE_DEBUG (DANCE_LOG_DETAILED_TRACE,
                             (LM_DEBUG, DLINFO
                              ACE_TEXT ("LocalityManager_i::init - ")
                              ACE_TEXT ("Invoking configuration handler for <%C>\n"),
                              props[i].name.in ()));
                config->configure (props[i]);
              }
          }
      }
    else
      {
        DANCE_ERROR (DANCE_LOG_MAJOR_DEBUG_INFO,
                     (LM_WARNING, DLINFO
                      ACE_TEXT ("LocalityManager_i::init - ")
                      ACE_TEXT ("Warning: No configuration properties\n")));
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

    Plugin_Manager::INTERCEPTORS interceptors =
      PLUGIN_MANAGER::instance ()->fetch_interceptors ();

    for (Plugin_Manager::INTERCEPTORS::iterator i = interceptors.begin ();
         i != interceptors.end ();
         ++i)
      {
        if (!CORBA::is_nil (*i))
          {
            (*i)->preprocess_plan (this->plan_);
          }
      }

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

    PLUGIN_MANAGER::instance ()->get_installation_order (this->handler_order_);

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

    for (Plugin_Manager::INSTALL_ORDER::const_iterator i = this->handler_order_.begin ();
         i != this->handler_order_.end ();
         ++i)
      {
        DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                     (LM_TRACE, DLINFO
                      ACE_TEXT ("LocalityManager_i::install_instances - ")
                      ACE_TEXT ("Starting installation of %C type instances\n"),
                      i->c_str ()));

        INSTANCE_LIST &inst_list = this->instance_handlers_[*i];

        for (INSTANCE_LIST::const_iterator j = inst_list.begin ();
             j != inst_list.end ();
             ++j)
          {
            DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                         (LM_TRACE, DLINFO
                          ACE_TEXT ("LocalityManager_i::install_instances - ")
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
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("LocalityManager_i::install_instances - ")
                      ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    Event_List completed_events;
    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_WARNING, DLINFO
                      ACE_TEXT ("LocalityManager_i::install_instances - ")
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
            DANCE_ERROR (DANCE_LOG_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::install_instances - ")
                          ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(DAnCE::Utility::extract_and_throw_exception < Deployment::StartError >
              (event.contents_.in ()) ||
              DAnCE::Utility::extract_and_throw_exception < Deployment::InvalidProperty >
              (event.contents_.in ()) ||
              DAnCE::Utility::extract_and_throw_exception < Deployment::InvalidNodeExecParameter >
              (event.contents_.in ()) ||
              DAnCE::Utility::extract_and_throw_exception < Deployment::InvalidComponentExecParameter >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::install_instances - ")
                          ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StartError (event.id_.c_str (),
                                            "Unknown exception");
          }

        DANCE_DEBUG (DANCE_LOG_MAJOR_EVENT,
                     (LM_INFO, DLINFO
                      ACE_TEXT ("LocalityManager_i::install_instances - ")
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

    ::Deployment::Connections *conn_cmp = 0;
    ACE_NEW_THROW_EX (conn_cmp,
                      ::Deployment::Connections (this->plan_.connection.length ()),
                      CORBA::NO_MEMORY ());
    ACE_Auto_Ptr< ::Deployment::Connections > conn_safe (conn_cmp);
    CORBA::ULong conn_pos (0);

    for (CORBA::ULong i = 0;
         i < this->plan_.connection.length ();
         ++i)
      {
        const ::Deployment::PlanConnectionDescription &conn =
          this->plan_.connection[i];

        if (conn.externalReference.length () > 0)
          {
            // connections with external reference endpoints
            // we do not know how to resolve here; we just
            // collect them and allow connect handlers/interceptors
            // to handle them later
            conn_cmp->length (conn_pos + 1);
            (*conn_cmp)[conn_pos].name = conn.name.in ();
            (*conn_cmp)[conn_pos].endpoint.length (1);
            (*conn_cmp)[conn_pos].endpoint[0] = CORBA::Object::_nil ();
            ++conn_pos;
          }
        else
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
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("LocalityManager_i::collect_references - ")
                      ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    Event_List completed_events;
    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_WARNING, DLINFO
                      ACE_TEXT ("LocalityManager_i::collect_references - ")
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
            DANCE_ERROR (DANCE_LOG_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::collect_references - ")
                          ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(DAnCE::Utility::extract_and_throw_exception < Deployment::StartError >
              (event.contents_.in ()) ||
              DAnCE::Utility::extract_and_throw_exception < Deployment::InvalidProperty >
              (event.contents_.in ())))
          {
            DANCE_ERROR (DANCE_LOG_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::collect_references - ")
                          ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StartError (event.id_.c_str (),
                                            "Unknown exception");
          }

        CORBA::Object_var obj_ref;

        if (event.contents_.ptr () != 0)
          event.contents_ >>= CORBA::Any::to_object (obj_ref);
        else
          {
            DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                         (LM_DEBUG, DLINFO
                          ACE_TEXT ("LocalityManager_i::collect_references - ")
                          ACE_TEXT ("No reference returned for connection <%C>\n"),
                          event.id_.c_str ()));
          }

        conn_cmp->length (conn_pos + 1);
        (*conn_cmp)[conn_pos].name = event.id_.c_str ();
        (*conn_cmp)[conn_pos].endpoint.length (1);
        (*conn_cmp)[conn_pos].endpoint[0] = obj_ref;
        ++conn_pos;
      }

    providedReference = conn_safe.release ();
  }

  void
  LocalityManager_i::finishLaunch (const ::Deployment::Connections & providedReference,
                                   ::CORBA::Boolean start)
  {
    DANCE_TRACE ("LocalityManager_i::finishLaunch");

    typedef std::map < std::string, CORBA::ULong > ConnMap;
    ConnMap conns;

    Deployment_Completion completion (this->scheduler_);

    DANCE_DEBUG (DANCE_LOG_MINOR_EVENT,
                 (LM_TRACE, DLINFO
                  ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                  ACE_TEXT ("Starting finishLaunch, received %u references, ")
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

        DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                     (LM_TRACE, DLINFO
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
            DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                         (LM_TRACE, DLINFO
                          ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                          ACE_TEXT ("Skipping connection <%C>\n"),
                          conn.name.in ()));
            continue;
          }

        DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                     (LM_DEBUG, DLINFO
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
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                      ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    Event_List completed_events;
    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_WARNING, DLINFO
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
            DANCE_ERROR (DANCE_LOG_ERROR, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                             ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(DAnCE::Utility::extract_and_throw_exception < Deployment::StartError >
              (event.contents_.in ()) ||
              DAnCE::Utility::extract_and_throw_exception < Deployment::InvalidConnection >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                             ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StartError (event.id_.c_str (),
                                            "Unknown exception");
          }
      }

    dispatched = 0;

    for (Plugin_Manager::INSTALL_ORDER::const_iterator i = this->handler_order_.begin ();
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
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                      ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_WARNING, DLINFO
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
            DANCE_ERROR (DANCE_LOG_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::finishLaunch - ")
                          ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(DAnCE::Utility::extract_and_throw_exception < Deployment::StartError >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
                         (LM_ERROR, DLINFO
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
  LocalityManager_i::disconnect_connections (void)
  {
    DANCE_TRACE ("LocalityManager_i::disconnect_connections");

    typedef std::map < std::string, CORBA::ULong > ConnMap;

    Deployment_Completion completion (this->scheduler_);

    DANCE_DEBUG (DANCE_LOG_MINOR_EVENT,
                 (LM_TRACE, DLINFO
                  ACE_TEXT ("LocalityManager_i::disconnect_connections - ")
                  ACE_TEXT ("Starting disconnect_connections, ")
                  ACE_TEXT ("have %u connections\n"),
                  this->plan_.connection.length ()
                  ));

    CORBA::ULong dispatched (0);

    for (CORBA::ULong i = 0; i < this->plan_.connection.length (); ++i)
      {
        const char * name = this->plan_.connection[i].name.in ();
        CORBA::ULong j (0);

        const ::Deployment::PlanConnectionDescription &conn =
          this->plan_.connection[i];

        DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO, (LM_TRACE, DLINFO
                         ACE_TEXT ("LocalityManager_i::disconnect_connections - ")
                         ACE_TEXT ("Connection <%C> has %u endpoints\n"),
                         conn.name.in (),
                         conn.internalEndpoint.length ()));

        if (conn.internalEndpoint.length () == 2)
          {
            if (conn.internalEndpoint[1].provider)
              j = 1;
          }
        else if (conn.internalEndpoint[0].provider &&
                 conn.externalReference.length () == 0)
          {
            DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                         (LM_TRACE, DLINFO
                          ACE_TEXT ("LocalityManager_i::disconnect_connections - ")
                          ACE_TEXT ("Skipping connection <%C>\n"),
                          conn.name.in ()));
            continue;
          }

        DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                     (LM_DEBUG, DLINFO
                      ACE_TEXT ("LocalityManager_i::disconnect_connections - ")
                      ACE_TEXT ("Starting disconnect connection <%C>\n"),
                      name));

        CORBA::ULong instRef =
          conn.internalEndpoint[j].instanceRef;
        CORBA::ULong implRef =
          this->plan_.instance[instRef].implementationRef;

        const char  *inst_type =
          Utility::get_instance_type (this->plan_.implementation[implRef].execParameter);

        Disconnect_Instance *event (0);
        Event_Future result;
        completion.accept (result);

        ACE_NEW_THROW_EX (event,
                          Disconnect_Instance (this->plan_,
                                               i,
                                               inst_type,
                                               result),
                          CORBA::NO_MEMORY ());

        this->scheduler_.schedule_event (event);
        ++dispatched;
      }

    ACE_Time_Value tv (ACE_OS::gettimeofday () + ACE_Time_Value (this->spawn_delay_));

    if (!completion.wait_on_completion (&tv))
      {
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("LocalityManager_i::disconnect_connections - ")
                      ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    Event_List completed_events;
    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (DANCE_LOG_WARNING,
                     (LM_WARNING, DLINFO
                      ACE_TEXT ("LocalityManager_i::disconnect_connections - ")
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
            DANCE_ERROR (DANCE_LOG_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::disconnect_connections - ")
                          ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(DAnCE::Utility::extract_and_throw_exception < Deployment::StartError >
              (event.contents_.in ()) ||
              DAnCE::Utility::extract_and_throw_exception < Deployment::InvalidConnection >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::disconnect_connections - ")
                          ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StartError (event.id_.c_str (),
                                            "Unknown exception");
          }
      }
  }

  void
  LocalityManager_i::start (void)
  {
    DANCE_TRACE ("LocalityManager_i::start");

    Deployment_Completion completion (this->scheduler_);
    CORBA::ULong dispatched (0);

    for (Plugin_Manager::INSTALL_ORDER::const_iterator i = this->handler_order_.begin ();
         i != this->handler_order_.end ();
         ++i)
      {
        INSTANCE_LIST &inst_list = this->instance_handlers_[*i];

        for (INSTANCE_LIST::const_iterator j = inst_list.begin ();
             j != inst_list.end ();
             ++j)
          {
            DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                         (LM_DEBUG, DLINFO
                          ACE_TEXT ("LocalityManager_i::start - ")
                          ACE_TEXT ("Scheduling start for instance %C\n"),
                          plan_.instance[*j].name.in ()));

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
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("LocalityManager_i::start - ")
                      ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    Event_List completed_events;
    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (DANCE_LOG_WARNING,
                     (LM_WARNING, DLINFO
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
            DANCE_ERROR (DANCE_LOG_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::start - ")
                          ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(DAnCE::Utility::extract_and_throw_exception < Deployment::StartError >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::start - ")
                          ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StartError (event.id_.c_str (),
                                            "Unknown exception");
          }

        DANCE_DEBUG (DANCE_LOG_MAJOR_EVENT,
                     (LM_INFO, DLINFO
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
            DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                         (LM_DEBUG, DLINFO
                          ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                          ACE_TEXT ("Scheduling passivation for instance %C\n"),
                          this->plan_.instance[*j].name.in ()));

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
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                      ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    Event_List completed_events;
    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (DANCE_LOG_WARNING,
                     (LM_WARNING, DLINFO
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
            DANCE_ERROR (DANCE_LOG_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                          ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(DAnCE::Utility::extract_and_throw_exception < Deployment::StopError >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                          ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StopError (event.id_.c_str (),
                                           "Unknown exception");
          }

        DANCE_DEBUG (DANCE_LOG_MINOR_EVENT,
                     (LM_INFO, DLINFO
                      ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                      ACE_TEXT ("Instance <%C> successfully passivated\n"),
                      event.id_.c_str ()));
      }

    // Now disconnect all connections in the plan
    this->disconnect_connections();

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
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                      ACE_TEXT ("Timed out while waiting on completion of scheduler\n")));
      }

    tv = ACE_Time_Value::zero;

    completion.completed_events (completed_events);

    if (completed_events.size () != dispatched)
      {
        DANCE_ERROR (DANCE_LOG_WARNING,
                     (LM_WARNING, DLINFO
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
            DANCE_ERROR (DANCE_LOG_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                          ACE_TEXT ("Failed to get future value for current instance\n")));
            continue;
          }

        if (event.exception_ &&
            !(DAnCE::Utility::extract_and_throw_exception < Deployment::StopError >
              (event.contents_.in ()))
            )
          {
            DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR, (LM_ERROR, DLINFO
                             ACE_TEXT ("LocalityManager_i::destroyApplication - ")
                             ACE_TEXT ("Error: Unknown exception propagated\n")));
            throw ::Deployment::StopError (event.id_.c_str (),
                                           "Unknown exception");
          }

        DANCE_DEBUG (DANCE_LOG_MINOR_EVENT, (LM_INFO, DLINFO
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

    DANCE_DEBUG (DANCE_LOG_MAJOR_EVENT,
                 (LM_DEBUG, DLINFO
                  ACE_TEXT ("DAnCE LocalityManager shutdown request received for UUID <%C>\n"),
                  uuid_.c_str ()));

    this->orb_->shutdown ();

    // Explicitly close the plugin manager to release memory.
    PLUGIN_MANAGER::close ();

    this->poa_ = PortableServer::POA::_nil ();
    this->orb_ = CORBA::ORB::_nil ();
  }
}
