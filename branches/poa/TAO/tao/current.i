// ============================================================================
//
// = LIBRARY
//     TAO
// 
// = FILENAME
//     current.cpp
//
// = VERSION
//     $Id$
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================


int
TAO_POA_Current::clear (void)
{
  poa_impl_ = 0;
  object_id_ = 0;
  in_upcall_ = 0;
  object_key_ = 0;
}

int
TAO_POA_Current::context_is_valid (void)
{
  return (poa_impl_ != 0 && object_id_ != 0 && in_upcall_ != 0 && object_key_ != 0);
}

void
TAO_POA_Current::POA (TAO_POA *impl)
{
  poa_impl_ = impl;
}

TAO_POA *
TAO_POA_Current::POA (void)
{
  return poa_impl_;
}

void
TAO_POA_Current::object_id (const PortableServer::ObjectId &id)
{
  object_id_ = &id;
}

const PortableServer::ObjectId &
TAO_POA_Current::object_id (void)
{
  return *object_id_;
}

void
TAO_POA_Current::object_key (const TAO::ObjectKey &key)
{
  object_key_ = &key;
}

const TAO::ObjectKey &
TAO_POA_Current::object_key (void)
{
  return *object_key_;
}

void
TAO_POA_Current::in_upcall (int yesno)
{
  in_upcall_ = yesno;
}

int
TAO_POA_Current::in_upcall (void)
{
  return in_upcall_;
}
