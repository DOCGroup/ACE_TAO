#include "ace/Get_Opt.h"

void
Options::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "dp:v", 0);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'p':
	  this->addr_.set (ACE_OS::atoi (get_opt.optarg));
	  break;
	case 'd':
	  break;
	case 'v': // Verbose mode.
	  verbose = 1;
	  break;
	default:
	  break;
	}
    }
}

