/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_DATA_BLOCK_H)
#define JAWS_DATA_BLOCK_H

#include "ace/Singleton.h"

#include "JAWS/Pipeline.h"

class JAWS_IO_Handler;
class JAWS_Dispatch_Policy;
class JAWS_Data_Block;

typedef JAWS_Pipeline_Abstract_Handler<JAWS_Data_Block>
        JAWS_Pipeline_Handler;

class JAWS_Data_Block : public ACE_Data_Block
// = TITLE
//   Defines the communication unit between pipeline components
{
public:
  JAWS_Pipeline_Task *task (void);
  ACE_INET_Addr *addr (void);
  JAWS_IO_Handler * handler (void);
  JAWS_Dispatch_Policy * policy (void);

  void task (JAWS_Pipeline_Handler *taskp);
  void addr (ACE_INET_Addr *addrp);
  void handler (JAWS_IO_Handler * handlerp);
  void policy (JAWS_Dispatch_Policy * policyp);

private:
  JAWS_IO_Handler *handler_;
  JAWS_Dispatch_Policy *policy_;
  JAWS_Pipeline_Handler *task_;
  ACE_INET_Addr *addr_;
};

class JAWS_Pipeline_Accept_Task : public JAWS_Pipeline_Handler
{
public:
  virtual int handle_put (JAWS_Data_Block *data, ACE_Time_Value *tv);

private:
};

typedef ACE_Singleton<JAWS_Pipeline_Accept_Task, ACE_SYNCH_MUTEX>
        JAWS_Pipeline_Accept_Task_Singleton;

#endif /* !defined (JAWS_DATA_BLOCK_H) */
