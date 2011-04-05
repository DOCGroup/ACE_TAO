// $Id$

#include "tao/PortableServer/Key_Adapters.h"

#include "ace/ACE.h"
#include "ace/OS_NS_string.h"
#include "ace/Map_T.h"

///////////////////////////////////////////////////////////////////////////////

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Incremental_Key_Generator::TAO_Incremental_Key_Generator (void)
  : counter_ (0)
{
}

int
TAO_Incremental_Key_Generator::operator() (PortableServer::ObjectId &id)
{
  // Resize to accommodate the counter.
  id.resize (sizeof this->counter_);
  
  char *tmp = reinterpret_cast<char *> (&this->counter_);
  
  for (CORBA::ULong i = 0; i < id.size (); ++i)
    {
      id[i] = tmp[i];
    }
/*
  // Add new key data.
  ACE_OS::memcpy (id.get_buffer (),
                  &++this->counter_,
                  sizeof this->counter_);
*/
  // Success.
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

u_long
TAO_ObjectId_Hash::operator () (const PortableServer::ObjectId &id) const
{
  return ACE::hash_pjw ((const char *) id.get_allocator ().address (*id.begin ()),
                        id.size ());
}

////////////////////////////////////////////////////////////////////////////////

int
TAO_Ignore_Original_Key_Adapter::encode (const PortableServer::ObjectId &,
                                         const ACE_Active_Map_Manager_Key &active_key,
                                         PortableServer::ObjectId &modified_key)
{
  // Size of active key.
  size_t const active_key_size = active_key.size ();

  // Resize to accommodate both the original data and the new active key.
  modified_key.resize (active_key_size);

  // Copy active key data into user key.
  active_key.encode (modified_key.get_allocator ().address (*modified_key.begin ()));

  // Success.
  return 0;
}

int
TAO_Ignore_Original_Key_Adapter::decode (const PortableServer::ObjectId &modified_key,
                                         ACE_Active_Map_Manager_Key &active_key)
{
  // Read off value of index and generation.
  active_key.decode (modified_key.get_allocator ().address (*modified_key.begin ()));

  // Success.
  return 0;
}

int
TAO_Ignore_Original_Key_Adapter::decode (const PortableServer::ObjectId &modified_key,
                                         PortableServer::ObjectId &original_key)
{
  original_key.resize (modified_key.size ());
  
  for (CORBA::ULong i = 0; i < modified_key.size (); ++i)
    {
      original_key[i] = modified_key[i];
    }
/*
  // Smartly copy all the data; <original_key does not own the data>.
  original_key.replace (modified_key.maximum (),
                        modified_key.length (),
                        const_cast <CORBA::Octet *>
                                   (modified_key.get_buffer ()),
                        0);
*/
  // Success.
  return 0;
}

////////////////////////////////////////////////////////////////////////////////

int
TAO_Preserve_Original_Key_Adapter::encode (const PortableServer::ObjectId &original_key,
                                           const ACE_Active_Map_Manager_Key &active_key,
                                           PortableServer::ObjectId &modified_key)
{
  // Size of active key.
  size_t const active_key_size = active_key.size ();

  // Resize to accommodate both the original data and the new active key.
  modified_key.resize (static_cast <CORBA::ULong> (active_key_size)
                         + original_key.size ());

  // Copy active key data into user key.
  active_key.encode (modified_key.get_allocator ().address (*modified_key.begin ()));
  
  for (CORBA::ULong i = 0; i < original_key.size (); ++i)
    {
      modified_key[i + active_key_size] = original_key[i];
    }
/*
  // Copy the original key after the active key.
  ACE_OS::memcpy (modified_key.get_buffer () + active_key_size,
                  original_key.get_buffer (),
                  original_key.length ());
*/
  // Success.
  return 0;
}

int
TAO_Preserve_Original_Key_Adapter::decode (const PortableServer::ObjectId &modified_key,
                                           ACE_Active_Map_Manager_Key &active_key)
{
  // Read off value of index and generation.
  active_key.decode (modified_key.get_allocator ().address (*modified_key.begin ()));

  // Success.
  return 0;
}

int
TAO_Preserve_Original_Key_Adapter::decode (const PortableServer::ObjectId &modified_key,
                                           PortableServer::ObjectId &original_key)
{
  // Size of active key.
  size_t const active_key_size = ACE_Active_Map_Manager_Key::size ();
  size_t newsize = modified_key.size () - active_key_size;
  original_key.resize (newsize);
  
  for (CORBA::ULong i = 0; i < newsize; ++i)
    {
      original_key[i] = modified_key[i + active_key_size];
    }
/*
  // Smartly copy all the data; <original_key does not own the data>.
  original_key.replace (static_cast <CORBA::ULong>
                            (modified_key.maximum () - active_key_size),
                        static_cast <CORBA::ULong>
                            (modified_key.length () - active_key_size),
                        const_cast <CORBA::Octet *>
                            (modified_key.get_buffer ()) + active_key_size,
                        0);
*/
  // Success.
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

