// $Id$

#include "Pipeline.h"

JAWS_Protocol_Pipeline::JAWS_Protocol_Pipeline (void)
{
}

JAWS_Protocol_Pipeline::JAWS_Protocol_Pipeline (JAWS_Protocol_Pipeline *pp)
{
  this->pipeline (pp);
}

JAWS_Protocol_Pipeline::~JAWS_Protocol_Pipeline (void)
{
}

int
JAWS_Protocol_Pipeline::pull (ACE_Message_Block &mb)
{
  int result = 0;

  if (this->pipeline () != 0)
    result = this->pipeline ()->pull (mb);

  if (result == 0)
    result = this->hook (mb);

  return result;
}

int
JAWS_Protocol_Pipeline::init (int, char *[])
{
  return 0;
}

int
JAWS_Protocol_Pipeline::fini (void)
{
  return 0;
}

void
JAWS_Protocol_Pipeline::insert (JAWS_Protocol_Pipeline *pp)
{
  if (pp == 0)
    return;

  pp->append (this->pipeline ());
  this->pipeline (pp);
}

void
JAWS_Protocol_Pipeline::append (JAWS_Protocol_Pipeline *pp)
{
  if (pp == 0)
    return;

  JAWS_Protocol_Pipeline *p;
  for (p = this; p->pipeline () != 0; p = p->pipeline ())
    ;
  p->pipeline (pp);
}

JAWS_Protocol_Pipeline *
JAWS_Protocol_Pipeline::pipeline (JAWS_Protocol_Pipeline *)
{
  return 0;
}

JAWS_Protocol_Filter::JAWS_Protocol_Filter (void)
  : pipeline_ (0)
{
}

JAWS_Protocol_Filter::JAWS_Protocol_Filter (JAWS_Protocol_Pipeline *pp)
  : pipeline_ (pp)
{
}

JAWS_Protocol_Filter::~JAWS_Protocol_Filter (void)
{
}

void
JAWS_Protocol_Filter::add (JAWS_Protocol_Pipeline *component)
{
  JAWS_Protocol_Pipeline::insert (component);
}

JAWS_Protocol_Pipeline *
JAWS_Protocol_Filter::pipeline (JAWS_Protocol_Pipeline *pp)
{
  if (pp != 0)
    this->pipeline_ = pp;

  return this->pipeline_;
}
