
// $Id$

#include "Condition_i.h"
#include "ace/Task.h"

/* In order to test our Condition we'll derive from ACE_Task<> so that
   we can have several threads accessing the condition variable
   together.
 */
class Test : public ACE_Task<ACE_NULL_SYNCH>
{
public:
     // Construct the condition variable with an initial value.
    Test( Condition::value_t _value );
    ~Test(void);

     // Open the Task with enough threads to make a useful test.
    int open(void);
    
protected:
     // Each thread will do work on the Condition.
    int svc(void);

     // Override this method to modify the Condition in some way.
    virtual void modify(void) = 0;
     // Override this to test the Condition in some way.
    virtual void test(void) = 0;

     // How many threads to use in the test.  This is also used in the
     // modify() and test() methods of the derivatives.
    static const int max_threads_;

     // We want to sleep for a random amount of time to simulate
     // work.  The seed is necessary for proper random number generation.
    ACE_RANDR_TYPE seed_;

     // This is the actual condition variable set.
    Condition condition_;
};

// Set the number of threads.
const int Test::max_threads_ = 5;

// Initialize the condition variable.
Test::Test( Condition::value_t _value )
        : condition_(_value)
{
    ;
}

Test::~Test(void)
{
    ;
}

// Seed the random number generator and start the threads.
int Test::open(void)
{
    seed_ = ACE_OS::gettimeofday().usec();
    
    ACE_OS::srand( seed_ );
            
    return this->activate(THR_NEW_LWP, max_threads_);
}

/* Each thread will modify the condition variable in some way and then 
   wait for the condition to be satisfied.  The derived classes
   overload modify() and test() to implement a specific test of the
   Condition class.
 */
int Test::svc(void)
{
        // Take a moment before we modify the condition.  This will
        // cause test() in other threads to delay a bit.
    int stime = ACE_OS::rand_r( seed_ ) % 5;
    ACE_OS::sleep(abs(stime)+2);
    
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() befor modify, condition_ is:  %d\n", (int)condition_ ));

     // Change the condition variable's value
    modify();
    
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() after modify, condition_ is:  %d\n", (int)condition_ ));

     // Test for the condition we want
    test();
            
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() leaving.\n" ));
    
    return(0);
}

/* Test Condition::operator!=()
   The task's svc() method will increment the condition variable and
   then wait until the variable's value reaches max_threads_.
 */
class Test_ne : public Test
{
public:
     // Initialize the condition variable to zero since we're counting up.
    Test_ne(void)
            : Test(0)
        {
            ACE_DEBUG ((LM_INFO, "\n(%P|%t|%T)\tTesting condition_ != %d\n", max_threads_));
        }

     // Increment the variable
    void modify(void)
        {
            ++condition_;
        }

     // Wait until it equals max_threads_
    void test(void)
        {
            condition_ != max_threads_;
        }
};

/* Test Condition::operator>=()
   Each svc() method will decrement the condition variable and wait
   until it is less than max_threads_.  To do this correctly, we have
   to be careful where we start the condition variable.
 */
class Test_ge : public Test
{
public:
     // For max_threads_ == 5, we will start the condition variable at 
     // the value 9.  When the "last" thread decrements it, the value
     // will be 4 which satisfies the condition.
    Test_ge(void)
            : Test(max_threads_*2-1)
        {
            ACE_DEBUG ((LM_INFO, "\n(%P|%t|%T)\tTesting condition_ >= %d\n", max_threads_));
        }

     // Decrement by one...
    void modify(void)
        {
            --condition_;
        }

     // while( value >= max_threads_ ) wait();
    void test(void)
        {
            condition_ >= max_threads_;
        }
};

/* Test Condition::operator<=()
   This time we will increment the condition until it is greater than
   max_threads_.  Again, we have to be careful where we start the
   value and how we increment.
 */
class Test_le : public Test
{
public:
     // I'm starting the value at 1 so that if we increment by one in
     // each thread, the "last" thread (of 5) will set the value to
     // 6.  Since I actually increment by 2, we could start somewhat lower.
    Test_le(void)
            : Test(1)
        {
            ACE_DEBUG ((LM_INFO, "\n(%P|%t|%T)\tTesting condition_ <= %d\n", max_threads_));
        }

     // Try out Condition::operator+=(int)
     // This will cause the third thread to satisfy the condition.
    void modify(void)
        {
            condition_ += 2;
        }

     // while( value <= max_threads_ ) wait();
    void test(void)
        {
            condition_ <= max_threads_;
        }
};

/* For our final test, we'll go after Condition::operator=(Condition::Compare)
   By deriving from Condition::Compare we can perform any arbitrary
   test on the value of the condition variable.
 */
class Test_fo : public Test
{
public:
     // We'll be using operator*=(int) to increment the condition
     // variable, so we need to start with a non-zero value.
    Test_fo(void)
            : Test(1)
        {
            ACE_DEBUG ((LM_INFO, "\n(%P|%t|%T)\tTesting condition_ == FunctionObject\n" ));
        }

     // Double the value for each thread that we have.
    void modify(void)
        {
            condition_ *= 2;
        }

     /* Derive our CompareFunction and provide the operator() that
        performs our test.  In this case, we'll compare the value to
        the number 32.
      */
    class CompareFunction : public Condition::Compare
    {
    public:
         // When this returns non-zero, the condition test operator
         // will unblock in each thread.
        int operator() ( Condition::value_t _value )
            {
                return _value == 32;
            }
    };

     // Create the CompareFunction and wait for the condition variable 
     // to reach the state we want.
    void test(void)
        {
            CompareFunction compare;
            condition_ == compare;
        }
};

/* In main() we just instantiate each of the four test objects that we 
   created.  After open()ing each, we wait() for it's threads to exit.
 */
int main(int, char **)
{
    Test_ne test_ne;
    test_ne.open();
    test_ne.wait();
    
    Test_ge test_ge;
    test_ge.open();
    test_ge.wait();
    
    Test_le test_le;
    test_le.open();
    test_le.wait();
    
    Test_fo test_fo;
    test_fo.open();
    test_fo.wait();
    
    return(0);
}
