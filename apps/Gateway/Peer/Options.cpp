#include "ace/Get_Opt.h"

void
Options::print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG, "%n [-p port] [-t timeout] [-v]\n"));
}

long
Options::timeout (void) const
{
  return this->timeout_;
}

u_short
Options::port (void) const
{
  return this->port_;
}

u_short
Options::verbose (void) const
{
  return this->verbose_;
}

void
Options::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "p:t:v", 0);

  char *to = ACE_OS::getenv ("TIMEOUT");

  if (to == 0) 
    this->timeout_ = Options::DEFAULT_TIMEOUT;
  else
    this->timeout_ = ACE_OS::atoi (to);

  this->port_ = ACE_DEFAULT_PEER_SERVER_PORT;
  this->verbose_ = 0;

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'p':
	  this->port_ = ACE_OS::atoi (get_opt.optarg);
	  break;
          /* NOTREACHED */
	case 't':
          // Timeout
	  this->timeout_ = ACE_OS::atoi (get_opt.optarg);
	  break;
          /* NOTREACHED */
	case 'v':
          // Verbose mode.
	  this->verbose_ = 1;
	  break;
          /* NOTREACHED */
	default:
          this->print_usage_and_die ();
          /* NOTREACHED */
	}
    }
}

