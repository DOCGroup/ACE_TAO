//
// $Id$
//
#include "Basic.h"
#include "LB_server.h"

ACE_RCSID (GenericFactory, Basic, "$Id$")

Basic::Basic (CORBA::Object_ptr object_group,
              CosLoadBalancing::LoadManager_ptr lm,
              CORBA::ORB_ptr orb,
              const char *loc)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
  this->object_group_ = CORBA::Object::_duplicate (object_group);
  this->lm_ = CosLoadBalancing::LoadManager::_duplicate (lm);
  this->location_.length (1);
  this->location_[0].id = CORBA::string_dup (loc);
}

char *
Basic::get_string (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO Load Balancing Works!");
}

void
Basic::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->lm_->remove_member (this->object_group_.in (),
                                this->location_
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (PortableGroup::ObjectNotFound, ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "Caught exception");
      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_CATCH (PortableGroup::MemberNotFound, ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "Caught exception");
      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_ENDTRY;

}
