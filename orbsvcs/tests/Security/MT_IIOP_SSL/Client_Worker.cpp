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
Client_Worker::validate_connection (void)
{
  // Ping the object 100 times, ignoring all exceptions.
  // It would be better to use validate_connection() but the test must
  // run on minimum CORBA builds too!

  for (int j = 0; j != 100; ++j)
   {
      try
        {
          this->server_->ping ();

          if(TAO_debug_level > 0)
            ACE_DEBUG (( LM_DEBUG,
                         "******** VALIDATED *******\n"));
        }
      catch (const CORBA::Exception&)
        {
        }
   }
}

int
Client_Worker::svc (void)
{

  try
    {
      // Validate connections befire doing any actual work..
      this->validate_connection ();

       this->server_->validate_protocol ();

       for (int i = 0; i < this->niterations_; ++i)
         {
           this->server_->test_method (i);

           if (TAO_debug_level > 0)
             ACE_DEBUG ((LM_DEBUG,
                         " (%P|%t) iteration = %d\n",
                         i));
         }
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Got an exception\n"));
      ex._tao_print_exception ("Client_Worker : Exception Raised");

      return -1;
    }
  return 0;
}
