// file      : Utility/ReferenceCounting/Interface.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_REFERENCE_COUNTING_INTERFACE_HPP
#define UTILITY_REFERENCE_COUNTING_INTERFACE_HPP

#include "Utility/ExH/Compound.hpp"
#include "Utility/ExH/System/Exception.hpp"
#include "Utility/ExH/Logic/Exception.hpp"
#include "Utility/ExH/Logic/DescriptiveException.hpp"

namespace Utility
{
  namespace ReferenceCounting
  {
    // Interface to a reference-countable object. Note that _remove_ref ()
    // member function has a no-throw semantic. Even though it can lead to
    // a diagnostic loss it was made no-throw because it has a destructor
    // semantic.

    class Interface
    {
    public:
      typedef
      unsigned long
      count_t;

      typedef
      ExH::System::Exception
      SystemException;

      typedef ExH::Logic::DescriptiveException Exception;
      
    public:
      virtual void
      add_ref () const = 0;

      virtual void
      remove_ref () const throw () = 0;

      virtual count_t
      refcount_value () const = 0;

    protected:
      Interface () throw ();

      virtual
      ~Interface () throw ();

    protected:
      virtual void
      add_ref_i () const = 0;

      virtual bool
      remove_ref_i () const = 0;

      virtual count_t
      refcount_value_i () const = 0;

    private:
      // Copy semanic is not supported.
      Interface (Interface const&) throw ();
      Interface&
      operator= (Interface const&) throw ();
    };

    template <typename Type>
    Type*
    add_ref (Type* ptr);
  }
}

#include "Utility/ReferenceCounting/Interface.tpp"
#include "Utility/ReferenceCounting/Interface.ipp"

#endif  // UTILITY_REFERENCE_COUNTING_INTERFACE_HPP

//$Id$
