// -*- C++ -*-
// $Id$

#include "Executor_Destructor_Interceptor.h"
#include "Exec_Dest_Status.h"

#include "DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/DAnCE_Utility.h"

namespace DAnCE
{
  // Implementation skeleton constructor
  Executor_Destructor::Executor_Destructor (void)
  {
  }

  // Implementation skeleton destructor
  Executor_Destructor::~Executor_Destructor (void)
  {
    for (std::set < std::string >::iterator i = instance_ids_.begin ();
         i != instance_ids_.end ();
         ++i)
      {
        if (!dance_test::EXEC_DEST_STATUS::instance ()->check_destructor (*i))
          {
            ACE_ERROR ((LM_EMERGENCY, 
                        ACE_TEXT ("Error:  Destructor for instance <%C> not invoked\n"),
                        i->c_str ()));
          }
        else 
          { 
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Success!  Destructor for instance <%C> was invoked!\n"),
                        i->c_str ()));
          }
      }
  }

  void
  Executor_Destructor::configure (const ::Deployment::Properties &)
  {
    // Add your implementation here
  }

  void
  Executor_Destructor::preprocess_plan (::Deployment::DeploymentPlan &plan)
  {
    for (CORBA::ULong i = 0; i < plan.instance.length (); ++i)
      {
        ::Deployment::MonolithicDeploymentDescription &mdd = 
          plan.implementation[plan.instance[i].implementationRef];
        
        if (ACE_OS::strcmp (DAnCE::Utility::get_instance_type (mdd.execParameter),
                            ::DAnCE::DANCE_LOCALITYMANAGER) != 0)
          this->instance_ids_.insert (plan.instance[i].name.in ());
      }
  }

  void
  Executor_Destructor::pre_install (::Deployment::DeploymentPlan &,
                                    ::CORBA::ULong)
  {
    // Add your implementation here
  }

  void Executor_Destructor::post_configured (const ::Deployment::DeploymentPlan & ,
                                             ::CORBA::ULong ,
                                             const ::CORBA::Any &)
  {
    // Add your implementation here
  }

  void Executor_Destructor::post_activate (const ::Deployment::DeploymentPlan & ,
                                           ::CORBA::ULong ,
                                           const ::CORBA::Any & )
  {
    // Add your implementation here
  }

  void Executor_Destructor::post_passivate (const ::Deployment::DeploymentPlan & ,
                                            ::CORBA::ULong ,
                                            const ::CORBA::Any &)
  {
    // Add your implementation here
  }

  void Executor_Destructor::post_remove (const ::Deployment::DeploymentPlan &plan,
                                         ::CORBA::ULong instRef,
                                         const ::CORBA::Any & )
  {
  }
}


extern "C"
{
  ::DAnCE::DeploymentInterceptor_ptr
  create_Executor_Destructor (void)
  {
    return new DAnCE::Executor_Destructor ();
  }
}
