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
ACE_Active_Map_Manager_Key::operator== (const ACE_Active_Map_Manager_Key &rhs)
{
  return 
    this->index_ == rhs.index_ &&
    this->generation_ == rhs.generation_;
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

