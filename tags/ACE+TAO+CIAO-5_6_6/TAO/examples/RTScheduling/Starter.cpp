//$Id$

#include "Starter.h"

#include "ace/OS_NS_sys_time.h"

Starter::Starter (CORBA::ORB_ptr orb)
{
  // Initialize the naming service
  if (this->naming_client_.init (orb) != 0)
    ACE_ERROR ((LM_ERROR,
    " (%P|%t) Unable to initialize "
    "the TAO_Naming_Client. \n"));
}

void
Starter::init (void)
{
  this->resolve_synch_objs ();

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
      (*iterator).int_id_.in ()->go (static_cast<CORBA::Long> (base_time.sec ()));
    }
}

void
Starter::resolve_synch_objs (void)
{
  CosNaming::Name name (1);
  name.length (1);

  // Get the sender context.
  name [0].id =
    CORBA::string_dup ("Synch");

  CORBA::Object_var object =
    this->naming_client_->resolve (name);

  this->synch_context_ =
    CosNaming::NamingContext::_narrow (object.in ());


  CosNaming::BindingIterator_var iterator;
  CosNaming::BindingList_var binding_list;
  const CORBA::ULong chunk = 100;

  // Get the list of synchs registered for this sender.
  this->synch_context_->list (chunk,
            binding_list,
            iterator);

  // Add the receivers found in the bindinglist to the <receivers>.
  this->add_to_synchs (binding_list);

  if (!CORBA::is_nil (iterator.in ()))
    {
      CORBA::Boolean more = 1;

      // Check to see if there are more receivers listed.
      while (more)
        {
          more = iterator->next_n (chunk,
                                   binding_list);

          this->add_to_synchs (binding_list);
        }
    }

}


void
Starter::add_to_synchs (CosNaming::BindingList &binding_list)
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
        this->synch_context_->resolve (name);

      Synch_var synch_obj =
        Synch::_narrow (obj.in ());


      synch_obj->go (static_cast<CORBA::Long> (base_time.sec ()));

//        // Add this receiver to the receiver map.
//        this->synchs_.bind (synch_name,
//          synch_obj);
    }
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      Starter starter (orb.in ());

      starter.init ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
return 0;
}

