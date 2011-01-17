//$Id$

#include "Client_i.h"
#include "OBV_impl.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_time.h"

int
Checkpoint_Client_i::run (const char *name,
                          int argc,
                          ACE_TCHAR *argv[])
{
  // Initialize the client (read ior...).
  if (checkpoint.init (name,argc, argv) == -1)
    return -1;

  // Set random seed
  ACE_Time_Value now (ACE_OS::gettimeofday ());
  ACE_OS::srand ((unsigned int) now.sec () );


  try
    {
      ACE_DEBUG ((LM_DEBUG, "Send some random events:\n"));

      CORBA::Long value = 64;
      Event_var t_e (static_cast<Event*> (new Event_impl (value)));
      t_e->do_print ();
      checkpoint->put_event (t_e);

      value = 34;
      t_e = new Event_impl (value);
      t_e->do_print ();
      checkpoint->put_event (t_e);

      checkpoint->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("\n Exception in RMI");
      return -1;
    }

  return 0;
}

// Constructor.
Checkpoint_Client_i::Checkpoint_Client_i (void)
{
  //no-op
}

//Destructor.
Checkpoint_Client_i::~Checkpoint_Client_i (void)
{
  //no-op
}

