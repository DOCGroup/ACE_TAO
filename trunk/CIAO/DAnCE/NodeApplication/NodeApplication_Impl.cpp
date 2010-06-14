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
                                            const ACE_CString& node_name,
                                            const PROPERTY_MAP &properties)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    node_name_ (node_name),
    properties_ (),
    handler_ (properties,
              orb,
              poa)
{
  DANCE_TRACE ("NodeApplication_Impl::NodeApplication_Impl");
  PROPERTY_MAP::const_iterator i = properties.begin ();
  while (!i.done ())
    {
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::NodeApplication_Impl - ")
                    ACE_TEXT("Binding value for property '%C'\n"), i->key ().c_str ()));
      this->properties_.bind (i->key (), i->item ());
      i.advance ();
    }
  this->init ();
}

NodeApplication_Impl::~NodeApplication_Impl()
{
  DANCE_TRACE( "NodeApplication_Impl::~NodeApplication_Impl()");
}

void
NodeApplication_Impl::init()
{
  DANCE_TRACE( "NodeApplication_Impl::init()");

  /* TODO:  Lets move this stuff to the constructor, shall we?!? */
  /* TODO:  Might be nice to use a component configurator here to load the proper versions
     of the serveractivator.  */

  const ACE_TCHAR *ior = 0;

  if (get_property_value (DAnCE::INSTANCE_NC, this->properties_, ior) ||
      get_property_value (DAnCE::DOMAIN_NC, this->properties_, ior))
    {
      try
        {
          CORBA::Object_var obj = this->orb_->string_to_object (ior);
          this->instance_nc_ = CosNaming::NamingContext::_narrow (obj);
        }
      catch (const CORBA::Exception &e)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("NodeApplication_Impl::init - ")
                        ACE_TEXT("Unable to resolve the instance naming context:%C\n"),
                        e._info ().c_str ()));
        }
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::init - ")
                    ACE_TEXT("Successfully resolved the instance naming context.\n")));
    }
  else DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("NodeApplication_Impl::init - ")
                     ACE_TEXT("No instance NC was provided\n")));
}

void
NodeApplication_Impl::prepare_instances (const LocalitySplitter::TSubPlans& plans)
{
  DANCE_TRACE ("NodeApplication_Impl::prepare_instances");

  CORBA::ULong plan (0);
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
                       plan++,
                       sub_plan.UUID.in (),
                       sub_plan.instance.length ()
                       ));

      // the locality splitter makes sure every sub plan contains a Locality Manager
      // instance (creating default if necessary) and identifies it in the key

      CORBA::ULong loc_manager_instance = sub_plan_key.locality_manager_instance ();
      const ::Deployment::InstanceDeploymentDescription &lm_idd = sub_plan.instance[loc_manager_instance];

      DANCE_DEBUG (4, (LM_DEBUG, DLINFO
                        ACE_TEXT ("NodeApplication_Impl::prepare_instances - ")
                        ACE_TEXT ("Found Locality Manager instance %u:%C, deploying\n"),
                        loc_manager_instance,
                        lm_idd.name.in ()
                        ));

      // TODO: MCO - this is definitely not nice; what else can we do here?

      // Need to make a temp copy since we're modifying properties
      ::Deployment::DeploymentPlan lm_plan;
      lm_plan.instance.length (1);
      lm_plan.implementation.length (1);
      lm_plan.instance[0] = lm_idd;
      lm_plan.instance[0].implementationRef = 0;
      lm_plan.implementation[0] = sub_plan.implementation[lm_idd.implementationRef];

      // Need to add naming service reference to properties.
      CORBA::ULong pos = lm_plan.instance[0].configProperty.length ();
      lm_plan.instance[0].configProperty.length (pos + 1);
      lm_plan.instance[0].configProperty[pos].name = DAnCE::LOCALITY_NAMINGCONTEXT;
      lm_plan.instance[0].configProperty[pos].value <<= this->instance_nc_;

      CORBA::Any_var reference;
      this->handler_.install_instance (lm_plan,
                                       0,
                                       reference.out ());

      ::DAnCE::LocalityManager_var lm_ref;
      if (reference.in ()  >>= lm_ref)
        {
          this->localities_[lm_idd.name.in ()] = lm_ref._retn ();
          DANCE_DEBUG (4, (LM_INFO, DLINFO
                            ACE_TEXT("NodeApplication_Impl::prepare_instances - ")
                            ACE_TEXT("Successfully started Locality %C\n"),
                            lm_idd.name.in ()));
        }
      else
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT("NodeApplication_Impl::prepare_instances - ")
                            ACE_TEXT("Unable to resolve LocalityManager object reference\n")));
          throw ::Deployment::StartError (lm_idd.name.in (),
                                          "Unable to resolve LocalityManager object ref\n");
        }

      DANCE_DEBUG (4, (LM_DEBUG, DLINFO
                        ACE_TEXT ("NodeApplication_Impl::prepare_instances - ")
                        ACE_TEXT ("Invoking preparePlan on locality %C\n"),
                        lm_idd.name.in ()));

      this->prepare_instance (lm_idd.name.in (),
                              sub_plan);

      DANCE_DEBUG (9, (LM_DEBUG, DLINFO
                        ACE_TEXT ("NodeApplication_Impl::prepare_instances - ")
                        ACE_TEXT ("Successfully executed preparePlan on locality %C\n"),
                        lm_idd.name.in ()));
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
//  Deployment::Connections *tmp (0);

//   ACE_NEW_THROW_EX (tmp,
//                     Deployment::Connections (this->plan_.connection.length ()),
//                     CORBA::NO_MEMORY ());
//
//   Deployment::Connections_var retval (tmp);

  for (LOCALITY_MAP::const_iterator i = this->localities_.begin ();
       i != this->localities_.end (); ++i)
    {
      DANCE_DEBUG (4, (LM_INFO, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::start_launch_instances - ")
                       ACE_TEXT ("StartLaunching locality <%C>\n"),
                       i->first.c_str ()));

      try
        {
          i->second->startLaunch (prop,
                                  providedReference);
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
          ref <<= DAnCE::LocalityManager::_duplicate (i->second);

          i->second->destroyApplication (0);

          ::Deployment::DeploymentPlan plan;  // TODO: MCO - this really sucks; if there is a real need
                                              //             for the plan here we should probably get the
                                              //             correct sub plan passed somehow like in
                                              //             prepare_instances ()
          this->handler_.remove_instance (plan, /* not needed at this time */
                                          0, /* not needed at this time */
                                          ref);

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

      //this->localities_.erase (i);
    }

    if (flag)
      throw final_exception;
}
