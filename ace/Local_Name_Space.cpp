// Local_Name_Space.cpp
// $Id$

#if !defined (ACE_LOCAL_NAME_SPACE_C)
#define ACE_LOCAL_NAME_SPACE_C

#define ACE_BUILD_DLL
#include "ace/ACE.h"
#include "ace/Local_Name_Space.h"

ACE_USHORT16 *
ACE_NS_String::fast_rep (void) const
{
  ACE_TRACE ("ACE_NS_String::fast_rep");
  return this->rep_;
}

ACE_NS_String::operator ACE_WString () const
{
  ACE_TRACE ("ACE_NS_String::operator ACE_WString");
  return ACE_WString (this->rep_, (this->len_ / sizeof (ACE_USHORT16)) - 1);
}

size_t
ACE_NS_String::len (void) const
{
  ACE_TRACE ("ACE_NS_String::len");
  return this->len_;
}

char *
ACE_NS_String::char_rep (void) const
{
  ACE_TRACE ("ACE_NS_String::char_rep");
  ACE_WString w_string (this->rep_, (this->len_ / sizeof (ACE_USHORT16)) - 1);
  return w_string.char_rep ();
}

ACE_NS_String::ACE_NS_String (void)
  : len_ (0),
    rep_ (0)
{
  ACE_TRACE ("ACE_NS_String::ACE_NS_String");
}

ACE_NS_String::ACE_NS_String (const ACE_WString &s)
  : len_ ((s.length () + 1) * sizeof (ACE_USHORT16)),
    rep_ ((ACE_USHORT16 *) s.fast_rep ())
{
  ACE_TRACE ("ACE_NS_String::ACE_NS_String");
}

int
ACE_NS_String::strstr (const ACE_NS_String &s) const
{
  ACE_TRACE ("ACE_NS_String::strstr");

  if (this->len_ < s.len_)
    // If they're larger than we are they can't be a substring of us!
    return -1;
  else if (this->len_ == s.len_)
    // Check if we're equal.
    return *this == s ? 0 : -1;
  else
    {
      // They're smaller than we are...
      size_t len = (this->len_ - s.len_) / sizeof (ACE_USHORT16);
      size_t pat_len = s.len_ / sizeof (ACE_USHORT16) - 1;
      
      for (size_t i = 0; i <= len; i++)
	{
	  size_t j;

	  for (j = 0; j < pat_len; j++)
	    if (this->rep_[i + j] != s.rep_[j])
	      break;
	  
	  if (j == pat_len)
	    // Found a match!  Return the index.
	    return i;
	}
      
      return -1;
    }
}

int
ACE_NS_String::operator == (const ACE_NS_String &s) const
{
  ACE_TRACE ("ACE_NS_String::operator ==");
  return this->len_ == s.len_
    && ACE_OS::memcmp ((void *) this->rep_, 
		       (void *) s.rep_, this->len_) == 0;
}

int
ACE_NS_String::operator != (const ACE_NS_String &s) const
{
  ACE_TRACE ("ACE_NS_String::operator !=");
  return !this->operator == (s);
}

ACE_NS_String::ACE_NS_String (ACE_USHORT16 *dst, 
			      const ACE_USHORT16 *src, 
			      size_t bytes)
  : len_ (bytes), 
    rep_ (dst)
{
  ACE_TRACE ("ACE_NS_String::ACE_NS_String");
  ACE_OS::memcpy (this->rep_, src, bytes);
}

size_t 
ACE_NS_String::hash (void) const
{
  return ACE::hash_pjw (this->rep_);
}

ACE_NS_Internal::ACE_NS_Internal (void)
{
}

ACE_NS_Internal::ACE_NS_Internal (ACE_NS_String &value, const char *type)
  : value_ (value),
    type_ (type)
{
  ACE_TRACE ("ACE_NS_Internal::ACE_NS_Internal");
}

int
ACE_NS_Internal::operator == (const ACE_NS_Internal &s) const
{  
  ACE_TRACE ("ACE_NS_Internal::operator ==");
  return this->value_ == s.value_;
}

ACE_NS_String
ACE_NS_Internal::value (void)
{
  ACE_TRACE ("ACE_NS_Internal::value");
  return this->value_;
}

const char *
ACE_NS_Internal::type (void)
{
  ACE_TRACE ("ACE_NS_Internal::type");
  return this->type_;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
#if (1)
template class ACE_Hash_Map_Manager<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry <ACE_NS_String, ACE_NS_Internal>;
#else
template class ACE_Map_Manager<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Map_Entry <ACE_NS_String, ACE_NS_Internal>;
#endif
template class ACE_Unbounded_Set<ACE_Name_Binding>;
template class ACE_Unbounded_Set_Iterator<ACE_Name_Binding>;
template class ACE_Unbounded_Set<ACE_WString>;
template class ACE_Unbounded_Set_Iterator<ACE_WString>;
template class ACE_Set_Node<ACE_WString>;
template class ACE_Set_Node<ACE_Name_Binding>;
template class ACE_Guard<ACE_RW_Process_Mutex>;
template class ACE_Read_Guard<ACE_RW_Process_Mutex>;
template class ACE_Write_Guard<ACE_RW_Process_Mutex>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
#endif /* ACE_LOCAL_NAME_SPACE_C */
