//$Id$

#include "Counter_i.h"

// @@ Angelo, please add rcsid string here.
// Dtor-Ctor Implementation

CounterServant::CounterServant ( void )
  : count_ (0)
  // @@ Angelo, remove extra spaces in paranthesis.
{
}

CounterServant::~CounterServant ( void )
{
}


// Counter Interface Methods Implementation
// @@ Angelo, please fix the class name.
void
CounterServant::increment ( void )
{
  ++this->count_;
}

void
CounterServant::decrement ( void )
{
  --this->count_;
}

void
CounterServant::reset ( void )
{
  this->count_ = 0;
}

CORBA::Long
CounterServant::get_count ( void )
{
  return this->count_;
}
