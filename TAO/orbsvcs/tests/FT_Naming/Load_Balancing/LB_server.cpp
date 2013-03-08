// $Id$

#include "LB_server.h"
#include "Basic.h"

#include "TestC.h"
#include "ace/OS_NS_stdio.h"

LB_server::LB_server (int argc, ACE_TCHAR **argv)
  : argc_ (argc)
  , argv_ (argv)
  , ior_output_file_(ACE_TEXT("obj.ior"))
{
}

int
LB_server::destroy (void)
{
  try
    {
      this->naming_manager_->delete_object_group ("Basic Group");

      this->root_poa_->destroy (1, 1);

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("Exception caught while destroying LB_server\n"));
      return -1;
    }
  return 0;

}

CORBA::ORB_ptr
LB_server::orb (void)
{
  return this->orb_.in ();
}

CORBA::Object_ptr
LB_server::object_group (void)
{
  return this->object_group_.in ();
}

FT_Naming::NamingManager_ptr
LB_server::naming_manager (void)
{
  return this->naming_manager_.in ();
}

int
LB_server::write_ior_to_file (const char *ior)
{
  FILE *output_file =
    ACE_OS::fopen (this->ior_output_file_, "w");

  if (output_file == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cannot open output file for writing IOR:")));
      return -1;
    }

  ACE_OS::fprintf (output_file, "%s", ior);
  ACE_OS::fclose (output_file);
  return 0;
}

int
LB_server::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        this->ior_output_file_ = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-o <iorfile>")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
LB_server::start_orb_and_poa (void)
{
  try
    {
      // Initialise the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          -1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager ();

      poa_manager->activate ();

      ACE_Time_Value timeout (10); // Wait up to 10 seconds for the naming service
      if (name_svc_.init (this->orb_.in (), &timeout) != 0)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("LB_server: Could not connect to naming ")
                           ACE_TEXT ("service.\n")),
                           -1);

      CORBA::Object_var obj =
        this->orb_->resolve_initial_references ("NamingManager");

      this->naming_manager_ =
        FT_Naming::NamingManager::_narrow (obj.in ());

      if (CORBA::is_nil (this->naming_manager_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to get Naming ")
                           ACE_TEXT ("Manager Reference\n")),
                          -1);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("Exception raised initialising ORB or POA"));
      return -1;
    }

  return 0;

}

int
LB_server::create_object_group (void)
{
  try
    {
      if (this->parse_args (argc_, argv_) != 0)
        return -1;

      PortableGroup::Criteria criteria (1);
      criteria.length (1);

      PortableGroup::Property &mem_style = criteria[0];
      mem_style.nam.length (1);

      // Set the membership style property
      mem_style.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.MembershipStyle");
      PortableGroup::MembershipStyleValue msv =
        PortableGroup::MEMB_APP_CTRL;
      mem_style.val <<= msv;

      this->object_group_ = this->naming_manager_->create_object_group (
        "Basic Group",
        FT_Naming::ROUND_ROBIN,
        criteria);

      CORBA::String_var ior =
        this->orb_->object_to_string (this->object_group_.in ());

      this->write_ior_to_file (ior.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("Exception raised while creating object group"));
      return -1;
    }

  return 0;

}

int
LB_server::register_servant (Basic *servant, const char *loc)
{
  try
    {
      Test::Basic_var basic =
        servant->_this ();

      PortableGroup::Location location (1);
      location.length (1);

      location[0].id = CORBA::string_dup (loc);

      this->object_group_ =
        this->naming_manager_->add_member (this->object_group_.in (),
                                           location,
                                           basic.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT ("Exception raised while registering servant"));
      return -1;
    }

  return 0;
}

TAO_Naming_Client&
LB_server::name_svc ()
{
  return name_svc_;
}
