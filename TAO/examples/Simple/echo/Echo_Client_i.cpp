//$Id$

#include "Echo_Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

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

  TAO_ENV_DECLARE_NEW_ENV;

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
                                                     TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG,
                      "\nString echoed by client \n%s\n",
                      s.in ()));
        }

      if (client.shutdown () == 1)
        client->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);

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


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Client<Echo,Echo_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Client<Echo,Echo_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
