#include "LB_server.h"
#include "Basic.h"
#include "TestC.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

LB_server::LB_server (int &argc, ACE_TCHAR **&argv, LB_Server_Options &options)
  : argc_ (argc),
    argv_ (argv),
    options_ (options)
{
}

int
LB_server::destroy ()
{
  try
    {
      if (this->object_group_created_ && !CORBA::is_nil (this->lm_.in ()))
        this->lm_->delete_object (this->fcid_.in ());

      if (!CORBA::is_nil (this->root_poa_.in ()))
        this->root_poa_->destroy (1, 1);

      if (!CORBA::is_nil (this->orb_.in ()))
        this->orb_->destroy ();
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception caught while destroying LB_server\n");
      return -1;
    }
  return 1;
}

CORBA::ORB_ptr
LB_server::orb ()
{
  return this->orb_.in ();
}

CORBA::Object_ptr
LB_server::object_group ()
{
  return this->object_group_.in ();
}

CosLoadBalancing::LoadManager_ptr
LB_server::load_manager ()
{
  return this->lm_.in ();
}

int
LB_server::write_ior_to_file (char const *ior)
{
  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;
  File_Ptr output_file (ACE_OS::fopen (this->options_.group_file, "w"),
                        ACE_OS::fclose);

  if (!output_file)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                  this->options_.group_file));
      return -1;
    }

  if (ACE_OS::fprintf (output_file.get (), "%s", ior) < 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cannot write IOR to output file: %s\n"),
                  this->options_.group_file));
      return -1;
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
                           ACE_TEXT ("(%P|%t) Unable to initialize the POA.\n")),
                          -1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());
      if (CORBA::is_nil (this->root_poa_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Unable to narrow the RootPOA.\n")),
                          -1);

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager ();
      poa_manager->activate ();

      CORBA::Object_var obj =
        this->orb_->resolve_initial_references ("LoadManager");
      this->lm_ = CosLoadBalancing::LoadManager::_narrow (obj.in ());

      if (CORBA::is_nil (this->lm_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Unable to get Load Manager Reference\n")),
                          -1);
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception raised initialising ORB or POA");
      return -1;
    }

  return 1;
}

int
LB_server::create_object_group ()
{
  try
    {
      char const * const repository_id = "IDL:Test/Basic:1.0";
      PortableGroup::Criteria criteria (1);
      criteria.length (1);

      PortableGroup::Property &property = criteria[0];
      property.nam.length (1);
      property.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.MembershipStyle");

      PortableGroup::MembershipStyleValue const membership_style =
        PortableGroup::MEMB_APP_CTRL;
      property.val <<= membership_style;

      this->object_group_ = this->lm_->create_object (repository_id,
                                                      criteria,
                                                      this->fcid_.out ());
      this->object_group_created_ = true;
      CORBA::String_var ior =
        this->orb_->object_to_string (this->object_group_.in ());

      if (this->write_ior_to_file (ior.in ()) == -1)
        return -1;
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception raised while creating object group");
      return -1;
    }

  return 1;
}

int
LB_server::get_object_group ()
{
  try
    {
      this->object_group_ = this->orb_->string_to_object (this->options_.group_ior);
      if (CORBA::is_nil (this->object_group_.in ()))
        return -1;
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception raised while getting object group");
      return -1;
    }

  return 1;
}

int
LB_server::register_servant (Basic *servant, std::string const &location_name)
{
  try
    {
      Test::Basic_var basic = servant->_this ();
      PortableGroup::Location location (1);
      location.length (1);
      location[0].id = CORBA::string_dup (location_name.c_str ());
      this->lm_->add_member (this->object_group_.in (), location, basic.in ());
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception raised while registering servant");
      return -1;
    }

  return 1;
}
