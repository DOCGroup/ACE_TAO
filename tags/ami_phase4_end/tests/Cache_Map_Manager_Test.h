// $Id$

// Define class needed for generating templates. IBM C++ requires this to
// be in its own file for auto template instantiation.

#ifndef __ACE_CACHE_MAP_MANAGER_TEST_H
#define __ACE_CACHE_MAP_MANAGER_TEST_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class hash_key
{
public:
  u_long operator () (size_t t) const
    {
      // Simply returns t
      return t;
    }
};

#endif /* __ACE_CACHE_MAP_MANAGER_TEST_T */
