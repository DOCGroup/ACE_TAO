#include "LB_server.h"
#include "Basic.h"

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include <memory>

LB_server::LB_server (int argc, ACE_TCHAR **argv)
  : argc_ (argc),
    argv_ (argv),
    ior_output_file_ (ACE_TEXT ("obj.ior"))
{
}

int
LB_server::destroy ()
{
  try
    {
      if (!CORBA::is_nil (this->lm_.in ()) &&
          !CORBA::is_nil (this->object_group_.in ()))
        this->lm_->delete_object (this->fcid_.in ());

      if (!CORBA::is_nil (this->root_poa_.in ()))
        this->root_poa_->destroy (1, 1);

      if (!CORBA::is_nil (this->orb_.in ()))
        this->orb_->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Exception caught while destroying LB_server\n");
      return -1;
    }
  return 1;
}

CORBA::ORB_ptr
LB_server::orb () const
{
  return this->orb_.in ();
}

CORBA::Object_ptr
LB_server::object_group () const
{
  return this->object_group_.in ();
}

CosLoadBalancing::LoadManager_ptr
LB_server::load_manager () const
{
  return this->lm_.in ();
}

int
LB_server::write_ior_to_file (char const *ior) const
{
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (this->ior_output_file_, ACE_TEXT ("w")),
    ACE_OS::fclose);

  if (!output_file)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open output file for writing IOR\n"),
                        -1);
    }

  ACE_OS::fprintf (output_file.get (), "%s", ior);
  return 0;
}

int
LB_server::parse_args ()
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        this->ior_output_file_ = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile>\n"),
                           this->argv_[0]),
                          -1);
      }
  return 0;
}

int
LB_server::start_orb_and_poa ()
{
  try
    {
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager ();
      poa_manager->activate ();

      CORBA::Object_var obj =
        this->orb_->resolve_initial_references ("LoadManager");
      this->lm_ = CosLoadBalancing::LoadManager::_narrow (obj.in ());

      if (CORBA::is_nil (this->lm_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get Load Manager Reference\n"),
                          -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception raised initialising ORB or POA");
      return -1;
    }

  return 1;
}

int
LB_server::create_object_group ()
{
  if (this->parse_args () != 0)
    return -1;

  try
    {
      char const * const repository_id = "IDL:Test/Basic:1.0";

      PortableGroup::Criteria criteria (1);
      criteria.length (1);

      PortableGroup::Property &property = criteria[0];
      property.nam.length (1);

      property.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.MembershipStyle");

      PortableGroup::MembershipStyleValue const msv =
        PortableGroup::MEMB_APP_CTRL;
      property.val <<= msv;

      this->object_group_ = this->lm_->create_object (repository_id,
                                                      criteria,
                                                      this->fcid_.out ());

      CORBA::String_var ior =
        this->orb_->object_to_string (this->object_group_.in ());

      if (this->write_ior_to_file (ior.in ()) != 0)
        return -1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Exception raised while creating object group");
      return -1;
    }

  return 1;
}

int
LB_server::register_servant (Basic *servant, std::string const &loc)
{
  try
    {
      Test::Basic_var basic =
        servant->_this ();

      PortableGroup::Location location (1);
      location.length (1);

      location[0].id = CORBA::string_dup (loc.c_str ());

      this->lm_->add_member (this->object_group_.in (),
                             location,
                             basic.in ());
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception raised while registering servant");
      return -1;
    }

  return 1;
}
