// $Id$

#include "LB_server.h"
#include "Simple.h"
#include "Factory.h"

#include "TestC.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"

const char *location1 = "MyLocation 1";
const char *location2 = "MyLocation 2";
const char *location3 = "MyLocation 3";

const ACE_TCHAR* group_file = ACE_TEXT ("group.ior");
const ACE_TCHAR* factory1_ior = ACE_TEXT ("file://factory1.ior");
const ACE_TCHAR* factory2_ior = ACE_TEXT ("file://factory2.ior");
const ACE_TCHAR* factory3_ior = ACE_TEXT ("file://factory3.ior");

LB_server::LB_server (int argc, ACE_TCHAR ** argv)
  : argc_ (argc)
  , argv_ (argv)
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
      this->orb_->run ();
      //ACE_Time_Value period (0, 10);
      //while (1)
      //  {
      //    this->orb_->perform_work (&period);
      //  }
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
LB_server::write_ior_to_file (const char *ior, const char * ior_file)
{
  FILE *output_file =
    ACE_OS::fopen (ior_file, "w");

  if (output_file == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Cannot open output file %s for writing IOR:", ior_file));
      return -1;
    }

  ACE_OS::fprintf (output_file, "%s", ior);
  ACE_OS::fclose (output_file);
  return 0;
}

int
LB_server::parse_args (int& argc, ACE_TCHAR **& argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:x:y:z:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        group_file = get_opts.opt_arg ();
        break;
      case 'x':
        factory1_ior = get_opts.opt_arg ();
        break;
      case 'y':
        factory2_ior = get_opts.opt_arg ();
        break;
      case 'z':
        factory3_ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-n <server_id> -o <group_ior_file> -x <factory1_ior> "
                           "-y <factory2_ior> -z <factory3_ior> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

int
LB_server::init (void)
{
  try
    {
      // Initialise the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);

      this->parse_args (this->argc_, this->argv_);

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

      this->create_object_group (location1, location2, location3);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised in LB_server::init()");
      return -1;
    }

  return 1;

}

int
LB_server::create_object_group (const char *loc_1, const char *loc_2, const char *loc_3)
{
  try
    {
      const char *repository_id = "IDL:Test/Simple:1.0";

      CORBA::Object_var obj = this->orb_->string_to_object (factory1_ior);
      PortableGroup::GenericFactory_var factory1_obj =
        PortableGroup::GenericFactory::_narrow (obj.in ());

      obj = this->orb_->string_to_object (factory2_ior);
      PortableGroup::GenericFactory_var factory2_obj =
        PortableGroup::GenericFactory::_narrow (obj.in ());

      obj = this->orb_->string_to_object (factory3_ior);
      PortableGroup::GenericFactory_var factory3_obj =
        PortableGroup::GenericFactory::_narrow (obj.in ());

      PortableGroup::FactoriesValue  factory_infos;
      factory_infos.length (3);

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

      factory_infos[0].the_factory = factory1_obj._retn ();
      factory_infos[0].the_criteria = factory_criteria;

      factory_infos[1].the_factory = factory2_obj._retn ();
      factory_infos[1].the_criteria = factory_criteria;

      factory_infos[2].the_factory = factory3_obj._retn ();
      factory_infos[2].the_criteria = factory_criteria;

      PortableGroup::Location & location1 = factory_infos[0].the_location;
      location1.length (1);
      PortableGroup::Location & location2 = factory_infos[1].the_location;
      location2.length (1);
      PortableGroup::Location & location3 = factory_infos[2].the_location;
      location3.length (1);

      location1[0].id = CORBA::string_dup (loc_1);
      location2[0].id = CORBA::string_dup (loc_2);
      location3[0].id = CORBA::string_dup (loc_3);

      property_two.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.Factories");
      property_two.val <<= factory_infos;

      property_three.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.InitialNumberMembers");
      PortableGroup::InitialNumberMembersValue init_value = 3;
      property_three.val <<= init_value;

      property_four.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.MinimumNumberMembers");
      PortableGroup::MinimumNumberMembersValue min_value = 3;
      property_four.val <<= min_value;

      this->object_group_ = this->lm_->create_object (repository_id,
                                                      criteria,
                                                      this->fcid_.out ());

      CORBA::String_var ior =
        this->orb_->object_to_string (this->object_group_.in ());

      this->write_ior_to_file (ior.in (), ACE_TEXT_ALWAYS_CHAR (group_file));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Exception raised while creating object group");
      return -1;
    }

  return 1;

}
