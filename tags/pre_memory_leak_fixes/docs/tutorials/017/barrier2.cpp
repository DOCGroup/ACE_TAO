
// $Id$

#include "Barrier_i.h"
#include "ace/Task.h"

/* We'll use a simple Task<> derivative to test our new Barrier
   object.
*/
class Test : public ACE_Task<ACE_NULL_SYNCH>
{
public:

        // Construct the object with a desired thread count
    Test(int _threads);

        // Open/begin the test.  As usual, we have to match the
        // ACE_Task signature.
    int open(void * _unused = 0);

        // Change the threads_ value for the next invocation of open()
    void threads(int _threads);

        // Get the current threads_ value.
    int threads(void);

        // Perform the test
    int svc(void);

protected:
        // How many threads the barrier will test.
    u_int threads_;

        // The Barrier object we'll use in our tests below
    Barrier barrier_;

        // This lets us pick one (eg -- the first) thread as the
        // "controller" for our little test...
    ACE_Atomic_Op<ACE_Mutex,u_int> tcount_;
};

/* Construct the object & initialize the threads value for open() to
   use.
*/
Test::Test(int _threads)
        : threads_(_threads), tcount_(0)
{
}

/* As usual, our open() will create one or more threads where we'll do 
   the interesting work.
*/  
int Test::open(void * _unused)
{
    ACE_UNUSED_ARG(_unused);

        // One thing about the barrier:  You have to tell it how many
        // threads it will be synching.  The threads() mutator on my
        // Barrier class lets you do that and hides the implementation 
        // details at the same time.
    barrier_.threads(threads_);

        // Activate the tasks as usual...
    return this->activate(THR_NEW_LWP, threads_, 1);
}

void Test::threads(int _threads)
{
    threads_ = _threads;
}

int Test::threads(void)
{
    return threads_;
}

/* svc() will execute in each thread & do a few things with the
   Barrier we have.
 */
int Test::svc(void)
{
        // Say hello to everyone first.
    ACE_DEBUG(( LM_INFO, "(%P|%t|%T) Created\n" ));

        // Increment and save the "tcount" value.  We'll use it in
        // just a moment...
    int me = ++tcount_;

        // Wait for all initial threads to get to this point before we
        // go any further.  This is standard barrier usage...
    barrier_.wait();

        // Setup our random number generator.
    ACE_Time_Value now(ACE_OS::gettimeofday());
    ACE_RANDR_TYPE seed = now.usec();
    ACE_OS::srand(seed);
    int delay;

        // We'll arbitrarily choose the first activated thread to be
        // the controller.  After it sleeps a few seconds, it will add 
        // five threads.
    if( me == 1 )
    {
            // Sleep from 1 to 10 seconds so that some of the other
            // threads will be into their for() loop.
        delay = ACE_OS::rand_r(seed)%10;
        ACE_OS::sleep(abs(delay)+1);

            // Make ourselves the barrier owner so that we can change
            // the number of threads.  This should be done with care...
        barrier_.owner( ACE_OS::thr_self() );

            // Add 5 threads to the barrier and then activate() to
            // make them real.  Notice the third parameter to
            // activate().  Without this parameter, the threads won't
            // be created.
        if( barrier_.threads(threads_+5) == 0 )
        {
            this->activate(THR_NEW_LWP,5,1);
        }
    }

        // This for() loop represents an "infinite" work loop in an
        // application. The theory is that the threads are dividing up 
        // some work but need to "recalibrate" if more threads are
        // added.  I'll just do five iterations so that the test
        // doesn't run forever.
    int i;
    for( i = 0 ; i < 5 ; ++i )
    {
            // The sleep() represents time doing work.
        delay = ACE_OS::rand_r(seed)%7;
        ACE_OS::sleep(abs(delay)+1);

        ACE_DEBUG(( LM_INFO, "(%P|%t|%T)\tThread %.2d of %.2d iteration %.2d\n", me, threads_, i ));
 
            // If the local threads_ variable doesn't match the number 
            // in the barrier, then the controller must have changed
            // the thread count.  We'll wait() for everyone and then
            // recalibrate ourselves before continuing.
        if( this->threads_ != barrier_.threads() )
        {
            ACE_DEBUG(( LM_INFO, "(%P|%t|%T) Waiting for thread count to increase to %d from %d\n",
                        barrier_.threads(), this->threads_ ));

                // Wait for all our sibling threads...
            barrier_.wait();

                // Set our local variable so that we don't come here again.
            this->threads_ = barrier_.threads();

                // Recalibration can be anything you want.  At this
                // point, we know that all of the threads are synch'd
                // and ready to go.
        }
    }

        // Re-synch all of the threads before they exit.  This isn't
        // really necessary but I like to do it.
    barrier_.done();

    return(0);
}

/* Our test application...
 */
int main(int, char**)
{
        // Create the test object with 5 threads
    Test test(5);

        // and open it to test the barrier.
    test.open();
        // Now wait for them all to exit.
    test.wait();

    return(0);
}
