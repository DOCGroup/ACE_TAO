#include "LB_server.h"
#include "Simple.h"
#include "Factory.h"

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <cstdio>
#include <memory>

LB_server::LB_server (int argc, ACE_TCHAR **argv)
  : argc_ (argc),
    argv_ (argv)
{
}

int
LB_server::run ()
{
  try
    {
      ACE_Time_Value period (0, 10);
      while (true)
        {
          this->orb_->perform_work (&period);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught while running LB_server\n");
      return -1;
    }
  return 0;
}

int
LB_server::destroy ()
{
  try
    {
      this->lm_->delete_object (this->fcid_.in ());

      this->root_poa_->destroy (true, true);

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Exception caught while destroying LB_server\n");
      return -1;
    }
  return 0;
}

int
LB_server::write_ior_to_file (char const *ior)
{
  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;
  File_Ptr output_file (ACE_OS::fopen (this->options_.ior_output_file.c_str (),
                                       ACE_TEXT ("w")),
                        ACE_OS::fclose);

  if (!output_file)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cannot open output file for writing IOR\n")));
      return -1;
    }

  std::fprintf (output_file.get (), "%s", ior);
  return 0;
}

int
LB_server::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        this->options_.ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile>\n"),
                           argv[0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
LB_server::start_orb_and_poa ()
{
  try
    {
      // Initialise the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);

      if (this->parse_args (this->argc_, this->argv_) != 0)
        return -1;

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

      this->lm_ =
        CosLoadBalancing::LoadManager::_narrow (obj.in ());

      if (CORBA::is_nil (this->lm_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get Load Manager Reference\n"),
                          -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised initialising ORB or POA");
      return -1;
    }

  return 0;
}

int
LB_server::create_object_group (char const *loc_1, char const *loc_2)
{
  try
    {
      char const *const repository_id = "IDL:Test/Simple:1.0";

      Factory factory_object1;
      Factory factory_object2;

      PortableGroup::GenericFactory_var factory_obj1 =
        factory_object1._this ();

      PortableGroup::GenericFactory_var factory_obj2 =
        factory_object2._this ();

      PortableGroup::FactoriesValue factory_infos;
      factory_infos.length (2);

      PortableGroup::Criteria criteria;
      criteria.length (4);

      PortableGroup::Criteria factory_criteria;
      factory_criteria.length (1);

      PortableGroup::Property &property_one = criteria[0];
      property_one.nam.length (1);
      PortableGroup::Property &property_two = criteria[1];
      property_two.nam.length (1);
      PortableGroup::Property &property_three = criteria[2];
      property_three.nam.length (1);
      PortableGroup::Property &property_four = criteria[3];
      property_four.nam.length (1);

      PortableGroup::Property &factory_property_one = factory_criteria[0];
      factory_property_one.nam.length (1);

      property_one.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.MembershipStyle");

      PortableGroup::MembershipStyleValue const msv = PortableGroup::MEMB_INF_CTRL;
      property_one.val <<= msv;

      factory_infos[0].the_factory = factory_obj1._retn ();
      factory_infos[0].the_criteria = factory_criteria;

      factory_infos[1].the_factory = factory_obj2._retn ();
      factory_infos[1].the_criteria = factory_criteria;

      PortableGroup::Location &location1 = factory_infos[0].the_location;
      location1.length (1);
      PortableGroup::Location &location2 = factory_infos[1].the_location;
      location2.length (1);

      location1[0].id = CORBA::string_dup (loc_1);
      location2[0].id = CORBA::string_dup (loc_2);

      property_two.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.Factories");
      property_two.val <<= factory_infos;

      property_three.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.InitialNumberMembers");
      PortableGroup::InitialNumberMembersValue const init_value = 2;
      property_three.val <<= init_value;

      property_four.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.MinimumNumberMembers");
      PortableGroup::MinimumNumberMembersValue const min_value = 2;
      property_four.val <<= min_value;

      this->object_group_ = this->lm_->create_object (repository_id,
                                                      criteria,
                                                      this->fcid_.out ());

      CORBA::String_var ior =
        this->orb_->object_to_string (this->object_group_.in ());

      if (this->write_ior_to_file (ior.in ()) != 0)
        return -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Exception raised while creating object group");
      return -1;
    }

  return 0;
}
