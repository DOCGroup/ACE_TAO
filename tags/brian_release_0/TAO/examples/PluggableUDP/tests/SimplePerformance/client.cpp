// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/PluggableUDP/tests/SimplePerformance
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//    This is the client for the UDP simple performance test.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "testC.h"

// The following include file forces DIOP to be linked into the
// executable and initialized for static builds.
#include "tao/Strategies/advanced_resource.h"

#if defined (VXWORKS)
# undef ACE_MAIN
# define ACE_MAIN testClient
#endif

const char *iorFile = "file://test.ior";
ACE_UINT32 niter = 10;
ACE_UINT32 SIZE_BLOCK= 256;

class Client
{
  // = TITLE
  //   Run the client thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run the client threads.
  //
public:
  Client (Simple_Server_ptr server, ACE_UINT32 niterations);
  // ctor

  virtual ~Client (void) {};

  virtual int svc (void);
  // The thread entry point.

private:
  Simple_Server_var server_;
  // The server.

  ACE_UINT32 niterations_;
  // The number of iterations on each client thread.
};


//int testClient (char* orbName, char* iorFile)
int main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
          CORBA::ORB_init (argc,
                           argv,
                           "ORB_Test_Client"
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->string_to_object (iorFile ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Simple_Server_var server =
        Simple_Server::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             iorFile),
                            1);
        }

      CORBA::String_var string =
        orb->object_to_string (server.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Client: orb->object_to_string:\n%s\n",
                  string.in ()));

      Client client (server.in (), niter);

      client.svc ();

      //ACE_DEBUG ((LM_DEBUG, "threads finished\n"));

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}


// ****************************************************************

Client::Client (Simple_Server_ptr server,
                ACE_UINT32 niterations)
  :  server_ (Simple_Server::_duplicate (server)),
     niterations_ (niterations)
{
}


int
Client::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      Octet_Seq octetSeq(SIZE_BLOCK);
      Char_Seq charSeq(SIZE_BLOCK);
      ACE_High_Res_Timer timer;
      printf("Start sending %d Msgs...\n",this->niterations_);

      charSeq.length(SIZE_BLOCK);
      octetSeq.length(SIZE_BLOCK);

      // This sets up the connector, so that we do not incur
      // the overhead on the first call in the loop.
      server_->sendCharSeq (charSeq ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      timer.start ();

      ACE_UINT32 client_count = 0;
      for (ACE_UINT32 i = 0; i < this->niterations_; ++i)
        {
          client_count++;

          server_->sendCharSeq (charSeq ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          //server_->sendOctetSeq (octetSeq ACE_ENV_ARG_PARAMETER);

          //ACE_DEBUG ((LM_DEBUG, "."));
        }
      timer.stop ();

      ACE_Time_Value measured;
      timer.elapsed_time (measured);

      //ACE_DEBUG ((LM_DEBUG, "...finished\n"));

      long dur = measured.sec () * 1000000 + measured.usec ();
      ACE_DEBUG ((LM_DEBUG,
                  "Time for %u Msgs: %u usec \n",
                  this->niterations_,
                  measured.sec () * 1000000 + measured.usec ()));

      ACE_DEBUG ((LM_DEBUG, "Time for 1 Msg: %u usec, %u calls/sec\n",
                  dur / this->niterations_,
                  1000000 / (dur / this->niterations_)));

      server_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "MT_Client: exception raised");
    }
  ACE_ENDTRY;
  return 0;
}

