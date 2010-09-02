// file      : Utility/ReferenceCounting/ExternalLockImpl.ipp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

namespace Utility
{
  namespace ReferenceCounting
  {
    /*
    inline
    ExternalLockRefCounter::ExternalLockRefCounter (ACE_Lock* lock)
        : lock_ (lock),
          ref_count_ (1)
    {
    }

    inline
    void
    ExternalLockRefCounter::init (ACE_Lock* lock)
    {
      lock_ = lock;
    }

    inline
    ExternalLockRefCounter::~ExternalLockRefCounter ()
    {
    }

    inline
    ACE_Lock*
    ExternalLockRefCounter::lock_i ()
    {
      return lock_;
    }

    inline
    void
    ExternalLockRefCounter::_add_ref ()
    {
      if (lock_)
      {
        Guard_ guard (*lock_);
        _add_ref_i ();
      }
      else
      {
        _add_ref_i ();
      }
    }

    inline
    void
    ExternalLockRefCounter::_remove_ref ()
    {
      bool destroy = false;
      {
        if (lock_)
        {
          Guard_ guard (*lock_);
          destroy = _remove_ref_i ();
        }
        else
        {
          destroy = _remove_ref_i ();
        }
      }
      if (destroy) delete this;
    }

    inline
    unsigned long
    ExternalLockRefCounter::_refcount_value ()
    {
      if (lock_)
      {
        Guard_ guard (*lock_);
        return _refcount_value_i ();
      }
      else
      {
        return _refcount_value_i ();
      }
    }

    inline
    void
    ExternalLockRefCounter::_add_ref_i ()
    {
      ref_count_++;
    }

    inline
    bool
    ExternalLockRefCounter::_remove_ref_i ()
    {
      bool destroy = false;
      if (ref_count_ > 0)
      {
        if (--ref_count_ == 0) destroy = true;
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    "ExternalLockRefCounter::_remove_ref() "
                    " _remove_ref() called while ref_coundt == 0\n"
                   ));
      }
      return destroy;
    }

    inline
    unsigned long
    ExternalLockRefCounter::_refcount_value_i ()
    {
      return ref_count_;
    }
    */
  }
}

//$Id$
