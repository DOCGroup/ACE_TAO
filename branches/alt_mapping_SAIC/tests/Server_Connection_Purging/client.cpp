// $Id$
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/SOCK_Connector.h"
#include "ace/TP_Reactor.h"
#include "ace/Reactor.h"

const ACE_TCHAR *host = ACE_TEXT("localhost");
static int port = 10008;
const int iter = 55;
int purged_handles = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("h:p:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        host = get_opts.opt_arg ();
        break;
      case 'p':
        port = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-h <host> "
                           "-p <port>"
                           "-i <iterations>"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

class Purging_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_input (ACE_HANDLE h);
  virtual int handle_close (ACE_HANDLE h,
                            ACE_Reactor_Mask m);
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) == -1)
    return -1;

  ACE_Select_Reactor sr;

  ACE_Reactor reac (&sr);

  ACE_Reactor::instance (&reac);

  ACE_SOCK_Stream stream[iter];

  ACE_SOCK_Connector connector[iter];

  Purging_Handler ph[iter];

  ACE_INET_Addr addr (port,
                      host);


  ACE_Reactor *singleton =
    ACE_Reactor::instance ();

  for (int i = 0; i != iter; ++i)
    {
      if (connector[i].connect (stream[i],
                                addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error while connecting: %p\n",
                           "client"),
                          -1);

      if (stream[i].get_handle () == ACE_INVALID_HANDLE)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Got invalid handles after connecting the [%d] time\n",i),
                          -1);
      if (singleton->register_handler (stream[i].get_handle (),
                                       &ph[i],
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
  for (int j = 0; j != iter; ++j)
    {
      singleton->remove_handler (stream[j].get_handle (),
                                 ACE_Event_Handler::READ_MASK);
    }

  if ((iter - purged_handles) > 20)
    ACE_ERROR_RETURN ((LM_ERROR,
                      "(%P|%t) Purging hasnt worked at all\n"),
                       -1);

  return 0;
}
