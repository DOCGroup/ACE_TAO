
// $Id$

#include "Barrier_i.h"

/* Initialize the threads_ count to zero and the barrier_ pointer to a
   safe value.  At the same time, we remember the thread that created
   us so that we can allow it to change the thread count.
*/
Barrier::Barrier(void)
   : threads_(0)
    ,barrier_(0)
    ,new_barrier_(0)
{
    owner_ = ACE_OS::thr_self();
}

/* Ensure that barrier_ get's deleted so that we don't have a memory leak.
 */
Barrier::~Barrier(void)
{
    delete barrier_;
}

void Barrier::owner( ACE_thread_t _owner )
{
    owner_ = _owner;
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

        // If the threads() mutator has been used, new_barrier_ will
        // point to a new ACE_Barrier instance.  We'll use a
        // traditional double-check here to move that new object into
        // place and cleanup the old one.
    if( new_barrier_ )
    {
            // mutex so that only one thread can do this part.
        ACE_Guard<ACE_Mutex> mutex(barrier_mutex_);

            // We only want the first thread to plug in the new barrier...
        if( new_barrier_ )
        {
                // out with the old and in with the new.
            delete barrier_;
            barrier_ = new_barrier_;
            new_barrier_ = 0;
        }
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
        // Ensure we have a valid thread count.
    if( ! threads_.value() )
    {
        return -1;
    }

        // If a barrier already exists, we'll arrange for it to be
        // replaced through the wait() method above.
    if( barrier_ )
    {
            // Create the new barrier that wait() will install for us.
        ACE_NEW_RETURN(new_barrier_,ACE_Barrier(threads_.value()),-1);

            // Wait for our siblings to synch before continuing
        if( _wait )
        {
            barrier_->wait();
        }
    }
    else
    {
            // Create the initial barrier.
        ACE_NEW_RETURN(barrier_,ACE_Barrier(threads_.value()),-1);
    }

    return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op <ACE_Mutex, u_int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op <ACE_Mutex, u_int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
