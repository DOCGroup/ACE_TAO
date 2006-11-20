/* -*- c++ -*- */
// $Id$

#ifndef JAWS_CACHE_MAP_MANAGER_H
#define JAWS_CACHE_MAP_MANAGER_H

template <class EXT_ID>
class JAWS_Cache_Map_Entry
{
public:

  JAWS_Cache_Map_Entry ( const EXT_ID &ext_id
                       , ACE_Message_Block *&int_id
                       , JAWS_Cache_Map_Entry<EXT_ID, INT_ID> *next = 0
                       , JAWS_Cache_Map_Entry<EXT_ID, INT_ID> *prev = 0
                       );

  JAWS_Cache_Map_Entry ( JAWS_Cache_Map_Entry<EXT_ID, INT_ID> *next
                       , JAWS_Cache_Map_Entry<EXT_ID, INT_ID> *prev
                       );

private:

  ~JAWS_Cache_Map_Entry (void) {}

public:

  EXT_ID ext_id_;
  ACE_Message_Block *int_id_;

  time_t last_update_;
  int ref_count_;

  JAWS_Cache_Map_Entry<EXT_ID, INT_ID> *next_;
  JAWS_Cache_Map_Entry<EXT_ID, INT_ID> *prev_;

};


template <class EXT_ID, class EQ_FUNC, class ACE_LOCK>
class JAWS_Map_Manager
{
public:

  JAWS_Map_Manager (ACE_Allocator *alloc = 0);
  JAWS_Map_Manager (size_t size, ACE_Allocator *alloc = 0);

  int open ( size_t size = ACE_DEFAULT_MAP_SIZE
           , ACE_Allocator *alloc = 0);

  int close (void);

  ~JAWS_Map_Manager (void);

  int bind ( const EXT_ID &key
           , const void * const &data
           , size_t size
           , JAWS_Cache_Map_Entry<EXT_ID> *&entry
           );

  int rebind ( const EXT_ID &key
             , const void * const &data
             , size_t size
             , JAWS_Cache_Map_Entry<EXT_ID> *&entry
             );

  int find (const EXT_ID &key, JAWS_Cache_Map_Entry<EXT_ID> *&entry);

  int unbind (JAWS_Cache_Map_Entry<EXT_ID> *entry);

  ACE_Message_Block * make_message_block ( const void * const &data
                                         , size_t size);

  ACE_LOCK lock_;

  JAWS_Cache_Map_Entry<EXT_ID> *head_;
  JAWS_Cache_Map_Entry<EXT_ID> *tail_;

  ACE_Allocator *allocator_;

};

template <class EXT_ID, class ACE_LOCK>
class JAWS_Cache_Replacement
{
public:

  JAWS_Cache_Replacement (ACE_Allocator *alloc = 0);
  JAWS_Cache_Replacement (size_t max_entries, ACE_Allocator *alloc = 0);

  virtual int open ( size_t max_entries = ACE_DEFAULT_MAP_SIZE
                   , ACE_Allocator *alloc = 0);

  virtual int close (void);

  virtual ~JAWS_Cache_Replacement (void);

  virtual int insert (JAWS_Cache_Map_Entry<EXT_ID> *entry) = 0;
  virtual int update (JAWS_Cache_Map_Entry<EXT_ID> *entry) = 0;
  virtual int remove (JAWS_Cache_Map_Entry<EXT_ID> *entry) = 0;
  virtual int expire (JAWS_Cache_Map_Entry<EXT_ID> *&entry) = 0;

  JAWS_Cache_Map_Entry<EXT_ID> *table_;

  ACE_Allocator *allocator_;
  ACE_LOCK lock_;

};

template <class EXT_ID, class ACE_LOCK>
class JAWS_LRU_Cache_Replacement
  : public JAWS_Cache_Replacement<EXT_ID, ACE_LOCK>
{
public:

  JAWS_LRU_Cache_Replacement (ACE_Allocator *alloc = 0);
  JAWS_LRU_Cache_Replacement (size_t max_entries, ACE_Allocator *alloc = 0);

  int open ( size_t max_entries = ACE_DEFAULT_MAP_SIZE
           , ACE_Allocator *alloc = 0);

  int close (void);

  ~JAWS_LRU_Cache_Replacement (void);

  int insert (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  int update (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  int remove (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  int expire (JAWS_Cache_Map_Entry<EXT_ID> *&entry);

  int head_;
  int tail_;

  JAWS_LRU_Cache_Replacement_Info *info_;

};


template <class EXT_ID, class INT_ID, class HASH_FUNC, class EQ_FUNC, class ACE_LOCK>
class JAWS_Cache_Map_Manager
{
public:


  JAWS_Map_Manager<EXT_ID, INT_ID, EQ_FUNC, ACE_
};

#endif /* JAWS_CACHE_MAP_MANAGER_H */
