#ifndef ACE_LOCAL_NAME_SPACE_CPP
#define ACE_LOCAL_NAME_SPACE_CPP

#include "ace/ACE.h"
#include "ace/Local_Name_Space.h"
#include "ace/RW_Process_Mutex.h"

ACE_RCSID (ace,
           Local_Name_Space,
           "$Id$")

ACE_NS_String::~ACE_NS_String (void)
{
  if (this->delete_rep_)
    delete [] this->rep_;
}

ACE_WCHAR_T *
ACE_NS_String::fast_rep (void) const
{
  ACE_TRACE ("ACE_NS_String::fast_rep");
  return this->rep_;
}

ACE_NS_String::operator ACE_NS_WString () const
{
  ACE_TRACE ("ACE_NS_String::operator ACE_NS_WString");
  return ACE_NS_WString (this->rep_,
                         (this->len_ / sizeof (ACE_WCHAR_T)) - 1);
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
  ACE_NS_WString w_string (this->rep_,
                           (this->len_ / sizeof (ACE_WCHAR_T)) - 1);
  return w_string.char_rep ();
}

ACE_NS_String::ACE_NS_String (void)
  : len_ (0),
    rep_ (0),
    delete_rep_ (0)
{
  ACE_TRACE ("ACE_NS_String::ACE_NS_String");
}

ACE_NS_String::ACE_NS_String (const ACE_NS_WString &s)
  : len_ ((s.length () + 1) * sizeof (ACE_WCHAR_T)),
    rep_ (s.rep ()),
    delete_rep_ (1)
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
      const size_t len = (this->len_ - s.len_) / sizeof (ACE_WCHAR_T);
      const size_t pat_len = s.len_ / sizeof (ACE_WCHAR_T) - 1;

      for (size_t i = 0; i <= len; ++i)
        {
          size_t j;

          for (j = 0; j < pat_len; ++j)
            if (this->rep_[i + j] != s.rep_[j])
              break;

          if (j == pat_len)
            // Found a match!  Return the index.
            return ACE_static_cast (int, i);
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

ACE_NS_String::ACE_NS_String (ACE_WCHAR_T *dst,
                              const ACE_WCHAR_T *src,
                              size_t bytes)
  : len_ (bytes),
    rep_ (dst),
    delete_rep_ (0)
{
  ACE_TRACE ("ACE_NS_String::ACE_NS_String");
  ACE_OS::memcpy (this->rep_, src, bytes);
}

u_long
ACE_NS_String::hash (void) const
{
  return ACE::hash_pjw
    (ACE_reinterpret_cast (char *, ACE_const_cast (ACE_WCHAR_T *,
                                                   this->rep_)),
     this->len_);
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#if (1)
template class ACE_Hash_Map_Entry<ACE_NS_String, ACE_NS_Internal>;
template class ACE_Hash<ACE_NS_String>;
template class ACE_Equal_To<ACE_NS_String>;
template class ACE_Hash_Map_Manager_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>;
#else
template class ACE_Map_Entry<ACE_NS_String, ACE_NS_Internal>;
template class ACE_Map_Manager<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
#endif
template class ACE_Unbounded_Set<ACE_Name_Binding>;
template class ACE_Unbounded_Set_Iterator<ACE_Name_Binding>;
template class ACE_Unbounded_Set<ACE_NS_WString>;
template class ACE_Unbounded_Set_Iterator<ACE_NS_WString>;
template class ACE_Node<ACE_NS_WString>;
template class ACE_Node<ACE_Name_Binding>;
template class ACE_Guard<ACE_RW_Process_Mutex>;
template class ACE_Read_Guard<ACE_RW_Process_Mutex>;
template class ACE_Write_Guard<ACE_RW_Process_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if (1)
#pragma instantiate ACE_Hash_Map_Entry<ACE_NS_String, ACE_NS_Internal>
#pragma instantiate ACE_Hash<ACE_NS_String>
#pragma instantiate ACE_Equal_To<ACE_NS_String>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>
#else
#pragma instantiate ACE_Map_Entry<ACE_NS_String, ACE_NS_Internal>
#pragma instantiate ACE_Map_Manager<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>
#endif
#pragma instantiate ACE_Unbounded_Set<ACE_Name_Binding>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Name_Binding>
#pragma instantiate ACE_Unbounded_Set<ACE_NS_WString>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_NS_WString>
#pragma instantiate ACE_Node<ACE_NS_WString>
#pragma instantiate ACE_Node<ACE_Name_Binding>
#pragma instantiate ACE_Guard<ACE_RW_Process_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_RW_Process_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_RW_Process_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_LOCAL_NAME_SPACE_CPP */
