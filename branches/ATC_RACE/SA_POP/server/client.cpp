#include "client.h"
#include "DriverC.h"
#include "ace/Get_Opt.h"

const char *ior = "file://server.ior";
const char *plan = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:d:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'd':
        plan = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-d <deployment plan uri> "
                           "\n",
                           argv [0]),
                          -1);
      }
  if (plan != 0)
    {
      // Indicates sucessful parsing of the command line
      return 0;
    }

  else
    {
      ACE_ERROR ((LM_ERROR, "URI of the deployment plan is necessary!\n"));
      return -1;
    }
}

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      ::CIAO::RACE::SA_POP::Driver_var driver =
          ::CIAO::RACE::SA_POP::Driver::_narrow(tmp.in ());

      if (CORBA::is_nil (driver.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Driver reference <%s>\n", ior), 1);
        }

      driver->deploy_plan (plan);

      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Exception caught:");
      return 1;
    }
  return 0;
}

using namespace ::SA_POP;

// Constructor.
UIOption::UIOption (std::string descrip, std::string undo_descrip,
                    ::CIAO::RACE::SA_POP::Driver_ptr driver)
: descrip_ (descrip),
  undo_descrip_ (undo_descrip),
  is_invoked_ (false),
  is_active_ (true),
  driver_ (driver)
{
  // Nothing to do.
};

// Destructor.
UIOption::~UIOption (void)
{
  // Nothing to do.
};

// Get description of option for display in UI.
std::string UIOption::get_descrip (void)
{
  if (!this->is_invoked ())
    return this->descrip_;
  return this->undo_descrip_;
};

// Get whether option is active.
bool UIOption::is_active (void)
{
  return this->is_active_;
};

// Get whether option is invoked.
bool UIOption::is_invoked (void)
{
  return this->is_invoked_;
};

// Do option action (if not invoked, invoke; otherwise, undo).
bool UIOption::do_action (void)
{
  if (!this->is_active ())
    return false;
  if (this->is_invoked ())
    return this->undo ();
  return this->invoke ();
};


// Constructor.
GoalOption::GoalOption (std::string descrip, std::string undo_descrip,
                        ::CIAO::RACE::SA_POP::Driver_ptr driver,
                        ::CIAO::RACE::GoalStructure goal)
: UIOption (descrip, undo_descrip, driver),
  goal_ (goal)
{
  // Nothing to do.
};

// Destructor.
GoalOption::~GoalOption (void)
{
  // Nothing to do.
};

// Invoke option action (plan and deploy opstring for goal).
bool GoalOption::invoke (void)
{
  if (this->is_invoked ())
    return false;

  // Plan and deploy opstring for goal, and update invocation flag.
  this->driver_->deploy_goal (this->goal_);
  this->is_invoked_ = true;
  return true;
};

// Undo option action (remove deployment plan).
bool GoalOption::undo (void)
{
  if (!this->is_invoked ())
    return false;

  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****
  // Remove deployment plan and update invocation flag.
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****
  this->is_invoked_ = false;
  return true;
};

// Constructor.
DeployOption::DeployOption (std::string descrip, std::string undo_descrip,
                            ::CIAO::RACE::SA_POP::Driver_ptr driver,
                            std::string dp_uri)
: UIOption (descrip, undo_descrip, driver),
  dp_uri_ (dp_uri)
{
  // Nothing to do.
};

// Destructor.
DeployOption::~DeployOption (void)
{
  // Nothing to do.
};

// Invoke option action (deploy plan).
bool DeployOption::invoke (void)
{
  if (this->is_invoked ())
    return false;

  // Deploy plan and update invocation flag.
  this->driver_->deploy_plan (CORBA::string_dup (this->dp_uri_.c_str ()));
  this->is_invoked_ = true;
  return true;
};

// Undo option action (remove deployment plan).
bool DeployOption::undo (void)
{
  if (!this->is_invoked ())
    return false;

  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****
  // Remove deployment plan and update invocation flag.
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****
  this->is_invoked_ = false;
  return true;
};

