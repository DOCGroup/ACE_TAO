/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_PIPELINE_TASKS_H)
#define JAWS_PIPELINE_TASKS_H

#include "ace/Singleton.h"

#include "JAWS/Pipeline.h"

class JAWS_Data_Block;

typedef JAWS_Pipeline_Abstract_Handler<JAWS_Data_Block>
        JAWS_Pipeline_Handler;

class JAWS_Pipeline_Accept_Task : public JAWS_Pipeline_Handler
{
public:
  virtual int handle_put (JAWS_Data_Block *data, ACE_Time_Value *tv);
};

typedef ACE_Singleton<JAWS_Pipeline_Accept_Task, ACE_SYNCH_MUTEX>
        JAWS_Pipeline_Accept_Task_Singleton;

#endif /* !defined (JAWS_DATA_BLOCK_H) */
