// $Id$

#include "Naming_Service.h"

#include "orbsvcs/Naming/Naming_Server.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Daemon_Utilities.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Task.h"

// Default Constructor.
TAO_Naming_Service::TAO_Naming_Service (void)
  : time_ (0),
    num_threads_ (1)
{
}

// Constructor taking command-line arguments.
TAO_Naming_Service::TAO_Naming_Service (int argc, ACE_TCHAR* argv[])
  : time_ (0),
    num_threads_ (1)
{
  this->init (argc, argv);
}


// Initialize the state of the TAO_Naming_Service object
int
TAO_Naming_Service::init (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // Check if -ORBDaemon is specified and if so, daemonize at this moment,
      // -ORBDaemon in the ORB core is faulty, see bugzilla 3335
      TAO_Daemon_Utility::check_for_daemon (argc, argv);

      // Initialize the ORB
      this->orb_ = CORBA::ORB_init (argc, argv);

      // Parse the args for '-t' option. If '-t' option is passed, do
      // the needful and then remove the option from the list of
      // arguments.
      this->parse_args (argc, argv);

      // This function call initializes the naming service and returns
      // '-1' in case of an exception.
      int const result = this->my_naming_server_.init_with_orb (argc,
                                                                argv,
                                                                this->orb_.in ());

      if (result == -1)
        return result;

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_Naming_Service::init");
      return -1;
    }

  return 0;
}

int
TAO_Naming_Service::parse_args (int &argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("-t:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 't':
          {
            int const time = ACE_OS::atoi (get_opts.opt_arg ());
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
        case 'n':
          {
            int const nt = ACE_OS::atoi (get_opts.opt_arg ());
            if (nt >= 1)
              this->num_threads_ = nt;

            // Remove the option '-n' from argv []
            // to avoid any confusion that might result.
            for (int i = get_opts.opt_ind (); i != argc; ++i)
              argv [i-2 ] = argv [i];

            // Decrement the value of argc to reflect the removal
            // of '-n' option.
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

class ORB_Runner : public ACE_Task_Base
{
public:
  ORB_Runner (CORBA::ORB_ptr o, long t)
    : orb_(CORBA::ORB::_duplicate (o)),
      time_(t)
  {}

  int svc (void)
  {
  if (!CORBA::is_nil (orb_.in ()))
    {
      if (time_ == 0)
        {
          this->orb_->run ();
        }
      else
        {
          ACE_Time_Value tv (time_);
          this->orb_->run (tv);
        }
    }

  return 0;
  }

private:
  CORBA::ORB_var orb_;
  long time_;
};

int
TAO_Naming_Service::run (void)
{
  ORB_Runner runner (this->orb_.in(), time_);
  if (this->num_threads_ == 1)
    return runner.svc();
  else
    {
      int grpid = runner.activate ( THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED,
                                    this->num_threads_);
      if (grpid == -1)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                          ACE_TEXT ("Naming_Service(%P)::run %p\n"),
                          ACE_TEXT ("thread acitvation")));
          return -1;
        }
      runner.wait();
    }
  return 0;
}

void
TAO_Naming_Service::shutdown (void)
{
  if (!CORBA::is_nil (orb_.in ()))
    {
      this->orb_->shutdown (0);
    }
}

int
TAO_Naming_Service::fini (void)
{
  this->my_naming_server_.fini();

  try
  {
    // destroy implies shutdown
    if (!CORBA::is_nil (orb_.in ()))
      {
        this->orb_->destroy ();
      }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("TAO_Naming_Service::fini");
    return -1;
  }
  return 0;
}

// Destructor.
TAO_Naming_Service::~TAO_Naming_Service (void)
{
}
