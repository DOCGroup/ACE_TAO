
// $Id$

#include "Barrier_i.h"

/* Initialize the threads_ count to zero and the barrier_ pointer to a
   safe value.  At the same time, we remember the thread that created
   us so that we can allow it to change the thread count.
*/
Barrier::Barrier(void)
   : threads_(0)
    ,barrier_(0)
{
    owner_ = ACE_OS::thr_self();
}

/* Ensure that barrier_ get's deleted so that we don't have a memory leak.
 */
Barrier::~Barrier(void)
{
    delete barrier_;
}

// Report on the number of threads.
u_int Barrier::threads(void)
{
    return threads_.value();
}

/* Allow the owning thread to (re)set the number of threads.
   make_barrier() is called because it will wait() if we were already
   configured.  Typical usage would be for the worker threads to
   wait() while the primary (eg -- owner) thread adjusts the thread
   count.

   For instance:
       In the worker threads:
           if( myBarrier.threads() != current_thread_count )
               myBarrier.wait();

       In the primary thread:
           if( myBarrier.threads() != current_thread_count )
               myBarrier.threads( current_thread_count, 1 );
 */
int Barrier::threads( u_int _threads, int _wait )
{
    if( ! ACE_OS::thr_equal(ACE_OS::thr_self(), owner_) )
    {
        return -1;
    }

    threads_ = _threads;
    
    return make_barrier(_wait);
}

/* Wait for all threads to synch if the thread count is valid.  Note
   that barrier_ will be 0 if the threads() mutator has not been
   invoked.
*/
int Barrier::wait(void)
{
    if( ! barrier_ )
    {
        return -1;
    }

    return barrier_->wait();
}

/* Wait for all threads to synch.  As each thread passes wait(), it
   will decrement our thread counter.  (That is why we had to make
   threads_ an atomic op.)  When the last thread decrements the
   counter it will also delete the ACE_Barrier & free up a little
   memory.
*/
int Barrier::done(void)
{
    if( this->wait() == -1 )
    {
        return -1;
    }

    --threads_;

    if( ! threads_.value() )
    {
        delete barrier_;
        barrier_ = 0;
    }

    return 0;
}

/* This will build the actual barrier.  I broke this code out of the
   threads() mutator in case it might be useful elsewhere.
   If a barrier already exists, we will wait for all threads before
   creating a new one.  This trait is what allows the threads mutator
   to be used as shown above.
 */
int Barrier::make_barrier( int _wait )
{
        // Wait for and delete any existing barrier.
    if( barrier_ )
    {
        if( _wait )
        {
            barrier_->wait();
        }
        delete barrier_;
    }

        // Ensure we have a valid thread count.
    if( ! threads_.value() )
    {
        return -1;
    }

        // Create the actual barrier.  Note that we initialize it with 
        // threads_.value() to set its internal thread count.  If the
        // 'new' fails we will return -1 to the caller.
    ACE_NEW_RETURN(barrier_,ACE_Barrier(threads_.value()),-1);

    return 0;
}
