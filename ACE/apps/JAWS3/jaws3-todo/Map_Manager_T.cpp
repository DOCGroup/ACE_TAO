/* -*- c++ -*- */
// $Id$

#ifndef JAWS_CACHE_MAP_MANAGER_T_CPP
#define JAWS_CACHE_MAP_MANAGER_T_CPP

#include "jaws3/Map_Manager_T.h"

template <class EXT_ID>
JAWS_Cache_Map_Entry<EXT_ID>
::JAWS_Cache_Map_Entry ( const EXT_ID &ext_id
                       , ACE_Message_Block *&int_id
                       , JAWS_Cache_Map_Entry<EXT_ID, INT_ID> *next
                       , JAWS_Cache_Map_Entry<EXT_ID, INT_ID> *prev
                       )
  : ext_id_ (ext_id)
  , int_id_ (int_id)
  , ref_count_ (1)
  , next_ (next)
  , prev_ (prev)
  , replacement_index_ (-1)
{
}

template <class EXT_ID>
JAWS_Cache_Map_Entry<EXT_ID>
::JAWS_Cache_Map_Entry ( JAWS_Cache_Map_Entry<EXT_ID, INT_ID> *next
                       , JAWS_Cache_Map_Entry<EXT_ID, INT_ID> *prev
                       )
  : next_ (next)
  , prev_ (prev)
{
}


template <class EXT_ID, class EQ_FUNC, class ACE_LOCK>
class JAWS_Map_Manager
{
public:

  JAWS_Map_Manager (ACE_Allocator *alloc = 0);
  JAWS_Map_Manager (size_t size, ACE_Allocator *alloc = 0);

  //FUZZ: disable check_for_lack_ACE_OS
  int open ( size_t size = ACE_DEFAULT_MAP_SIZE,
             ACE_Allocator *alloc = 0);

  int close (void);

  ~JAWS_Map_Manager (void);

  int bind ( const EXT_ID &key
           , const void * const &data
           , size_t size
           , JAWS_Cache_Map_Entry<EXT_ID> *&entry
           );
  //FUZZ: enable check_for_lack_ACE_OS

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

#define JAWS_CACHE_REPLACEMENT_TEMPLATE \
        template <class EXT_ID, class ACE_LOCK> \
        JAWS_Cache_Replacement<EXT_ID, ACE_LOCK>

#define JAWS_LRU_CACHE_REPLACEMENT_TEMPLATE \
        template <class EXT_ID, class ACE_LOCK> \
        JAWS_Cache_Replacement<EXT_ID, ACE_LOCK>

#define JAWS_LFU_CACHE_REPLACEMENT_TEMPLATE \
        template <class EXT_ID, class ACE_LOCK> \
        JAWS_Cache_Replacement<EXT_ID, ACE_LOCK>

#define JAWS_FIFO_CACHE_REPLACEMENT_TEMPLATE \
        template <class EXT_ID, class ACE_LOCK> \
        JAWS_Cache_Replacement<EXT_ID, ACE_LOCK>

JAWS_CACHE_REPLACEMENT_TEMPLATE::JAWS_Cache_Replacement (ACE_Allocator *alloc)
{
  this->open (ACE_DEFAULT_MAP_SIZE, alloc);
}

JAWS_Cache_Replacement::
JAWS_Cache_Replacement (size_t max_entries, ACE_Allocator *alloc)
{
  this->open (max_entries, alloc);
}

int
JAWS_CACHE_REPLACEMENT_TEMPLATE::open ( size_t max_entries
                                      , ACE_Allocator *alloc
                                      )
{
  this->max_entries_ = max_entries;
  this->allocator_ = alloc;

  size_t bytes = max_entries * sizeof (JAWS_Cache_Map_Entry<EXT_ID> *);
  void *ptr;

  ACE_ALLOCATOR_RETURN (ptr, this->allocator_->malloc (bytes), -1);
}

int
JAWS_CACHE_REPLACEMENT_TEMPLATE::close (void)
{
}

  virtual ~JAWS_Cache_Replacement (void);

  virtual int insert (JAWS_Cache_Map_Entry<EXT_ID> *entry) = 0;
  virtual int update (JAWS_Cache_Map_Entry<EXT_ID> *entry) = 0;
  virtual int remove (JAWS_Cache_Map_Entry<EXT_ID> *entry) = 0;
  virtual JAWS_Cache_Map_Entry<EXT_ID> * expire (void) = 0;

  JAWS_Cache_Map_Entry<EXT_ID> **table_;
  size_t max_entries_;

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

  //FUZZ: disable check_for_lack_ACE_OS
  int open ( size_t max_entries = ACE_DEFAULT_MAP_SIZE,
             ACE_Allocator *alloc = 0);

  int close (void);
  //FUZZ: enable check_for_lack_ACE_OS

  ~JAWS_LRU_Cache_Replacement (void);

  int insert (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  int update (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  int remove (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  JAWS_Cache_Map_Entry<EXT_ID> * expire (void);

protected:

  void insert_i (JAWS_Cache_Map_Entry<EXT_ID> *entry)
  {
    if (this->tail_ == -1)
      this->tail_ = 0;

    this->table_[this->tail_] = entry;
    this->next_[this->tail_] = this->tail_+1;
    this->prev_[this->tail_+1] = this->tail_;
    this->next_[this->tail_+1] = -1;

    if (this->head_ == -1)
      this->head_ = 0;

    this->count_++;
  }

  void update_i (int i)
  {
    if (i == this->tail_)
      return;

    JAWS_Cache_Map_Entry<EXT_ID> *entry = this->table_[i];
    this->remove_i (i);
    this->insert_i (entry);
  }

  void remove_i (int i)
  {
    if (this->count_ == 0 || i >= this->count_)
      return;

    --this->count_;
    if (i == this->tail_)
      this->tail_ = this->prev_[i];
    if (i == this->head_)
      this->head_ = this->next_[i];

    if (i < this->count_)
      {
        this->swap_i (i, this->count_);
        i = this->count_;
      }

    int p_i = this->prev_[i];
    int n_i = this->next_[i];
    this->next_[p_i] = n_i;
    this->prev_[n_i] = p_i;

    if (this->count_ == 0)
      this->head_ = -1;
  }

  void swap_i (int i, int j)
  {
    int p_i, p_j, n_i, n_j;

    p_i = this->prev_[i];
    n_i = this->next_[i];
    p_j = this->prev_[j];
    n_j = this->next_[j];

    this->next_[p_i] = j;
    this->prev_[n_i] = j;
    this->next_[p_j] = i;
    this->prev_[n_j] = i;

    JAWS_Cache_Map_Entry<EXT_ID> *entry = this->table_[i];
    this->table_[i] = this->table_[j];
    this->table_[j] = entry;

    this->table_[i]->replacement_index_ = i;
    this->table_[j]->replacement_index_ = j;

    if (this->head_ == i || this->head_ == j)
      this->head_ = (this->head_ == i) ? j : i;

    if (this->tail_ == i || this->tail_ == j)
      this->tail_ = (this->tail_ == i) ? j : i;
  }

private:

  int count_;

  int head_;
  int tail_;

  int *next_;
  int *prev_;

};

template <class EXT_ID, class ACE_LOCK>
class JAWS_LFU_Cache_Replacement
  : public JAWS_Cache_Replacement<EXT_ID, ACE_LOCK>
{
public:

  JAWS_LFU_Cache_Replacement (ACE_Allocator *alloc = 0);
  JAWS_LFU_Cache_Replacement (size_t max_entries, ACE_Allocator *alloc = 0);

  //FUZZ: disable check_for_lack_ACE_OS
  int open ( size_t max_entries = ACE_DEFAULT_MAP_SIZE,
             ACE_Allocator *alloc = 0);

  int close (void);
  //FUZZ: enable check_for_lack_ACE_OS

  ~JAWS_LFU_Cache_Replacement (void);

  int insert (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  int update (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  int remove (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  JAWS_Cache_Map_Entry<EXT_ID> * expire (void);

protected:

  int sift_up (int i)
  {
    JAWS_Cache_Map_Entry<EXT_ID> *entry = this->table_[i];

    while (i > 0)
      {
        int p = this->parent (i);

        if (entry->ref_count_ >= this->table_[p]->ref_count_)
          break;

        this->table_[i] = this->table_[p];
        this->table_[i]->replacement_index_ = i;

        i = p;
      }

    this->table_[i] = entry;
    this->table_[i]->replacement_index_ = i;

    return i;
  }

  int sift_down (int i)
  {
    JAWS_Cache_Map_Entry<EXT_ID> *entry = this->table_[i];

    while (this->left (i) < this->count_)
      {
        int l, r;

        l = this->left (i);
        r = this->right (i);

        if (r >= this->count_
            || this->table_[l]->ref_count_ <= this->table_[r]->ref_count_)
          r = l;

        if (entry->ref_count_ <= this->table_[r]->ref_count_)
          break;

        this->table_[i] = this->table_[r];
        this->table_[i]->replacement_index_ = i;

        i = r;
      }

    this->table_[i] = entry;
    this->table_[i]->replacement_index_ = i;

    return i;
  }

  int sift (int i)
  {
    int i_sifted;

    i_sifted = this->sift_up (i);
    if (i_sifted == i)
      i_sifted = this->sift_down (i);

    return i_sifted;
  }

  int parent (int i) { return --i/2; }
  int left (int i) { return 2*i+1; }
  int right (int i) { return 2*i+2; }

private:

  int count_;

};

template <class EXT_ID, class ACE_LOCK>
class JAWS_FIFO_Cache_Replacement
  : public JAWS_Cache_Replacement<EXT_ID, ACE_LOCK>
{
public:

  JAWS_FIFO_Cache_Replacement (ACE_Allocator *alloc = 0);
  JAWS_FIFO_Cache_Replacement (size_t max_entries, ACE_Allocator *alloc = 0);

  //FUZZ: disable check_for_lack_ACE_OS
  int open ( size_t max_entries = ACE_DEFAULT_MAP_SIZE,
             ACE_Allocator *alloc = 0);

  int close (void);
  //FUZZ: enable check_for_lack_ACE_OS

  ~JAWS_FIFO_Cache_Replacement (void);

  int insert (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  int update (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  int remove (JAWS_Cache_Map_Entry<EXT_ID> *entry);
  JAWS_Cache_Map_Entry<EXT_ID> * expire (void);

  int head_;
  int tail_;

  JAWS_FIFO_Cache_Replacement_Info *info_;

};


template <class EXT_ID, class INT_ID, class HASH_FUNC, class EQ_FUNC, class ACE_LOCK>
class JAWS_Cache_Map_Manager
{
public:


  JAWS_Map_Manager<EXT_ID, INT_ID, EQ_FUNC, ACE_
};

#endif /* JAWS_CACHE_MAP_MANAGER_T_CPP */
