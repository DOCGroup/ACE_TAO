/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Connection_Cache_Manager.h
//
// = AUTHOR
//    Bala Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_CONNECTION_CACHE_MANAGER_H
#define TAO_CONNECTION_CACHE_MANAGER_H
#include "ace/pre.h"



#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Cache_ExtId;
class TAO_Cache_IntId;

class TAO_Export TAO_Connection_Cache_Manager
{
  // = TITLE
  //     TAO_Connection_Cache_Manager is an abstract class for

  // = DESCRIPTION
  //
public:

  TAO_Connection_Cache_Manager (void);
  // Constructor

  virtual ~TAO_Connection_Cache_Manager (void);
  // Destructor

  virtual int bind (const TAO_Cache_ExtId &ext_id,
                    const TAO_Cache_IntId &int_id) = 0;
  // Associate <ext_id> with <int_id>. If <ext_id> is already in the
  // map then the <Map_Entry> is not changed.  Returns 0 if a new
  // entry is bound successfully, returns 1 if an attempt is made to
  // bind an existing entry, and returns -1 if failures occur.

  virtual int find (const TAO_Cache_ExtId &key,
                    TAO_Cache_IntId &value) = 0;
  // Lookup entry<key,value> in the cache. If it is not found, returns -1.
  // If the <key> is located in the MAP object, the CACHING_STRATEGY is
  // notified of it via notify_find (int result, ATTRIBUTES &attribute).
  // If notify_find also returns 0 (success), then this function returns
  // 0 (success) and sets the cached value in <value>.




};

#if defined (__ACE_INLINE__)
# include "tao/Connection_Cache_Manager.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_CONNECTION_CACHE_MANAGER_H*/
