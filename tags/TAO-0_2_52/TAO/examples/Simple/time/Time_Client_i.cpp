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
Time_Client_i::run (int argc,
                    char *argv[])
{
  // Initialize the client.
  if (client.init (argc, argv) == -1)
    return -1;

  TAO_TRY 
    {
      //Make the RMI.
      CORBA::Long timedate = client->time (TAO_TRY_ENV);
      
      // Print out value
      char *ascii_timedate =
        ACE_OS::ctime (ACE_reinterpret_cast (time_t *,
                                             &timedate));
      ACE_DEBUG ((LM_DEBUG,
                  "string time is %s\n",
                  ascii_timedate));

      if (client.shutdown () == 1)
        client->shutdown (TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("\tException");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}


