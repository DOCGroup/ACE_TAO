// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace TAO_FtRt
{

ACE_INLINE
UUID::UUID()
{
  create(rep_.uuid);
}

ACE_INLINE
UUID::UUID(int)
{
}

/// construct an UUID from the binary represetation
ACE_INLINE
UUID::UUID(const unsigned char* id)
{
  ACE_OS::memcpy(rep_.uuid, id, 16);
}

ACE_INLINE
bool UUID::operator == (const UUID& other) const
{
  return ACE_OS::memcmp(this->rep_.uuid, other.rep_.uuid, BINRARY_LENGTH) == 0;
}

ACE_INLINE
bool UUID::operator != (const UUID& other) const
{
  return !(*this == other);
}


ACE_INLINE
bool UUID::is_valid() const
{
  return !this->rep_.timestamp.hi;
}


ACE_INLINE
void UUID::to_binary(unsigned char* binary_rep) const
{
  ACE_OS::memcpy(binary_rep, rep_.uuid, 16);
}

ACE_INLINE
void UUID::to_string(ACE_CString& string) const
{
  string.resize(STRING_LENGTH-1);
  this->to_string(&string[0]);
}
}
TAO_END_VERSIONED_NAMESPACE_DECL
