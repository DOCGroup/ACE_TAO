#include "JAWS_Pipeline.h"

JAWS_Pipeline::JAWS_Pipeline ()
{
}

int
JAWS_Pipeline::open (void *)
{
  // Simply call into the virtual svc() method.
  if (this->svc () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "JAWS_Pipeline::svc"),
                      -1);
  return 0;
}

int
JAWS_Pipeline::close (u_long)
{
  return 0;
}

#include "JAWS_Pipeline_Handler.cpp"
