/* -*- c++ -*- */
#ifndef JAWS_PIPELINE_HANDLER_H
#define JAWS_PIPELINE_HANDLER_H

#include "JAWS_Pipeline.h"

/// Base class for pipeline components that process data of TYPE.
template <class TYPE>
class JAWS_Pipeline_Handler : public JAWS_Pipeline_Task
{
public:
  JAWS_Pipeline_Handler ();

  int put (ACE_Message_Block *mb, ACE_Time_Value *tv = nullptr) override;

  /// Callback hook for specialized data processing.
  virtual int handle_put (TYPE *data, ACE_Time_Value *tv) = 0;
};

#include "JAWS_Pipeline_Handler.cpp"

#endif /* !defined (JAWS_PIPELINE_HANDLER_H) */
