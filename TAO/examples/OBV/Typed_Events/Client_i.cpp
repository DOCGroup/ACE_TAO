//$Id$

#include "Client_i.h"
#include "Event_Types_impl.h"
#include "ace/Get_Opt.h"


int
Checkpoint_Client_i::run (char *name,
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
      // Make factories to unmarshal OBV, when getting back a list
      // of events which raised an alarm condition.

      // TAO_OBV_REGISTER_FACTORY (Event_factory);
      // This one not (see header file)

      TAO_OBV_REGISTER_FACTORY (Temperature_factory);
      TAO_OBV_REGISTER_FACTORY (Position_factory);
      TAO_OBV_REGISTER_FACTORY (Log_Msg_factory);
      TAO_OBV_REGISTER_FACTORY (Event_List_factory);
      TAO_OBV_REGISTER_FACTORY (Event_List_Link_factory);

      ACE_DEBUG ((LM_DEBUG, "Send some random events:\n"));

      double temperature = random_number (29,34);
      Temperature_var t_e (new Temperature_impl (temperature));
      t_e->origin_id_ (KITCHEN);
      t_e->do_print ();
      checkpoint->put_event (t_e, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      temperature = random_number (25,30);
      t_e = new Temperature_impl (temperature);
      t_e->origin_id_ (BATHROOM);
      t_e->do_print ();
      checkpoint->put_event (t_e, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Point point = { random_number (0,4),
                      random_number (0,4),
                      random_number (0,4)  };
      Position_var p_e (new Position_impl (point));
      p_e->origin_id_ (JONAS);
      p_e->do_print ();
      checkpoint->put_event (p_e, ACE_TRY_ENV);
      ACE_TRY_CHECK;


      int urgent = (random_number (0,2) > 1) ? 1 : 0;
      const char *a_text = (random_number (0,2) > 1) ? "Coffee is cold."
                                                     : "I want pasta.";
      Log_Msg_var l_e (new Log_Msg_impl (urgent, a_text));
      l_e->origin_id_ (JONAS);
      l_e->do_print ();
      checkpoint->put_event (l_e, ACE_TRY_ENV);
      ACE_TRY_CHECK;


      ACE_DEBUG ((LM_DEBUG, "\nNow getting the alarms:\n"));

      Event_List_var list (checkpoint->get_critical_events (ACE_TRY_ENV));
      ACE_TRY_CHECK;

      for (Event_List_Iterator i (list); i.next (); i.advance ())
        {
          i.next ()-> do_print ();
        }

      if (checkpoint.shutdown () == 1)
        checkpoint->shutdown (ACE_TRY_ENV);

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"\n Exception in RMI");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

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


// A random number in the range of min to max.
double random_number (double min, double max)
{
  double range = max - min;
  return (min + (range * ACE_OS::rand () / (RAND_MAX + 1.0)));
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Client<Checkpoint,Checkpoint_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Client<Checkpoint,Checkpoint_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
