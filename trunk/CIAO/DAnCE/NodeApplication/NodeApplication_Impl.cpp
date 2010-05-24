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

#include <string>

#ifdef GEN_OSTREAM_OPS
#include <iostream>
#include <sstream>
#endif /* GEN_OSTREAM_OPS */

using namespace DAnCE;
using namespace DAnCE::Utility;

NodeApplication_Impl::NodeApplication_Impl (CORBA::ORB_ptr orb,
                                            PortableServer::POA_ptr poa,
                                            ::Deployment::DeploymentPlan& plan,
                                            const ACE_CString& node_name,
                                            const PROPERTY_MAP &properties)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    plan_ (plan),
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
NodeApplication_Impl::init_instances (void)
{
  DANCE_TRACE ("NodeApplication::init_instances");
  
  for (CORBA::ULong i = 0; i < plan_.instance.length (); ++i)
    {
      CORBA::ULong impl_idx = plan_.instance[i].implementationRef;
      
      if (impl_idx >= plan_.implementation.length ())
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                           ACE_TEXT("NodeApplication_Impl::init_instances - ")
                           ACE_TEXT("Invalid impelmentation index\n")));
        }

      const char *instance_type = 
        DAnCE::Utility::get_instance_type (plan_.implementation[impl_idx].execParameter);
      
      if (instance_type)
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                         ACE_TEXT("NodeApplication_Impl::init_instances - ")
                         ACE_TEXT("Considering instance %u:%C with type %C\n"),
                         i,
                         plan_.instance[i].name.in (),
                         instance_type));
      else
        {
          DANCE_DEBUG (4, (LM_DEBUG, DLINFO
                           ACE_TEXT ("NodeApplication_Impl::init_instances - ")
                           ACE_TEXT ("Skipping instance %u:%C\n"),
                           i,
                           plan_.instance[i].name.in ()
                           ));
          continue;
        }

      if (ACE_OS::strcmp (instance_type,
                          DANCE_LOCALITYMANAGER) == 0)
        {
          DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                           ACE_TEXT("NodeApplication_Impl::init_instances - ")
                           ACE_TEXT("Found LocalityManager instance, deploying\n")));
          
          
          // Need to add naming service reference to properties. 
          CORBA::ULong pos = this->plan_.instance[i].configProperty.length ();
          this->plan_.instance[i].configProperty.length (pos + 1);
          this->plan_.instance[i].configProperty[pos].name = DAnCE::LOCALITY_NAMINGCONTEXT;
          this->plan_.instance[i].configProperty[pos].value <<= this->instance_nc_;
          
          CORBA::Any_var reference;
          this->handler_.install_instance (this->plan_,
                                           i,
                                           reference.out ());
          
          ::DAnCE::LocalityManager_var lm_ref;
          if (reference.in ()  >>= lm_ref)
            {
              this->localities_[plan_.instance[i].name.in ()] = lm_ref._retn ();
              DANCE_DEBUG (4, (LM_INFO, DLINFO
                               ACE_TEXT("NodeApplication_Impl::init_instances - ")
                               ACE_TEXT("Successfully started Locality %C\n"),
                               plan_.instance[i].name.in ()));
            }
          else
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                               ACE_TEXT("NodeApplication_Impl::init_instances - ")
                               ACE_TEXT("Unable to resolve LocalityManager object reference\n")));
              throw ::Deployment::StartError (plan_.instance[i].name.in (),
                                              "Unable to resolve LocalityManager object ref\n");
            }
        }
    }
}

void 
NodeApplication_Impl::prepare_instances (void)
{
  DANCE_TRACE ("NodeApplication_Impl::prepare_instances");

  typedef DAnCE::Split_Plan < DAnCE::Locality_Splitter > Splitter;
  Splitter split_plan;
  split_plan.split_plan (this->plan_);

  Splitter::TSubPlans &plans = split_plan.plans ();
  
  // @@ Ideally, it would be nice to have the key for the subplans
  // be the name of the locality instance.
  
  CORBA::ULong plan (0);
  // for each sub plan
  for (Splitter::TSubPlanIterator i = plans.begin ();
       i != plans.end ();
       ++i)
    {
      ::Deployment::DeploymentPlan &sub_plan = 
        (*i).int_id_;

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT ("NodeApplication_Impl::prepare_instances - ")
                       ACE_TEXT ("Considering sub-plan %u with %u instances\n"),
                       plan++,
                       sub_plan.instance.length ()
                       ));
      
      bool locality_found (false);

      // We need to find the locality
      for (CORBA::ULong j = 0; j < sub_plan.instance.length (); ++j)
        {
          ::Deployment::InstanceDeploymentDescription &inst = 
            sub_plan.instance[j];
          ::Deployment::MonolithicDeploymentDescription &mdd =
              sub_plan.implementation[inst.implementationRef];
          
          const char *instance_type = 
            DAnCE::Utility::get_instance_type (mdd.execParameter);
          
          if (instance_type &&
              ACE_OS::strcmp (instance_type,
                              DANCE_LOCALITYMANAGER) == 0)
            {
              locality_found = true;
              DANCE_DEBUG (4, (LM_DEBUG, DLINFO
                               ACE_TEXT ("NodeApplication_Impl::prepare_instances - ")
                               ACE_TEXT ("Invoking preparePlan on locality %C\n"),
                               inst.name.in ()));
              this->prepare_instance (inst.name.in (),
                                      sub_plan);
            }
        }
      
      if (!locality_found &&
          sub_plan.instance.length () != 0)
        {
          // Need to spawn default locality. 
          DANCE_DEBUG (4, (LM_DEBUG, DLINFO
                           ACE_TEXT ("NodeApplication_Impl::prepare_instances - ")
                           ACE_TEXT ("No locality manager found, creating a default locality ")
                           ACE_TEXT ("named <%C>\n"),
                           sub_plan.instance[0].name.in ()
                           ));
          
          // Create minimal plan required for locality manager spawn
          // This is a temporary workaround.
          ::Deployment::DeploymentPlan tmp_plan;
          tmp_plan.UUID = this->plan_.UUID;
          tmp_plan.implementation.length (1);
          tmp_plan.instance.length (1);
          tmp_plan.instance[0].implementationRef = 0;
          tmp_plan.instance[0].name = sub_plan.instance[0].name.in ();
          
          // Need to add naming service reference to properties. 
          tmp_plan.instance[0].configProperty.length (1);
          tmp_plan.instance[0].configProperty[0].name = DAnCE::LOCALITY_NAMINGCONTEXT;
          tmp_plan.instance[0].configProperty[0].value <<= this->instance_nc_;

          CORBA::Any_var reference;
          this->handler_.install_instance (tmp_plan,
                                           0,
                                           reference.out ());
          
          ::DAnCE::LocalityManager_var lm_ref;
          if (reference.in ()  >>= lm_ref)
            {
              this->localities_[tmp_plan.instance[0].name.in ()] = lm_ref._retn ();
              DANCE_DEBUG (4, (LM_INFO, DLINFO
                               ACE_TEXT("NodeApplication_Impl::prepare_instances - ")
                               ACE_TEXT("Successfully started Locality %C\n"),
                               tmp_plan.instance[0].name.in ()));
            }
          else
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                               ACE_TEXT("NodeApplication_Impl::prepare_instances - ")
                               ACE_TEXT("Unable to resolve LocalityManager object reference\n")));
              throw ::Deployment::StartError (tmp_plan.instance[0].name.in (),
                                              "Unable to resolve default LocalityManager object ref\n");
            }

          this->prepare_instance (sub_plan.instance[0].name.in (),
                                  sub_plan);
        }
      
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
                    Deployment::Connections (this->plan_.connection.length ()),
                    CORBA::NO_MEMORY ());
  
  Deployment::Connections_var retval (tmp);
  
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

          this->handler_.remove_instance (this->plan_,
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
