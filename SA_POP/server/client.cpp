#include <iostream>
#include "client.h"
#include "DriverC.h"
#include "ace/Get_Opt.h"
#include "ace/UUID.h"

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
//      ACE_ERROR ((LM_ERROR, "URI of the deployment plan is necessary!\n"));
//      return -1;
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      ::CIAO::RACE::SA_POP::Driver_var driver =
          ::CIAO::RACE::SA_POP::Driver::_narrow(tmp.in ());

      if (CORBA::is_nil (driver.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                            "Nil Driver reference <%s>\n", ior), 1);
        }

      if (plan != 0) {
        driver->deploy_plan (plan);
      } else {
        // Get number of goal conditions.
        unsigned int num_goals = 0;
        std::cout << "Number of goals: ";
        std::cin >> num_goals;
        std::cout << std::endl;

        // Create goal map.
        ::SA_POP::GoalMap goal_map;
        goal_map.clear ();
        for (unsigned int loop = 0; loop < num_goals; loop++) {
          // Get goal ID and utility.
          ::SA_POP::CondID cond_id;
          ::SA_POP::Utility util;
          std::cout << "Goal condition ID: ";
          std::cin >> cond_id;
          std::cout << std::endl;
          std::cout << "Goal utility: ";
          std::cin >> util;
          std::cout << std::endl;

          goal_map.insert (std::make_pair (cond_id, util));
        }

        // Create goal option.
        ::SA_POP::GoalOption goal_opt (
          "Plan and deploy opstring for user-specified goal.",
          "Tear down deployed opstring for user-specified goal.", driver.in (),
          goal_map);

        // Activate goal option.
        std::cout << "Activating goal option to plan and deploy opstring." << std::endl;
        goal_opt.do_action ();
      }

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
  driver_ (::CIAO::RACE::SA_POP::Driver::_duplicate (driver))
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

// Constructor for a goal with only goal conditions.
GoalOption::GoalOption (std::string descrip, std::string undo_descrip,
            ::CIAO::RACE::SA_POP::Driver_ptr driver,
            ::SA_POP::GoalMap goal_conds)
: UIOption (descrip, undo_descrip, driver)
{
  // Create goal with specified goal conditions and default values for the rest.
  this->create_def_goal (goal_conds);
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

// Create and set internal goal using specified goal conditions and defaults.
void GoalOption::create_def_goal (const ::SA_POP::GoalMap goal_conds)
{
  // Add goal conditions.
  ::CORBA::ULong cond_index = 0;
  for (::SA_POP::GoalMap::const_iterator cond_iter = goal_conds.begin ();
        cond_iter != goal_conds.end (); cond_iter++)
  {
    // Goal condition to populate.
    ::CIAO::RACE::GoalCondition cond_desc;

    // Set condition ID and utility.
    cond_desc.condID = cond_iter->first;
    cond_desc.utility = static_cast< ::CORBA::Long> (cond_iter->second);

    // Add goal condition to goal idl.
    this->goal_.goalConds.length (cond_index + 1);
    this->goal_.goalConds[cond_index] = cond_desc;

    // Increment link index.
    cond_index++;
  }

  // Create and set UUID.
  ACE_Utils::UUID goal_uuid;
  ACE_Utils::UUID_GENERATOR::instance ()->generateUUID (goal_uuid);
  this->goal_.UUID = CORBA::string_dup (goal_uuid.to_string()->c_str());

  // Provide default name.
  this->goal_.name = "DefaultGoalName";

  // Create default time.
  ::CIAO::RACE::TimeValue def_time;
  def_time.sec = 0;
  def_time.usec = 0;

  // Set start window to [default, default].
  this->goal_.startWindow.earliest = def_time;
  this->goal_.startWindow.latest = def_time;
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

