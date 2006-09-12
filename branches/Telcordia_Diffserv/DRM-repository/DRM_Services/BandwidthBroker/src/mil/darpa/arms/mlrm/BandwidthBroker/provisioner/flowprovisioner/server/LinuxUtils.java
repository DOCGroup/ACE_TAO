// $Id: FlowProvisioningImpl.java
//The DARPA ARMS Program
// This software/interface/output
// is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Technology Drive
// Piscataway, NJ 08854-4157 

package mil.darpa.arms.mlrm.BandwidthBroker.provisioner.flowprovisioner.server;
import java.io.*;

public class LinuxUtils 
{

   public static void _issueCmd (String cmds, String host, boolean loopback) throws IOException
   {
      String fname;
      FileWriter fr = new FileWriter (fname = host+ "_" + cmds.length ());
      fr.write (cmds, 0, cmds.length ());
      fr.close ();
      if (loopback == false)
         _issueCmdViaSSH (host, fname);
   }

   private static void _issueCmdViaSSH (String host, String cmdFile) throws IOException
   {
      Process p;
      String command =  "ssh -T " + host + " < " + cmdFile + " > output" ;
      String cmd [] = {"sh", "-c", command};

     try {
      Runtime r = Runtime.getRuntime ();
      p = r.exec (cmd);
       System.out.println ("Waiting for commands to be executed completely on :"+host);
      /*InputStream bof = new BufferedInputStream (new FileInputStream ("input1"));
      bof = p.getInputStream ();
      byte [] arr = new byte [5000];
      bof.read (arr, 0, bof.available ()); 

      bof.close ();
      InputStream bof2 = new BufferedInputStream (new FileInputStream ("error1"));
      bof2 = p.getErrorStream ();

      byte [] arr2 = new byte [5000];
      bof2.read (arr2, 0, 100); 
      
       bof2.close ();

      OutputStream bof3 = new BufferedOutputStream (new FileOutputStream ("output1"));
	bof3 = p.getOutputStream ();
        byte [] arr3 = new byte [5000];
        bof3.write (arr3, 0, 100); 
        
	bof3.flush ();
	bof3.close ();
     */
      //Commented this out since on redhat 9.0 the thread doesnt return, tried getting the different streams of the process in case they cause the blocking in van : Kirthika
      // p.waitFor ();
       System.out.println ("Finished commands" );
      }
      catch (Exception e)
      {
	e.printStackTrace ();
        throw new IOException ();
      }
     }

    public static void main (String [] args)
      {
	 try{
	 LinuxUtils._issueCmdViaSSH ("access1", "access1_1242_t");
	}
	catch (Exception e)
	{
	   e.printStackTrace ();
         }
      }
       
   
}
