#include "LB_server.h"
#include "Basic.h"

#include "TestC.h"

ACE_RCSID (Application_Controlled,
           LB_server,
           "$Id$")

LB_server::LB_server (int argc, char **argv)
  : argc_ (argc)
  , argv_ (argv)
{
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

CosLoadBalancing::LoadManager_ptr
LB_server::load_manager (void)
{
  return this->lm_.in ();
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
LB_server::create_object_group (void)
{
  ACE_TRY_NEW_ENV
    {
      const char *repository_id = "IDL:Test/Basic:1.0";

      PortableGroup::Criteria criteria (1);
      criteria.length (1);

      PortableGroup::Property &property = criteria[0];
      property.nam.length (1);

      property.nam[0].id =
        CORBA::string_dup ("omg.org.PortableGroup.MembershipStyle");

      PortableGroup::MembershipStyleValue msv =
        PortableGroup::MEMB_APP_CTRL;
      property.val <<= msv;

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

int
LB_server::register_servant (Basic *servant, const char *loc)
{
  ACE_TRY_NEW_ENV
    {
      Test::Basic_var basic =
        servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableGroup::Location location (1);
      location.length (1);

      location[0].id = CORBA::string_dup (loc);

      this->lm_->add_member (this->object_group_.in (),
                             location,
                             basic.in ()
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception raised while registering servant");
      return -1;
    }
  ACE_ENDTRY;

  return 1;
}
