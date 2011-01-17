// $Id$

// -*- C++ -*-
#ifndef INNER_CB_H
#define INNER_CB_H

#include "innerS.h"
#include "middleC.h"

class  Inner_callback_i : public virtual POA_AMI_InnerHandler
{
public:
  //Constructor
  Inner_callback_i (PortableServer::POA_ptr p,
                    AMH_MiddleResponseHandler_ptr rh);

  //Destructor
  virtual ~Inner_callback_i(void);

  virtual void answer (const char * ami_return_val);

  virtual void answer_excep (Messaging::ExceptionHolder * excep_holder);

private:
  PortableServer::POA_var poa_;
  AMH_MiddleResponseHandler_var response_handler_;
};

#endif
