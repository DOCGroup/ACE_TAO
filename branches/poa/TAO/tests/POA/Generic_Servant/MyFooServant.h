#if !defined (MYFOOSERVANT_H)
#define MYFOOSERVANT_H

#include "FooS.h"

class MyFooServant : public POA_Foo
{
public:
  MyFooServant (PortableServer::POA_ptr poa, 
                CORBA::Long value);
  
  virtual ~MyFooServant (void);

  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  virtual CORBA::Long doit (CORBA::Environment &env);

protected:
  PortableServer::POA_var poa_;
  CORBA::Long value_;
};

#endif /* MYFOOSERVANT_H */
