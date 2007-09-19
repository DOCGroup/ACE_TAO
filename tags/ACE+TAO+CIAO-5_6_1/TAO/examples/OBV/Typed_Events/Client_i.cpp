//$Id$

#include "Client_i.h"
#include "Event_Types_impl.h"

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


  try
    {
      // Make factories to unmarshal OBV, when getting back a list
      // of events which raised an alarm condition.

      // TAO_OBV_REGISTER_FACTORY (Event_factory);
      // This one not (see header file)

      TAO_OBV_REGISTER_FACTORY (Temperature_factory, Temperature);
      TAO_OBV_REGISTER_FACTORY (Position_factory, Position);
      TAO_OBV_REGISTER_FACTORY (Log_Msg_factory, Log_Msg);
      TAO_OBV_REGISTER_FACTORY (Event_List_factory, Event_List);
      TAO_OBV_REGISTER_FACTORY (Event_List_Link_factory, Event_List_Link);

      ACE_DEBUG ((LM_DEBUG, "Send some random events:\n"));

      CORBA::Float temperature = random_number (29, 34);
      Temperature_var t_e (static_cast<Temperature*> (new Temperature_impl (temperature)));
      t_e->origin_id_ (KITCHEN);
      t_e->do_print ();
      checkpoint->put_event (t_e);

      temperature = random_number (25,30);
      t_e = new Temperature_impl (temperature);
      t_e->origin_id_ (BATHROOM);
      t_e->do_print ();
      checkpoint->put_event (t_e);

      Point point = { random_number (0,4),
                      random_number (0,4),
                      random_number (0,4)  };
      Position_var p_e (static_cast<Position*> (new Position_impl (point)));
      p_e->origin_id_ (JONAS);
      p_e->do_print ();
      checkpoint->put_event (p_e);


      int urgent = (random_number (0,2) > 1) ? 1 : 0;
      const char *a_text = (random_number (0,2) > 1) ? "Coffee is cold."
                                                     : "I want pasta.";
      Log_Msg_var l_e (static_cast<Log_Msg*> (new Log_Msg_impl (urgent, a_text)));
      l_e->origin_id_ (JONAS);
      l_e->do_print ();
      checkpoint->put_event (l_e);


      ACE_DEBUG ((LM_DEBUG, "\nNow getting the alarms:\n"));

      Event_List_var list (checkpoint->get_critical_events ());

      for (Event_List_Iterator i (list); i.next (); i.advance ())
        {
          i.next ()-> do_print ();
        }

      if (checkpoint.shutdown () == 1)
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


// A random number in the range of min to max.
CORBA::Float random_number (double min, double max)
{
  double range = max - min;
  return static_cast<CORBA::Float> ((min + (range * ACE_OS::rand () / (RAND_MAX + 1.0))));
}

