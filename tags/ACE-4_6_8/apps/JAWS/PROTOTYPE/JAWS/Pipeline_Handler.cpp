// $Id$

#ifndef JAWS_PIPELINE_HANDLER_CPP
#define JAWS_PIPELINE_HANDLER_CPP

#include "JAWS/Pipeline_Handler.h"

ACE_RCSID(JAWS, Pipeline_Handler, "$Id$")

template <class TYPE>
JAWS_Pipeline_Abstract_Handler<TYPE>::JAWS_Pipeline_Abstract_Handler (void)
{
}

template <class TYPE>
JAWS_Pipeline_Abstract_Handler<TYPE>::~JAWS_Pipeline_Abstract_Handler (void)
{
}

template <class TYPE> int
JAWS_Pipeline_Abstract_Handler<TYPE>::put (ACE_Message_Block *mb,
                                           ACE_Time_Value *tv)
{
  TYPE *data = (TYPE *) mb->data_block ();

  int status = this->handle_put (data, tv);

  return status;
}

#endif /* !defined (JAWS_PIPELINE_HANDLER_CPP) */
