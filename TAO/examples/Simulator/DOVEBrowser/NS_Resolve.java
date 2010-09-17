// $Id$

// ============================================================================
//
//
// = FILENAME
//    NS_Resolve.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//    Modified for new protocol by Hans Ridder <ridder@veritas.com>
//    Further improved by Kevin Regan <kregan@infoglide.com>
//
// = DESCRIPTION
//   Resolves the initial reference to the Naming service,
//   the orb has to be given as a parameter to the
//   resolve_name_service call.
//
// ============================================================================

import org.omg.CORBA.*;
import java.net.*;
import java.io.*;

public class NS_Resolve
{
  private static final String ACE_DEFAULT_MULTICAST_ADDR = "224.9.9.2";
  private static final int TAO_DEFAULT_NAME_SERVER_REQUEST_PORT = 10013;
  private static final String TAO_SERVICEID_NAMESERVICE = "NameService";

  int nameServicePort_;

  public NS_Resolve (String nameServicePort)
  {
    if (nameServicePort != null)
      {
        // If a name service port string was given, parse it
        nameServicePort_ = Integer.parseInt (nameServicePort);
      }
    else
      {
        // Otherwise, just use the default TAO name service port
        nameServicePort_ = TAO_DEFAULT_NAME_SERVER_REQUEST_PORT;
      }
  }

  public org.omg.CORBA.Object resolve_name_service (org.omg.CORBA.ORB orb)
  {
    MulticastSocket sendSocket = null;
    ServerSocket listenSocket = null;
    Socket replySocket = null;

    try
      {
        // Create the multicast socket at any port
        sendSocket = new MulticastSocket(0);

        // Create a socket at any port for the Naming Service answer
        listenSocket = new ServerSocket(0);

        // Create a message with the port and service name in it,
        // length and port number are in network byte order
        ByteArrayOutputStream msg = new ByteArrayOutputStream();
        int dataLength = TAO_SERVICEID_NAMESERVICE.length() + 3;
        msg.write((dataLength >> 8) & 0xff);
        msg.write(dataLength & 0xff);
        msg.write((listenSocket.getLocalPort() >> 8) & 0xff);
        msg.write(listenSocket.getLocalPort() & 0xff);
        msg.write(TAO_SERVICEID_NAMESERVICE.getBytes());
        msg.write(0);

        // Define the group for the multicast
        InetAddress group = InetAddress.getByName(ACE_DEFAULT_MULTICAST_ADDR);
        // Create a datagram with the message and send it
        sendSocket.send(new DatagramPacket(msg.toByteArray(),
                                           msg.size(),
                                           group, nameServicePort_));

        // Wait 3 seconds for the Naming Service to connect
        listenSocket.setSoTimeout(3000);
        replySocket = listenSocket.accept();

        // @@ The restriction right now is that the length of the IOR cannot be longer than 4096
        char[] reply = new char[4096];

        // Receive the reply (0 terminated string) or time out
        replySocket.setSoTimeout(3000);
        InputStream in = replySocket.getInputStream();
        int length;
        for (length = 0; length < reply.length; length++)
          {
            int c = in.read();

            if (c == -1)
              {
                throw new IOException("Unexpected EOF.");
              }

            reply[ length ] = (char) c;

            if (c == 0)
              {
                break;
              }
          }

        // Convert the String into ??
        return orb.string_to_object(new String(reply, 2, length-2));
      }
    catch (SocketException e)
      {
        System.err.println (e);
      }
    catch (java.io.InterruptedIOException e)
      {
        System.err.println ("NS_Resolve: The receive lasted too long");
      }
    catch(org.omg.CORBA.SystemException e)
      {
        System.err.println(e);
      }
    catch (java.io.IOException e)
      {
        System.err.println (e);
      }
    finally
      {
        // Close the sockets.

        if (sendSocket != null)
          {
            sendSocket.close();
          }

        if (listenSocket != null)
          {
            try
              {
                listenSocket.close();
              }
            catch (IOException e) {}
          }

        if (replySocket != null)
          {
            try
              {
                replySocket.close();
              }
            catch (IOException e) {}
          }
      }

    return null;
  }
}
