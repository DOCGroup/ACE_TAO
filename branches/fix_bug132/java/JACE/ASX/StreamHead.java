/*************************************************
 *
 * = PACKAGE
 *    JACE.ASX
 *
 * = FILENAME
 *    StreamHead.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.ASX;

import JACE.OS.*;

/**
 * Standard module that acts as the head of a ustream.
 */

public class StreamHead extends Task
{
  // Module that acts as the head of a Stream.

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
	this.waterMarks (cmd, mb.cont ().length ());
	ioc.rval (0);
      break;
    default:
      return 0;
    }
  return ioc.rval ();
  }

  /* Performs canonical flushing at the ACE_Stream Head */

  private int canonicalFlush (MessageBlock mb)
  {
    String s = mb.base ();
    long f = (new Long (s)).longValue ();

    if ((f & TaskFlags.ACE_FLUSHR) != 0)
      {
	this.flush (TaskFlags.ACE_FLUSHALL);
	f &= ~TaskFlags.ACE_FLUSHR;
      }
    if ((f & TaskFlags.ACE_FLUSHW) != 0)
      return this.reply (mb, null);
    return 0;
  }

  // Will block forever to add the given MessageBlock
  public int put (MessageBlock mb) 
  {
      return this.put (mb, null);
  }

  // tv is absolute time
  public int put (MessageBlock mb, TimeValue tv)
  {
    int res = 0;
    if (mb.msgType () == MessageType.MB_IOCTL
	&& (res = this.control (mb)) == -1)
      return res;
    
    if (this.isWriter ())
      {
	return this.putNext (mb, tv);
      }
    else /* this.isReader () */
      {      
	switch (mb.msgType ())
	  {
	  case MessageType.MB_FLUSH:
	    return this.canonicalFlush (mb);
	  default:
	    break;
	  }
      
	try
	  {
	    return this.putq (mb, tv);
	  }
	catch (InterruptedException e)
	  {
	    return -1;
	  }
      }
  }

  public void dump ()
  {
  }

  public int handleTimeout (TimeValue tv, Object obj)
  {
    return 0;
  }

}
