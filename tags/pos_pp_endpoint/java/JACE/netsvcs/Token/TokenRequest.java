package JACE.netsvcs.Token;

import java.io.*;
import JACE.ASX.*;
import JACE.OS.*;

/**
 * Request for an operation on a lock.  This is compatible with the
 * C++ ACE version.  The US-ASCII character encoding is used for
 * String to byte conversions (and vice versa).  If
 * that encoding isn't supported, it attempts to use the default
 * encoding (but prints a message).  Users probably never need to
 * deal with this class directly.  The notify field isn't used
 * in JACE (or in C++ ACE as far as I can tell).
 *
 *@author Everett Anderson
 */
public class TokenRequest
{
  /** Maximum length for a token name */
  public final static int MAX_TOKEN_NAME_LEN = 40;

  /** Maximum length for a client ID */
  public final static int MAX_CLIENT_ID_LEN = 276;
  
  /** Length of the fixed size header */
  protected final static int HEADER_LEN = 40;

  /** Maximum length of any TokenRequest (total) */
  protected final static int MAX_LEN = 359;
  
  /**
   * Dump this TokenRequest's state out to a String.
   */
  public String toString() 
  {
    return "TokenRequest(" + this.length() + ", " +
      this.tokenType_ + ", " +
      this.proxyType_ + ", " + 
      this.operationType_ + ", " +
      this.requeuePosition_ + ", " +
      this.notify_ + ", " +
      this.useTimeout_ + ", " +
      this.sec_ + ", " +
      this.usec_ + ", " +
      this.arg_ + ", " +
      this.tokenName_ + ", " +
      this.clientID_ + ")";
  }
  
  /** Default constructor. */
  public TokenRequest() 
  {
    // Remember that the length is transmitted first
    tokenType_ = 0;
    proxyType_ = 0;
    operationType_ = 0;
    requeuePosition_ = 0;
    notify_ = 0;
    useTimeout_ = 0;
    sec_ = 0;
    usec_ = 0;
    arg_ = 0;
    tokenName_ = "";
    clientID_ = "";
    // Transmission is "<10 ints><token name>(null):<clientID>(null)"
    
    charEncoding_ = "US-ASCII";
    
    buffer_ = new byte[MAX_LEN];
  }
  
  /** 
   * Create a request which doesn't use timeouts.
   *
   *@param tokenType type of token (usually a constant in LockTypes)
   *@param proxyType type of proxy (usually a constant in LockTypes)
   *@param operationType type of operation (usually a constant in 
   *                     LockOperations)
   *@param tokenName name of the token to operate on
   *@param clientID name of the client requesting an operation
   *
   *@see LockTypes
   *@see LockOperations
   */
  public TokenRequest(int tokenType,
		      int proxyType,
		      int operationType,
		      String tokenName,
		      String clientID) 
  {
    this();
    
    this.tokenType_ = tokenType;
    this.proxyType_ = proxyType;
    this.operationType_ = operationType;
    this.tokenName_ = tokenName;
    this.clientID_ = clientID;
  }
  
  /** 
   * Create a request which uses the given absolute time timeout.
   *
   *@param tokenType type of token (usually a constant in LockTypes)
   *@param proxyType type of proxy (usually a constant in LockTypes)
   *@param operationType type of operation (usually a constant in 
   *                     LockOperations)
   *@param tokenName name of the token to operate on
   *@param clientID name of the client requesting an operation
   *@param tv absolute time timeout to process the request by
   *
   *@see LockTypes
   *@see LockOperations
   */  
  public TokenRequest(int tokenType,
		      int proxyType,
		      int operationType,
		      String tokenName,
		      String clientID,
		      TimeValue tv) 
  {
    this(tokenType, 
	 proxyType,
	 operationType,
	 tokenName,
	 clientID);
    
    this.useTimeout_ = 1;
    this.sec_ = (int)tv.sec();
    this.usec_ = tv.nanos() / 1000;
  }
  
  /**
   * Return the length of this TokenRequest.
   * <P>
   * Details:
   * <PRE>
   * Fixed size header of length HEADER_LEN
   * token name
   * null
   * :
   * client ID
   * null
   * </PRE>
   */
  public int length() 
  { 
    return (HEADER_LEN + 
	    this.tokenName_.length() + 
	    this.clientID_.length() + 3);
  }

  /** Accessor for the token type. */  
  public int tokenType() 
  {
    return this.tokenType_;
  }
  /** Set the token type. */
  public void tokenType(int type) 
  {
    this.tokenType_ = type;
  }
  /** Accessor for the proxy type. */
  public int proxyType() 
  {
    return this.proxyType_;
  }
  /** Set the proxy type. */
  public void proxyType(int type) 
  {
    this.proxyType_ = type;
  }
  /** Accessor for the operation type. */
  public int operationType() 
  {
    return this.operationType_;
  }
  /** Set the operation type. */
  public void operationType(int type) 
  {
    this.operationType_ = type;
  }
  /** Accessor for the requeue position.  This only makes
   * sense for a renew operation. */
  public int requeuePosition() 
  {
    return this.requeuePosition_;
  }
  /** Set the requeue position.  This only makes sense for
   * a renew operation. */
  public void requeuePosition(int position) 
  {
    this.requeuePosition_ = position;
  }
  /** Accessor for the flag to determine if a timeout should be used. */
  public boolean useTimeout() 
  {
    return (this.useTimeout_ == 1 ? true : false);
  }
  /** Set the flag to enable or disable use of timeouts.
   */
  public void useTimeout(boolean useIt) 
  {
    this.useTimeout_ = (useIt == true ? 1 : 0);
  }

  /**
   * Set the absolute time timeout to the given TimeValue's value, and
   * enable the useTimeout flag.
   */
  public void useTimeout (TimeValue timeout)
  {
    this.useTimeout_ = 1;
    this.sec_ = (int)timeout.sec ();
    this.usec_ = timeout.nanos () / 1000;
  }

  /**
   * Accessor for the seconds component of the absolute time timeout.
   */
  public int sec() 
  {
    return this.sec_;
  }
  /** Set the seconds component of the timeout. */
  public void sec(int sec) 
  {
    this.sec_ = sec;
  }
  /** Accessor for the usec component of the timeout. */
  public int usec() 
  {
    return this.usec_;
  }
  /** Set the usec component of the timeout. */
  public void usec(int usec) 
  {
    this.usec_ = usec;
  }
  /** Accessor for the arg value.  (unused in JACE) */
  public int arg() 
  {
    return this.arg_;
  }
  /** Set the arg value.  (unused in JACE) */
  public void arg(int arg) 
  {
    this.arg_ = arg;
  }
  
  /** Accessor for the name of the token. */
  public String tokenName() 
  {
    return this.tokenName_;
  }
  
  /** Set the name of the token. */
  public void tokenName(String name) 
  {
    this.tokenName_ = name;
  }
  
  /** Accessor for the client identification string. */
  public String clientID() 
  {
    return this.clientID_;
  }
  
  /** Set the client identification string. */
  public void clientID(String ID) 
  {
    this.clientID_ = ID;
  }
  
  /**
   * Construct a String from a specific subset of the byte array.
   * The string begins at the start index and ends at either the
   * end of the buffer or the first byte with value 0 (null).
   * Tries to use the specified encoding, but uses the default
   * platform encoding if it isn't available.
   *
   *@param start beginning index in the buffer
   *@param bufferLength total length of the buffer
   */
  protected String stringFromBuffer(int start, int bufferLength) 
  {
    int end = start;
    String result = null;

    while (end < bufferLength && this.buffer_[end] != 0)
      end++;
    
    try {
      result = new String(this.buffer_,
			  start,
			  end - start,
			  this.charEncoding_);
    } catch (UnsupportedEncodingException e) {
      ACE.ERROR(this.charEncoding_ + " is not supported!");
      result = new String (this.buffer_, start, end - start);
    }

    return result;
  }
  
  /**
   * Read the request in from the given InputStream.
   */
  public void streamInFrom(InputStream is) 
    throws IOException, EOFException
  {
    BufferedInputStream bis = new BufferedInputStream(is,
						      MAX_LEN);
    DataInputStream dis = new DataInputStream(bis);

    streamInFrom (dis);
  }

  /**
   * Read the request in from the given DataInputStream.
   */
  public void streamInFrom (DataInputStream dis) 
    throws IOException, EOFException
  {
    int length = dis.readInt();
    if (length > MAX_LEN || length < HEADER_LEN + 5) 
      throw new IOException ("Invalid TokenRequest length: " + length);

    this.tokenType_ = dis.readInt();
    this.proxyType_ = dis.readInt();
    this.operationType_ = dis.readInt();
    this.requeuePosition_ = dis.readInt();
    this.notify_ = dis.readInt();
    this.useTimeout_ = dis.readInt();
    this.sec_ = dis.readInt();
    this.usec_ = dis.readInt();
    this.arg_ = dis.readInt();
    
    int total = dis.read(this.buffer_, 0, length - HEADER_LEN);
    
    this.tokenName_ = this.stringFromBuffer(0, total);
    
    this.clientID_ = this.stringFromBuffer(this.tokenName_.length() + 2,
					   total);
    
    if (this.tokenName_.length() > MAX_TOKEN_NAME_LEN ||
	this.clientID_.length() > MAX_CLIENT_ID_LEN) 
      throw new IOException("Exceeds maximum token name or client ID");
  }
  
  /**
   * Write the request out to the given OutputStream.
   */
  public void streamOutTo (OutputStream os) 
    throws IOException 
  {
    BufferedOutputStream bos = new BufferedOutputStream(os);
    DataOutputStream dos = new DataOutputStream(bos);
    
    streamOutTo (dos);
  }

  /**
   * Write the request out to the given DataOutputStream.  Tries to use
   * the specified encoding to the convert the token name and client ID
   * to bytes, but uses the platform default encoding if necessary.
   */
  public void streamOutTo (DataOutputStream dos)
    throws IOException
  {
    dos.writeInt(this.length());
    dos.writeInt(this.tokenType_);
    dos.writeInt(this.proxyType_);
    dos.writeInt(this.operationType_);
    dos.writeInt(this.requeuePosition_);

    dos.writeInt(this.notify_);
    dos.writeInt(this.useTimeout_);
    dos.writeInt(this.sec_);
    dos.writeInt(this.usec_);
    dos.writeInt(this.arg_);
    
    StringBuffer data = new StringBuffer(this.tokenName_.length() +
					 this.clientID_.length() +
					 3);
    
    data.append(this.tokenName_);
    data.append('\0');
    data.append(':');
    data.append(this.clientID_);
    data.append('\0');

    byte buf [] = null;
    String dataString = data.toString ();
    try {
      buf = dataString.getBytes (this.charEncoding_);
    } catch (UnsupportedEncodingException e) {
      ACE.ERROR (charEncoding_ + " is unsupported, trying to use default");
      buf = dataString.getBytes ();
    }

    dos.write(buf, 0, buf.length);
    dos.flush();
  }
  
  private int tokenType_;
  private int proxyType_;
  private int operationType_;
  private int requeuePosition_;
  private int notify_;
  private int useTimeout_;
  private int sec_;
  private int usec_;
  private int arg_;
  
  private String tokenName_;
  private String clientID_;
  
  private byte buffer_[];
  
  /**
   * Character encoding to use for converting the token name and
   * client ID to/from bytes.
   */
  protected String charEncoding_;
}
