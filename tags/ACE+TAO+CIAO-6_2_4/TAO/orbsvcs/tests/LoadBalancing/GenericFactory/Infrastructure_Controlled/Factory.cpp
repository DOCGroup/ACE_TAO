// $Id$

#include "Factory.h"
#include "Simple.h"

#include "TestC.h"

Factory::Factory (void)
{
   this->fcid_ = 0;
}

CORBA::Object_ptr
Factory::create_object (
    const char * /*type_id*/,
    const PortableGroup::Criteria & /*the_criteria*/,
    PortableGroup::GenericFactory::FactoryCreationId_out fcid)
{
  Simple *servant;

  ACE_NEW_THROW_EX (servant,
                    Simple,
                    CORBA::NO_MEMORY ());


  PortableServer::ServantBase_var safe_servant = servant;

  CORBA::ULong tmp_fcid = 0;
  tmp_fcid = this->fcid_;

  while (this->factory_map_.find (this->fcid_) == 0)
    {
      this->fcid_++;
      if (this->fcid_ == tmp_fcid)
        throw PortableGroup::ObjectNotCreated ();
    }

  tmp_fcid = this->fcid_;

  PortableGroup::GenericFactory::FactoryCreationId *my_fcid = 0;

  ACE_NEW_THROW_EX (my_fcid,
                    PortableGroup::GenericFactory::FactoryCreationId,
                    CORBA::NO_MEMORY ());

  fcid = my_fcid;

  *my_fcid <<= tmp_fcid;

  this->poa_ =
    servant->_default_POA ();

  this->oid_ = this->poa_->servant_to_id (servant);

  return servant->_this ();

}

void
Factory::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId &
      fcid)
{
  CORBA::ULong my_fcid = 0;

  if (fcid >>= my_fcid)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);
      Factory_Map::ENTRY *entry = 0;
      if (this->factory_map_.find (my_fcid, entry) == 0)
        {
          if (this->factory_map_.unbind (my_fcid) != 0)
            throw CORBA::INTERNAL ();
        }
    }
  else
    throw PortableGroup::ObjectNotFound ();

  if (my_fcid == this->fcid_)
    {
      this->poa_->deactivate_object (this->oid_.in ());
    }
  else
    {
      throw PortableGroup::ObjectNotFound ();
    }
}
