// -*- C++ -*-
//
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
TAO_Target_Specification::target_specifier (TAO::ObjectKey const &key)
{
  this->specifier_ = TAO_Target_Specification::Key_Addr;
  // @@ Bala: this is a good recipe for a crash, if the <key> was on
  //    the stack or is otherwise destroyed then you are in big
  //    trouble.
  // @@ Carlos: As suggested by you I have documented that in the
  //    headerfile.
  // @@ Bala: beware, documentation is good, code that works in
  // general is better....  but you are probably right in this case, i
  // suspect this stuff goes right in the critical path, right? So
  // making a copy of the object key would be too expensive..
  this->u_.object_key_ = &key;
}

ACE_INLINE void
TAO_Target_Specification::target_specifier (IOP::TaggedProfile const &profile)
{
  this->specifier_ = TAO_Target_Specification::Profile_Addr;
  this->u_.profile_ = &profile;
}

ACE_INLINE void
TAO_Target_Specification::target_specifier (IOP::IOR &ior,
                                            CORBA::ULong prof_index)
{
  this->specifier_ = TAO_Target_Specification::Reference_Addr;
  this->u_.ior_ = const_cast<IOP::IOR *> (&ior);
  this->profile_index_ = prof_index;

}

ACE_INLINE TAO::ObjectKey const *
TAO_Target_Specification::object_key () const
{
  if (this->specifier_ == TAO_Target_Specification::Key_Addr)
    return this->u_.object_key_;

  return nullptr;
}

ACE_INLINE IOP::TaggedProfile const *
TAO_Target_Specification::profile () const
{
  if (this->specifier_ == TAO_Target_Specification::Profile_Addr)
    return this->u_.profile_;

  return nullptr;
}

ACE_INLINE CORBA::ULong
TAO_Target_Specification::iop_ior (IOP::IOR *&ior) const
{
  if (this->specifier_ == TAO_Target_Specification::Reference_Addr)
    {
      ior = this->u_.ior_;
      return this->profile_index_;
    }

  ior = nullptr;
  return 0;
}

ACE_INLINE TAO_Target_Specification::TAO_Target_Address
TAO_Target_Specification::specifier () const
{
  return this->specifier_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
