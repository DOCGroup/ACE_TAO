// $Id$

#include "Naming_Service.h"

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID(Naming_Service, Naming_Service, "$Id$")

// Default Constructor.

TAO_Naming_Service::TAO_Naming_Service (void)
  : time_ (0)
{
}

// Constructor taking command-line arguments.
TAO_Naming_Service::TAO_Naming_Service (int argc,
                                        ACE_TCHAR* argv[])
  : time_ (0)
{
  this->init (argc, argv);
}


// Initialize the state of the TAO_Naming_Service object
int
TAO_Naming_Service::init (int argc,
                          ACE_TCHAR* argv[])
{
  int result;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Copy command line parameter.
      ACE_Argv_Type_Converter command_line(argc, argv);

      // Initialize the ORB
      this->orb_ =
        CORBA::ORB_init (command_line.get_argc(), command_line.get_ASCII_argv(), 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse the args for '-t' option. If '-t' option is passed, do
      // the needful and then remove the option from the list of
      // arguments.
      this->parse_args (command_line.get_argc(), command_line.get_TCHAR_argv());

      // This function call initializes the naming service and returns
      // '-1' in case of an exception.
      result = this->my_naming_server_.init_with_orb (command_line.get_argc(),
                                                      command_line.get_TCHAR_argv(),
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
TAO_Naming_Service::parse_args (int &argc,
                                ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_LIB_TEXT("-t:"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 't':
          {
            int time = ACE_OS::atoi (get_opts.opt_arg ());
            if (time >= 0)
              this->time_ = time;

            // Remove the option '-t' from argv []
            // to avoid any confusion that might result.
            for (int i = get_opts.opt_ind (); i != argc; ++i)
              argv [i-2 ] = argv [i];

            // Decrement the value of argc to reflect the removal
            // of '-t' option.
            argc = argc - 2;
            break;
          }
        case '?':
        default:
          // Don't do anything. The TAO_Naming_Server::parse_args ()
          // takes care of indicating an error in case of error.
          break;
        }
    }
  return 0;
}

// Run the ORB event loop.
int
TAO_Naming_Service::run (ACE_ENV_SINGLE_ARG_DECL)
{
  if (time_ == 0)
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
  else
    {
      ACE_Time_Value tv (time_);
      this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  return 0;
}

// Destructor.
TAO_Naming_Service::~TAO_Naming_Service (void)
{
  // Destructor
}
