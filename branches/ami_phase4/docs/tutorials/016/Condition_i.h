// $Id$

#ifndef CONDITION_H
#define CONDITION_H

#include "ace/Synch.h"

/** A wrapper for ACE_Condition<>.  When you're using an
    ACE_Condition<> you have to have three things: - Some variable
    that embodies the condition you're looking for - A mutex to
    prevent simultaneous access to that variable from different
    threads - An ACE_Condition<> that enables blocking on state
    changes in the variable The class I create here will contain those
    three things.  For the actual condition variable I've chosen an
    integer.  You could easily turn this class into a template
    parameterized on the condition variable's data type if 'int' isn't
    what you want.  */
class Condition
{
public:
  // From here on I'll use value_t instead of 'int' to make any
  // future upgrades easier.
  typedef int value_t;
    
  // Initialize the condition variable
  Condition (value_t value = 0);
  ~Condition (void);

  /* I've created a number of arithmetic operators on the class that
     pass their operation on to the variable.  If you turn this into a
     template then some of these may not be appropriate...  For the
     ones that take a parameter, I've stuck with 'int' instead of
     'value_t' to reinforce the fact that you'll need a close look at
     these if you choose to change the 'value_t' typedef.  */

  // Increment & decrement
  Condition &operator++ (void);
  Condition &operator-- (void);

  // Increase & decrease
  Condition &operator+= (int inc);
  Condition &operator-= (int inc);

  // Just to be complete
  Condition &operator*= (int inc);
  Condition &operator/= (int inc);
  Condition &operator%= (int inc);

  // Set/Reset the condition variable's value
  Condition &operator= (value_t value);

  /* These four operators perform the actual waiting.  For instance:

     operator!=(int _value)

     is implemented as:

     Guard guard(mutex_)
     while( value_ != _value )
     condition_.wait();

     This is the "typical" use for condition mutexes.  Each of the
     operators below behaves this way for their respective
     comparisions.

     To use one of these in code, you would simply do:

     Condition mycondition;
     ...
     // Wait until the condition variable has the value 42
     mycondition != 42
     ...  */

  // As long as the condition variable is NOT EQUAL TO <value>, we wait
  int operator!= (value_t value);

  // As long as the condition variable is EXACTLY EQUAL TO <value>, we
  // wait
  int operator== (value_t value);

  // As long as the condition variable is LESS THAN OR EQUAL TO
  // <value>, we wait
  int operator<= (value_t value);

  // As long as the condition variable is GREATER THAN OR EQUAL TO
  // <value>, we wait
  int operator>= (value_t value);

  // Return the value of the condition variable
  operator value_t (void);

  /* In addition to the four ways of waiting above, I've also create a
     method that will invoke a function object for each iteration of
     the while() loop.  Derive yourself an object from
     Condition::Compare and overload operator()(value_t) to take
     advantage of this.  Have the function return non-zero when you
     consider the condition to be met.  */
  class Compare
  {
  public:
    virtual int operator() (value_t value) = 0;
  };

  /* Wait on the condition until _compare(value) returns non-zero.
     This is a little odd since we're not really testing equality.
     Just be sure that _compare(value_) will return non-zero when you
     consider the condition to be met.  */
  int operator== (Compare & compare);
                    
private:
  // Prevent copy construction and assignment.
  Condition (const Condition &condition);
  Condition &operator= (const Condition &condition);

  /* Typedefs make things easier to change later.
     ACE_Condition_Thread_Mutex is used as a shorthand for
     ACE_Condition<ACE_Thread_Mutex> and also because it may provide
     optimizations we can use.  */
  typedef ACE_Thread_Mutex mutex_t;
  typedef ACE_Condition_Thread_Mutex condition_t;
  typedef ACE_Guard<mutex_t> guard_t;

  // The mutex that keeps the data save
  mutex_t mutex_;

  // The condition mutex that makes waiting on the condition easier.
  condition_t *condition_;

  // The acutal variable that embodies the condition we're waiting
  // for.
  value_t value_;

  // Accessors for the two mutexes.
  mutex_t &mutex (void)
  {
    return this->mutex_;
  }

  condition_t &condition (void)
  {
    return *this->condition_;
  }

  // This particular accessor will make things much easier if we
  // decide that 'int' isn't the correct datatype for value_.  Note
  // that we keep this private and force clients of the class to use
  // the cast operator to get a copy of the value.
  value_t &value (void)
  {
    return this->value_;
  }
};

#endif /* CONDITION_H */
