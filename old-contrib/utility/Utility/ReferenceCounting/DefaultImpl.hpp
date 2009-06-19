// file      : Utility/ReferenceCounting/DefaultImpl.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_REFERENCE_COUNTING_DEFAULT_IMPL_HPP
#define UTILITY_REFERENCE_COUNTING_DEFAULT_IMPL_HPP

#include "Utility/ExH/Compound.hpp"
#include "Utility/ExH/Logic/DescriptiveException.hpp"

#include "Utility/Synch/Policy/Null.hpp"

#include "Utility/ReferenceCounting/Interface.hpp"

namespace Utility
{
  namespace ReferenceCounting
  {
    // Default reference counter implementation with parameterised
    // synchronization policy. It is assumed that none of the SynchPolicy
    // types throw any logic exceptions. If in fact they do then these
    // exceptions won't be handled and will be automatically converted
    // to system exceptions.

    template <typename SynchPolicy = Utility::Synch::Policy::Null>
    class DefaultImpl : public virtual Interface
    {
    public:
      class InconsistentState_ {};

      typedef
      ExH::Compound<InconsistentState_, Exception>
      InconsistentState;

    public:
      DefaultImpl ();

      virtual
      ~DefaultImpl () throw ();

    public:
      virtual void
      add_ref () const;

      virtual void
      remove_ref () const throw ();

      virtual count_t
      refcount_value () const;

    protected:
      virtual void
      add_ref_i () const;

      virtual bool
      remove_ref_i () const;

      virtual count_t
      refcount_value_i () const;

      typename SynchPolicy::Mutex&
      lock_i () const throw ();

    protected:
      typedef
      typename SynchPolicy::Mutex
      Mutex_;

      typedef
      typename SynchPolicy::ReadGuard
      ReadGuard_;

      typedef
      typename SynchPolicy::WriteGuard
      WriteGuard_;

    protected:
      mutable count_t ref_count_;

    private:
      mutable Mutex_  lock_;

    private:
      // Copy semanic is not supported.
      DefaultImpl (DefaultImpl const&) throw ();
      DefaultImpl&
      operator= (DefaultImpl const&) throw ();
    };
  }
}

#include "Utility/ReferenceCounting/DefaultImpl.ipp"

#endif  // UTILITY_REFERENCE_COUNTING_DEFAULT_IMPL_HPP

//$Id$
