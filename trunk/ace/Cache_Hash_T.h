/* -*- c++ -*- */
// $Id$

#ifndef ACE_CACHE_HASH_T_H
#define ACE_CACHE_HASH_T_H

#include "ace/OS.h"
#include "ace_Cache_Object.h"

// Forward declaration
template <class EXT_ID, class INT_ID, class EQ_FUNC>
class ACE_Hash_Bucket_Manager;

template <class EXT_ID, class HASH_FUNC, class EQ_FUNC>
class ACE_Cache_Hash
{
public:

  typedef ACE_Hash_Bucket_Manager<EXT_ID, ACE_Cache_Object *, EQ_FUNC>
          CACHE_BUCKET_MANAGER;

  ACE_Cache_Hash (ACE_Allocator *alloc = 0, size_t size = 521);
  // 521 == smallest number > 512 that is prime.  Why is a prime
  // number important?  I am trying to maximize scattering when using
  // mod on the hashed value.  This might be bogus though.

  virtual ~ACE_Cache_Hash (void);

  int find (const EXT_ID &ext_id) const;
  int find (const EXT_ID &ext_id, ACE_Cache_Object *&int_id) const;
  int bind (const EXT_ID &ext_id, ACE_Cache_Object *const &int_id);
  int trybind (const EXT_ID &ext_id, ACE_Cache_Object *&int_id);
  int rebind (const EXT_ID &ext_id, ACE_Cache_Object *const &int_id,
	      EXT_ID &old_ext_id, ACE_Cache_Object *&old_int_id);

  int unbind (const EXT_ID &ext_id);
  int unbind (const EXT_ID &ext_id, ACE_Cache_Object *&int_id);

  size_t size (void) const;

protected:

  virtual unsigned long hash (const EXT_ID &ext_id) const;
  int isprime (unsigned long number) const;
  int new_cachebucket (size_t idx);

private:

  ACE_Allocator *allocator_;
  size_t size_;

  ACE_SYNCH_MUTEX lock_;
  CACHE_BUCKET_MANAGER **hashtable_;

};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace_Cache_Hash_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* ACE_CACHE_HASH_T_H */
