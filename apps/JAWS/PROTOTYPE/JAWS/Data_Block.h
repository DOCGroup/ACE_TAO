/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_DATA_BLOCK_H)
#define JAWS_DATA_BLOCK_H

#include "ace/Singleton.h"
#include "JAWS/Pipeline.h"

class JAWS_IO_Handler;
class JAWS_Dispatch_Policy;
class JAWS_Data_Block;
class JAWS_Pipeline_Handler;

class JAWS_Data_Block : public ACE_Data_Block
// = TITLE
//   Defines the communication unit between pipeline components
{
public:
  JAWS_Pipeline_Handler *task (void);
  JAWS_IO_Handler *io_handler (void);
  JAWS_Dispatch_Policy *policy (void);

  void task (JAWS_Pipeline_Handler *taskp);
  void io_handler (JAWS_IO_Handler * handlerp);
  void policy (JAWS_Dispatch_Policy * policyp);

private:
  JAWS_IO_Handler *io_handler_;
  JAWS_Dispatch_Policy *policy_;
  JAWS_Pipeline_Handler *task_;
};

#endif /* !defined (JAWS_DATA_BLOCK_H) */
