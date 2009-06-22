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
                    ACE_TCHAR *argv[])
{
  // Initialize the client.
  if (client_.init (name, argc, argv) == -1)
    return -1;

  try
    {
      while (1)
        {
          char buf[BUFSIZ];

          // Get the input message which has to be displayed.
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("ECHO? ")));

          if (ACE_OS::fgets (buf, sizeof buf, stdin) == 0)
            break;

          CORBA::String_var s = client_->echo_string (buf);

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\nString echoed by client \n%C\n"),
                      s.in ()));
        }

      if (client_.do_shutdown () == 1)
        client_->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("\n Exception in RMI");
      return -1;
    }

  return 0;
}
