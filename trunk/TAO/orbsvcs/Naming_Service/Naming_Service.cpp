// $Id$

#include "ace/Get_Opt.h"
#include "Naming_Service.h"

ACE_RCSID(Naming_Service, Naming_Service, "$Id$")

// Default Constructor.

TAO_Naming_Service::TAO_Naming_Service (void)
  : time_ (0)
{
}

// Constructor taking command-line arguments.
TAO_Naming_Service::TAO_Naming_Service (int argc,
                                        char* argv[])
  : time_ (0)
{
  this->init (argc, argv);
}


// Initialize the state of the TAO_Naming_Service object
int
TAO_Naming_Service::init (int argc,
                          char *argv[])
{
  int result;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->argc_ = argc;
      this->argv_ = argv;

      // Initialize the ORB
      this->orb_ =
        CORBA::ORB_init (this->argc_, this->argv_, 0, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Parse the args for '-t' option. If '-t' option is passed, do
      // the needful and then remove the option from the list of
      // arguments.
      this->parse_args (this->argc_, this->argv_);

      // This function call initializes the naming service and returns
      // '-1' in case of an exception.
      result = this->my_naming_server_.init_with_orb (this->argc_,
                                                      this->argv_,
                                                      this->orb_.in ());

      if (result == -1)
        return result;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Naming_Service::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
TAO_Naming_Service::parse_args (int argc,
                                char *argv [])
{
  ACE_Get_Opt get_opts (argc, argv, "b:do:p:s:t:f:m:");
  int c;
  int time = 0;
  int i = 0;
  int count_argv = 0;

  while ((c = get_opts ()) != -1)
    {
      ++count_argv;
      switch (c)
        {
        case 't':
          time = ACE_OS::atoi (get_opts.optarg);
          if (time >= 0)
            this->time_ = time;

          // Remove the option '-t' from argv []
          // to avoid any confusion that might result.
          {
            // Added unneeded '{ & }' just to satisfy Win32
            for (i = count_argv; i != argc; ++i)
              argv [i] = argv [i+2];
          }

          // Decrement the value of this->argc_ to reflect the removal
          // of '-t' option.
          argc = argc-2;
          break;

        case '?':
        default:
          // Donot do anything. The TAO_Naming_Server::parse_args ()
          // takes care of indicating an error in case of error.
          break;
        }
    }
  return 0;
}

// Run the ORB event loop.
int
TAO_Naming_Service::run (CORBA_Environment& ACE_TRY_ENV)
{
  if (time_ == 0)
    {
      this->orb_->run (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
    }
  else
    {
      ACE_Time_Value tv (time_);
      this->orb_->run (tv, ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
    }

  return 0;
}

// Destructor.
TAO_Naming_Service::~TAO_Naming_Service (void)
{
  // Destructor
}
