// $Id$

ACE_INLINE void
ACE_RMCast_Header_Size::add_to_header_size (size_t size)
{
  this->header_size_ += size;
}

ACE_INLINE size_t
ACE_RMCast_Header_Size::header_size (void) const
{
  return this->header_size_;
}

