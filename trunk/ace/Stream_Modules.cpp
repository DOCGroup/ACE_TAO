// Stream_Modules.cpp
// $Id$

#if !defined (ACE_STREAM_MODULES_C)
#define ACE_STREAM_MODULES_C

#define ACE_BUILD_DLL
#include "ace/Stream_Modules.h"

#if !defined (__ACE_INLINE__)
#include "ace/Stream_Modules.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Stream_Head)

template <ACE_SYNCH_1> 
ACE_Stream_Head<ACE_SYNCH_2>::ACE_Stream_Head (void) 
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::ACE_Stream_Head");
}

template <ACE_SYNCH_1> 
ACE_Stream_Head<ACE_SYNCH_2>::~ACE_Stream_Head (void) 
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::~ACE_Stream_Head");
}

template <ACE_SYNCH_1> void
ACE_Stream_Head<ACE_SYNCH_2>::dump (void) const
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::dump");
}

// ACE_Module that act as the head and tail of a Stream.

template <ACE_SYNCH_1> int
ACE_Stream_Head<ACE_SYNCH_2>::open (void *)
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::open");
  return 0;
}

template <ACE_SYNCH_1> int
ACE_Stream_Head<ACE_SYNCH_2>::close (u_long)
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::close");
  return 0;
}

template <ACE_SYNCH_1> int
ACE_Stream_Head<ACE_SYNCH_2>::svc (void)
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::svc");
  return -1;
}

template <ACE_SYNCH_1> int
ACE_Stream_Head<ACE_SYNCH_2>::control (ACE_Message_Block *mb)
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::control");
  ACE_IO_Cntl_Msg *ioc = (ACE_IO_Cntl_Msg *) mb->rd_ptr ();
  ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds cmd;

  switch (cmd = ioc->cmd ())
    {
    case ACE_IO_Cntl_Msg::SET_LWM:
    case ACE_IO_Cntl_Msg::SET_HWM:
      this->water_marks (cmd, *(size_t *) mb->cont ()->rd_ptr ());
      ioc->rval (0);
      break;
    default:
      return 0;
    }
  return ioc->rval ();
}

// Performs canonical flushing at the ACE_Stream Head.

template <ACE_SYNCH_1> int
ACE_Stream_Head<ACE_SYNCH_2>::canonical_flush (ACE_Message_Block *mb)
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::canonical_flush");
  char *cp = mb->rd_ptr ();

  if (ACE_BIT_ENABLED (*cp, ACE_Task_Flags::ACE_FLUSHR))
    {
      this->flush (ACE_Task_Flags::ACE_FLUSHALL);
      ACE_CLR_BITS (*cp, ACE_Task_Flags::ACE_FLUSHR);
    }

  if (ACE_BIT_ENABLED (*cp, ACE_Task_Flags::ACE_FLUSHW))
    return this->reply (mb);
  else
    mb->release ();
  return 0;
}

template <ACE_SYNCH_1> int 
ACE_Stream_Head<ACE_SYNCH_2>::put (ACE_Message_Block *mb, 
				   ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::put");
  int res = 0;

  if (mb->msg_type () == ACE_Message_Block::MB_IOCTL
      && (res = this->control (mb)) == -1)
    return res;

  if (this->is_writer ())
    return this->put_next (mb, tv);
  else // this->is_reader () 
    {      
      switch (mb->msg_type ())
	{
	case ACE_Message_Block::MB_FLUSH:
	  return this->canonical_flush (mb);
	default:
	  break;
	}
      
      return this->putq (mb, tv);
    }
}

template <ACE_SYNCH_1> int 
ACE_Stream_Head<ACE_SYNCH_2>::init (int, char *[])
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::init");
  return 0;
}

template <ACE_SYNCH_1> int 
ACE_Stream_Head<ACE_SYNCH_2>::info (char **strp, size_t length) const
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::info");
  const char *name = this->name ();
  
  if (*strp == 0 && (*strp = ACE_OS::strdup (name)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, name, length);
  return ACE_OS::strlen (name);
}

template <ACE_SYNCH_1> int 
ACE_Stream_Head<ACE_SYNCH_2>::fini (void)
{
  ACE_TRACE ("ACE_Stream_Head<ACE_SYNCH_2>::fini");
  return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Stream_Tail)

template <ACE_SYNCH_1> 
ACE_Stream_Tail<ACE_SYNCH_2>::ACE_Stream_Tail (void) 
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::ACE_Stream_Tail");
}

template <ACE_SYNCH_1> 
ACE_Stream_Tail<ACE_SYNCH_2>::~ACE_Stream_Tail (void) 
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::~ACE_Stream_Tail");
}

template <ACE_SYNCH_1> void
ACE_Stream_Tail<ACE_SYNCH_2>::dump (void) const
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::dump");
}

template <ACE_SYNCH_1> int
ACE_Stream_Tail<ACE_SYNCH_2>::open (void *)
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::open");
  return 0;
}

template <ACE_SYNCH_1> int
ACE_Stream_Tail<ACE_SYNCH_2>::close (u_long)
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::close");
  return 0;
}

template <ACE_SYNCH_1> int
ACE_Stream_Tail<ACE_SYNCH_2>::svc (void)
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::svc");
  return -1;
}

template <ACE_SYNCH_1> int
ACE_Stream_Tail<ACE_SYNCH_2>::control (ACE_Message_Block *mb)
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::control");
  ACE_IO_Cntl_Msg *ioc = (ACE_IO_Cntl_Msg *) mb->rd_ptr ();
  ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds cmd;
  
  switch (cmd = ioc->cmd ())
    {
    case ACE_IO_Cntl_Msg::SET_LWM:
    case ACE_IO_Cntl_Msg::SET_HWM:
      {
	size_t size = *(size_t *) mb->cont ()->rd_ptr ();

	this->water_marks (cmd, size);
	this->sibling ()->water_marks (cmd, size);
	ioc->rval (0);
	break;
      }
    default:
      mb->msg_type (ACE_Message_Block::MB_IOCNAK);
    }
  return this->reply (mb);
}

// Perform flush algorithm as though we were the driver.

template <ACE_SYNCH_1> int
ACE_Stream_Tail<ACE_SYNCH_2>::canonical_flush (ACE_Message_Block *mb)
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::canonical_flush");
  char *cp = mb->rd_ptr ();

  if (ACE_BIT_ENABLED (*cp, ACE_Task_Flags::ACE_FLUSHW))
    {
      this->flush (ACE_Task_Flags::ACE_FLUSHALL);
      ACE_CLR_BITS (*cp, ACE_Task_Flags::ACE_FLUSHW);
    }

  if (ACE_BIT_ENABLED (*cp, ACE_Task_Flags::ACE_FLUSHR))
    {
      this->sibling ()->flush (ACE_Task_Flags::ACE_FLUSHALL);
      return this->reply (mb);
    }
  else
    mb->release ();

  return 0;
}

template <ACE_SYNCH_1> int 
ACE_Stream_Tail<ACE_SYNCH_2>::put (ACE_Message_Block *mb, 
				   ACE_Time_Value *)
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::put");

  if (this->is_writer ())
    {
      switch (mb->msg_type ())
	{
	case ACE_Message_Block::MB_IOCTL:
	  return this->control (mb);
	  /* NOTREACHED */
	default:
	  mb->release ();
	}
    }

  return -1;
}

template <ACE_SYNCH_1> int 
ACE_Stream_Tail<ACE_SYNCH_2>::init (int, char *[])
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::init");
  return 0;
}

template <ACE_SYNCH_1> int 
ACE_Stream_Tail<ACE_SYNCH_2>::info (char **strp, size_t length) const
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::info");
  const char *name = this->name ();
  
  if (*strp == 0 && (*strp = ACE_OS::strdup (name)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, name, length);
  return ACE_OS::strlen (name);
}

template <ACE_SYNCH_1> int 
ACE_Stream_Tail<ACE_SYNCH_2>::fini (void)
{
  ACE_TRACE ("ACE_Stream_Tail<ACE_SYNCH_2>::fini");
  return 0;
}

// ACE_ALLOC_HOOK_DEFINE(ACE_Thru_Task)

template <ACE_SYNCH_1> 
ACE_Thru_Task<ACE_SYNCH_2>::ACE_Thru_Task (void) 
{
  ACE_TRACE ("ACE_Thru_Task<ACE_SYNCH_2>::ACE_Thru_Task");
}

template <ACE_SYNCH_1> 
ACE_Thru_Task<ACE_SYNCH_2>::~ACE_Thru_Task (void) 
{
  ACE_TRACE ("ACE_Thru_Task<ACE_SYNCH_2>::~ACE_Thru_Task");
}

template <ACE_SYNCH_1> void
ACE_Thru_Task<ACE_SYNCH_2>::dump (void) const
{
  ACE_TRACE ("ACE_Thru_Task<ACE_SYNCH_2>::dump");
}

template <ACE_SYNCH_1> int 
ACE_Thru_Task<ACE_SYNCH_2>::open (void *)
{
  ACE_TRACE ("ACE_Thru_Task<ACE_SYNCH_2>::open");
  return 0; 
}

template <ACE_SYNCH_1> int
ACE_Thru_Task<ACE_SYNCH_2>::close (u_long)
{
  ACE_TRACE ("ACE_Thru_Task<ACE_SYNCH_2>::close");
  return 0; 
}

template <ACE_SYNCH_1> int
ACE_Thru_Task<ACE_SYNCH_2>::svc (void)
{
  ACE_TRACE ("ACE_Thru_Task<ACE_SYNCH_2>::svc");
  return -1;
}

template <ACE_SYNCH_1> int 
ACE_Thru_Task<ACE_SYNCH_2>::put (ACE_Message_Block *msg, 
				 ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_Thru_Task<ACE_SYNCH_2>::put");
  return this->put_next (msg, tv);
}

template <ACE_SYNCH_1> int 
ACE_Thru_Task<ACE_SYNCH_2>::init (int, char *[])
{
  ACE_TRACE ("ACE_Thru_Task<ACE_SYNCH_2>::init");
  return 0;
}

template <ACE_SYNCH_1> int 
ACE_Thru_Task<ACE_SYNCH_2>::info (char **strp, 
				  size_t length) const
{
  ACE_TRACE ("ACE_Thru_Task<ACE_SYNCH_2>::info");
  const char *name = this->name ();
  
  if (*strp == 0 && (*strp = ACE_OS::strdup (name)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, name, length);
  return ACE_OS::strlen (name);
}

template <ACE_SYNCH_1> int 
ACE_Thru_Task<ACE_SYNCH_2>::fini (void)
{
  ACE_TRACE ("ACE_Thru_Task<ACE_SYNCH_2>::fini");
  return 0;
}

#endif /* ACE_STREAM_MODULES_C */
