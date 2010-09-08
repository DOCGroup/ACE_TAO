// $Id$

#include "LB_server.h"
#include "Simple.h"
#include "Factory.h"

#include "TestC.h"
#include "ace/OS_NS_stdio.h"

LB_server::LB_server (int argc, ACE_TCHAR **argv)
  : argc_ (argc)
  , argv_ (argv)
  , ior_output_file_(ACE_TEXT("obj.ior"))
{
}

CORBA::ORB_ptr
LB_server::orb (void)
{
  return this->orb_.in ();
}

int
LB_server::run (void)
{
  try
    {
      ACE_Time_Value period (0, 10);
      while (1)
        {
          this->orb_->perform_work (&period);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught while running LB_server\n");
      return -1;
    }
  return 1;

}


int
LB_server::destroy (void)
{
  try
    {
      this->lm_->delete_object (this->fcid_.in ());

      this->root_poa_->destroy (1, 1);

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Exception caught while destroying LB_server\n");
      return -1;
    }
  return 1;

}

int
LB_server::write_ior_to_file (const char *ior)
{
  FILE *output_file =
    ACE_OS::fopen (this->ior_output_file_, "w");

  if (output_file == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Cannot open output file for writing IOR:"));
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
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
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
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

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
                          1);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised initialising ORB or POA");
      return -1;
    }

  return 1;

}

int
LB_server::create_object_group (const char *loc_1, const char *loc_2)
{
  try
    {
      const char *repository_id = "IDL:Test/Simple:1.0";

      Factory factory_object1;
      Factory factory_object2;

      if (this->parse_args (argc_, argv_) != 0)
              return -1;

      PortableGroup::GenericFactory_var factory_obj1 =
        factory_object1._this ();

      PortableGroup::GenericFactory_var factory_obj2 =
        factory_object2._this ();

      PortableGroup::FactoriesValue  factory_infos;
      factory_infos.length (2);

      PortableGroup::Criteria criteria ;
      criteria.length (4);

      PortableGroup::Criteria factory_criteria ;
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

      PortableGroup::MembershipStyleValue msv =
        PortableGroup::MEMB_INF_CTRL;
      property_one.val <<= msv;

      factory_infos[0].the_factory = factory_obj1._retn ();
      factory_infos[0].the_criteria = factory_criteria;

      factory_infos[1].the_factory = factory_obj2._retn ();
      factory_infos[1].the_criteria = factory_criteria;

      PortableGroup::Location & location1 = factory_infos[0].the_location;
      location1.length (1);
      PortableGroup::Location & location2 = factory_infos[1].the_location;
      location2.length (1);

      location1[0].id = CORBA::string_dup (loc_1);
      location2[0].id = CORBA::string_dup (loc_2);

      property_two.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.Factories");
      property_two.val <<= factory_infos;

      property_three.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.InitialNumberMembers");
      PortableGroup::InitialNumberMembersValue init_value = 2;
      property_three.val <<= init_value;

      property_four.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.MinimumNumberMembers");
      PortableGroup::MinimumNumberMembersValue min_value = 2;
      property_four.val <<= min_value;

      this->object_group_ = this->lm_->create_object (repository_id,
                                                      criteria,
                                                      this->fcid_.out ());

      CORBA::String_var ior =
        this->orb_->object_to_string (this->object_group_.in ());

      this->write_ior_to_file (ior.in ());

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Exception raised while creating object group");
      return -1;
    }

  return 1;

}
