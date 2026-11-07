// Test the event server.

#include "ace/OS_main.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Stream.h"
#include "ace/Service_Config.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/UPIPE_Connector.h"
#include "ace/Truncate.h"
#include <algorithm>
#include <cstring>
#include <memory>

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "Options.h"
#include "Consumer_Router.h"
#include "Event_Analyzer.h"
#include "Supplier_Router.h"
#include "ace/Sig_Adapter.h"
#include "Message_Block_Owner.h"

#if defined (ACE_HAS_THREADS)

using MT_Stream = ACE_Stream<ACE_MT_SYNCH>;
using MT_Module = ACE_Module<ACE_MT_SYNCH>;

static void end_event_loop_handler (int, siginfo_t *, ucontext_t *)
{
  ACE_Reactor::end_event_loop ();
}

// Handle SIGINT and terminate the entire application.

class Quit_Handler : public ACE_Sig_Adapter
{
public:
  Quit_Handler ();
  Quit_Handler (Quit_Handler const &) = delete;
  Quit_Handler (Quit_Handler &&) = delete;
  Quit_Handler &operator= (Quit_Handler const &) = delete;
  Quit_Handler &operator= (Quit_Handler &&) = delete;
  int handle_input (ACE_HANDLE fd) override;
};

Quit_Handler::Quit_Handler ()
  : ACE_Sig_Adapter (end_event_loop_handler)
{
  // Register to trap input from the user.
  if (ACE_Event_Handler::register_stdin_handler (this,
                                                 ACE_Reactor::instance (),
                                                 ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_stdin_handler"));
  // Register to trap the SIGINT signal.
  else if (ACE_Reactor::instance ()->register_handler
           (SIGINT, this) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_handler"));
}

int
Quit_Handler::handle_input (ACE_HANDLE)
{
  options.stop_timer ();
  ACE_DEBUG ((LM_INFO, " (%t) closing down the test\n"));
  options.print_results ();

  ACE_Reactor::end_event_loop ();
  return 0;
}

static ACE_THR_FUNC_RETURN
consumer (void *)
{
  ACE_UPIPE_Stream c_stream;
  ACE_UPIPE_Addr c_addr (ACE_TEXT ("/tmp/conupipe"));

  bool const verb = options.verbose ();
  size_t const msiz = options.message_size ();
  time_t secs {};
  time_t par1 {};
  time_t par2 {};
  time_t currsec {};

  if (verb)
    cout << "consumer starting connect" << endl;

  ACE_UPIPE_Connector con;

  if (con.connect (c_stream, c_addr) == -1)
    ACE_DEBUG ((LM_INFO, " (%t) connect failed\n"));
  else
    cout << "consumer :we're connected" << endl;

  bool done {};
  size_t cnt {};
  ACE_OS::time (&currsec);

  par1 = currsec;

  ACE_Message_Block *mb_p {};
  while (!done && (c_stream.recv (mb_p) != -1))
    {
      UPIPE_Event_Server::Message_Block_Owner message {mb_p};
      if (mb_p->length () > 1)
        {
          cnt++;
          if (verb)
            {
              cout << " consumer received message !!!!!! "
                  << mb_p->rd_ptr () << endl;
            }
        }
      else
        {
          if (verb)
            {
              cout << "consumer got last mb"
                  << static_cast<char> (*mb_p->rd_ptr ()) << endl;
            }
          c_stream.close ();
          done = true;
        }
    }

  ACE_OS::time (&currsec);
  par2 = currsec;

  secs = par2 - par1;

  if (secs <= 0)
    secs = 1;

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("consumer got %B messages of size %B ")
              ACE_TEXT ("within %: seconds\n"),
              cnt, msiz, secs));

  ACE_OS::sleep (2);
  cout << "consumer terminating " << endl;
  return ACE_THR_FUNC_RETURN_NULL;
}

static ACE_THR_FUNC_RETURN
supplier (void *dummy)
{
  ACE_UPIPE_Stream s_stream;
  ACE_UPIPE_Addr serv_addr (ACE_TEXT ("/tmp/supupipe"));
  ACE_UPIPE_Connector con;

  size_t const iter = options.iterations ();
  bool const verb = options.verbose ();
  size_t const msiz = options.message_size ();
  cout << "supplier starting connect" << endl;

  if (con.connect (s_stream, serv_addr) == -1)
    ACE_DEBUG ((LM_INFO, " (%t) connect failed\n"));

  cout << "supplier : we're connected" << endl;
  size_t n {};
  char const * const message = static_cast<char const *> (dummy);

  while (n < iter)
    {
      ACE_Message_Block *mb_p {};
      ACE_NEW_RETURN (mb_p, ACE_Message_Block (msiz), ACE_THR_FUNC_RETURN_NULL);
      UPIPE_Event_Server::Message_Block_Owner message_block {mb_p};
      if (msiz != 0)
        {
          std::memset (mb_p->rd_ptr (), 0, msiz);
          size_t const message_length =
            std::min (std::strlen (message), msiz - 1);
          std::memcpy (mb_p->rd_ptr (), message, message_length);
        }
      mb_p->length (msiz);
      if (verb)
        cout << "supplier sending 1 message_block" << endl;
      if (s_stream.send (mb_p) == -1)
        {
          cout << "supplier send failed" << endl;
          return ACE_THR_FUNC_RETURN_NULL;
        }
      message_block.release ();
      n++;
    }

  ACE_Message_Block *mb_p {};
  ACE_NEW_RETURN (mb_p, ACE_Message_Block (10), ACE_THR_FUNC_RETURN_NULL);
  UPIPE_Event_Server::Message_Block_Owner last_message_block {mb_p};
  mb_p->length (1);
  *mb_p->rd_ptr () = 'g';

  cout << "supplier sending last message_block" << endl;

  if (s_stream.send (mb_p) == -1)
    {
      cout << "supplier send last mb failed" << endl;
      return ACE_THR_FUNC_RETURN_NULL;
    }
  last_message_block.release ();

  ACE_NEW_RETURN (mb_p, ACE_Message_Block (10), ACE_THR_FUNC_RETURN_NULL);
  UPIPE_Event_Server::Message_Block_Owner very_last_message_block {mb_p};
  mb_p->length (0);

  if (verb)
    cout << "supplier sending very last message_block" << endl;

  if (s_stream.send (mb_p) == -1)
    {
      cout << "supplier send very last mb failed" << endl;
      return ACE_THR_FUNC_RETURN_NULL;
    }
  very_last_message_block.release ();

  ACE_OS::sleep (2);
  cout << "supplier terminating" << endl;
  return ACE_THR_FUNC_RETURN_NULL;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config daemon;

  options.parse_args (argc, argv);
  options.start_timer ();

  // Primary ACE_Stream for EVENT_SERVER application.
  MT_Stream event_server;

  // Enable graceful shutdowns....
  Quit_Handler quit_handler;

  // Create the modules..

  std::unique_ptr<Supplier_Router> supplier_router =
    std::make_unique<Supplier_Router> (ACE_Thread_Manager::instance ());
  std::unique_ptr<MT_Module> supplier_module =
    std::make_unique<MT_Module> (ACE_TEXT ("Supplier_Router"), supplier_router.get ());
  // ACE_Module owns its tasks after construction.
  supplier_router.release ();

  std::unique_ptr<Event_Analyzer> analyzer_writer = std::make_unique<Event_Analyzer> ();
  std::unique_ptr<Event_Analyzer> analyzer_reader = std::make_unique<Event_Analyzer> ();
  std::unique_ptr<MT_Module> analyzer_module =
    std::make_unique<MT_Module> (ACE_TEXT ("Event_Analyzer"),
                                 analyzer_writer.get (), analyzer_reader.get ());
  analyzer_writer.release ();
  analyzer_reader.release ();

  std::unique_ptr<Consumer_Router> consumer_router =
    std::make_unique<Consumer_Router> (ACE_Thread_Manager::instance ());
  std::unique_ptr<MT_Module> consumer_module =
    std::make_unique<MT_Module> (ACE_TEXT ("Consumer_Router"), nullptr,
                                 consumer_router.get ());
  consumer_router.release ();

  // Push the modules onto the event_server stream.

  // ACE_Stream owns a module once push starts; it links before opening its tasks.
  MT_Module *supplier_module_ptr = supplier_module.release ();
  if (event_server.push (supplier_module_ptr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push (Supplier_Router)")), -1);

  MT_Module *analyzer_module_ptr = analyzer_module.release ();
  if (event_server.push (analyzer_module_ptr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push (Event_Analyzer)")), -1);

  MT_Module *consumer_module_ptr = consumer_module.release ();
  if (event_server.push (consumer_module_ptr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push (Consumer_Router)")), -1);

  // Set the high and low water marks appropriately.

  int wm = ACE_Utils::truncate_cast<int> (options.low_water_mark ());

  if (event_server.control (ACE_IO_Cntl_Msg::SET_LWM, &wm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push (setting low watermark)")), -1);

  wm = ACE_Utils::truncate_cast<int> (options.high_water_mark ());

  if (event_server.control (ACE_IO_Cntl_Msg::SET_HWM, &wm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push (setting high watermark)")), -1);

  // spawn the two threads.
  char supplier_message[] = "hello";

  if (ACE_Thread_Manager::instance ()->spawn (consumer, nullptr,
                                             THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn")), 1);

  else if (ACE_Thread_Manager::instance ()->spawn (supplier, supplier_message,
                                                  THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn")), 1);

  // Perform the main event loop waiting for the user to type ^C or to
  // enter a line on the ACE_STDIN.

  ACE_Reactor::instance ()->run_reactor_event_loop ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("main exiting\n")));

  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("test not defined for this platform\n")),
                    -1);
}
#endif /* ACE_HAS_THREADS */
