// $Id$

#include "orbsvcs/FT_ReplicationManagerC.h"
#include <ace/Get_Opt.h>
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

// A simple class for building a client that "controls' the
// Replication Manager (right now it just shuts it down).
class TAO_FT_ReplicationManagerController
{
/////////////////////////////
// Public interface.
public:
  TAO_FT_ReplicationManagerController ();
  virtual ~TAO_FT_ReplicationManagerController ();

  int init (int &argc, ACE_TCHAR *argv[]);
  int parse_args (int &argc, ACE_TCHAR *argv[]);
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
  void usage (FILE* out) const;

/////////////////////////////
// Data members.
private:
  CORBA::ORB_var orb_;
  FT::ReplicationManager_var replication_manager_;
  const ACE_TCHAR *rm_ior_;
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

int TAO_FT_ReplicationManagerController::init (int &argc, ACE_TCHAR *argv[])
{
  int result = 0;

  try
  {
    // Initialize the ORB.
    this->orb_ = CORBA::ORB_init (argc, argv);

    // Parse arguments.
    result = this->parse_args (argc, argv);
    if (result == 0)
    {
      CORBA::Object_var obj = CORBA::Object::_nil ();
      if (this->rm_ior_ != 0)
      {
        obj = this->orb_->string_to_object (
          ACE_TEXT_ALWAYS_CHAR (this->rm_ior_));
      }
      else
      {
        obj = this->orb_->resolve_initial_references (
          "ReplicationManager");
      }
      this->replication_manager_ = FT::ReplicationManager::_narrow (
        obj.in());
      if (CORBA::is_nil (this->replication_manager_.in()))
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("TAO_FT_ReplicationManagerController::init: ")
          ACE_TEXT ("Could not get Replication Manager's IOR.\n")
        ));
        result = -1;
      }
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT (
        "TAO_FT_ReplicationManagerController::init:\n"));
    result = -1;
  }

  return result;
}


int TAO_FT_ReplicationManagerController::parse_args (int &argc, ACE_TCHAR *argv[])
{
  int result = 0;

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:x"));
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
        ACE_OS::fprintf (stderr, "%s: Unknown argument - %c\n",
                         ACE_TEXT_ALWAYS_CHAR (argv[0]), c);
        this->usage(stderr);
        result = -1;
        break;

      case '?':
        this->usage(stderr);
        result = -1;
    }
  }

  return result;
}

void TAO_FT_ReplicationManagerController::usage (FILE* out) const
{
  ACE_OS::fprintf (out, "usage"
                        " -k <replication manager ior file>"
                        " -x (shutdown the Replication Manager)\n");
}

int TAO_FT_ReplicationManagerController::run ()
{
  int result = 0;

  try
  {
    if (this->shutdown_ == 1)
    {
      this->replication_manager_->shutdown ();
      ACE_Time_Value tv (0, 500000);
      ACE_OS::sleep (tv);
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("TAO_FT_ReplicationManagerController::run:\n"));
    result = -1;
  }

  return result;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
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
