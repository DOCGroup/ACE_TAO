// $Id$

#include "JAWS_Pipeline.h"

JAWS_Pipeline::JAWS_Pipeline (void)
{
}

int
JAWS_Pipeline::open (void * = 0)
{
  // Simply call into the virtual svc() method.
  if (this->svc() == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "JAWS_Pipeline::svc"), -1);
  return 0;
}

int
JAWS_Pipeline::close (u_long = 0)
{
  return 0;
}

template <class TYPE>
JAWS_Pipeline_Handler<TYPE>::JAWS_Pipeline_Handler (void)
{
}

template <class TYPE> int
JAWS_Pipeline_Handler<TYPE>::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  TYPE *data = (TYPE *) mb->data_block ();
  status = this->handle_input (data, tv);
  return (status != -1) ? this->put_next (mb, tv) : -1;
}
