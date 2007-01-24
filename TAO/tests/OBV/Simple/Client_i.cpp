//$Id$

#include "Client_i.h"
#include "OBV_impl.h"

#include "tao/ORB_Core.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_time.h"

int
Checkpoint_Client_i::run (const char *name,
                          int argc,
                          char *argv[])
{
  // Initialize the client (read ior...).
  if (checkpoint.init (name,argc, argv) == -1)
    return -1;

  // Set random seed
  ACE_Time_Value now (ACE_OS::gettimeofday ());
  ACE_OS::srand ((unsigned int) now.sec () );

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG, "Send some random events:\n"));

      CORBA::Long value = 64;
      Event_var t_e (static_cast<Event*> (new Event_impl (value)));
      t_e->do_print ();
      checkpoint->put_event (t_e ACE_ENV_ARG_PARAMETER);

      value = 34;
      t_e = new Event_impl (value);
      t_e->do_print ();
      checkpoint->put_event (t_e ACE_ENV_ARG_PARAMETER);

      if (checkpoint.shutdown () == 1)
        checkpoint->shutdown ();

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"\n Exception in RMI");
      return -1;
    }
  ACE_ENDTRY;

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

