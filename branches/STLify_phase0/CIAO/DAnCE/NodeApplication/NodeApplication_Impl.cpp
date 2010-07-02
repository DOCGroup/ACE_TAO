// $Id$
#include "NodeApplication_Impl.h"

#include "ace/OS_Memory.h"

#include "ace/streams.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Object.h"
#include "tao/ORB.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "Deployment/Deployment_BaseC.h"
#include "Deployment/Deployment_ApplicationC.h"
#include "Deployment/Deployment_PlanErrorC.h"
#include "Deployment/Deployment_ApplicationManagerC.h"
#include "DAnCE/DAnCE_Utility.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/DAnCE_LocalityManagerC.h"
#include "DAnCE/LocalityManager/Scheduler/Plugin_Manager.h"
#include "DAnCE/LocalityManager/Scheduler/Deployment_Completion.h"
#include "LocalityManager/Scheduler/Events/Install.h"
#include "LocalityManager/Scheduler/Events/Remove.h"
#include "Split_Plan/Locality_Splitter.h"
#include "Split_Plan/Split_Plan.h"

#include <string>

#ifdef GEN_OSTREAM_OPS
#include <iostream>
#include <sstream>
#endif /* GEN_OSTREAM_OPS */

using namespace DAnCE;
using namespace DAnCE::Utility;

NodeApplication_Impl::NodeApplication_Impl (CORBA::ORB_ptr orb,
                                            PortableServer::POA_ptr poa,
                                            DAnCE::ArtifactInstallation_ptr installer,
                                            const ACE_CString& node_name,
                                            const DAnCE::Utility::PROPERTY_MAP &properties)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    installer_ (DAnCE::ArtifactInstallation::_duplicate (installer)),
    node_name_ (node_name),
    scheduler_ (),
    spawn_delay_ (30)
{
  DANCE_TRACE ("NodeApplication_Impl::NodeApplication_Impl");

  Utility::get_property_value (DAnCE::LOCALITY_TIMEOUT,
                               properties, this->spawn_delay_);

  ::Deployment::Properties prop;

  // Spawn thread pool
  // @Todo:  We can probably move this up into the NodeManager and
  // share the thread pool among several node applications.
  this->scheduler_.activate_scheduler (0);
}

NodeApplication_Impl::~NodeApplication_Impl()
{
  DANCE_TRACE( "NodeApplication_Impl::~NodeApplication_Impl()");
  this->scheduler_.terminate_scheduler ();
}

void
NodeApplication_Impl::prepare_instances (const LocalitySplitter::TSubPlans& plans)
{
  DANCE_TRACE ("NodeApplication_Impl::prepare_instances");

  CORBA::ULong plan (0);
  std::list < Event_Future > prepared_instances;
  Deployment_Completion completion (this->scheduler_);

  // for each sub plan
  LocalitySplitter::TSubPlanConstIterator plans_end (plans, 1);
  for (LocalitySplitter::TSubPlanConstIterator i (plans);
       i != plans_end;
       ++i)
    {
      const ::Deployment::DeploymentPlan &sub_plan =
        (*i).int_id_;

      const LocalitySplitter::TSubPlanKey &sub_plan_key =
        (*i).ext_id_;

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::prepare_instances - ")
                       ACE_TEXT ("Considering sub-plan %u:%C with %u instances\n"),
                       plan,
                       sub_plan.UUID.in (),
                       sub_plan.instance.length ()
                       ));

      // the locality splitter makes sure every sub plan contains a Locality Manager
      // instance (creating default if necessary) and identifies it in the key

      CORBA::ULong loc_manager_instance = sub_plan_key.locality_manager_instance ();
      const ::Deployment::InstanceDeploymentDescription &lm_idd
        = sub_plan.instance[loc_manager_instance];


      DANCE_DEBUG (4, (LM_DEBUG, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::prepare_instances - ")
                       ACE_TEXT ("Found Locality Manager instance %u:%C, deploying\n"),
                       loc_manager_instance,
                       lm_idd.name.in ()
                       ));

      this->sub_plans_ [ lm_idd.name.in () ] = SUB_PLAN (loc_manager_instance,
                                                         sub_plan);

      Install_Instance *event (0);
      Event_Future result;
      completion.accept (result);

      ACE_NEW_THROW_EX (event,
                        Install_Instance (this->sub_plans_ [ lm_idd.name.in () ].second,
                                          loc_manager_instance,
                                          DAnCE::DANCE_LOCALITYMANAGER,
                                          result
                                          ),
                        CORBA::NO_MEMORY ());


      prepared_instances.push_back (result);
      this->scheduler_.schedule_event (event);
      ++plan;
    }

  ACE_Time_Value tv (ACE_OS::gettimeofday () + ACE_Time_Value (this->spawn_delay_));

  if (!completion.wait_on_completion (&tv))
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                       ACE_TEXT("NodeApplication_Impl::prepare_instances - ")
                       ACE_TEXT("Timed out while waiting on completion of scheduler\n")));
    }

  tv = ACE_Time_Value::zero;

  plan = 0;
  for (std::list < Event_Future >::iterator i = prepared_instances.begin ();
       i != prepared_instances.end ();
       ++i)
    {
      Event_Result event;
      if (i->get (event, &tv) != 0)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT("NodeApplication_Impl::prepare_instances - ")
                           ACE_TEXT("Failed to get future value for current instance\n")));
          continue;
        }

      if (event.exception_)
        {
          DAnCE::Utility::throw_exception_from_any (event.contents_.in ());
        }

      ::DAnCE::LocalityManager_var lm_ref;
      if (event.contents_.ptr () &&
          event.contents_.in ().impl () &&
          (event.contents_.in ()  >>= lm_ref) &&
          !CORBA::is_nil (lm_ref.in ()))
        {
          this->localities_[event.id_] = ::DAnCE::LocalityManager::_duplicate (lm_ref);
          DANCE_DEBUG (4, (LM_INFO, DLINFO
                           ACE_TEXT("NodeApplication_Impl::prepare_instances - ")
                           ACE_TEXT("Successfully started Locality %C\n"),
                           event.id_.c_str ()));
        }
      else
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT("NodeApplication_Impl::prepare_instances - ")
                           ACE_TEXT("Unable to resolve LocalityManager object reference\n")));
          throw ::Deployment::StartError (event.id_.c_str (),
                                          "Unable to resolve LocalityManager object ref\n");
        }

      DANCE_DEBUG (4, (LM_DEBUG, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::prepare_instances - ")
                       ACE_TEXT ("Invoking preparePlan on locality %C\n"),
                       event.id_.c_str ()));


      this->prepare_instance (event.id_.c_str (),
                              (this->sub_plans_[event.id_].second));

      DANCE_DEBUG (9, (LM_DEBUG, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::prepare_instances - ")
                       ACE_TEXT ("Successfully executed preparePlan on locality %C\n"),
                       event.id_.c_str ()));
    }
}

void
NodeApplication_Impl::prepare_instance (const char *name,
                                        const Deployment::DeploymentPlan &plan)
{
  DANCE_TRACE ("NodeApplication_Impl::prepare_instance");

  ::Deployment::ApplicationManager_var app;

  try
    {
      app = this->localities_[name]->preparePlan (plan,
                                                  0);
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::prepare_instance - ")
                       ACE_TEXT ("Locality <%C> successfully prepared.\n"),
                       name));
    }
  catch (CORBA::Exception &ex)
    {
      DANCE_ERROR (2, (LM_ERROR, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::prepare_instance - ")
                       ACE_TEXT ("Caugt unexpected CORBA exception while invoking preparePlan %C\n"),
                       ex._info ().c_str ()));
    }

  if (CORBA::is_nil (app.in ()))
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::prepare_instance - ")
                       ACE_TEXT ("Error: Didn't get a valid reference back from LM preparePlan ")
                       ACE_TEXT ("for locality %C\n"),
                       name));

      // For the time being, we don't need to cache this reference.
      // it's the same as the reference we used to invoke preparePlan.
    }

}

void
NodeApplication_Impl::start_launch_instances (const Deployment::Properties &prop,
                                              Deployment::Connections_out providedReference)
{
  DANCE_TRACE ("NodeApplication_Impl::start_launch_instances");
  Deployment::Connections *tmp (0);

  ACE_NEW_THROW_EX (tmp,
                   Deployment::Connections (),
                   CORBA::NO_MEMORY ());
  
  providedReference = tmp;
  CORBA::ULong retval_pos (0);
  
  for (LOCALITY_MAP::const_iterator i = this->localities_.begin ();
       i != this->localities_.end (); ++i)
    {
      DANCE_DEBUG (4, (LM_INFO, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::start_launch_instances - ")
                       ACE_TEXT ("StartLaunching locality <%C>\n"),
                       i->first.c_str ()));

      try
        {
          Deployment::Connections_var instance_references;
          i->second->startLaunch (prop,
                                  instance_references.out ());
          
          
          providedReference->length (retval_pos + instance_references->length ());
          
          for (CORBA::ULong ir_pos = 0;
               ir_pos < instance_references->length ();
               ++ir_pos)
            {
              providedReference[retval_pos++] = instance_references[ir_pos];
            }
        }
      catch (Deployment::PlanError &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("NodeApplication_Impl::start_launch_instances - ")
                           ACE_TEXT ("Caught PlanError Exception %C:%C\n"),
                           ex.name.in (),
                           ex.reason.in ()));
          throw;
        }
      catch (Deployment::StartError &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("NodeApplication_Impl::start_launch_instances - ")
                           ACE_TEXT ("Caught StartError Exception %C:%C\n"),
                           ex.name.in (),
                           ex.reason.in ()));
          throw;
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("NodeApplication_Impl::start_launch_instances - ")
                           ACE_TEXT ("Caught CORBA Exception %C\n"),
                           ex._info ().c_str ()));
          throw;
        }
    }
}

void
NodeApplication_Impl::finishLaunch (const ::Deployment::Connections & providedReference,
                                    ::CORBA::Boolean start)
{
  DANCE_TRACE ("NodeApplication_Impl::finishLaunch");


  for (LOCALITY_MAP::const_iterator i = this->localities_.begin ();
       i != this->localities_.end (); ++i)
    {
      DANCE_DEBUG (4, (LM_INFO, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::finish_launch_instances - ")
                       ACE_TEXT ("FinishLaunching locality <%C>\n"),
                       i->first.c_str ()));

      try
        {
          i->second->finishLaunch (providedReference,
                                   start);
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("NodeApplication_Impl::finish_launch_instances - ")
                           ACE_TEXT ("Caught CORBA Exception %C\n"),
                           ex._info ().c_str ()));
          throw;
        }
    }
}

void
NodeApplication_Impl::start ()
{
  DANCE_TRACE( "NodeApplication_Impl::start");

  for (LOCALITY_MAP::const_iterator i = this->localities_.begin ();
       i != this->localities_.end (); ++i)
    {
      DANCE_DEBUG (4, (LM_INFO, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::start - ")
                       ACE_TEXT ("Starting locality <%C>\n"),
                       i->first.c_str ()));

      try
        {
          i->second->start ();
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("NodeApplication_Impl::start - ")
                           ACE_TEXT ("Caught CORBA Exception %C\n"),
                           ex._info ().c_str ()));
          throw;
        }
    }
}

void
NodeApplication_Impl::remove_instances (void)
{
  DANCE_TRACE ("NodeApplication_Impl::remove_instances");

  ::Deployment::StopError final_exception;
  bool flag (false);

  std::list < Event_Future > removed_instances;

  Deployment_Completion completion (this->scheduler_);

  for (LOCALITY_MAP::iterator i = this->localities_.begin ();
       i != this->localities_.end (); ++i)
    {
      DANCE_DEBUG (4, (LM_INFO, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::remove_instances - ")
                       ACE_TEXT ("Removing locality <%C>\n"),
                       i->first.c_str ()));

      try
        {
          CORBA::Any ref;
          ref <<= ::DAnCE::LocalityManager::_duplicate (i->second);

          i->second->destroyApplication (0);

          Remove_Instance *event (0);
          Event_Future result;

          PLAN_MAP::iterator sub_plan;

          if ((sub_plan = this->sub_plans_.find (i->first)) !=
              this->sub_plans_.end ())
            {
              ACE_NEW (event,
                       Remove_Instance (sub_plan->second.second,
                                        sub_plan->second.first,
                                        ref,
                                        DANCE_LOCALITYMANAGER,
                                        result));

              removed_instances.push_back (result);
              completion.accept (result);

              this->scheduler_.schedule_event (event);
            }
          else
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                               ACE_TEXT ("NodeApplication_Impl::remove_instances - ")
                               ACE_TEXT ("Unable to find sub plan for instance <%C>\n"),
                               i->first.c_str ()));
            }
        }
      catch (::Deployment::StopError &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("NodeApplication_Impl::remove_instances - ")
                           ACE_TEXT ("Caught StopError final_exception %C, %C\n"),
                           ex.name.in (),
                           ex.reason.in ()));
          Utility::test_and_set_exception (flag,
                                           final_exception,
                                           ex.name.in (),
                                           ex.reason.in ());

        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("NodeApplication_Impl::remove_instances - ")
                           ACE_TEXT ("Caught CORBA Final_Exception %C\n"),
                           ex._info ().c_str ()));
          Utility::test_and_set_exception (flag,
                                           final_exception,
                                           "Unknown CORBA Final_Exception",
                                           ex._info ().c_str ());
        }
    }

  ACE_Time_Value tv (ACE_OS::gettimeofday () + ACE_Time_Value (this->spawn_delay_));

  if (completion.wait_on_completion (&tv))
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                       ACE_TEXT("NodeApplication_Impl::remove_instances - ")
                       ACE_TEXT("Timed out while waiting on completion of scheduler\n")));
    }

  tv = ACE_Time_Value::zero;

  for (std::list < Event_Future >::iterator i = removed_instances.begin ();
       i != removed_instances.end ();
       ++i)
    {
      try
        {
          Event_Result event;

          if (i->get (event,
                      &tv) != 0)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                               ACE_TEXT ("NodeApplication_Impl::remove_instances - ")
                               ACE_TEXT ("Failed to get future value for current instance\n")));
              continue;
            }

          using DAnCE::Utility::extract_and_throw_exception;

          if (event.exception_ &&
              !(extract_and_throw_exception< ::Deployment::StopError > (event.contents_.in ())))
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                               ACE_TEXT ("NodeApplication_Impl::remove_instances - ")
                               ACE_TEXT ("Unexpected exception thrown during removal of ")
                               ACE_TEXT ("instance <%C>\n"),
                               event.id_.c_str ()));

              throw ::Deployment::StopError (event.id_.c_str (),
                                             "Unknown exception thrown from remove_instance\n");
            }

        }
      catch (::Deployment::StopError &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("NodeApplication_Impl::remove_instances - ")
                           ACE_TEXT ("Caught StopError final_exception %C, %C\n"),
                           ex.name.in (),
                           ex.reason.in ()));
          Utility::test_and_set_exception (flag,
                                           final_exception,
                                           ex.name.in (),
                                           ex.reason.in ());

        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT ("NodeApplication_Impl::remove_instances - ")
                           ACE_TEXT ("Caught CORBA Final_Exception %C\n"),
                           ex._info ().c_str ()));
          Utility::test_and_set_exception (flag,
                                           final_exception,
                                           "Unknown CORBA Final_Exception",
                                           ex._info ().c_str ());
        }

    }
  if (flag)
    throw final_exception;
}
