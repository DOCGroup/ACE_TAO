// file      : Utility/Synch/Policy/Null.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_SYNCH_POLICY_NULL_HPP
#define UTILITY_SYNCH_POLICY_NULL_HPP

namespace Utility
{
  namespace Synch
  {
    namespace Policy
    {

      class NullMutex
      {
      };

      class NullGuard
      {
      public:
        explicit
        NullGuard (NullMutex&) throw ();

      private:
        NullGuard (NullGuard const&) throw ();

        NullGuard&
        operator= (NullGuard const&) throw ();
      };

      struct Null
      {
        typedef
        NullMutex
        Mutex;

        typedef
        NullGuard
        ReadGuard;

        typedef
        NullGuard
        WriteGuard;
      };
    }
  }
}

#include "Utility/Synch/Policy/Null.ipp"

#endif  // UTILITY_SYNCH_POLICY_NULL_HPP
//$Id$
