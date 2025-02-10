#ifndef SYNCH_I_H
#define SYNCH_I_H

#include "SynchS.h"

class SYNCH_Export Synch_i : public POA_Synch
{
 public:
  Synch_i ();
  virtual void go (CORBA::Long base_time);

  int synched ();

  ACE_Time_Value* base_time ();

 private:
  int synched_;
  ACE_Time_Value base_time_;
};

#endif /*SYNCH_I_H*/
