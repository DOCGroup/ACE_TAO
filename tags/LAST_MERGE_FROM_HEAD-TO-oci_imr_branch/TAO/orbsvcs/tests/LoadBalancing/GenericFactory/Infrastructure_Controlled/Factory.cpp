#include "Factory.h"
#include "Simple.h"

#include "TestC.h"

ACE_RCSID (Infrastructure_Controlled,
           Factory,
           "$Id$");

Factory::Factory (void)
{
   this->fcid_ = 0;
}

CORBA::Object_ptr
Factory::create_object (
    const char * /*type_id*/,
    const PortableGroup::Criteria & /*the_criteria*/,
    PortableGroup::GenericFactory::FactoryCreationId_out fcid
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::InvalidProperty,
                   PortableGroup::CannotMeetCriteria))
{
  Simple *servant;

  ACE_NEW_THROW_EX (servant,
		    Simple,
		    CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ServantBase_var safe_servant = servant;

  CORBA::ULong tmp_fcid = 0;
  tmp_fcid = this->fcid_; 

  while (this->factory_map_.find (this->fcid_) == 0)
    {
      this->fcid_++;
      if (this->fcid_ == tmp_fcid)
        ACE_THROW_RETURN (PortableGroup::ObjectNotCreated (),
			  CORBA::Object::_nil ());
    }

  tmp_fcid = this->fcid_;

  PortableGroup::GenericFactory::FactoryCreationId *my_fcid = 0;

  ACE_NEW_THROW_EX (my_fcid,
		    PortableGroup::GenericFactory::FactoryCreationId,
		    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  fcid = my_fcid;

  *my_fcid <<= tmp_fcid;

  this->poa_ =
    servant->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  this->oid_ = this->poa_->servant_to_id (servant
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);

}

void
Factory::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId &
      fcid
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   PortableGroup::ObjectNotFound))
{
  CORBA::ULong my_fcid = 0;

  if (fcid >>= my_fcid)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);
      Factory_Map::ENTRY *entry = 0;
      if (this->factory_map_.find (my_fcid, entry) == 0)
        {
	  if (this->factory_map_.unbind (my_fcid) != 0)
	    ACE_THROW (CORBA::INTERNAL ());
	}
    }
  else
    ACE_THROW (PortableGroup::ObjectNotFound ());

  if (my_fcid == this->fcid_)
    {
      this->poa_->deactivate_object (this->oid_.in ()
		                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (PortableGroup::ObjectNotFound ());
    }
}
