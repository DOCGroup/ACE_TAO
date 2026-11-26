#include <cstring>
#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
ACE_Active_Map_Manager_Key::ACE_Active_Map_Manager_Key ()
  : key_data_ {ACE_UINT32_MAX, 0}
{
  // If you change the slot index, please change ACE_Map_Manager::free_list_id()
  // accordingly.
}

ACE_INLINE
ACE_Active_Map_Manager_Key::ACE_Active_Map_Manager_Key (ACE_UINT32 slot_index,
                                                        ACE_UINT32 slot_generation)
  : key_data_ {slot_index, slot_generation}
{
}

ACE_INLINE ACE_UINT32
ACE_Active_Map_Manager_Key::slot_index () const
{
  return this->key_data_.slot_index_;
}

ACE_INLINE ACE_UINT32
ACE_Active_Map_Manager_Key::slot_generation () const
{
  return this->key_data_.slot_generation_;
}

ACE_INLINE bool
ACE_Active_Map_Manager_Key::operator== (ACE_Active_Map_Manager_Key const &rhs) const
{
  return
    this->key_data_.slot_index_ == rhs.key_data_.slot_index_ &&
    this->key_data_.slot_generation_ == rhs.key_data_.slot_generation_;
}

ACE_INLINE bool
ACE_Active_Map_Manager_Key::operator!= (ACE_Active_Map_Manager_Key const &rhs) const
{
  return !this->operator== (rhs);
}

ACE_INLINE void
ACE_Active_Map_Manager_Key::slot_index (ACE_UINT32 i)
{
  this->key_data_.slot_index_ = i;
}

ACE_INLINE void
ACE_Active_Map_Manager_Key::slot_generation (ACE_UINT32 g)
{
  this->key_data_.slot_generation_ = g;
}

ACE_INLINE void
ACE_Active_Map_Manager_Key::increment_slot_generation_count ()
{
  ++this->key_data_.slot_generation_;
}

/* static */
ACE_INLINE size_t
ACE_Active_Map_Manager_Key::size ()
{
  return sizeof (ACE_UINT32) + sizeof (ACE_UINT32);
}

ACE_INLINE void
ACE_Active_Map_Manager_Key::decode (void const *data)
{
  // Copy the information from the user buffer into the key.
  std::memcpy (std::addressof (this->key_data_), data, sizeof this->key_data_);
}

ACE_INLINE void
ACE_Active_Map_Manager_Key::encode (void *data) const
{
  // Copy the key data to the user buffer.
  std::memcpy (data, std::addressof (this->key_data_), sizeof this->key_data_);
}

ACE_END_VERSIONED_NAMESPACE_DECL
