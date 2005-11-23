// -*- C++ -*-

//=============================================================================
/**
 *  @file    String_Manager_T.h
 *
 *  $Id$
 *
 *  @author Aniruddha Gokhale
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_STRING_MANAGER_T
#define TAO_STRING_MANAGER_T

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/CORBA_String.h"
#include "tao/String_Traits_Base_T.h"

/****************************************************************/

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
template <typename charT>
class String_Manager
{
public:
  typedef charT character_type;
  typedef TAO::details::string_traits_base <charT> s_traits;

  /// Default CTOR will initialize the underlying ptr_ to empty string.
  inline String_Manager (void) : ptr_ (s_traits::default_initializer())
  {
  }

  /// copy constructor
  inline String_Manager (const String_Manager &rhs) : ptr_ (s_traits::duplicate (rhs.ptr_))
  {
  }

  /// constructor from const char* makes a copy.
  inline String_Manager (const character_type *s) : ptr_ (s_traits::duplicate (s))
  {
  }

  /// destructor
  inline ~String_Manager (void) {
    s_traits::release (this->ptr_);
  }

  /// Assignment from another managed type
  inline String_Manager &operator= (const String_Manager &rhs) {
    // Strongly exception safe by means of copy and non-throwing swap
    // technique.
    String_Manager <character_type> tmp (rhs);
    character_type * old_ptr = this->ptr_;
    this->ptr_ = tmp.ptr_;
    tmp.ptr_  = old_ptr;
    return *this;
  }

  /// Assignment from var type will make a copy
  inline String_Manager &operator= (const s_traits::string_var& var) {
    // Strongly exception safe by means of copy and non-throwing swap
    // technique.
    String_Manager <character_type> tmp (var.in ());
    character_type * old_ptr = this->ptr_;
    this->ptr_ = tmp.ptr_;
    tmp.ptr_  = old_ptr;
    return *this;
  }

  /// Assignment from a constant * will make a copy
  inline String_Manager &operator= (const character_type *p) {
    // Strongly exception safe by means of copy and non-throwing swap
    // technique.
    String_Manager <character_type> tmp (p);
    character_type * old_ptr = this->ptr_;
    this->ptr_ = tmp.ptr_;
    tmp.ptr_ = old_ptr;
    return *this;
  }

  /// assignment from char* will not make a copy. The String_Manager will now
  /// own the string.
  inline String_Manager &operator= (character_type *p) {
    s_traits::release (this->ptr_);
    this->ptr_ = p;
    return *this;
  }

  /// cast  (read-only)
  inline operator const character_type*() const {
    return this->ptr_;
  }

  /// for in parameter.
  inline const character_type *in (void) const {
    return this->ptr_;
  }

  /// for inout parameter.
  inline character_type *&inout (void) {
    return this->ptr_;
  }

  /// for out parameter.
  inline character_type *&out (void) {
    s_traits::release (this->ptr_);
    this->ptr_ = 0;
    return this->ptr_;
  }

  /// for string of return type.
  character_type *_retn (void) {
    character_type *temp = this->ptr_;
    this->ptr_ = 0;
    return temp;
  }

private:
  /// The underlying string
  character_type *ptr_;
};
}

typedef TAO::String_Manager<CORBA::Char> TAO_String_Manager;
typedef TAO::String_Manager<CORBA::WChar> TAO_WString_Manager;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_STRING_MANAGER_T */
