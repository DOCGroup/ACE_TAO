package netsvcs.Time;

import JACE.SOCK_SAP.*;
import java.io.*; 
import java.net.*;
import JACE.OS.*;

public class ClientTest {

  public static void main(String args[])
    {
      // Don't worry about processing command line right now

      int port = 7990;
      
      String host = new String("flamenco.cs.wustl.edu");  
      // Why isn't the default defined?


      SOCKStream cli_stream = new SOCKStream();

      INETAddr remote_addr;
      try {
	remote_addr = new INETAddr(port, host);
      } catch (UnknownHostException uhe) {
	ACE.ERROR("UnknownHostException " + uhe);
	return;
      }

      SOCKConnector con = new SOCKConnector();

      ACE.DEBUG("Starting connect...");

      // Can't do non-blocking connect in Java!  Yippee!
 
      try {

	con.connect(cli_stream, remote_addr);

      } catch (SocketException se) {
	
	ACE.ERROR("Socket Exception " + se);
	return;

      } catch (IOException ie) {

	ACE.ERROR("IOException " + ie);
	return;
      }
    
      String input;  // need new string here?
      BufferedReader in
	= new BufferedReader(new InputStreamReader(System.in));
 
      try {

	do {

	  input = in.readLine();
	  
	  if (input.equals("quit"))
	    break;

	  cli_stream.send(new StringBuffer(input + "\n"));

	  StringBuffer result = new StringBuffer();
	  int chars = cli_stream.recv(result);
	  if (chars == -1)
	    System.out.println("Evil!");
	  else
	    System.out.println(result);

	} while (true);

	// No close_writer, etc...
	cli_stream.close();

      } catch (IOException ie) {

	ACE.ERROR("IOException, loop: " + ie);
	return;
      }
    }
};


      

	

	

	
      
