// $Id$ 
// ============================================================================
//
// 
// = FILENAME
//    NS_Resolve.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   Resolves the initial reference to the Naming service,
//   the orb has to be given as a parameter to the
//   resolve_name_service call. 
// 
//
// ============================================================================




import org.omg.CORBA.*;
import java.net.*;
import java.io.*;


public class NS_Resolve
{
  
  private static final String ACE_DEFAULT_MULTICAST_ADDR = "224.9.9.2";
  private static final int TAO_DEFAULT_NAME_SERVER_REQUEST_PORT = 10013;
  private static final int MULTICAST_SEND_PORT = 10060;
  private static final int MULTICAST_RECEIVE_PORT = 10061;
  private static final int TAO_SERVICEID_NAMESERVICE = 0;
  private static final int TAO_SERVICEID_TRADINGSERVICE = 1;

  org.omg.CORBA.Object name_service_;
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
      try
	{
	  // Create a message with the multicast receive port in it
	  ByteArrayOutputStream byte_stream_ = new ByteArrayOutputStream ();
      	  byte_stream_.write ((int)((MULTICAST_RECEIVE_PORT&0xff00)>>>8));
	  byte_stream_.write ((int)(MULTICAST_RECEIVE_PORT&0x00ff));
      	  byte_stream_.write ((int)((TAO_SERVICEID_NAMESERVICE&0xff00)>>>8));
	  byte_stream_.write ((int)(TAO_SERVICEID_NAMESERVICE&0x00ff));
	  byte[] msg = byte_stream_.toByteArray();


	  // Define the group for the multicast

	  InetAddress group = InetAddress.getByName(ACE_DEFAULT_MULTICAST_ADDR);
	  // Create the multicast socket at any port you want
	  MulticastSocket multicastsocket_ = new MulticastSocket(MULTICAST_SEND_PORT);
	  // Create a socket for the answer of the Naming Service 
	  DatagramSocket socket_ = new DatagramSocket (MULTICAST_RECEIVE_PORT);
	  // Give three seconds time for the Naming Service to respond
	  socket_.setSoTimeout (3000);
	  // Build a packet with the port number in it
	  DatagramPacket  hello = new DatagramPacket(msg, msg.length,
						     group, nameServicePort_);

	  // Send the packet
	  multicastsocket_.send (hello);

	  // @@ The restriction right now is that the length of the IOR cannot be longer than 4096
	  byte[] buf = new byte[4096];
	  // Set up packet which can be received
	  DatagramPacket recv = new DatagramPacket (buf, buf.length);	  
	  // Receive a packet or time out
	  socket_.receive (recv);

	  // Determine the length of the IOR
	  int length;
	  for (length = 0; buf[length] != 0; length++);

	  // Store the IOR in a String
       	  String name_service_ior_ = new String (recv.getData (),0,length);

	  // Convert the String into
	  return orb.string_to_object (name_service_ior_);
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
      return null;
      
    } 

};





