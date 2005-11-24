// -*- C++ -*-
//=============================================================================
/**
 *  @file    CORBA_String.h
 *
 *  $Id$
 *
 *  Header file for the CORBA string types.
 *
 *  @author DOC Group at Wash U, UCI, and Vanderbilt U.
 */
//=============================================================================

#ifndef TAO_CORBA_STRING_H
#define TAO_CORBA_STRING_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"
#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/String_Traits_Base_T.h"
// For the (W)String_var and (W)String_out iostream operators.
#include "ace/iosfwd.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template <typename charT> class String_Manager;  // Forward declaration.
}

typedef TAO::String_Manager<CORBA::Char> TAO_String_Manager;
typedef TAO::String_Manager<CORBA::WChar> TAO_WString_Manager;

namespace TAO
{
  template <typename charT>
  class String_var
  {
  public:
    typedef charT character_type;
    typedef TAO::details::string_traits_base <character_type> s_traits;

    /// Default constructor.
    inline String_var (void) : ptr_ (0)
    {
    }

    /// Constructor, owns p.
    inline String_var (character_type *p) : ptr_ (p)
    {
    }

    /// constructor. Makes a copy of p.
    inline String_var (const character_type *p) : ptr_ (s_traits::duplicate (p))
    {
    }

    /// copy constructor.
    inline String_var (const String_var<charT> &s)
    {
      String_var <charT> tmp (s.ptr_);
      std::swap (this->ptr_, tmp.ptr_);
    }

    /// destructor.
    inline ~String_var (void)
    {
      s_traits::release (this->ptr_);
    }

    /// assignment operator.
    inline String_var &operator= (character_type *p)
    {
      if (this->ptr_ != p)
        {
          s_traits::release (this->ptr_);
          this->ptr_ = p;
        }
      return *this;
    }

    /// assignment to a const char*.  Makes a copy.
    inline String_var &operator= (const character_type *p)
    {
      String_var <charT> tmp (p);
      std::swap (this->ptr_, tmp.ptr_);
    }

    /// assignment operator.
    inline String_var &operator= (const String_var<character_type> &s)
    {
      String_var <charT> tmp (s.ptr_);
      std::swap (this->ptr_, tmp.ptr_);
      return *this;
    }

    /// Spec-defined read/write version.
    inline operator character_type *&()
    {
      return this->ptr_;
    }

    /// only read privileges.
    inline operator const character_type *() const
    {
      return this->ptr_;
    }

    /// allows access and modification using an slot.
    inline character_type &operator[] (CORBA::ULong slot)
    {
      // We need to verify bounds else raise some exception.
      return this->ptr_[slot];
    }

    /// allows only accessing thru an slot.
    inline character_type operator[] (CORBA::ULong slot) const
    {
      // We need to verify bounds else raise some exception.
      return this->ptr_[slot];
    }

    /// for in parameter.
    inline const character_type *in (void) const
    {
      return this->ptr_;
    }

    /// for inout parameter.
    inline character_type *&inout (void)
    {
      return this->ptr_;
    }

    /// for out parameter.
    inline character_type *&out (void)
    {
      s_traits::release (this->ptr_);
      this->ptr_ = 0;
      return this->ptr_;
    }

    /// for string of return type.
    inline character_type *_retn (void)
    {
      character_type *temp = this->ptr_;
      this->ptr_ = 0;
      return temp;
    }

    /// TAO extension.
    inline character_type *ptr (void)
    {
      return this->ptr_;
    }

  private:
    /// instance.
    character_type *ptr_;
  };

  /**
   * @class String_out
   *
   * @brief String_out
   *
   * To support the memory management for "out" parameter passing
   * mode.  ORBOS/97-05-15, Appendix C.2 defines a CORBA::String_out
   * class
   */
  template <typename charT>
  class String_out
  {
  public:
    typedef charT character_type;
    typedef TAO::details::string_traits_base <character_type> s_traits;
    typedef typename s_traits::string_mgr string_mgr;

    /// Construction from a reference to a string.
    inline String_out (character_type *&p) : ptr_ (s)
    {
      this->ptr_ = 0;
    }

    /// Construction from a var.
    inline String_out (String_var <character_type> &p) : ptr_ (p.out ())

    /// Construction from a TAO_String_Manager.
    inline String_out (string_mgr &p) : ptr_ (p.out ())
    {
    }

    /// Copy constructor.
    inline String_out (const String_out<charT> &s) : ptr_ (s.ptr_)
    {
    }

    /// Assignment from a string_out.
    inline String_out &operator= (const String_out<charT> &s)
    {
      this->ptr_ = s.ptr_;
      return *this;
    }

    /// Assignment from a string.
    inline String_out &operator= (character_type *p)
    {
      this->ptr_ = p;
      return *this;
    }

    /// Assignment from a constant char*.
    inline String_out& operator= (const character_type* p)
    {
      this->ptr_ = s_traits::duplicate (p);
      return *this;
    }

    /// Cast.
    inline operator character_type *&()
    {
      return this->ptr_;
    }

    /// Return underlying instance.
    inline character_type *&ptr (void)
    {
      return this->ptr_;
    }

  private:
    /// Instance.
    character_type *&ptr_;

    // assignment from _var disallowed
    void operator= (const typename s_traits::string_var &);
  };

  /**
   * @struct TAO-specific @c {W}String_var Equality Functor
   *
   * This functor exist to simplify usage of @c {W}String_var in
   * containers.
   */
  struct String_Var_Equal_To
  {
    bool operator() (CORBA::String_var const & lhs,
                     CORBA::String_var const & rhs) const;

    bool operator() (CORBA::WString_var const & lhs,
                     CORBA::WString_var const & rhs) const;
  };
}

namespace CORBA
{
  typedef TAO::String_var <char> String_var;
  typedef TAO::String_out <char> String_out;
  typedef TAO::String_var <CORBA::WChar> WString_var;
  typedef TAO::String_out <CORBA::WChar> WString_out;
}


# if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

TAO_Export ostream &
operator<< (ostream &, const CORBA::String_var &);
TAO_Export istream &
operator>> (istream &, CORBA::String_var &);
TAO_Export ostream &
operator<< (ostream &, CORBA::String_out &);
TAO_Export istream &
operator>> (istream &, CORBA::String_out &);
TAO_Export ostream &
operator<< (ostream &, const CORBA::WString_var &);
TAO_Export istream &
operator>> (istream &, CORBA::WString_var &);
TAO_Export ostream &
operator<< (ostream &, CORBA::WString_out &);
TAO_Export istream &
operator>> (istream &, CORBA::WString_out &);

# endif /* ACE_LACKS_IOSTREAM_TOTALLY */

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CORBA_String.inl"
#endif /* ! __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_CORBA_STRING_H */
