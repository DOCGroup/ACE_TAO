// -*- C++ -*-

//=============================================================================
/**
 *  @file    String_Manager_T.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_STRING_MANAGER_T
#define TAO_STRING_MANAGER_T

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_NS_string.h"
#include "tao/Basic_Types.h"
#include "tao/String_Traits_Base_T.h"

#include <algorithm>

/****************************************************************/

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
template <typename charT>
class String_Manager_T
{
public:
  typedef charT character_type;
  typedef TAO::details::string_traits_base <charT> s_traits;

  /// Default CTOR will initialize the underlying ptr_ to empty string.
  inline String_Manager_T (void) : ptr_ (s_traits::default_initializer())
  {
  }

  /// Copy constructor
  inline String_Manager_T (const String_Manager_T<charT> &rhs) :
    ptr_ (s_traits::duplicate (rhs.ptr_))
  {
  }

  /// Constructor from const char* makes a copy.
  inline String_Manager_T (const character_type *s) :
    ptr_ (s_traits::duplicate (s))
  {
  }

  /// Destructor
  inline ~String_Manager_T (void) {
    s_traits::release (this->ptr_);
  }

  /// Assignment from another managed type
  inline String_Manager_T &operator= (const String_Manager_T<charT> &rhs) {
    // Strongly exception safe by means of copy and non-throwing swap
    // technique.
    String_Manager_T <character_type> tmp (rhs);
    std::swap (this->ptr_, tmp.ptr_);
    return *this;
  }

  /// Assignment from var type will make a copy
  inline String_Manager_T &operator= (const typename s_traits::string_var& value) {
    // Strongly exception safe by means of copy and non-throwing swap
    // technique.
    String_Manager_T <character_type> tmp (value.in ());
    std::swap (this->ptr_, tmp.ptr_);
    return *this;
  }

  /// Assignment from a constant * will make a copy
  inline String_Manager_T &operator= (const character_type *p) {
    // Strongly exception safe by means of copy and non-throwing swap
    // technique.
    String_Manager_T <character_type> tmp (p);
    std::swap (this->ptr_, tmp.ptr_);
    return *this;
  }

  /// Assignment from char* will not make a copy. The String_Manager_T will now
  /// own the string.
  inline String_Manager_T &operator= (character_type *p) {
    s_traits::release (this->ptr_);
    this->ptr_ = p;
    return *this;
  }

  /// Cast (read-only)
  inline operator const character_type*() const {
    return this->ptr_;
  }

  /// For in parameter.
  inline const character_type *in (void) const {
    return this->ptr_;
  }

  /// For inout parameter.
  inline character_type *&inout (void) {
    return this->ptr_;
  }

  /// for out parameter.
  inline character_type *&out (void) {
    s_traits::release (this->ptr_);
    this->ptr_ = 0;
    return this->ptr_;
  }

  /// For string of return type.
  inline character_type *_retn (void) {
    character_type *temp = this->ptr_;
    this->ptr_ = 0;
    return temp;
  }

private:
  /// The underlying string
  character_type *ptr_;
};

  typedef TAO::String_Manager_T<CORBA::Char> String_Manager;
  typedef TAO::String_Manager_T<CORBA::WChar> WString_Manager;
}

inline bool operator== (const TAO::String_Manager &lhs, const TAO::String_Manager &rhs)
{
  return ACE_OS::strcmp (lhs.in(), rhs.in ()) == 0;
}

inline bool operator< (const TAO::String_Manager &lhs, const TAO::String_Manager &rhs)
{
  return ACE_OS::strcmp (lhs.in(), rhs.in ()) < 0;
}

inline bool operator== (const TAO::WString_Manager &lhs, const TAO::WString_Manager &rhs)
{
  return ACE_OS::strcmp (lhs.in(), rhs.in ()) == 0;
}

inline bool operator< (const TAO::WString_Manager &lhs, const TAO::WString_Manager &rhs)
{
  return ACE_OS::strcmp (lhs.in(), rhs.in ()) < 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_STRING_MANAGER_T */
