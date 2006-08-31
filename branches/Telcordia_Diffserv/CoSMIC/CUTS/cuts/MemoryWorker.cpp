// $Id$

#include "cuts/MemoryWorker.h"
#include "ace/Guard_T.h"

#define CUTS_MEMORY_ALLOC_SIZE 1024

#if !defined (__CUTS_INLINE__)
#include "cuts/MemoryWorker.inl"
#endif

CUTS_ACTION_TABLE_BEGIN (CUTS_Memory_Worker, CUTS_Worker)
CUTS_ACTION_TABLE_ENTRY ("Allocate_Memory",
                         CUTS_Memory_Worker::Allocate_Memory);
CUTS_ACTION_TABLE_ENTRY ("Deallocate_Memory",
                         CUTS_Memory_Worker::Deallocate_Memory);
CUTS_ACTION_TABLE_END (CUTS_Memory_Worker)

//
// CUTS_Memory_Worker
//
CUTS_Memory_Worker::CUTS_Memory_Worker (void)
{

}

//
// ~CUTS_Memory_Worker
//
CUTS_Memory_Worker::~CUTS_Memory_Worker (void)
{
  // delete all the remaining memory in the container
  Memory_Allocations::iterator iter;

  for ( iter = this->memory_.begin ();
        iter != this->memory_.end ();
        iter ++)
  {
    delete [] (*iter);
  }
}

//
// allocate
//
void CUTS_Memory_Worker::allocate (size_t kilobytes)
{
  while (kilobytes -- > 0)
  {
    char * allocation = new char [CUTS_MEMORY_ALLOC_SIZE];

    do
    {
      ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);
      this->memory_.push_back (allocation);
    } while (0);
  }
}

//
// deallocate
//
void CUTS_Memory_Worker::deallocate (size_t kilobytes)
{
  // Make sure we are not trying to deallocate more memory
  // that what is currently allocated.
  if (kilobytes > this->memory_.size ())
    kilobytes = this->memory_.size ();

  while (kilobytes -- > 0)
  {
    char * memory = 0;
    do
    {
      ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);

      // get the next allocation on the <memory_> stack
      memory = this->memory_.front ();
      this->memory_.pop_front ();
    } while (0);

    // delete the piece of
    delete [] memory;
  }
}
