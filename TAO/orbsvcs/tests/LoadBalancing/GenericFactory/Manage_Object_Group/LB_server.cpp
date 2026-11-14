#include "LB_server.h"
#include "Basic.h"
#include "Simple.h"

#include <cstdio>
#include <string>

LB_server::LB_server (int argc, ACE_TCHAR **argv)
  : argc_ (argc),
    argv_ (argv)
{
}

int
LB_server::destroy ()
{
  try
    {
      if (!CORBA::is_nil (this->lm_.in ()))
        {
          if (!CORBA::is_nil (this->basic_object_group_.in ()))
            this->lm_->delete_object (this->basic_fcid_.in ());
          if (!CORBA::is_nil (this->simple_object_group_.in ()))
            this->lm_->delete_object (this->simple_fcid_.in ());
        }

      if (!CORBA::is_nil (this->root_poa_.in ()))
        this->root_poa_->destroy (true, true);

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
LB_server::orb ()
{
  return this->orb_.in ();
}

CORBA::Object_ptr
LB_server::get_basic_object_group ()
{
  return this->basic_object_group_.in ();
}

CORBA::Object_ptr
LB_server::get_simple_object_group ()
{
  return this->simple_object_group_.in ();
}

CosLoadBalancing::LoadManager_ptr
LB_server::load_manager ()
{
  return this->lm_.in ();
}

int
LB_server::start_orb_and_poa ()
{
  try
    {
      // Initialise the ORB.
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

      this->lm_ =
        CosLoadBalancing::LoadManager::_narrow (obj.in ());

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
LB_server::create_basic_object_group ()
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

      PortableGroup::MembershipStyleValue const msv =
        PortableGroup::MEMB_APP_CTRL;
      property.val <<= msv;

      this->basic_object_group_ = this->lm_->create_object (repository_id,
                                                            criteria,
                                                            this->basic_fcid_.out ());
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
LB_server::create_simple_object_group ()
{
  try
    {
      char const * const repository_id = "IDL:Test/Simple:1.0";

      PortableGroup::Criteria criteria (1);
      criteria.length (1);

      PortableGroup::Property &property = criteria[0];
      property.nam.length (1);

      property.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.MembershipStyle");

      PortableGroup::MembershipStyleValue const msv =
        PortableGroup::MEMB_APP_CTRL;
      property.val <<= msv;

      this->simple_object_group_ = this->lm_->create_object (repository_id,
                                                             criteria,
                                                             this->simple_fcid_.out ());
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
LB_server::remove_basic_member ()
{
  try
    {
      PortableGroup::Location location (1);
      location.length (1);

      location[0].id = CORBA::string_dup ("MyLocation 1");

      std::string const loc = "MyLocation 1";

      this->lm_->remove_member (this->basic_object_group_.in (),
                                location);

      std::printf ("Removed Basic Member at location %s\n\n", loc.c_str ());
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception raised while deleting servant");
      return -1;
    }

  return 1;
}

int
LB_server::remove_simple_member ()
{
  try
    {
      PortableGroup::Location location (1);
      location.length (1);

      location[0].id = CORBA::string_dup ("MyLocation 1");

      std::string const loc = "MyLocation 1";

      this->lm_->remove_member (this->simple_object_group_.in (),
                                location);

      std::printf ("Removed Simple Member at location %s\n\n", loc.c_str ());
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception raised while deleting servant");
      return -1;
    }

  return 1;
}

int
LB_server::register_basic_servant (Basic *servant, std::string const &loc)
{
  try
    {
      Test::Basic_var basic =
        servant->_this ();

      PortableGroup::Location location (1);
      location.length (1);

      location[0].id = CORBA::string_dup (loc.c_str ());

      this->lm_->add_member (this->basic_object_group_.in (),
                             location,
                             basic.in ());

      CORBA::Short const number = servant->number ();

      std::printf ("Added Basic member %d at location %s\n",
                   number,
                   loc.c_str ());
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception raised while registering servant");
      return -1;
    }

  return 1;
}

int
LB_server::register_simple_servant (Simple *servant, std::string const &loc)
{
  try
    {
      Test::Simple_var simple =
        servant->_this ();

      PortableGroup::Location location (1);
      location.length (1);

      location[0].id = CORBA::string_dup (loc.c_str ());

      this->lm_->add_member (this->simple_object_group_.in (),
                             location,
                             simple.in ());

      CORBA::Short const number = servant->number ();

      std::printf ("Added Simple member %d at location %s\n",
                   number,
                   loc.c_str ());
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception raised while registering servant");
      return -1;
    }

  return 1;
}
