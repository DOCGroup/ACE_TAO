// -*- C++ -*-

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_INLINE
  Profile_Transport_Resolver::Profile_Transport_Resolver (
    CORBA::Object *p,
    TAO_Stub *stub,
    bool block)
    : obj_ (p)
    , stub_ (stub)
    , transport_ (0)
    , profile_ (0)
    , inconsistent_policies_ (0)
    , is_released_ (false)
    , blocked_ (block)
  {
  }

  ACE_INLINE TAO_Stub *
  Profile_Transport_Resolver::stub () const
  {
    return this->stub_;
  }

  ACE_INLINE CORBA::Object_ptr
  Profile_Transport_Resolver::object () const
  {
    return this->obj_;
  }

  ACE_INLINE TAO_Profile *
  Profile_Transport_Resolver::profile () const
  {
    return this->profile_;
  }

  ACE_INLINE TAO_Transport *
  Profile_Transport_Resolver::transport () const
  {
    return this->transport_.get ();
  }

  ACE_INLINE bool
  Profile_Transport_Resolver::blocked_connect () const
  {
    return this->blocked_;
  }

  ACE_INLINE void
  Profile_Transport_Resolver::transport_released () const
  {
    this->is_released_ = true;
  }

  ACE_INLINE CORBA::PolicyList *
  Profile_Transport_Resolver::inconsistent_policies () const
  {
    return this->inconsistent_policies_;
  }

  ACE_INLINE CORBA::PolicyList *
  Profile_Transport_Resolver::steal_inconsistent_policies ()
  {
    CORBA::PolicyList *tmp = this->inconsistent_policies_;
    this->inconsistent_policies_ = 0;
    return tmp;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
