
// $Id$ 

#include "mpool.h"

#if !defined (ACE_LACKS_SYSV_SHMEM)

/*
  Set the values of all of the constants.  This guarantees that client
  and server don't get confused.
 */
const int Constants::SEM_KEY_1 = ACE_DEFAULT_SEM_KEY + 1;
const int Constants::SEM_KEY_2 = ACE_DEFAULT_SEM_KEY + 2;

const int Constants::SHMSZ = 27;
const char * Constants::SHMDATA = "abcdefghijklmnopqrstuvwxyz";

const char * Constants::PoolName = "SharedMemoryPool";
const char * Constants::RegionName = "Alphabet";

/*
  We have to create a copy of the _name parameter in case the caller
  has dynamically allocated it.  The pool_ is set to NULL & will be
  allocated by the accessor.
 */
Allocator::Allocator (const char *_name)
  : name_ (ACE_OS::strdup (_name)),
    pool_ (0)
{
  if (name_ == 0)
    ACE_ERROR ((LM_ERROR, "(%P) %p",
                "Allocator::Allocator cannot strdup pool name"));
}

Allocator::~Allocator (void)
{
  /*
    strdup() uses malloc(), so we must use free() to clean up.
    */
  if (name_)
    ACE_OS::free (name_);

  // delete doesn't really care if you give it a NULL pointer.
  delete pool_;
}

/*
  Allocate the pool.  Since we return a reference, we'll be in really
  bad shape if the new fails.  This is a great place to throw an
  exception!
  The other concern is thread safety.  If two threads call here at
  about the same time, we may create the pool twice.  We can't use a
  Singleton because we want to have multiple Allocator instances.  The 
  Singleton techniques can be used though.
 */

Allocator::pool_t & 
Allocator::pool (void)
{
  if (pool_ == 0)
    pool_ = new pool_t (name_);

  return *pool_;
}

#endif /* ACE_LACKS_SYSV_SHMEM */
