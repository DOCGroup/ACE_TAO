/*************************************************
 *
 * = PACKAGE
 *    JACE.ASX
 *
 * = FILENAME
 *    MessageType.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.ASX;

/**
 * <hr>
 * <h2>SYNOPSIS</h2>
 *<blockquote>
 *     Message types used by ACE.MessageBlock.
 *</blockquote>
 *
 * <h2>DESCRIPTION</h2>
 *<blockquote>
 *     Defines bit masks used to identify various types of messages.
 *</blockquote>
 *
 *<h2>NOTES</h2>
 *<blockquote>
 * This class is not intended to be instantiable.
 *</blockquote>
 */
public class MessageType
{
  // = Data and protocol messages (regular and priority)
  /** regular data */
  public static final int MB_DATA     = 0x01;

  /** protocol control */
  public static final int MB_PROTO    = 0x02;

  /** regular data */
  public static final int MB_OBJECT   = 0x09;


  // = Control messages (regular and priority)
  /** line break */
  public static final int MB_BREAK    = 0x03;

  /** pass file pointer */
  public static final int MB_PASSFP   = 0x04;

  /** post an event to an event queue */
  public static final int MB_EVENT    = 0x05;

  /** generate process signal */
  public static final int MB_SIG      = 0x06;

  /** ioctl; set/get params */
  public static final int MB_IOCTL    = 0x07;

  /** set various stream head options */
  public static final int MB_SETOPTS  = 0x08; 


  // = Control messages (high priority; go to head of queue)
  /** acknowledge ioctl */
  public static final int MB_IOCACK   = 0x81;

  /** negative ioctl acknowledge */
  public static final int MB_IOCNAK   = 0x82;

  /** priority proto message */
  public static final int MB_PCPROTO  = 0x83;

  /** generate process signal */
  public static final int MB_PCSIG    = 0x84;

  /** generate read notification */
  public static final int MB_READ     = 0x85;

  /** flush your queues */
  public static final int MB_FLUSH    = 0x86;

  /** stop transmission immediately */
  public static final int MB_STOP     = 0x87;

  /** restart transmission after stop */
  public static final int MB_START    = 0x88;

  /** line disconnect */
  public static final int MB_HANGUP   = 0x89;

  /** fatal error used to set u.u_error */
  public static final int MB_ERROR    = 0x8a;

  /** post an event to an event queue */
  public static final int MB_PCEVENT  = 0x8b; 


  /** Normal priority messages */
  public static final int MB_NORMAL   = 0x00;

  /** High priority control messages */
  public static final int MB_PRIORITY = 0x80; 

  // Default private constructor to avoid instantiation
  private MessageType ()
  {
  }
}

