// $Id$

ACE_INLINE
ACE_Active_Map_Manager_Key::ACE_Active_Map_Manager_Key (void)
  : slot_index_ (~0),
    slot_generation_ (0)
{
  // If you change ~0, please change ACE_Map_Manager::free_list_id()
  // accordingly.
}

ACE_INLINE
ACE_Active_Map_Manager_Key::ACE_Active_Map_Manager_Key (ACE_UINT32 slot_index,
                                                        ACE_UINT32 slot_generation)
  : slot_index_ (slot_index),
    slot_generation_ (slot_generation)
{
}

ACE_INLINE ACE_UINT32
ACE_Active_Map_Manager_Key::slot_index (void) const
{
  return this->slot_index_;
}

ACE_INLINE ACE_UINT32
ACE_Active_Map_Manager_Key::slot_generation (void) const
{
  return this->slot_generation_;
}

ACE_INLINE int
ACE_Active_Map_Manager_Key::operator== (const ACE_Active_Map_Manager_Key &rhs) const
{
  return
    this->slot_index_ == rhs.slot_index_ &&
    this->slot_generation_ == rhs.slot_generation_;
}

ACE_INLINE int
ACE_Active_Map_Manager_Key::operator!= (const ACE_Active_Map_Manager_Key &rhs) const
{
  return !this->operator== (rhs);
}

ACE_INLINE void
ACE_Active_Map_Manager_Key::slot_index (ACE_UINT32 i)
{
  this->slot_index_ = i;
}

ACE_INLINE void
ACE_Active_Map_Manager_Key::slot_generation (ACE_UINT32 g)
{
  this->slot_generation_ = g;
}

ACE_INLINE void
ACE_Active_Map_Manager_Key::increment_slot_generation_count (void)
{
  ++this->slot_generation_;
}

/* static */
ACE_INLINE size_t
ACE_Active_Map_Manager_Key::size (void)
{
  return sizeof (ACE_UINT32) + sizeof (ACE_UINT32);
}

ACE_INLINE void
ACE_Active_Map_Manager_Key::decode (const void *d)
{
  // Cast so that we can do pointer arithmetic.
  const char *data = (const char *) d;

  // Grab the slot_index first.
  ACE_OS::memcpy (&this->slot_index_,
                  data,
                  sizeof this->slot_index_);

  // Move along...
  data += sizeof this->slot_index_;

  // Grab the slot_generation second.
  ACE_OS::memcpy (&this->slot_generation_,
                  data,
                  sizeof this->slot_generation_);
}

ACE_INLINE void
ACE_Active_Map_Manager_Key::encode (void *d) const
{
  // Cast so that we can do pointer arithmetic.
  char *data = (char *) d;

  // Grab the slot_index first.
  ACE_OS::memcpy (data,
                  &this->slot_index_,
                  sizeof this->slot_index_);

  // Move along...
  data += sizeof this->slot_index_;

  // Grab the slot_generation second.
  ACE_OS::memcpy (data,
                  &this->slot_generation_,
                  sizeof this->slot_generation_);
}
