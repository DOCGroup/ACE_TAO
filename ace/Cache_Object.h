/* -*- c++ -*- */
// $Id$

// @@ James, please add a standard "header" here like you see in all
// the other ACE headerfiles.

#ifndef ACE_CACHE_OBJECT_H
#define ACE_CACHE_OBJECT_H

#include "ace/OS.h"
#include "ace/Synch.h"
#include "ace/Malloc.h"

// @@ James, can you please update ALL of these classes and methods to
// use the standard ACE commenting style, i.e., add = TITLE and =
// DESCRIPTION headers and comments for each method.

// Cache bucket -- use Hash_Bucket to hold cacheable objects.

class ACE_Cache_Object
{
public:
  ACE_Cache_Object (const void *, size_t);
  virtual ~ACE_Cache_Object (void);

  const void *data (void) const;
  size_t size (void) const;
  u_int count (void) const;

  int acquire (void);
  int release (void);

  time_t last_access (void) const;
  time_t first_access (void) const;

  u_int priority (void) const;

  enum { ACE_CO_REFERENCED, ACE_CO_COUNTED };

  void *heap_item (void) const;
  void heap_item (void *item);

protected:
  virtual u_int count_i (void) const = 0;
  virtual int acquire_i (void) = 0;
  virtual int release_i (void) = 0;
  virtual u_int priority_i (void) const = 0;

private:
  const void *data_;
  size_t size_;

  time_t last_access_;
  time_t first_access_;
  time_t new_last_access_;

  void *heap_item_;
};

class ACE_Referenced_Cache_Object : public ACE_Cache_Object
{
public:
  ACE_Referenced_Cache_Object (const void *, size_t);
  virtual ~ACE_Referenced_Cache_Object (void);

protected:
  virtual u_int count_i (void) const;
  virtual int acquire_i (void);
  virtual int release_i (void);
  virtual u_int priority_i (void) const;

private:
  /* MUTABLE */ ACE_SYNCH_RW_MUTEX count_;
};

class ACE_Counted_Cache_Object : public ACE_Cache_Object
{
public:
  ACE_Counted_Cache_Object (const void *, size_t);
  virtual ~ACE_Counted_Cache_Object (void);

protected:
  virtual u_int count_i (void) const;
  virtual int acquire_i (void);
  virtual int release_i (void);
  virtual u_int priority_i (void) const;

private:
  u_int count_;
  u_int new_count_;
  /* MUTABLE */ ACE_SYNCH_MUTEX lock_;
};

class ACE_Cache_Object_Factory
{
public:
  ACE_Cache_Object_Factory (ACE_Allocator *alloc = 0);
  virtual ~ACE_Cache_Object_Factory (void);

  int open (ACE_Allocator *alloc = 0);

  virtual ACE_Cache_Object * create (const void *, size_t) = 0;
  virtual void destroy (ACE_Cache_Object *) = 0;

protected:
  ACE_Allocator *allocator_;
};

class ACE_Referenced_Cache_Object_Factory : public ACE_Cache_Object_Factory
{
public:
  ACE_Referenced_Cache_Object_Factory (ACE_Allocator *alloc = 0);
  virtual ~ACE_Referenced_Cache_Object_Factory (void);

  virtual ACE_Cache_Object * create (const void *, size_t);
  virtual void destroy (ACE_Cache_Object *);
};

class ACE_Counted_Cache_Object_Factory : public ACE_Cache_Object_Factory
{
public:
  ACE_Counted_Cache_Object_Factory (ACE_Allocator *alloc = 0);
  virtual ~ACE_Counted_Cache_Object_Factory (void);

  virtual ACE_Cache_Object * create (const void *, size_t);
  virtual void destroy (ACE_Cache_Object *);
};

#endif /* UTL_CACHE_OBJECT_H */
