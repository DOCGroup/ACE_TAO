// file      : Utility/ReferenceCounting/DefaultImpl.ipp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

namespace Utility
{
  namespace ReferenceCounting
  {
    // c-tor & d-tor

    template <typename SynchPolicy>
    DefaultImpl<SynchPolicy>::
    DefaultImpl ()
        : ref_count_ (1),
          lock_ ()
    {
    }

    template <typename SynchPolicy>
    DefaultImpl<SynchPolicy>::
    ~DefaultImpl () throw ()
    {
    }

    // add_ref, remove_ref and refcount_value member functions

    template <typename SynchPolicy>
    void DefaultImpl<SynchPolicy>::
    add_ref () const
    {
      WriteGuard_ guard (lock_);
      add_ref_i ();
    }

    template <typename SynchPolicy>
    void DefaultImpl<SynchPolicy>::
    remove_ref () const throw ()
    {
      bool destroy (false);
      try
      {
        WriteGuard_ guard (lock_);
        destroy = remove_ref_i ();
      }
      catch (...)
      {
        // there is nothing we can do
      }

      if (destroy) delete this;
    }

    template <typename SynchPolicy>
    Interface::count_t DefaultImpl<SynchPolicy>::
    refcount_value () const
    {
      ReadGuard_ guard (lock_);
      return refcount_value_i ();
    }

    // add_ref_i, remove_ref_i and refcount_value_i member functions

    template <typename SynchPolicy>
    void DefaultImpl<SynchPolicy>::
    add_ref_i () const
    {
      ref_count_++;
    }

    template <typename SynchPolicy>
    bool DefaultImpl<SynchPolicy>::
    remove_ref_i () const
    {
      bool destroy (false);
      if (ref_count_ > 0)
      {
        if (--ref_count_ == 0) destroy = true;
      }
      else
      {
        throw InconsistentState (
          "Utility::ReferenceCounting::DefaultImpl::_remove_ref_i: "
          "reference counter is zero.");
      }
      return destroy;

    }

    template <typename SynchPolicy>
    Interface::count_t DefaultImpl<SynchPolicy>::
    refcount_value_i () const
    {
      return ref_count_;
    }

    template <typename SynchPolicy>
    typename SynchPolicy::Mutex& DefaultImpl<SynchPolicy>::
    lock_i() const throw ()
    {
      return lock_;
    }
  }
}
//$Id$
