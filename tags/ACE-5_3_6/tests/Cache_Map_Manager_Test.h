/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Cache_Map_Manager_Test.h
//
// = DESCRIPTION
//    Define class needed for generating templates. IBM C++ requires this to
//    be in its own file for auto template instantiation.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef __ACE_CACHE_MAP_MANAGER_TEST_H
#define __ACE_CACHE_MAP_MANAGER_TEST_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Hash_Key
{
public:
  u_long operator () (size_t t) const
    {
      // Simply returns t
      return ACE_static_cast (u_long, t);
    }
};

#endif /* __ACE_CACHE_MAP_MANAGER_TEST_T */
