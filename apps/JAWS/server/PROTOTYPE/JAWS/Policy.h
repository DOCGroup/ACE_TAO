/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_POLICY_H)
#define JAWS_POLICY_H

#include "JAWS/Concurrency.h"

class JAWS_IO;
class JAWS_IO_Handler;
class JAWS_IO_Handler_Factory;

class JAWS_Synch_Dispatch_Policy : public JAWS_Dispatch_Policy
{
public:
  virtual JAWS_Concurrency_Base * concurrency (void);
  virtual JAWS_IO * io (void);

private:
  JAWS_IO_Handler_Factory *factory_;
  JAWS_IO_Handler *ioh_;
  JAWS_IO &io_;
};

#endif /* !defined (JAWS_POLICY_H) */
