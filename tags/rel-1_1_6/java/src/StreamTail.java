/*************************************************
 *
 * = PACKAGE
 *    JACE.ASX
 *
 * = FILENAME
 *    StreamTail.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.ASX;

import JACE.OS.*;

/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 *     Standard module that acts as the tail of a ustream.
 *</blockquote>
 */

public class StreamTail extends Task
{
  // Module that acts as the tail of a Stream.

  public int open (Object obj)
  {
    return 0;
  }

  public int close (long l)
  {
    return 0;
  }

  public int svc ()
  {
    return -1;
  }

  private int control (MessageBlock mb)
  {
    IOCntlMsg ioc = (IOCntlMsg) mb.obj ();
    int cmd = ioc.cmd ();

    switch (cmd)
      {
      case IOCntlCmds.SET_LWM:
      case IOCntlCmds.SET_HWM:
	{
	  int size = mb.cont ().length ();
	  
	  this.waterMarks (cmd, size);
	  this.sibling ().waterMarks (cmd, size);
	  ioc.rval (0);
	  break;
	}
      default:
	mb.msgType (MessageType.MB_IOCNAK);
      }
    return this.reply (mb, null);
  }

  // Perform flush algorithm as though we were the driver
  private int canonicalFlush (MessageBlock mb)
  {
    String s = mb.base ();
    long f = (new Long (s)).longValue ();

    if ((f & TaskFlags.ACE_FLUSHW) != 0)
      {
	this.flush (TaskFlags.ACE_FLUSHALL);
	f &= ~TaskFlags.ACE_FLUSHW;
      }
    if ((f & TaskFlags.ACE_FLUSHR) != 0)
      {
	this.sibling ().flush (TaskFlags.ACE_FLUSHALL);
	return this.reply (mb, null);
      }
    return 0;
  }

  // put the given MessageBlock without a timeout (block forever if
  // necessary)
  public int put (MessageBlock mb) 
  {
    return this.put (mb, null);
  }

  // tv is an absolute time timeout
  public int put (MessageBlock mb, TimeValue tv)
  {
    if (this.isWriter ())
    {
      switch (mb.msgType ())
	{
	case MessageType.MB_IOCTL:
	  return this.control (mb);
	  /* NOTREACHED */
	default:
	  break;
	}
    }

    return -1;
  }

  public void dump ()
  {
  }

  public int handleTimeout (TimeValue tv, Object obj)
  {
    return 0;
  }

}
