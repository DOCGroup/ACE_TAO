// $Id: FlowProvisioningImpl.java
//The DARPA ARMS Program
// This software/interface/output
// is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Technology Drive
// Piscataway, NJ 08854-4157 

package mil.darpa.arms.mlrm.BandwidthBroker.monitor;
import java.io.*;

public class LinuxUtils 
{

   public static void _issueCmd (String cmds, String host, String outFile) throws IOException
   {
      String fname;
      FileWriter fr = new FileWriter (fname = host+ "_" + cmds.length ());
      fr.write (cmds, 0, cmds.length ());
      fr.close ();
      _issueCmdViaSSH (host, fname, outFile);
   }

   private static void _issueCmdViaSSH (String host, String cmdFile,
                                        String outFile) throws IOException
   {
      Process p;
      String command =  "ssh -T " + host + " < " + cmdFile + " > " + outFile;
      String cmd [] = {"sh", "-c", command};

      Runtime r = Runtime.getRuntime ();
      p = r.exec (cmd);
      try 
      {
         p.waitFor ();
      }
      catch (Exception e)
      {
        throw new IOException ();
      }


       
   }
}
