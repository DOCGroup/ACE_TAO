//$Id$

// @@ Angelo, please put header with author, file name, etc. here.

#ifndef COUNTER_I_H_
#define COUNTER_I_H_

#include "CounterS.h"

class Counter_Servant : public POA_Counter
{
public:

  // = Ctor-Dtor Declaration

  Counter_Servant (void);
  virtual ~Counter_Servant (void);

  // = Counter Interface Methods Overloading.

  virtual void increment (void);
  virtual void decrement (void);
  virtual void reset (void);
  virtual CORBA::Long get_count (void);

protected:
  CORBA::Long count_;
};

#endif /*COUNTER_I_H_*/
