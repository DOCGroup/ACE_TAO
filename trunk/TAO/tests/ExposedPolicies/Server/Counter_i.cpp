//$Id$

#include "Counter_i.h"

// Dtor-Ctor Implementation

CounterServant::CounterServant ( void ) : count_ (0) 
{

}

CounterServant::~CounterServant ( void )
{

}


// Counter Interface Methods Implementation

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

