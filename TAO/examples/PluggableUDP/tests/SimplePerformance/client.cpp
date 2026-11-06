
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  This is the client for the UDP simple performance test.
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "testC.h"

// The following include file forces DIOP to be linked into the
// executable and initialized for static builds.
#include "tao/Strategies/advanced_resource.h"

#if defined (ACE_VXWORKS) && !defined (__RTP__)
# undef ACE_MAIN
# define ACE_MAIN testClient
#endif

struct Client_Options
{
  ACE_TCHAR const *ior { ACE_TEXT ("file://test.ior") };
};

static int
parse_args (Client_Options &options, int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

constexpr ACE_UINT32 niter { 10 };
constexpr ACE_UINT32 SIZE_BLOCK { 256 };

/**
 * @class Client
 *
 * @brief Run the client benchmark.
 */
class Client
{
public:
  /// ctor
  Client (Simple_Server_ptr server, ACE_UINT32 niterations);

  Client (Client const &) = delete;
  Client &operator= (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client &&) = delete;

  ~Client () = default;

  /// The thread entry point.
  int svc ();

private:
  /// The server.
  Simple_Server_var server_ {};

  /// The number of iterations on each client thread.
  ACE_UINT32 niterations_ {};
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
          CORBA::ORB_init (argc,
                           argv,
                           "ORB_Test_Client");

      Client_Options options;
      if (parse_args (options, argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (options.ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.ior),
                            1);
        }

      CORBA::String_var string =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Client: orb->object_to_string: <%C>\n",
                  string.in ()));

      Client client (server.in (), niter);

      client.svc ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
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
Client::svc ()
{
  try
    {
      Char_Seq charSeq(SIZE_BLOCK);
      ACE_High_Res_Timer timer;
      ACE_OS::printf ("Start sending %u Msgs...\n", this->niterations_);

      charSeq.length (SIZE_BLOCK);

      // This sets up the connector, so that we do not incur
      // the overhead on the first call in the loop.
      server_->sendCharSeq (charSeq);

      timer.start ();

      for (ACE_UINT32 i = 0; i < this->niterations_; ++i)
        {
          server_->sendCharSeq (charSeq);

        }
      timer.stop ();

      ACE_Time_Value measured {};
      timer.elapsed_time (measured);

      ACE_INT64 const duration =
        static_cast<ACE_INT64> (measured.sec ()) * 1000000 + measured.usec ();
      if (duration == 0 || this->niterations_ == 0)
        ACE_DEBUG ((LM_DEBUG, "Time not measurable, calculation skipped\n"));
      else
        {
          ACE_INT64 const usec_per_call = duration / this->niterations_;
          ACE_INT64 const calls_per_second =
            usec_per_call == 0 ? 0 : 1000000 / usec_per_call;
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Time for %u Msgs: %q usec\n"),
                      this->niterations_,
                      duration));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Time for 1 Msg: %q usec, %q calls/sec\n"),
                      usec_per_call,
                      calls_per_second));
        }

      for (int count = 0; count < 10; ++count)
        server_->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("MT_Client: exception raised");
    }
  return 0;
}
