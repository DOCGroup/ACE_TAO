//$Id$

#include "Time_Client_i.h"

// This is the interface program that accesses the remote object

// Constructor.
Time_Client_i::Time_Client_i (void)
{
  //no-op
}

//Destructor.
Time_Client_i::~Time_Client_i (void)
{
  //no-op
}

int
Time_Client_i::run (const char *name,
                    int argc,
                    char *argv[])
{
  // Initialize the client.
  if (client.init (name, argc, argv) == -1)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      //Make the RMI.
      CORBA::Long timedate = client->current_time (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      // Print out value
      char *ascii_timedate =
        ACE_OS::ctime (ACE_reinterpret_cast (time_t *,
                                             &timedate));
      ACE_DEBUG ((LM_DEBUG,
                  "string time is %s\n",
                  ascii_timedate));

      if (client.shutdown () == 1)
        client->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "\tException");
      return -1;
    }
  ACE_ENDTRY;
  

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class Client<Time,Time_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate Client<Time,Time_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
