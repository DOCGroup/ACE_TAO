#include "LB_server.h"
#include "Basic.h"
#include "Simple.h"

#include "TestC.h"
#include "ace/OS_NS_stdio.h"

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
      this->lm_->delete_object (this->basic_fcid_.in ()
                                ACE_ENV_ARG_PARAMETER);

      this->lm_->delete_object (this->simple_fcid_.in ()
                                ACE_ENV_ARG_PARAMETER);

      this->root_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);

      this->orb_->destroy ();
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
LB_server::get_basic_object_group (void)
{
  return this->basic_object_group_.in ();
}

CORBA::Object_ptr
LB_server::get_simple_object_group (void)
{
  return this->simple_object_group_.in ();
}

CosLoadBalancing::LoadManager_ptr
LB_server::load_manager (void)
{
  return this->lm_.in ();
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

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA"
                                               ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in ()
                                                      ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager ();

      poa_manager->activate ();

      CORBA::Object_var obj =
        this->orb_->resolve_initial_references ("LoadManager" ACE_ENV_ARG_PARAMETER);

      this->lm_ =
        CosLoadBalancing::LoadManager::_narrow (obj.in ()
                                                ACE_ENV_ARG_PARAMETER);

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
LB_server::create_basic_object_group (void)
{
  ACE_TRY_NEW_ENV
    {
      const char *repository_id = "IDL:Test/Basic:1.0";

      PortableGroup::Criteria criteria (1);
      criteria.length (1);

      PortableGroup::Property &property = criteria[0];
      property.nam.length (1);

      property.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.MembershipStyle");

      PortableGroup::MembershipStyleValue msv =
        PortableGroup::MEMB_APP_CTRL;
      property.val <<= msv;

      this->basic_object_group_ = this->lm_->create_object (repository_id,
                                                            criteria,
                                                            this->basic_fcid_.out ()
                                                            ACE_ENV_ARG_PARAMETER);

      CORBA::String_var ior =
        this->orb_->object_to_string (this->basic_object_group_.in ()
                                      ACE_ENV_ARG_PARAMETER);

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
LB_server::create_simple_object_group (void)
{
  ACE_TRY_NEW_ENV
    {
      const char *repository_id = "IDL:Test/Simple:1.0";

      PortableGroup::Criteria criteria (1);
      criteria.length (1);

      PortableGroup::Property &property = criteria[0];
      property.nam.length (1);

      property.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.MembershipStyle");

      PortableGroup::MembershipStyleValue msv =
        PortableGroup::MEMB_APP_CTRL;
      property.val <<= msv;

      this->simple_object_group_ = this->lm_->create_object (repository_id,
                                                             criteria,
                                                             this->simple_fcid_.out ()
                                                             ACE_ENV_ARG_PARAMETER);

      CORBA::String_var ior =
        this->orb_->object_to_string (this->simple_object_group_.in ()
                                      ACE_ENV_ARG_PARAMETER);

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
LB_server::remove_basic_member (void)
{
  ACE_TRY_NEW_ENV
    {
      PortableGroup::Location location (1);
      location.length (1);

      location[0].id = CORBA::string_dup ("MyLocation 1");

      const char *loc = "MyLocation 1";

      this->lm_->remove_member (this->basic_object_group_.in (),
                                location
                                ACE_ENV_ARG_PARAMETER);

      printf("Removed Basic Member at location %s\n\n", loc);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception raised while deleting servant");
      return -1;
    }
  ACE_ENDTRY;

  return 1;
}

int
LB_server::remove_simple_member (void)
{
  ACE_TRY_NEW_ENV
    {
      PortableGroup::Location location (1);
      location.length (1);

      location[0].id = CORBA::string_dup ("MyLocation 1");

      const char *loc = "MyLocation 1";

      this->lm_->remove_member (this->simple_object_group_.in (),
                                location
                                ACE_ENV_ARG_PARAMETER);

      printf("Removed Simple Member at location %s\n\n", loc);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception raised while deleting servant");
      return -1;
    }
  ACE_ENDTRY;

  return 1;
}

int
LB_server::register_basic_servant (Basic *servant, const char *loc)
{
  ACE_TRY_NEW_ENV
    {
      Test::Basic_var basic =
        servant->_this ();

      PortableGroup::Location location (1);
      location.length (1);

      location[0].id = CORBA::string_dup (loc);

      this->lm_->add_member (this->basic_object_group_.in (),
                             location,
                             basic.in ()
                             ACE_ENV_ARG_PARAMETER);

      CORBA::Short number = 0;
      number = servant->number ();

      printf("Added Basic member %d at location %s\n", number, loc);
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

int
LB_server::register_simple_servant (Simple *servant, const char *loc)
{
  ACE_TRY_NEW_ENV
    {
      Test::Simple_var simple =
        servant->_this ();

      PortableGroup::Location location (1);
      location.length (1);

      location[0].id = CORBA::string_dup (loc);

      this->lm_->add_member (this->simple_object_group_.in (),
                             location,
                             simple.in ()
                             ACE_ENV_ARG_PARAMETER);

      CORBA::Short number = 0;
      number = servant->number ();

      printf("Added Simple member %d at location %s\n", number, loc);
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
