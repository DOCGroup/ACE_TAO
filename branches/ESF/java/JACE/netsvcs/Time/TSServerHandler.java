package JACE.netsvcs.Time;

import java.io.*;
import java.util.*;
import java.net.*;
import JACE.OS.*;
import JACE.Connection.*;
import JACE.Reactor.*;
import JACE.SOCK_SAP.*;
import JACE.netsvcs.Handler;

/**
 * Created by TSServerAcceptor to handle time update requests.  Currently,
 * this simply sends back the current time (in seconds).
 *
 * @see JACE.netsvcs.Logger.ServerLoggingAcceptor
 */
public class TSServerHandler extends Handler
{
  /**
   * Reads in the given TimeRequest request and calls dispatch.
   *
   *@param request TimeRequest instance to use
   */
  public void processRequest (Object requestObject)
    throws SocketException, EOFException, IOException
  {
    TimeRequest request = (TimeRequest)requestObject;

    request.streamInFrom (peer ().dataInputStream ());

    this.dispatch (request);
  }

  /**
   * Sets the time value of the request to be the local time (in sec)
   * and sends it back to the client.
   */
  void dispatch(TimeRequest request) throws IOException
  {
    request.time ((int)(System.currentTimeMillis () / 1000));

    request.streamOutTo (peer().outputStream ());
  }

  /**
   * Creates a new instance of TimeRequest.
   */
  public Object newRequest ()
  {
    return new TimeRequest ();
  }
}
