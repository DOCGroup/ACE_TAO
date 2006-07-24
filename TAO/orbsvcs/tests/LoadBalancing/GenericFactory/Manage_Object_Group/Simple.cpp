#include "Simple.h"
#include "LB_server.h"


ACE_RCSID (Application_Controlled,
           Simple,
           "$Id$")


Simple::Simple (CORBA::Object_ptr object_group,
                CosLoadBalancing::LoadManager_ptr lm,
                CORBA::ORB_ptr orb,
                CORBA::Short num,
                const char *loc)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    number_ (num)
{
  this->object_group_ = CORBA::Object::_duplicate (object_group);
  this->lm_ = CosLoadBalancing::LoadManager::_duplicate (lm);
  this->location_ = CORBA::string_dup (loc);
}

CORBA::Short
Simple::number (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->number_;
}

char *
Simple::get_string (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->location_);
}

void
Simple::remove_member (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      PortableGroup::Location location (1);
      location.length (1);
      location[0].id = CORBA::string_dup (this->location_);
      this->lm_->remove_member (this->object_group_.in (),
                                location
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Removed Member at Location <%s>\n",
                      this->location_));

    }
  ACE_CATCH (PortableGroup::ObjectNotFound, ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "Caught exception in remove_member");
      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_CATCH (PortableGroup::MemberNotFound, ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "Caught exception in remove_member");
      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught while destroying member\n");
    }
  ACE_ENDTRY;

}

void
Simple::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
