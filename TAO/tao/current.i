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
TAO_POA_Current_Impl::clear (void)
{
  poa_impl_ = 0;
  object_id_ = 0;
  in_upcall_ = 0;
  object_key_ = 0;
}

int
TAO_POA_Current_Impl::context_is_valid (void)
{
  return (poa_impl_ != 0 && object_id_ != 0 && in_upcall_ != 0 && object_key_ != 0);
}

void
TAO_POA_Current_Impl::POA_impl (TAO_POA *impl)
{
  poa_impl_ = impl;
}

TAO_POA *
TAO_POA_Current_Impl::POA_impl (void)
{
  return poa_impl_;
}

void
TAO_POA_Current_Impl::object_id (const PortableServer::ObjId &the_id)
{
  object_id_ = &the_id;
}

const PortableServer::ObjId &
TAO_POA_Current_Impl::object_id (void)
{
  // This seems very tenuous
  return *object_id_;
}

void
TAO_POA_Current_Impl::object_key (const TAO::ObjectKey &the_key)
{
  object_key_ = &the_key;
}

const TAO::ObjectKey &
TAO_POA_Current_Impl::object_key (void)
{
  return &object_key_;
}

void
TAO_POA_Current_Impl::in_upcall (int yesno)
{
  in_upcall_ = yesno;
}

int
TAO_POA_Current_Impl::in_upcall (void)
{
  return in_upcall_;
}
