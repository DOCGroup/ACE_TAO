/*************************************************
 *
 * = PACKAGE
 *    netsvcs.Naming
 *
 * = FILENAME
 *    NameRequest.java
 *
 *@see netsvcs.Naming.NameHandler
 *
 *@author Everett Anderson
 *
 *************************************************/
package JACE.netsvcs.Naming;

import java.io.*;
import java.util.*;
import JACE.OS.*;
import JACE.Connection.*;
import JACE.Reactor.*;
import JACE.ASX.*;
import JACE.SOCK_SAP.*;

/**
 * Holds information including name, value, type, and request
 * type.  Used by both client and naming server as detailed in
 * NameHandler.  Compatible with the C++ ACE_Name_Request.
 *
 *@see NameHandler
 */
public class NameRequest
{
  /** Bind request type */
  public static final int BIND = 1; 

  /** Rebind request type */
  public static final int REBIND = 2; 

  /** Resolve request type */
  public static final int RESOLVE = 3; 

  /** Unbind request type */
  public static final int UNBIND  = 4; 

  /** List Names request type */
  public static final int LIST_NAMES = 5;

  /** List Values request type */
  public static final int LIST_VALUES = 13;

  /** List Types request type */
  public static final int LIST_TYPES = 21;

  /** List Name Entries request type */
  public static final int LIST_NAME_ENTRIES = 6;

  /** List Value Entries request type */
  public static final int LIST_VALUE_ENTRIES = 14;

  /** List Type Entries request type */
  public static final int LIST_TYPE_ENTRIES = 22;

  /** Type used to send a final "null" request when returning
   * a list of items */
  public static final int MAX_ENUM = 11; 
  /**
   * Maximum length of a NameRequest instance.
   * See C++ ACE Name_Request_Reply.h for the details of the
   * value of this constant.
   */
  public static final int MAX_LEN = 6182;

  /**
   * Default constructor.
   */
  public NameRequest () 
  {
    this.name_ = this.value_ = this.type_ = null;
    this.length_ = 32;
  } 

  /** 
   * Constructor
   *
   * @param requestType      Type of request this is (BIND, REBIND, etc)
   * @param name             Key to bind
   * @param value            Value to bind
   * @param type             Type to couple with the value
   * @param timeout          Timer information (not really used in JACE yet)
   */
  public NameRequest(int requestType,
		     String name,
		     String value,
		     String type,
		     TimeValue timeout)
  {
    this.requestType_ = requestType;

    if (timeout == null) {

      this.blockForever_ = 1;
      this.secTimeout_ = 0;
      this.usecTimeout_ = 0;
    } else {

      this.blockForever_ = 0;
      this.secTimeout_ = (int)timeout.sec();
      this.usecTimeout_ = (int)timeout.getMilliTime() * 1000;
    }

    // This is necessary to make sure null pointer exceptions are
    // avoided.  It makes it more consistent later on
    if (name == null)
      this.name_ = new String("");
    else
      this.name_ = new String(name);
    if (value == null)
      this.value_ = new String("");
    else
      this.value_ = new String(value);
    if (type == null)
      this.type_ = new String("");
    else
      this.type_ = new String(type);

    // Set the length
    this.calculateLength();
  }

  /** 
   * Calculate the transmission length (bytes) of this structure
   */
  private void calculateLength()
  {
    // The type is sent as an 8 bit data type (chars in the C++ version),
    // but the name and value are sent as 16 bit chars (ACE_USHORT16's in C++)

    this.length_ = 34 + this.type_.length() + 2 * (this.name_.length() +
						   this.value_.length());
  }

  /**
   * Return the transmission length
   */
  public int length() 
  { return this.length_; }

  /**
   * Return the name/key
   */
  public String name()
  { return new String(this.name_); }

  /**
   * Set the name/key
   * @param name    Name to set to
   */
  public void name(String name)
  {
    if (name == null)
      this.name_ = new String("");
    else
      this.name_ = new String(name);

    this.calculateLength();
  }

  /**
   * Return the value
   */
  public String value()
  { return new String(this.value_); }

  /** 
   * Set the value 
   * @param value   New value
   */
  public void value(String value)
  {
    if (value == null)
      this.value_ = new String("");
    else
      this.value_ = new String(value);

    this.calculateLength();
  }

  /**
   * Return the type
   */
  public String type()
  { return new String(this.type_); }

  /**
   * Set the type
   * @param type    New type
   */
  public void type(String type)
  {
    if (type == null)
      this.type_ = new String("");
    else
      this.type_ = new String(type);

    this.calculateLength();
  }

  /**
   * Fill the fields of this instance with data from the InputStream.
   */
  public void streamInFrom (InputStream is) throws IOException
  {
    BufferedInputStream bis = new BufferedInputStream (is);

    DataInputStream dis = new DataInputStream (bis);

    this.streamInFrom(dis);
  }

  /**
   * Fill the fields of this instance with data from the SOCKStream.
   */
  public void streamInFrom (SOCKStream sock) throws IOException
  {
    streamInFrom (sock.dataInputStream ());
  }

  /**
   * Fill the fields of this instance from the given DataInputStream
   *
   *@param dis     DataInputStream to read from
   */
  public void streamInFrom (DataInputStream dis) throws IOException
  {
    // Read the length (32 bits)
    length_ = dis.readInt();

    if (length_ > MAX_LEN)
      throw new IOException ("Invalid NameRequest length " + length_);

    // Read the request type
    requestType_ = dis.readInt();

    // Can we block forever to fulfill this request?  (unused)
    blockForever_ = dis.readInt();

    // How long until we should time out this request?  (unused)
    secTimeout_ = dis.readInt();
    usecTimeout_ = dis.readInt();

    // The sizes are in bytes, and there are two bytes per char
    // (ACE_USHORT16 in C++ land)
    int nameLen = dis.readInt() / 2;
    int valueLen = dis.readInt() / 2;

    int typeLen = dis.readInt();

    // Read the name -- just read chars since they're 16 bits.
    // Hopefully the SOCKStream has buffered the data
    char buf[] = new char[nameLen];
    for (int i = 0; i < nameLen; i++) {
      buf[i] = dis.readChar();
    }
    this.name_ = new String(buf);

    // Read the value
    buf = new char[valueLen];
    for (int i = 0; i < valueLen; i++)
      buf[i] = dis.readChar();
    this.value_ = new String(buf);
      
    // Read the type -- now we can use readFully since 
    // the type was sent as 8 bit chars
    byte tbuf[] = new byte[typeLen];
    dis.readFully(tbuf);
    this.type_ = new String(tbuf);

    // Skip the null char at the end
    dis.skipBytes(2);
  }

  /**
   * Write the data of this instance to the given SOCKStream.
   */
  public void streamOutTo (SOCKStream sock) throws IOException
  {
    streamOutTo (sock.dataOutputStream ());
  }

  /**
   * Write the data of this instance to the given OutputStream.
   */
  public void streamOutTo (OutputStream os) throws IOException
  {
    BufferedOutputStream bos = new BufferedOutputStream (os);
    DataOutputStream dos = new DataOutputStream (bos);

    streamOutTo (dos);
  }
  
  /**
   * Send this NameRequest out to the given DataOutputStream
   */
  public void streamOutTo (DataOutputStream dos) throws IOException
  {
    dos.writeInt(length_);
    dos.writeInt(requestType_);
    dos.writeInt(blockForever_);
    dos.writeInt(secTimeout_);
    dos.writeInt(usecTimeout_);

    // Byte sizes are sent, and the name and value are stored as
    // 16 bit char arrays (ACE_USHORT16 arrays in C++ version)
    dos.writeInt(this.name_.length() * 2);
    dos.writeInt(this.value_.length() * 2);
    dos.writeInt(this.type_.length());

    // Making sure the name_ wasn't null comes in handy
    // in situations like this
    dos.writeChars(this.name_);
    dos.writeChars(this.value_);
    dos.writeBytes(this.type_);

    // Null termination
    dos.writeChar(0);

    // Send it for real
    dos.flush();
  }

  /**
   * Set the requestType
   *@param type   Type to set to
   */
  public void requestType(int type)
  {
    this.requestType_ = type;
  }

  /**
   * Get requestType
   */
  public int requestType()
  {
    return this.requestType_;
  }

  /**
   * Can we block forever to fulfill the request? (unused)
   */
  public boolean blockForever()
  {
    return (this.blockForever_ != 0) ? true : false;
  }

  /** 
   * Allowed timeout (unused)
   */
  public int secTimeout()
  {
    return this.secTimeout_;
  }

  int length_;
  int requestType_;
  int blockForever_;
  int secTimeout_;
  int usecTimeout_;

  String name_;
  String value_;
  String type_;
};
