// $Id$

#if !defined (JAWS_PIPELINE_HANDLER_CPP)
#define JAWS_PIPELINE_HANDLER_CPP

#include "JAWS/Pipeline_Handler.h"

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
  TYPE *data = ACE_dynamic_cast (TYPE *,
                                 mb->data_block ());

  int status = this->handle_put (data, tv);

  switch (status)
    {
    case -1: /* error */
      break;
    case 0:  /* ok, move on to next task */
      break;
    default: /* do nothing */
      break;
    }

  // If handle_put() is successful, we continue to the next stage of the
  // pipeline.  Otherwise, we return.
  // return (status != -1) ? this->put_next (mb, tv) : -1;

  return status;
}

#endif /* !defined (JAWS_PIPELINE_HANDLER_CPP) */
