
#include "RM_Proxy.h"
#include "HMOptions.h"
#include "monitorC.h"
#include "HMOptions.h"
#include "CPULoadCalculator.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

#include "ace/Log_Msg.h"

#include <stdexcept>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "ArgPair.h"

RM_Proxy::RM_Proxy (CORBA::ORB_ptr orb)
: load_calc_ (0),
  orb_ (CORBA::ORB::_duplicate (orb))
{
  /// Initilize the ORB.
  ArgPair arg_pair = HMOptions::instance()->arg_pair();
  
  //CORBA::ORB_var orb =  CORBA::ORB_init (arg_pair.argc, arg_pair.argv, 
  //                                       "ORB_FOR_RM_Proxy");
  CORBA::Object_var obj = obtain_RM_ior (orb_.in ());

  if (CORBA::is_nil (obj))
  {
    ACE_DEBUG((LM_ERROR, "Nil Reference of ReplicationManager\n"));
    throw std::runtime_error ("Nil Reference of ReplicationManager");
  }

  /// Downcast the object reference to a reference of type HostMonitor.
  RM_var_ = ReplicationManager::_narrow (obj);
  if (CORBA::is_nil (RM_var_))
  {
    ACE_DEBUG((LM_ERROR, "Argument is not a ReplicationManager reference.\n"));
    throw std::runtime_error ("Argument is not a ReplicationManager reference.");
  }
}

RM_Proxy::~RM_Proxy ()
{
}

CORBA::Object_var RM_Proxy::obtain_RM_ior (CORBA::ORB_ptr orb)
{
  std::pair <char, std::string> ior_access = HMOptions::instance ()->ior_access ();

  if (ior_access.first == 'i') /// file based IOR
  {
     return orb->string_to_object (ior_access.second.c_str());
  }
  else if (ior_access.first == 'n')  /// Naming Service based IOR
  {
      CORBA::Object_var naming_obj =
      orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_DEBUG ((LM_ERROR,"Unable to get the Naming Service.\n"));


      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (ior_access.second.c_str());
      name[0].kind = CORBA::string_dup ("");

      return naming_context->resolve (name);
  }
  else
  {
    return orb->string_to_object (HMOptions::instance()->RM_ior ().c_str ());
  }
}

void RM_Proxy::setCPULoadCalculator (CPULoadCalculator *load_calc)
{
  this->load_calc_ = load_calc;
}

void RM_Proxy::proc_failure (const std::string & process_id)
{
  try
    {
      RM_var_->proc_failure (process_id.c_str());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }
}

int
RM_Proxy::pulse (void)
{
  //ACE_DEBUG((LM_ERROR,"host_id=%s\n",HMOptions::instance()->host_id().c_str()));
  //ACE_DEBUG((LM_ERROR,"load=%d\n", (int)load_calc_->percent_load()));
  RM_var_->util_update (HMOptions::instance()->host_id().c_str (), 
                        load_calc_->percent_load());
  return 0;
}

