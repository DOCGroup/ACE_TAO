//$Id$

#ifndef SYNCH_I_H
#define SYNCH_I_H

#include "SynchS.h"

class Synch_i : public POA_Synch
{
 public:

  Synch_i (void);
  virtual void go (CORBA::Long base_time
                   ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  int synched (void);

  ACE_Time_Value* base_time (void);

 private:
  int synched_;
  ACE_Time_Value base_time_;
};

#endif /*SYNCH_I_H*/
