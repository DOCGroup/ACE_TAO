/* -*- C++ -*- */
// $Id$

// Multiplexor.i

int 
Driver::link_from_below (ACE_Module *stream_head)
{
  ACE_TRACE ("Driver::link_from_below");
  ACE_Module *stream_tail = this->alloc_module (this);

  stream_head->link (stream_tail);
  if (stream_tail->reader ()->open () == -1
      || stream_tail->writer ()->open () == -1)
    {
      stream_tail->close ();
      return -1;
    }
  return 0;
}

int 
Driver::unlink_from_below (ACE_Module *)
{
  ACE_TRACE ("Driver::unlink_from_below");
  return -1;
}

ACE_Multiplexor::ACE_Multiplexor (void)
{
  ACE_TRACE ("ACE_Multiplexor::ACE_Multiplexor");
}

ACE_Multiplexor::~ACE_Multiplexor (void)
{
  ACE_TRACE ("ACE_Multiplexor::~ACE_Multiplexor");
}

int 
ACE_Multiplexor::link_from_above (Driver &ld)
{
  ACE_TRACE ("ACE_Multiplexor::link_from_above");
  return ld.link_from_below (this->alloc_lower_module (this));
}

int 
ACE_Multiplexor::link_from_above (ACE_Multiplexor &lm)
{
  ACE_TRACE ("ACE_Multiplexor::link_from_above");
  return lm.link_from_below (this->alloc_lower_module (this));
}

int 
ACE_Multiplexor::link_from_below (ACE_Module *stream_head)
{
  ACE_TRACE ("ACE_Multiplexor::link_from_below");
  ACE_Module *stream_tail = this->alloc_upper_module (this);

  stream_head->link (stream_tail);
  if (stream_tail->reader ()->open () == -1
      || stream_tail->writer ()->open () == -1)
    {
      stream_tail->close ();
      return -1;
    }
  return 0;
}

int 
ACE_Multiplexor::unlink_from_above (Driver &)
{
  ACE_TRACE ("ACE_Multiplexor::unlink_from_above");
  return -1;
}

int 
ACE_Multiplexor::unlink_from_above (ACE_Multiplexor &)
{
  ACE_TRACE ("ACE_Multiplexor::unlink_from_above");
  return -1;
}

int 
ACE_Multiplexor::unlink_from_below (ACE_Module *)
{
  ACE_TRACE ("ACE_Multiplexor::unlink_from_below");
  return -1;
}
