/*************************************************
 *
 * = PACKAGE
 *    JACE.Connection
 *
 * = FILENAME
 *    BlobHandler.java
 *
 *@author Prashant Jain
 *
 *************************************************/
package JACE.Connection;

import java.io.*;
import java.net.*;
import JACE.SOCK_SAP.*;
import JACE.ASX.*;
import JACE.OS.*;

public abstract class BlobHandler extends SvcHandler
{
  public BlobHandler (int length, int offset, String filename)
  {
    this.length_ = length;
    this.offset_ = offset;
    this.filename_ = filename;
  }

  public abstract int open (Object obj);

  protected int length_ = 0;
  protected int offset_ = 0;
  protected String filename_ = null;
}

