// $Id$

#include "Pipeline.h"

JAWS_Protocol_Pipeline::JAWS_Protocol_Pipeline (void)
{
}

JAWS_Protocol_Pipeline::JAWS_Protocol_Pipeline (JAWS_Protocol_Pipeline *pp)
{
  this->insert (pp);
}

JAWS_Protocol_Pipeline::~JAWS_Protocol_Pipeline (void)
{
}

int
JAWS_Protocol_Pipeline::pull (ACE_Message_Block &mb)
{
  int result = 0;

  if (this->prev_pipe () != 0)
    result = this->prev_pipe ()->pull (mb);

  if (result == 0)
    result = this->pull_hook (mb);

  return result;
}

int
JAWS_Protocol_Pipeline::push (ACE_Message_Block &mb)
{
  int result = 0;

  if (result == 0)
    result = this->push_hook (mb);

  if (this->next_pipe () != 0)
    result = this->next_pipe ()->push (mb);

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

  if (this->prev_pipe () != 0)
    this->prev_pipe ()->append (pp);
  pp->append (this);
}

void
JAWS_Protocol_Pipeline::append (JAWS_Protocol_Pipeline *pp)
{
  if (pp == 0)
    return;

  JAWS_Protocol_Pipeline *p;
  for (p = this; p->next_pipe () != 0; p = p->next_pipe ())
    ;
  p->next_pipe (pp);
  pp->prev_pipe (p);
}

JAWS_Protocol_Pipeline *
JAWS_Protocol_Pipeline::prev_pipe (JAWS_Protocol_Pipeline *)
{
  return 0;
}

JAWS_Protocol_Pipeline *
JAWS_Protocol_Pipeline::next_pipe (JAWS_Protocol_Pipeline *)
{
  return 0;
}

JAWS_Protocol_Filter::JAWS_Protocol_Filter (void)
  : prev_pipe_ (0),
    next_pipe_ (0)
{
}

JAWS_Protocol_Filter::JAWS_Protocol_Filter (JAWS_Protocol_Pipeline *pp)
  : JAWS_Protocol_Pipeline (pp),
    prev_pipe_ (0),
    next_pipe_ (0)
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
JAWS_Protocol_Filter::prev_pipe (JAWS_Protocol_Pipeline *pp)
{
  if (pp != 0)
    this->prev_pipe_ = pp;

  return this->prev_pipe_;
}

JAWS_Protocol_Pipeline *
JAWS_Protocol_Filter::next_pipe (JAWS_Protocol_Pipeline *pp)
{
  if (pp != 0)
    this->next_pipe_ = pp;

  return this->next_pipe_;
}
