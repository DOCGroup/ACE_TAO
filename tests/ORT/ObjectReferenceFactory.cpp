// $Id$

#include "ObjectReferenceFactory.h"
#include "tao/debug.h"


ObjectReferenceFactory::ObjectReferenceFactory (
  PortableInterceptor::ObjectReferenceFactory * old_orf)
  : old_orf_ (old_orf)
{
  // Claim ownership of the old ObjectReferenceFactory.
  CORBA::add_ref (old_orf);
}

ObjectReferenceFactory::~ObjectReferenceFactory (void)
{
  // No need to call CORBA::remove_ref() on this->old_orf_.  It is a
  // "_var" object, meaning that will be done automatically.
}

CORBA::Object_ptr
ObjectReferenceFactory::make_object (
    const char *repository_id,
    const PortableInterceptor::ObjectId & id)
{
  ACE_ASSERT (repository_id != 0);

  ACE_DEBUG ((LM_INFO,
              "Invoked custom ObjectReferenceFactory::make_object()\n"));

  return this->old_orf_->make_object (repository_id,
                                      id);
}
