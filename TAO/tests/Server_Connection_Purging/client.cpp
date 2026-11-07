#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/SOCK_Connector.h"
#include "ace/TP_Reactor.h"
#include "ace/Reactor.h"
#include <array>
#include <string>

constexpr int iteration_count {55};
int purged_handles {};

struct Client_Options
{
  std::basic_string<ACE_TCHAR> host {ACE_TEXT ("localhost")};
  int port {10008};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("h:p:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'h':
        options.host = get_opts.opt_arg ();
        break;
      case 'p':
        options.port = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -h <host> -p <port>\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

class Purging_Handler : public ACE_Event_Handler
{
public:
  int handle_input (ACE_HANDLE h) override;
  int handle_close (ACE_HANDLE h, ACE_Reactor_Mask m) override;
};

int
Purging_Handler::handle_input (ACE_HANDLE )
{
  return -1;
}

int
Purging_Handler::handle_close (ACE_HANDLE h,
                               ACE_Reactor_Mask )
{
  if (purged_handles % 10 == 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) purging handle [%d]\n",
                h));

  ++purged_handles;

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  if (parse_args (argc, argv, options) == -1)
    return -1;

  ACE_Select_Reactor sr;

  ACE_Reactor reac (&sr);

  ACE_Reactor::instance (&reac);

  std::array<ACE_SOCK_Stream, iteration_count> streams {};
  std::array<ACE_SOCK_Connector, iteration_count> connectors {};
  std::array<Purging_Handler, iteration_count> handlers {};
  ACE_INET_Addr const addr (options.port, options.host.c_str ());

  ACE_Reactor *const singleton = ACE_Reactor::instance ();

  for (int i {}; i != iteration_count; ++i)
    {
      if (connectors[i].connect (streams[i], addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error while connecting: %p\n",
                           "client"),
                          -1);

      if (streams[i].get_handle () == ACE_INVALID_HANDLE)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Got invalid handles after connecting the [%d] time\n",
                           i),
                          -1);
      if (singleton->register_handler (streams[i].get_handle (),
                                       &handlers[i],
                                       ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Registration failed\n"),
                          -1);

      // ACE_Time_Value tv (1);

      // while (singleton->handle_events (&tv) >= 1);
    }

  // Handle events moved to here to prevent this test taking best part
  // of a minute
  ACE_Time_Value tv (3);

  while (singleton->handle_events (&tv) >= 1)
    {
      // No action.
    }

  // Remove the handlers to avoid the possibility of the reactor
  // using any of them after they leave the scope (those that haven't
  // been closed and removed already, that is).
  for (int j {}; j != iteration_count; ++j)
    {
      singleton->remove_handler (streams[j].get_handle (),
                                 ACE_Event_Handler::READ_MASK);
    }

  if ((iteration_count - purged_handles) > 20)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) Purging hasnt worked at all\n"), -1);

  return 0;
}
