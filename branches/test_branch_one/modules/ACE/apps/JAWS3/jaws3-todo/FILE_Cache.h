/* -*- c++ -*- */
// $Id$

#ifndef JAWS_FILE_CACHE_H
#define JAWS_FILE_CACHE_H

class JAWS_FILE_Cache;

typedef const char * KEY;
typedef JAWS_FILE * VALUE;
typedef int ATTR;
typedef ACE_Pair <VALUE, ATTR> CACHE_VALUE;
typedef ACE_Equal_To<KEY> EQ_FUNC;
typedef ACE_Hash<KEY> HASH_FUNC;

#define JAWS_KEY_PAIR KEY, CACHE_VALUE
#define JAWS_MAP_FUNCS HASH_FUNC, EQ_FUNC
#define JAWS_NULL_SYNCH ACE_SYNCH_NULL_MUTEX
#define JAWS_MAP_MANAGER_T_ARGS \
        JAWS_KEY_PAIR, JAWS_MAP_FUNCS, JAWS_NULL_SYNCH

typedef ACE_Hash_Map_Manager_Ex<JAWS_MAP_MANAGER_T_ARGS>
        JAWS_FILE_HASH_MAP_MANAGER;
typedef ACE_Hash_Map_Iterator_Ex<JAWS_MAP_MANAGER_T_ARGS>
        JAWS_FILE_HASH_MAP_ITERATOR;
typedef ACE_Hash_Map_Reverse_Iterator_Ex<JAWS_MAP_MANAGER_T_ARGS>
        JAWS_FILE_HASH_MAP_REVERSE_ITERATOR;

typedef ACE_Map_Manager<JAWS_KEY_PAIR, JAWS_NULL_SYNCH>
        JAWS_FILE_MAP_MANAGER;
typedef ACE_Map_Iterator<JAWS_KEY_PAIR, JAWS_NULL_SYNCH>
        JAWS_FILE_MAP_ITERATOR;
typedef ACE_Map_Reverse_Iterator<JAWS_KEY_PAIR, JAWS_NULL_SYNCH>
        JAWS_FILE_MAP_REVERSE_ITERATOR;

// Need a specialization to decrement a reference count.
typedef ACE_Cleanup_Strategy<JAWS_KEY_PAIR, JAWS_FILE_HASH_MAP_MANAGER>
        JAWS_FILE_HASH_MAP_CLEANUP;

// Need a specialization to decrement a reference count.
typedef ACE_Cleanup_Strategy<JAWS_KEY_PAIR, JAWS_FILE_MAP_MANAGER>
        JAWS_FILE_MAP_CLEANUP;




class JAWS_Export JAWS_FILE_Cache
{
public:
};

#endif /* JAWS_FILE_CACHE_H */
