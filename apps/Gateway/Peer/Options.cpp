#include "ace/Get_Opt.h"
#include "Options.h"

// Static initialization.
Options *Options::instance_ = 0;

void
Options::print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG, "%n [-acv] [-p port] [-q max-queue-size] [-t timeout]\n"));
}

Options::Options (void)
{
}

Options *
Options::instance (void)
{
  if (Options::instance_ == 0)
    ACE_NEW_RETURN (Options::instance_, Options, 0);

  return Options::instance_;
}

long
Options::timeout (void) const
{
  return this->timeout_;
}

long
Options::max_queue_size (void) const
{
  return this->max_queue_size_;
}

u_short
Options::port (void) const
{
  return this->port_;
}

int
Options::enabled (int option) const
{
  return ACE_BIT_ENABLED (this->options_, option);
}

void
Options::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "acp:q:t:v", 0);

  char *to = ACE_OS::getenv ("TIMEOUT");

  if (to == 0) 
    this->timeout_ = Options::DEFAULT_TIMEOUT;
  else
    this->timeout_ = ACE_OS::atoi (to);

  this->port_ = ACE_DEFAULT_PEER_SERVER_PORT;

  ACE_SET_BITS (this->options_, Options::ACCEPTOR);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
        case 'a':
          // Become an Acceptor (this is the default behavior, so this
          // option is redundant).
          ACE_SET_BITS (this->options_, Options::ACCEPTOR);
          break;
          /* NOTREACHED */
        case 'c':
          // Become a Connector.
          ACE_SET_BITS (this->options_, Options::CONNECTOR);
          break;
          /* NOTREACHED */
	case 'p':
          // Set the port number.
	  this->port_ = ACE_OS::atoi (get_opt.optarg);
	  break;
          /* NOTREACHED */
	case 'q':
          // max queue size.
	  this->max_queue_size_ = ACE_OS::atoi (get_opt.optarg);
	  break;
          /* NOTREACHED */
	case 't':
          // Timeout
	  this->timeout_ = ACE_OS::atoi (get_opt.optarg);
	  break;
          /* NOTREACHED */
	case 'v':
          // Verbose mode.
	  ACE_SET_BITS (this->options_, Options::VERBOSE);
	  break;
          /* NOTREACHED */
	default:
          this->print_usage_and_die ();
          /* NOTREACHED */
	}
    }
}

