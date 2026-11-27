#if !defined (ACE_HAS_INLINED_OSCALLS)
# undef ACE_INLINE
# define ACE_INLINE
#endif /* ACE_HAS_INLINED_OSCALLS */

ACE_INLINE
HTTP_Hdr_Node::operator int () const
{
  return this->index_;
}

ACE_INLINE
HTTP_Hdr_Node::operator char const * () const
{
  return this->token_;
}

ACE_INLINE char const *
HTTP_Hdr_Node::format () const
{
  return this->format_;
}

ACE_INLINE HTTP_Hdr_Node const &
HTTP_HCodes::hcode (int type) const
{
  HTTP_Hdr_Node const **hn = this->header_nodes_->find (type);

  // No error checking!
  return **hn;
}

ACE_INLINE char const *
HTTP_Headers::header_token (int name) const
{
  HTTP_Hdr_Node const **hn = this->header_nodes_->find (name);
  return hn && *hn ? static_cast<char const *> (**hn) : nullptr;
}

ACE_INLINE char const *
HTTP_Headers::header_strings (int name) const
{
  HTTP_Hdr_Node const **hn = this->header_nodes_->find (name);
  return ((hn && *hn) ? (*hn)->format () : nullptr);
}
