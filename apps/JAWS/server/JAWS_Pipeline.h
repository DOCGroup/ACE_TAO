/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_PIPELINE_H)
#define JAWS_PIPELINE_H

#include "ace/Synch.h"
#include "ace/Service_Config.h"
#include "ace/Stream.h"
#include "ace/Module.h"
#include "ace/Task.h"

typedef ACE_Stream<ACE_NULL_SYNCH> JAWS_Pipeline_Stream;
typedef ACE_Module<ACE_NULL_SYNCH> JAWS_Pipeline_Module;
typedef ACE_Task<ACE_NULL_SYNCH> JAWS_Pipeline_Task;

class JAWS_Pipeline : public JAWS_Pipeline_Task
  // = TITLE
  //   Methods that are common to pipeline components
{
public:
  JAWS_Pipeline (void);
  // ACE_Task hooks

  virtual int open (void * = 0);
  virtual int close (u_long = 0);
};

template <class TYPE>
class JAWS_Pipeline_Handler : public JAWS_Pipeline_Task
  // = TITLE
  //   Methods that are common to pipeline components
{
public:
  JAWS_Pipeline_Handler (void);
  // ACE_Task hooks

  virtual int put (ACE_Message_Block *mb, ACE_Time_Value *tv = 0);
  // inherited from ACE_Task

  virtual int handle_input (TYPE *data, ACE_Time_Value *tv) = 0;
  // Callback hook for specialized data processing
};


#endif /* !defined (JAWS_PIPELINE_H) */
