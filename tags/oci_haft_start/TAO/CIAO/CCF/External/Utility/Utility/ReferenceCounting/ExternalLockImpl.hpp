// file      : Utility/ReferenceCounting/ExternalLockImpl.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_REFERENCE_COUNTING_EXTERNAL_LOCK_IMPL_HPP
#define UTILITY_REFERENCE_COUNTING_EXTERNAL_LOCK_IMPL_HPP

namespace Utility
{
  namespace ReferenceCounting
  {

    /*

    Not ported yet.

    class ExternalLockRefCounter : public virtual Util::RefCountBase
    {
    public:
      ExternalLockRefCounter (ACE_Lock* lock = 0);
      virtual ~ExternalLockRefCounter ();

      void init (ACE_Lock* lock);

    public:

      virtual void _add_ref ();
      virtual void _remove_ref ();
      virtual unsigned long _refcount_value ();

    protected:

      virtual void _add_ref_i ();
      virtual bool _remove_ref_i ();
      virtual unsigned long _refcount_value_i ();

      ACE_Lock* lock_i ();

    private:

      typedef ACE_Guard <ACE_Lock> Guard_;

      ACE_Lock* lock_;
      unsigned long ref_count_;

    private:
      ExternalLockRefCounter (const ExternalLockRefCounter& );
      void operator= (const ExternalLockRefCounter& );
    };

    */
  }
}

#include "Utility/ReferenceCounting/ExternalLockImpl.ipp"

#endif  // UTILITY_REFERENCE_COUNTING_EXTERNAL_LOCK_IMPL_HPP

