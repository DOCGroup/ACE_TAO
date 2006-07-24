// $Id$

#include "Client_Worker.h"
#include "tao/debug.h"

Client_Worker::Client_Worker (Simple_Server_ptr server,
                              int niterations)
  : server_ (Simple_Server::_duplicate(server)),
    niterations_ (niterations)
{
}

void
Client_Worker::validate_connection (ACE_ENV_SINGLE_ARG_DECL)
{
  // Ping the object 100 times, ignoring all exceptions.
  // It would be better to use validate_connection() but the test must
  // run on minimum CORBA builds too!

  for (int j = 0; j != 100; ++j)
   {
      ACE_TRY
        {
          this->server_->ping (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if(TAO_debug_level > 0)
            ACE_DEBUG (( LM_DEBUG,
                         "******** VALIDATED ******* \n"));
        }
      ACE_CATCHANY
        {
        }
      ACE_ENDTRY;
   }
}

int
Client_Worker::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Validate connections befire doing any actual work..
      this->validate_connection (ACE_ENV_SINGLE_ARG_PARAMETER);
       ACE_TRY_CHECK;

       this->server_->validate_protocol (ACE_ENV_SINGLE_ARG_PARAMETER);
       ACE_TRY_CHECK;

       for (int i = 0; i < this->niterations_; ++i)
         {
           this->server_->test_method (i ACE_ENV_ARG_PARAMETER);
           ACE_TRY_CHECK;

           if (TAO_debug_level > 0)
             ACE_DEBUG ((LM_DEBUG,
                         " (%P|%t) iteration = %d\n",
                         i));
         }
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Got an exception \n"));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client_Worker : Exception Raised");

      return -1;
    }
  ACE_ENDTRY;
  return 0;
}
