// -*- C++ -*-
// $Id$

#include <stdexcept>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "ace/Log_Msg.h"

#include "orbsvcs/orbsvcs/Naming/Naming_Client.h"

#include "RM_Proxy.h"
#include "HMOptions.h"

RM_Proxy::RM_Proxy (CORBA::ORB_ptr orb)
  : load_calc_ (0),
    orb_ (CORBA::ORB::_duplicate (orb))
{
  CORBA::Object_var obj = obtain_RM_ior (orb_.in ());

  if (CORBA::is_nil (obj))
    {
      ACE_DEBUG ((LM_ERROR,
                  "Nil Reference of ReplicationManager\n"));
      throw std::runtime_error ("Nil Reference of ReplicationManager");
    }

  /// Downcast the object reference to a reference of type HostMonitor.
  RM_var_ = ReplicationManager::_narrow (obj);
  
  if (CORBA::is_nil (RM_var_.in ()))
    {
      ACE_DEBUG ((LM_ERROR,
                  "Argument is not a ReplicationManager reference.\n"));
      throw std::runtime_error ("Argument is not a ReplicationManager reference.");
    }
}

RM_Proxy::~RM_Proxy (void)
{
}

CORBA::Object_ptr
RM_Proxy::obtain_RM_ior (CORBA::ORB_ptr orb)
{
  CORBA::Object_ptr retval = CORBA::Object::_nil ();
  
  std::pair<char, std::string> ior_access =
    HMOptions::instance ()->ior_access ();

  if (ior_access.first == 'i') /// file based IOR
    {
      retval = orb->string_to_object (ior_access.second.c_str());
      
      if (CORBA::is_nil (retval))
        {
          ACE_ERROR ((LM_ERROR,
                      "RM_Proxy: Null RM objref from OR file\n"));
        }
      else
        {
         // ACE_DEBUG ((LM_TRACE,
         //             "RM_Proxy: RM resolved from IOR file\n"));
        }
    }
  else if (ior_access.first == 'n')  /// Naming Service based IOR
    {
      TAO_Naming_Client naming_client;
      naming_client.init (orb);
      
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (ior_access.second.c_str());
      name[0].kind = CORBA::string_dup ("");

      retval =  naming_client->resolve (name);
      
      if (CORBA::is_nil (retval))
        {
          ACE_ERROR ((LM_ERROR,
                      "RM_Proxy: Null RM objref from Naming Service\n"));
        }
      else
        {
          //ACE_DEBUG ((LM_TRACE,
          //            "RM_Proxy: RM resolved from Naming Service\n"));
        }
    }
  else
    {
      retval =
        orb->string_to_object (HMOptions::instance ()->RM_ior ().c_str ());
      
      if (CORBA::is_nil (retval))
        {
          ACE_ERROR ((LM_ERROR,
                      "RM_Proxy: Null RM objref from IOR file\n"));
        }
      else
        {
          //ACE_DEBUG ((LM_TRACE,
          //            "RM_Proxy: RM resolved from IOR file\n"));
        }
    }
    
  return retval;
}

void
RM_Proxy::set_Utilization_Monitor (Utilization_Monitor *load_calc)
{
  this->load_calc_ = load_calc;
}

void
RM_Proxy::proc_failure (const std::string & process_id)
{
  try
    {
      if (CORBA::is_nil (RM_var_.in ()))
	      {
	        ACE_ERROR ((LM_ERROR,
	                    "RM_Proxy::proc_failure - "
		                  "RM reference is nil.\n"));
	        return;
	      }

      RM_var_->proc_failure (process_id.c_str());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "RM_Proxy::proc_failure: Exception caught:");
    }
}

int
RM_Proxy::pulse (void)
{
  //ACE_DEBUG((LM_ERROR,"host_id=%s\n",HMOptions::instance()->host_id().c_str()));
  //ACE_DEBUG((LM_ERROR,"load=%d\n", (int)load_calc_->percent_load()));
  RM_var_->util_update (HMOptions::instance ()->host_id ().c_str (), 
                        load_calc_->percent_load ());
  return 0;
}
