/*************************************************
 *
 * = PACKAGE
 *    ACE.SOCK_SAP
 *
 * = FILENAME
 *    INETAddr.java
 *
 *@author Chris Cleeland
 *
 *************************************************/
package ACE.SOCK_SAP;

import java.io.*;
import java.net.*;
import ACE.OS.*;

/**
 * <hr>
 * <p><b>TITLE</b><br>
 *     Defines an endpoint of a connection, encapsulating host and port.
 * This is only a part-way implementation of C++ ACE's ACE_INET_Addr.
 *
 * <p><b>LIMITATIONS</b><br>
 *     Currently the class is very limited in its capabilities; it will
 * be expanded in future revisions of ACE.
 */
public class INETAddr // extends Addr
{
  private InetAddress addr_;
  private int port_ = 0;
  /**
   */
  public INETAddr ()
    {
      // Do nothing constructor
    }

  /**
   * Create an INETAddr from a port/hostname
   *@param port port number to connect with server at
   *@param hostname hostname of the server
   */
  public INETAddr (int port, String hostname) throws UnknownHostException
  {
    super();
    port_ = port;
    addr_ = InetAddress.getByName(hostname);
				// Should really use getAllByName(),
				// but I don't think we do that in
				// C++ ACE, even.
  }

  /**
   * Create an INETAddr from an address.
   * @param address an address in the form "ip-number:port-number", <em>e.g.</em> <pre>tango.cs.wustl.edu:1234</pre> or <pre>128.252.166.57:1234</pre>; if no ':' is present address is assumed to be <b>INADDR_ANY</b> and address contains only the port number
   * @throws UnknownHostException
   */
  public INETAddr (String address) throws UnknownHostException
  {
    int colon = address.indexOf(':');
    if (colon != 0)
      {
	addr_ = InetAddress.getByName(address.substring(0, colon));
	address = address.substring(colon+1);
      }

    port_ = Integer.parseInt(address);
  }

  /**
   * Return the name of the host.
   */
  public String getHostName()
  {
    return addr_.getHostName();
  }

  /**
   * Return the dotted Internet address.
   */
  public String getHostAddr()
  {
    return addr_.toString();
  }

  /**
   * Return the port number.
   */
  public int getPortNumber()
  {
    return port_;
  }

  public String toString()
  {
    return getHostAddr() + Integer.toString(port_);
  }
}
