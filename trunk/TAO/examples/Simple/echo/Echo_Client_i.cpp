//$Id$

#include "Echo_Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_unistd.h"

// This is the interface program that accesses the remote object

// Constructor.
Echo_Client_i::Echo_Client_i (void)
{
  //no-op
}

//Destructor.
Echo_Client_i::~Echo_Client_i (void)
{
  //no-op
}

int
Echo_Client_i::run (const char *name,
                    int argc,
                    char *argv[])
{
  // Initialize the client.
  if (client.init (name,argc, argv) == -1)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      while (1)
        {
          char buf[BUFSIZ];

          // Get the input message which has to be displayed.
          ACE_DEBUG ((LM_DEBUG,
                      "ECHO? "));

          if (ACE_OS::fgets (buf,sizeof buf, stdin) == 0)
            break;

          CORBA::String_var s = client->echo_string (buf
                                                     ACE_ENV_ARG_PARAMETER);

          ACE_DEBUG ((LM_DEBUG,
                      "\nString echoed by client \n%s\n",
                      s.in ()));
        }

      if (client.shutdown () == 1)
        client->shutdown ();


    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"\n Exception in RMI");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

