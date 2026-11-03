// -*- C++ -*-


#ifndef JAWS_CACHE_OBJECT_H
#define JAWS_CACHE_OBJECT_H

#include "ace/Thread_Mutex.h"
#include "ace/Synch_Traits.h"
#include "ace/Malloc.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/Lock_Adapter_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Allocator;
ACE_END_VERSIONED_NAMESPACE_DECL


// Cache bucket -- use Hash_Bucket to hold cacheable objects.

class JAWS_Cache_Object
{
public:
  JAWS_Cache_Object (const void *, size_t);
  virtual ~JAWS_Cache_Object ();
  JAWS_Cache_Object (JAWS_Cache_Object const &) = delete;
  JAWS_Cache_Object (JAWS_Cache_Object &&) = delete;
  JAWS_Cache_Object &operator= (JAWS_Cache_Object const &) = delete;
  JAWS_Cache_Object &operator= (JAWS_Cache_Object &&) = delete;

  void *internal () const;
  void internal (void *);

  const void *data () const;
  size_t size () const;
  unsigned int count () const;

  int acquire ();
  int release ();

  time_t last_access () const;
  time_t first_access () const;

  unsigned int priority () const;

  enum { ACE_CO_REFERENCED, ACE_CO_COUNTED };

  void *heap_item () const;
  void heap_item (void *item);

  virtual ACE_Lock & lock () = 0;

protected:
  virtual unsigned int count_i () const = 0;
  virtual int acquire_i () = 0;
  virtual int release_i () = 0;
  virtual unsigned int priority_i () const = 0;

private:
  void *internal_;
  const void *data_;
  size_t size_;

  time_t last_access_;
  time_t first_access_;
  time_t new_last_access_;

  void *heap_item_;
};

class JAWS_Referenced_Cache_Object : public JAWS_Cache_Object
{
public:
  JAWS_Referenced_Cache_Object (const void *, size_t);
  ~JAWS_Referenced_Cache_Object () override;

  ACE_Lock & lock () override;

protected:
  unsigned int count_i () const override;
  int acquire_i () override;
  int release_i () override;
  unsigned int priority_i () const override;

private:
  mutable ACE_SYNCH_RW_MUTEX count_;
  mutable ACE_Lock_Adapter<ACE_SYNCH_RW_MUTEX> lock_adapter_;
};

class JAWS_Counted_Cache_Object : public JAWS_Cache_Object
{
public:
  JAWS_Counted_Cache_Object (const void *, size_t);
  ~JAWS_Counted_Cache_Object () override;

  ACE_Lock & lock () override;

protected:
  unsigned int count_i () const override;
  int acquire_i () override;
  int release_i () override;
  unsigned int priority_i () const override;

private:
  unsigned int count_;
  unsigned int new_count_;
  mutable ACE_SYNCH_MUTEX lock_;
  mutable ACE_Lock_Adapter<ACE_SYNCH_MUTEX> lock_adapter_;
};

class JAWS_Cache_Object_Factory
{
public:
  explicit JAWS_Cache_Object_Factory (ACE_Allocator *alloc = nullptr);
  virtual ~JAWS_Cache_Object_Factory ();

  int open (ACE_Allocator *alloc = nullptr);

  virtual JAWS_Cache_Object * create (const void *, size_t) = 0;
  virtual void destroy (JAWS_Cache_Object *) = 0;

protected:
  ACE_Allocator *allocator_;
};

class JAWS_Referenced_Cache_Object_Factory : public JAWS_Cache_Object_Factory
{
public:
  explicit JAWS_Referenced_Cache_Object_Factory (ACE_Allocator *alloc = nullptr);
  ~JAWS_Referenced_Cache_Object_Factory () override;

  JAWS_Cache_Object * create (const void *, size_t) override;
  void destroy (JAWS_Cache_Object *) override;
};

class JAWS_Counted_Cache_Object_Factory : public JAWS_Cache_Object_Factory
{
public:
  explicit JAWS_Counted_Cache_Object_Factory (ACE_Allocator *alloc = nullptr);
  ~JAWS_Counted_Cache_Object_Factory () override;

  JAWS_Cache_Object * create (const void *, size_t) override;
  void destroy (JAWS_Cache_Object *) override;
};

#endif /* JAWS_CACHE_OBJECT_H */
