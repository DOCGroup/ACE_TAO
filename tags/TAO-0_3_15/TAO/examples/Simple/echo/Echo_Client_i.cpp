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
Echo_Client_i::run (char *name,
                    int argc,
                    char *argv[])
{
  // Initialize the client.
  if (client.init (name,argc, argv) == -1)
    return -1;

  ACE_TRY_NEW_ENV 
    {
      while (1)
        {
          char buf[BUFSIZ];    
          
          // Get the input message which has to be displayed.
          ACE_DEBUG ((LM_DEBUG,
                      "ECHO? "));
          
          if (ACE_OS::fgets (buf,sizeof buf, stdin) == 0)
            break;
          
          CORBA::String_var s = client->echo_string (buf,
                                                     ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG,
                      "\nString echoed by client \n%s\n",
                      s.in ()));
        }

      if (client.shutdown () == 1)
        client->shutdown (ACE_TRY_ENV);
      
      ACE_TRY_CHECK;
       
    }
  ACE_CATCHANY 
    {
      ACE_TRY_ENV.print_exception ("\n Exception in RMI");
    }
  ACE_ENDTRY;

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Client<Echo,Echo_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Client<Echo,Echo_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
