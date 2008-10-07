// file      : Utility/ReferenceCounting/StrictPtr.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_REFERENCE_COUNTING_STRICT_PTR_HPP
#define UTILITY_REFERENCE_COUNTING_STRICT_PTR_HPP

#include "Utility/ExH/Compound.hpp"
#include "Utility/ExH/Logic/DescriptiveException.hpp"

#include "Utility/ReferenceCounting/Interface.hpp"

namespace Utility
{
  namespace ReferenceCounting
  {
    template <typename T>
    class StrictPtr
    {
    public:
      typedef
      T
      Type;

      class NotInitialized_ {};
      typedef
      ExH::Compound<NotInitialized_, ExH::Logic::DescriptiveException>
      NotInitialized;

    public:
      // c-tor's

      StrictPtr () throw ();

      explicit
      StrictPtr (Type* ptr) throw ();

      StrictPtr (StrictPtr<Type> const& s_ptr);

      template <typename Other>
      StrictPtr (StrictPtr<Other> const& s_ptr);
      // d-tor

      ~StrictPtr () throw ();

      // assignment & copy-assignment operators

      StrictPtr<Type>&
      operator= (Type* ptr) throw ();

      StrictPtr<Type>&
      operator= (StrictPtr<Type> const& s_ptr);

      template <typename Other>
      StrictPtr<Type>&
      operator= (StrictPtr<Other> const& s_ptr);

      // conversions

      // Note: implicit conversion (operator Type* ()) is not supported.

      // comparison

      bool
      operator== (Type* other) const throw ();

      bool
      operator!= (Type* other) const throw ();

      // accessors

      Type*
      operator-> () const;

      Type*
      in () const throw ();

      Type*
      retn() throw ();

    private:
      Type* ptr_;
    };

    // Specialization of add_ref function for StrictPtr<T>
    template <typename T>
    T*
    add_ref (StrictPtr<T> const& ptr);

    // Dynamic type conversion function for StrictPtr's
    template <typename D, typename S>
    StrictPtr<D>
    strict_cast (StrictPtr<S> const& s);
  }
}

#include "Utility/ReferenceCounting/StrictPtr.tpp"

#endif  // UTILITY_REFERENCE_COUNTING_STRICT_PTR_HPP

//$Id$
