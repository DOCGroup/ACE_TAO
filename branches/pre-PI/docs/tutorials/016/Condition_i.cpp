
// $Id$

// Get or declaration
#include "Condition_i.h"

/* Initialize the condition variable and create the condition mutex.
   Since I don't have any guarantees on the order of member variable
   initialization, I have to new the condition mutex instead of
   simply constructing it.
 */
Condition::Condition(value_t _value)
        : value_(_value)
{
    condition_ = new condition_t( this->mutex() );
}

Condition::~Condition(void)
{
     // Be sure we don't have a memeory leak
    delete condition_;
}

/* The cast operator is the easiest way to return a copy of the value
   to clients of the class.  It also allows us to use a private method 
   for getting a reference to the value when we need to modify it.
 */
Condition::operator Condition::value_t (void)
{
     // Place a guard around the variable so that it won't change as
     // we're copying it back to the client.
    guard_t    guard(mutex_);
    return value();
}

/* Traditional prefix increment operator.
   We place a guard around the operation so that we don't collide with 
   any other threads.  After the modification, we broadcast() a
   condition change to any waiting threads.  You can also use signal() 
   but that will only tell one thread about the change.  If that
   thread, in turn, invokes signal() then all threads will eventually
   find out.  I just thought it would be easier to use broadcast() and 
   be done with it.
 */
Condition & Condition::operator++ (void)
{
    guard_t    guard(mutex_);

    ++value();

    condition().broadcast();

    return *this;
}

/* The remaining operators all follow the same pattern that we have
   above.  They only differ in the modification they make to the value().
 */

Condition & Condition::operator-- (void)
{
    guard_t    guard(mutex_);

    --value();

    condition().broadcast();

    return *this;
}

Condition & Condition::operator+= (int _inc)
{
    guard_t    guard(mutex_);

    value() += _inc;

    condition().broadcast();

    return *this;
}

Condition & Condition::operator-= (int _inc)
{
    guard_t    guard(mutex_);

    value() -= _inc;

    condition().broadcast();

    return *this;
}

Condition & Condition::operator*= (int _inc)
{
    guard_t    guard(mutex_);

    value() *= _inc;

    condition().broadcast();

    return *this;
}

Condition & Condition::operator/= (int _inc)
{
    guard_t    guard(mutex_);

    value() /= _inc;

    condition().broadcast();

    return *this;
}

Condition & Condition::operator%= (int _inc)
{
    guard_t    guard(mutex_);

    value() %= _inc;

    condition().broadcast();

    return *this;
}

Condition & Condition::operator= ( value_t _value )
{
    guard_t    guard(mutex_);

    value() = _value;

    condition().broadcast();

    return *this;
}

/* Now we get into the comparison area.
   Each one follows the pattern we've already established for
   waiters.
 */

/*
   We begin with an equality operator that expects a function object.
   In the while() test we pass a copy of the value to the function
   object for evaluation.  The object can then do any comparision it
   wants to check for a desired condition.  When the function object
   returns non-zero, the condition is met and we leave.
 */
int Condition::operator== ( Condition::Compare & _compare )
{
    guard_t    guard(mutex_);

    while( ! _compare(this->value()) )
        condition().wait();

    return 0;
}

// As long as the variable equals _value, we wait...
int Condition::operator== ( value_t _value )
{
    guard_t    guard(mutex_);

    while( value() == _value )
        condition().wait();

    return 0;
}

// As long as the variable is not equal to _value, we wait...
int Condition::operator!= ( value_t _value )
{
    guard_t    guard(mutex_);

    while( value() != _value )
        condition().wait();

    return 0;
}

// As long as the variable is less than or equal to _value, we wait...
int Condition::operator<= ( value_t _value )
{
    guard_t    guard(mutex_);

    while( value() <= _value )
        condition().wait();

    return 0;
}

// As long as the variable is greater than or equal to _value, we wait...
int Condition::operator>= ( value_t _value )
{
    guard_t    guard(mutex_);

    while( value() >= _value )
        condition().wait();

    return 0;
}
