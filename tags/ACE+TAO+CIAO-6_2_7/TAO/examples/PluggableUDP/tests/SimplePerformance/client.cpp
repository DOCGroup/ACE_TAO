
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *  This is the client for the UDP simple performance test.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "testC.h"

// The following include file forces DIOP to be linked into the
// executable and initialized for static builds.
#include "tao/Strategies/advanced_resource.h"

#if defined (ACE_VXWORKS) && !defined (__RTP__)
# undef ACE_MAIN
# define ACE_MAIN testClient
#endif

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

ACE_UINT32 niter = 10;
ACE_UINT32 SIZE_BLOCK= 256;

/**
 * @class Client
 *
 * @brief Run the client thread
 *
 * Use the ACE_Task_Base class to run the client threads.
 */
class Client
{
public:
  /// ctor
  Client (Simple_Server_ptr server, ACE_UINT32 niterations);

  virtual ~Client (void) {};

  /// The thread entry point.
  virtual int svc (void);

private:
  /// The server.
  Simple_Server_var server_;

  /// The number of iterations on each client thread.
  ACE_UINT32 niterations_;
};


//int testClient (char* orbName, char* ior)
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
          CORBA::ORB_init (argc,
                           argv,
                           "ORB_Test_Client");

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      CORBA::String_var string =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Client: orb->object_to_string: <%C>\n",
                  string.in ()));

      Client client (server.in (), niter);

      client.svc ();

      //ACE_DEBUG ((LM_DEBUG, "threads finished\n"));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

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
  try
    {
      Octet_Seq octetSeq(SIZE_BLOCK);
      Char_Seq charSeq(SIZE_BLOCK);
      ACE_High_Res_Timer timer;
      ACE_OS::printf("Start sending %d Msgs...\n",this->niterations_);

      charSeq.length(SIZE_BLOCK);
      octetSeq.length(SIZE_BLOCK);

      // This sets up the connector, so that we do not incur
      // the overhead on the first call in the loop.
      server_->sendCharSeq (charSeq);

      timer.start ();

      ACE_UINT32 client_count = 0;
      for (ACE_UINT32 i = 0; i < this->niterations_; ++i)
        {
          client_count++;

          server_->sendCharSeq (charSeq);

          //server_->sendOctetSeq (octetSeq);

          //ACE_DEBUG ((LM_DEBUG, "."));
        }
      timer.stop ();

      ACE_Time_Value measured;
      timer.elapsed_time (measured);

      //ACE_DEBUG ((LM_DEBUG, "...finished\n"));

      time_t dur = measured.sec () * 1000000 + measured.usec ();
      if (dur == 0 || this->niterations_ == 0)
        ACE_DEBUG ((LM_DEBUG, "Time not measurable, calculation skipped\n"));
      else
      {
        ACE_DEBUG ((LM_DEBUG,
                    "Time for %u Msgs: %u usec\n",
                    this->niterations_,
                    dur));

        ACE_DEBUG ((LM_DEBUG, "Time for 1 Msg: %u usec, %u calls/sec\n",
                    dur / this->niterations_,
                    1000000 / (dur / this->niterations_)));
      }

      for (int c = 0; c < 10; ++c)
        server_->shutdown ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
    }
  return 0;
}
