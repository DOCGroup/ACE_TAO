// $Id$

#include "orbsvcs/FT_ReplicationManagerC.h"
#include <ace/Get_Opt.h>
#include <iostream>

// A simple class for building a client that "controls' the
// Replication Manager (right now it just shuts it down).
class TAO_FT_ReplicationManagerController
{
/////////////////////////////
// Public interface.
public:
  TAO_FT_ReplicationManagerController ();
  virtual ~TAO_FT_ReplicationManagerController ();

  int init (int & argc, char * argv[]);
  int parse_args (int & argc, char* argv[]);
  int run ();

/////////////////////////////
// Forbidden.
private:
  TAO_FT_ReplicationManagerController (
    const TAO_FT_ReplicationManagerController & rhs);
  TAO_FT_ReplicationManagerController & operator = (
    const TAO_FT_ReplicationManagerController & rhs);

/////////////////////////////
// Private implementation.
private:
  void usage (ostream & out) const;

/////////////////////////////
// Data members.
private:
  CORBA::ORB_var orb_;
  FT::ReplicationManager_var replication_manager_;
  const char * rm_ior_;
  int shutdown_;

};

TAO_FT_ReplicationManagerController::TAO_FT_ReplicationManagerController ()
  : orb_ (CORBA::ORB::_nil())
  , replication_manager_ (FT::ReplicationManager::_nil())
  , rm_ior_ (0)
  , shutdown_ (0)
{
}

TAO_FT_ReplicationManagerController::~TAO_FT_ReplicationManagerController ()
{
}

int TAO_FT_ReplicationManagerController::init (int & argc, char * argv[])
{
  int result = 0;

  ACE_TRY_NEW_ENV
  {
    // Initialize the ORB.
    this->orb_ = CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // Parse arguments.
    result = this->parse_args (argc, argv);
    if (result == 0)
    {
      CORBA::Object_var obj = CORBA::Object::_nil ();
      if (this->rm_ior_ != 0)
      {
        obj = this->orb_->string_to_object (
          this->rm_ior_ ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      else
      {
        obj = this->orb_->resolve_initial_references (
          "ReplicationManager" ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      this->replication_manager_ = FT::ReplicationManager::_narrow (
        obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (this->replication_manager_.in()))
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT (
            "TAO_FT_ReplicationManagerController::init: "
            "Could not get Replication Manager's IOR.\n")
        ));
        result = -1;
      }
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (
      ACE_ANY_EXCEPTION,
      ACE_TEXT (
        "TAO_FT_ReplicationManagerController::init: \n")
    );
    result = -1;
  }
  ACE_ENDTRY;

  return result;
}


int TAO_FT_ReplicationManagerController::parse_args (int & argc, char * argv[])
{
  int result = 0;

  ACE_Get_Opt get_opts (argc, argv, "k:x");
  int c;

  while (result == 0 && (c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'k':
      {
        this->rm_ior_ = get_opts.opt_arg ();
        break;
      }

      case 'x':
      {
        this->shutdown_ = 1;
        break;
      }

      default:
        std::cerr << argv[0] << ": Unknown argument -" << (char) c << std::endl;
        this->usage(std::cerr);
        result = -1;
        break;

      case '?':
        this->usage(std::cerr);
        result = -1;
    }
  }

  return result;
}

void TAO_FT_ReplicationManagerController::usage (ostream & out) const
{
  out << "usage"
      << " -k <replication manager ior file>"
      << " -x (shutdown the Replication Manager)"
      << std::endl;
}

int TAO_FT_ReplicationManagerController::run ()
{
  int result = 0;

  ACE_TRY_NEW_ENV
  {
    if (this->shutdown_ == 1)
    {
      this->replication_manager_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_Time_Value tv (0, 500000);
      ACE_OS::sleep (tv);
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (
      ACE_ANY_EXCEPTION,
      ACE_TEXT (
        "TAO_FT_ReplicationManagerController::run: \n")
    );
    result = -1;
  }
  ACE_ENDTRY;

  return result;
}

int main (int argc, char * argv[])
{
  int result = 0;
  TAO_FT_ReplicationManagerController rmctrl;
  result = rmctrl.init (argc, argv);
  if (result == 0)
  {
    result = rmctrl.run ();
  }
  return result;
}

