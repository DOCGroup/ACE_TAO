/* -*- c++ -*- */
#ifndef JAWS_PIPELINE_HANDLER_T_H
#define JAWS_PIPELINE_HANDLER_T_H

#include "JAWS/Export.h"
#include "JAWS/Pipeline.h"

template <class TYPE>
class JAWS_Pipeline_Abstract_Handler : public JAWS_Pipeline_Task
  // = TITLE
  //   Methods that are common to pipeline components
{
public:
  JAWS_Pipeline_Abstract_Handler ();
  ~JAWS_Pipeline_Abstract_Handler () override;
  // ACE_Task hooks

  int put (ACE_Message_Block *mb, ACE_Time_Value *tv = nullptr) override;
  // inherited from ACE_Task

  virtual int handle_put (TYPE *data, ACE_Time_Value *tv) = 0;
  // Callback hook for specialized data processing
};

#include "JAWS/Pipeline_Handler_T.cpp"

#endif /* !defined (JAWS_PIPELINE_HANDLER_T_H) */
