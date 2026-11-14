/* -*- c++ -*- */
#ifndef JAWS_PIPELINE_HANDLER_T_H
#define JAWS_PIPELINE_HANDLER_T_H

#include "JAWS/Export.h"
#include "JAWS/Pipeline.h"

/// Provides common task handling for pipeline components.
template <class TYPE>
class JAWS_Pipeline_Abstract_Handler : public JAWS_Pipeline_Task
{
public:
  JAWS_Pipeline_Abstract_Handler ();
  ~JAWS_Pipeline_Abstract_Handler () override;

  int put (ACE_Message_Block *mb, ACE_Time_Value *tv = nullptr) override;

  /// Process data supplied by the pipeline.
  virtual int handle_put (TYPE *data, ACE_Time_Value *tv) = 0;
};

#include "JAWS/Pipeline_Handler_T.cpp"

#endif /* !defined (JAWS_PIPELINE_HANDLER_T_H) */
