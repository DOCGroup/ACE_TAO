#include "LB_server.h"
#include "Simple.h"
#include "Factory.h"

#include "TestC.h"

ACE_RCSID (Infrastructure_Controlled,
           LB_server,
           "$Id$")

LB_server::LB_server (int argc, char **argv)
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
  ACE_TRY_NEW_ENV
    {
      ACE_Time_Value period (0, 10);
      while (1)
	{
          this->orb_->perform_work (&period);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught while running LB_server\n");
      return -1;
    }
  ACE_ENDTRY;
  return 1;

}


int
LB_server::destroy (void)
{
  ACE_TRY_NEW_ENV
    {
      this->lm_->delete_object (this->fcid_.in ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->root_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught while destroying LB_server\n");
      return -1;
    }
  ACE_ENDTRY;
  return 1;

}

int
LB_server::write_ior_to_file (const char *ior)
{
  FILE *output_file =
    ACE_OS::fopen ("obj.ior", "w");

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
LB_server::start_orb_and_poa (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialise the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA"
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        this->orb_->resolve_initial_references ("LoadManager" ACE_ENV_ARG_PARAMETER);

      this->lm_ =
        CosLoadBalancing::LoadManager::_narrow (obj.in ()
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->lm_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get Load Manager Reference\n"),
                          1);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception raised initialising ORB or POA");
      return -1;
    }
  ACE_ENDTRY;

  return 1;

}

int
LB_server::create_object_group (const char *loc1, const char *loc2)
{
  ACE_TRY_NEW_ENV
    {
      const char *repository_id = "IDL:Test/Simple:1.0";

      Factory factory_object1;
      Factory factory_object2;

      PortableGroup::GenericFactory_var factory_obj1 =
        factory_object1._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableGroup::GenericFactory_var factory_obj2 =
        factory_object2._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

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

      location1[0].id = CORBA::string_dup (loc1);
      location2[0].id = CORBA::string_dup (loc2);

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
                                                      this->fcid_.out ()
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        this->orb_->object_to_string (this->object_group_.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->write_ior_to_file (ior.in ());

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception raised while creating object group");
      return -1;
    }
  ACE_ENDTRY;

  return 1;

}
