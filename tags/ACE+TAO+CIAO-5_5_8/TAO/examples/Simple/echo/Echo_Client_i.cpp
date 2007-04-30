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


  try
    {
      while (1)
        {
          char buf[BUFSIZ];

          // Get the input message which has to be displayed.
          ACE_DEBUG ((LM_DEBUG,
                      "ECHO? "));

          if (ACE_OS::fgets (buf,sizeof buf, stdin) == 0)
            break;

          CORBA::String_var s = client->echo_string (buf);

          ACE_DEBUG ((LM_DEBUG,
                      "\nString echoed by client \n%s\n",
                      s.in ()));
        }

      if (client.shutdown () == 1)
        client->shutdown ();


    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("\n Exception in RMI");
      return -1;
    }

  return 0;
}

