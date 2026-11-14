#include "Factory.h"
#include "Simple.h"

#include "TestC.h"
#include "ace/OS_NS_unistd.h"

Factory::Factory (int id, int pong_delay)
  : fcid_ (0),
    id_ (id),
    pong_delay_ (pong_delay)
{
}

CORBA::Object_ptr
Factory::create_object (
    const char * /*type_id*/,
    const PortableGroup::Criteria & /*the_criteria*/,
    PortableGroup::GenericFactory::FactoryCreationId_out fcid)
{
  Simple *servant {};

  ACE_NEW_THROW_EX (servant,
                    Simple (this->id_, this->pong_delay_),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var safe_servant = servant;

  CORBA::ULong tmp_fcid = this->fcid_;
  while (this->factory_map_.find (this->fcid_) == 0)
    {
      ++this->fcid_;
      if (this->fcid_ == tmp_fcid)
        throw PortableGroup::ObjectNotCreated ();
    }

  tmp_fcid = this->fcid_;
  PortableGroup::GenericFactory::FactoryCreationId_var my_fcid;
  PortableGroup::GenericFactory::FactoryCreationId *my_fcid_ptr {};
  ACE_NEW_THROW_EX (my_fcid_ptr,
                    PortableGroup::GenericFactory::FactoryCreationId,
                    CORBA::NO_MEMORY ());
  my_fcid = my_fcid_ptr;
  *my_fcid <<= tmp_fcid;
  fcid = my_fcid._retn ();

  this->poa_ =
    servant->_default_POA ();

  this->oid_ = this->poa_->servant_to_id (servant);

  obj_ = servant->_this ();

  return obj_._retn ();
}

void
Factory::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId & fcid)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T delete_object\n"));
  ACE_OS::sleep (10);
  CORBA::ULong my_fcid {};

  if (!(fcid >>= my_fcid))
    throw PortableGroup::ObjectNotFound ();

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);
    Factory_Map::ENTRY *entry {};
    if (this->factory_map_.find (my_fcid, entry) == 0
        && this->factory_map_.unbind (my_fcid) != 0)
      throw CORBA::INTERNAL ();
  }

  if (my_fcid != this->fcid_)
    throw PortableGroup::ObjectNotFound ();

  this->poa_->deactivate_object (this->oid_.in ());
}
