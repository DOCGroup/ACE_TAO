#include "ObjectReferenceFactory.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/debug.h"

ACE_RCSID (Hello,
           ObjectReferenceFactory,
           "$Id$")

ObjectReferenceFactory::ObjectReferenceFactory (
  PortableInterceptor::ObjectReferenceFactory * old_orf)
  : old_orf_ (old_orf)
{
  CORBA::add_ref (old_orf);
}

ObjectReferenceFactory::~ObjectReferenceFactory (void)
{
}

CORBA::Object_ptr
ObjectReferenceFactory::make_object (
    const char *repository_id,
    const PortableInterceptor::ObjectId & id)
{
  ACE_ASSERT (repository_id != 0);

  CORBA::String_var s = PortableServer::ObjectId_to_string (id);

  CORBA::Object_var ref = this->old_orf_->make_object (repository_id, id);

  TAO_MProfile &mp = ref->_stubobj ()->base_profiles ();

  IOP::TaggedComponent mytag;
  const char* tag = s.in ();
  CORBA::ULong tag_id = 9654;
  size_t tag_length = ACE_OS::strlen (tag); 
  mytag.tag = tag_id;
  mytag.component_data.length (tag_length + 1);

  CORBA::Octet *buf = mytag.component_data.get_buffer ();
  ACE_OS::memcpy (buf, tag, tag_length + 1);
  buf[tag_length] = '\0';

  const CORBA::ULong profile_count = mp.profile_count ();

  for (CORBA::ULong i = 0; i < profile_count; ++i)
    {
      TAO_Profile *profile = mp.get_profile (i);
      profile->add_tagged_component (mytag);
    }

  return ref._retn ();
}
