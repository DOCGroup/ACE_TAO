//$Id$

#ifndef COUNTER_I_H_
#define COUNTER_I_H_

#include "CounterS.h"

class CounterServant : public POA_Counter {

public:
  
  // Ctor-Dtor Declaration
  CounterServant ( void );
  virtual ~CounterServant ( void  );

  // Counter Interface Methods Overloading.  
  virtual void increment ( void );
  virtual void decrement ( void );
  virtual void reset ( void );
  virtual CORBA::Long get_count ( void );

protected:
  CORBA::Long count_;
};

#endif
