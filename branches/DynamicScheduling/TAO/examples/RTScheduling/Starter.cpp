//$Id$

#include "Starter.h"


Starter::Starter (CORBA::ORB_ptr orb)
{
  // Initialize the naming service
  if (this->naming_client_.init (orb) != 0)
    ACE_ERROR ((LM_ERROR,
		" (%P|%t) Unable to initialize "
		"the TAO_Naming_Client. \n"));
}

void
Starter::init (ACE_ENV_SINGLE_ARG_DECL)
{
  this->resolve_synch_objs (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->fire ();
}

void
Starter::fire (void)
{
  ACE_Time_Value base_time = ACE_OS::gettimeofday ();
  for (Synchs::iterator iterator = this->synchs_.begin ();
       iterator != this->synchs_.end ();
       ++iterator)
    {
      (*iterator).int_id_.in ()->go (base_time.sec ());
    }
}

void
Starter::resolve_synch_objs (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNaming::Name name (1);
  name.length (1);

  // Get the sender context.
  name [0].id =
    CORBA::string_dup ("Synch");

  CORBA::Object_var object =
    this->naming_client_->resolve (name
				   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->synch_context_ =
    CosNaming::NamingContext::_narrow (object.in ());


  CosNaming::BindingIterator_var iterator;
  CosNaming::BindingList_var binding_list;
  const CORBA::ULong chunk = 100;

  // Get the list of synchs registered for this sender.
  this->synch_context_->list (chunk,
			      binding_list,
			      iterator
			      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Add the receivers found in the bindinglist to the <receivers>.
  this->add_to_synchs (binding_list
		       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!CORBA::is_nil (iterator.in ()))
    {
      CORBA::Boolean more = 1;

      // Check to see if there are more receivers listed.
      while (more)
        {
          more = iterator->next_n (chunk,
                                   binding_list
                                   ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          this->add_to_synchs (binding_list
			       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

}


void
Starter::add_to_synchs (CosNaming::BindingList &binding_list
			ACE_ENV_ARG_DECL)
{
  ACE_Time_Value base_time = ACE_OS::gettimeofday ();
  for (CORBA::ULong i = 0;
       i < binding_list.length ();
       i++)
    {
      // Get the receiver name from the binding list.
      ACE_CString synch_name =
        binding_list [i].binding_name [0].id.in ();

      ACE_DEBUG ((LM_DEBUG,
		  "Synch Name %s\n",
		  synch_name.c_str ()));

      CosNaming::Name name (1);
      name.length (1);
      name [0].id =
        CORBA::string_dup (synch_name.c_str ());

      // Resolve the reference of the receiver from the receiver
      // context.
      CORBA::Object_var obj =
        this->synch_context_->resolve (name
				       ACE_ENV_ARG_PARAMETER);

      Synch_var synch_obj =
        Synch::_narrow (obj.in ());


      synch_obj->go (base_time.sec ());

//        // Add this receiver to the receiver map.
//        this->synchs_.bind (synch_name,
//  			  synch_obj);
    }
}


int
main (int argc, char** argv)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
					    argv,
					    ""
					    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Starter starter (orb.in ());

      starter.init (ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;
return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_CString, Synch_var>;
template class ACE_Hash_Map_Manager<ACE_CString, Synch_var, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, Synch_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, Synch_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, Synch_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, Synch_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, Synch_var>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, Synch_var, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, Synch_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, Synch_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, Synch_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, Synch_var, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>


#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
