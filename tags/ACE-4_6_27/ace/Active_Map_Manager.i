// $Id$

ACE_INLINE 
ACE_Active_Map_Manager_Key::ACE_Active_Map_Manager_Key (void)
  : index_ (~0),
    generation_ (0)
{
  // If you change ~0, please change ACE_Map_Manager::free_list_id()
  // accordingly.
}

ACE_INLINE 
ACE_Active_Map_Manager_Key::ACE_Active_Map_Manager_Key (u_long index, 
                                                        u_long generation)
  : index_ (index),
    generation_ (generation)
{
}

ACE_INLINE u_long 
ACE_Active_Map_Manager_Key::index (void) const
{
  return this->index_;
}
  
ACE_INLINE u_long 
ACE_Active_Map_Manager_Key::generation (void) const
{
  return this->generation_;
}

ACE_INLINE int 
ACE_Active_Map_Manager_Key::operator== (const ACE_Active_Map_Manager_Key &rhs) const
{
  return 
    this->index_ == rhs.index_ &&
    this->generation_ == rhs.generation_;
}

ACE_INLINE int 
ACE_Active_Map_Manager_Key::operator!= (const ACE_Active_Map_Manager_Key &rhs) const
{
  return !this->operator== (rhs);
}

ACE_INLINE void 
ACE_Active_Map_Manager_Key::index (u_long i)
{
  this->index_ = i;
}
  
ACE_INLINE void 
ACE_Active_Map_Manager_Key::generation (u_long g)
{
  this->generation_ = g;
}

ACE_INLINE void 
ACE_Active_Map_Manager_Key::increment_generation_count (void)
{
  ++this->generation_;
}

/* static */
ACE_INLINE size_t
ACE_Active_Map_Manager_Key::size (void)
{
  return sizeof (u_long) + sizeof (u_long);
}

ACE_INLINE void 
ACE_Active_Map_Manager_Key::decode (const void *d)
{
  // Cast so that we can do pointer arithmetic.
  const char *data = (const char *) d;

  // Grab the index first.
  ACE_OS::memcpy (&this->index_,
                  data,
                  sizeof this->index_);

  // Move along...
  data += sizeof this->index_;

  // Grab the generation second.
  ACE_OS::memcpy (&this->generation_,
                  data,
                  sizeof this->generation_);
}

ACE_INLINE void 
ACE_Active_Map_Manager_Key::encode (void *d) const
{
  // Cast so that we can do pointer arithmetic.
  char *data = (char *) d;

  // Grab the index first.
  ACE_OS::memcpy (data,
                  &this->index_,
                  sizeof this->index_);

  // Move along...
  data += sizeof this->index_;

  // Grab the generation second.
  ACE_OS::memcpy (data,
                  &this->generation_,
                  sizeof this->generation_);
}
