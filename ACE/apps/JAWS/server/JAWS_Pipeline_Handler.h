/* -*- c++ -*- */
#ifndef JAWS_PIPELINE_HANDLER_H
#define JAWS_PIPELINE_HANDLER_H

#include "JAWS_Pipeline.h"

template <class TYPE>
class JAWS_Pipeline_Handler : public JAWS_Pipeline_Task
  // = TITLE
  //   Methods that are common to pipeline components
{
public:
  JAWS_Pipeline_Handler ();
  // ACE_Task hooks

  virtual int put (ACE_Message_Block *mb, ACE_Time_Value *tv = 0);
  // inherited from ACE_Task

  virtual int handle_put (TYPE *data, ACE_Time_Value *tv) = 0;
  // Callback hook for specialized data processing
};

#include "JAWS_Pipeline_Handler.cpp"

#endif /* !defined (JAWS_PIPELINE_HANDLER_H) */
