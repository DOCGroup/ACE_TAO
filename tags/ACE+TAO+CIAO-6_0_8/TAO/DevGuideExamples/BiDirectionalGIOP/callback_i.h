// $Id$

#ifndef CALLBACKI_H_
#define CALLBACKI_H_

#include "callbackS.h"

class  Callback_i : public virtual POA_Callback
{
public:
  Callback_i (CORBA::ORB_ptr orb);

  virtual ~Callback_i (void);

  virtual void shutdown (void);

  virtual void callback_method (void);

private:
  CORBA::ORB_var orb_;
};


#endif /* CALLBACKI_H_  */
