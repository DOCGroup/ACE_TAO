package mil.darpa.arms.mlrm.BandwidthBroker.ospf;

import org.snmp4j.*;
import org.snmp4j.smi.*;
import org.snmp4j.mp.*;
import org.snmp4j.event.*;
import org.snmp4j.transport.*;
import org.snmp4j.util.*;
import org.snmp4j.security.*;
import java.net.*;
import java.io.*;
import java.util.*;

import org.apache.log4j.*;
import org.snmp4j.asn1.BER;
import org.snmp4j.log.LogFactory;
import org.snmp4j.log.Log4jLogFactory;
import org.snmp4j.log.ConsoleLogFactory;


public class ReadOSPFDatabaseSNMP
{
   private int peekByte (String adv, int offset)
   {
      return Integer.parseInt(adv.substring(offset * 3, (offset*3) + 2), 16);
   }
   private class MyTransportMapping extends DefaultUdpTransportMapping
   {
      public MyTransportMapping (UdpAddress udpAddress) throws IOException
      {
         super (udpAddress);
	 socket.close ();
	 socket = new DatagramSocket (null);
	 socket.setReuseAddress (true);
	 socket.bind (new InetSocketAddress (udpAddress.getInetAddress(), udpAddress.getPort ()));
      }
   }
   
   static boolean tableEmpty = true;

   public void read (String routerIP) throws Exception
   {


      LogFactory.setLogFactory (new ConsoleLogFactory ());
      BER.setCheckSequenceLength (false);
      BasicConfigurator.configure ();
      Logger.getRootLogger().setLevel (Level.ALL);
      


//      Address targetAddress = GenericAddress.parse("udp:" + routerIP + "/161");
      Address targetAddress = GenericAddress.parse("udp:" + routerIP + "/5000");
      String hostname = InetAddress.getLocalHost ().getHostName ();
      hostname = hostname.substring (0, hostname.indexOf ("."));
      System.out.println ("Localhost = " + hostname);
      //InetAddress lh = InetAddress.getByName (hostname);
      //InetAddress lh = InetAddress.getByName ("pool1bb");
      //Random r = new Random ();
      //int port = 50000 + r.nextInt (10000);
      TransportMapping transport = new MyTransportMapping (new UdpAddress (hostname + "/0")); // + port));
      //System.out.println ("listen socket at " + lh);
      Snmp snmp = new Snmp(transport);
      //MPv3 mpv3 =
      //   (MPv3)snmp.getMessageProcessingModel(MessageProcessingModel.MPv3);
     //USM usm = new USM(SecurityProtocols.getInstance(),
     //                  new OctetString(mpv3.createLocalEngineID()), 0);
     //SecurityModels.getInstance().addSecurityModel(usm);
     //System.out.println ("Listening to the transport");
     transport.listen();
     //System.out.println ("Listen thread stared");

      
      CommunityTarget target = new CommunityTarget();
      target.setCommunity(new OctetString("local"));
      target.setAddress(targetAddress);
      target.setRetries(2);
      target.setTimeout(5000);
      target.setVersion(SnmpConstants.version1);
      
      OID oid = new OID (new int [] {1,3,6,1,2,1,14,4,1,8});
      OID[] oids = new OID [] {oid};
      //OID start = new OID (new int [] {1,3,6,1,2,1,2,2});
      
      // init the routing table computation variables

      TableUtils tu = new TableUtils (snmp, new DefaultPDUFactory ());
      TableListener tabListener = new TableListener () {
         public boolean next(TableEvent event)
         {
	    tableEmpty = false;
            VariableBinding [] vb = event.getColumns();
            for (int i = 0; i < vb.length; i++)
            {
               //System.out.println(vb [i].getOid().toString() + ":" + vb [i].getVariable().toString());
               String adv = vb [i].getVariable().toString();
               try
               {
               int lsType = peekByte (adv, 3);
               if (lsType == 1) // the advertisement is for a router
               {
                  String routerID = peekByte (adv, 8) + "." + 
                                    peekByte (adv, 9) + "." +
                                    peekByte (adv, 10) + "." +
                                    peekByte (adv, 11);
                  
                  Router r = new Router (routerID);
                  
                  int numLinks = peekByte (adv, 22)*8 + peekByte (adv, 23);
                  //System.out.println (routerID + ":" + numLinks);
                  int offset = 24;
                  for (int j = 0; j < numLinks; j++)
                  {
                     String linkID = peekByte (adv, offset + (j*12)) + "." + 
                                     peekByte (adv, offset + (j*12) + 1) + "." +
                                     peekByte (adv, offset + (j*12) + 2) + "." +
                                     peekByte (adv, offset + (j*12) + 3);

                     String linkAddr = peekByte (adv, offset + (j*12) + 4) + "." + 
                                     peekByte (adv, offset + (j*12) + 5) + "." +
                                     peekByte (adv, offset + (j*12) + 6) + "." +
                                     peekByte (adv, offset + (j*12) + 7);
                     int metric = peekByte (adv, offset + j*12 + 10)*8 + peekByte (adv, offset + j*12 + 11);
                     //System.out.println ("    " + linkID + "/" + linkAddr + "/" + metric);
                     r.addLink (linkID, linkAddr, metric);
                  }
               }
               }
               catch (Exception e)
               {
                  e.printStackTrace();
               }
               
            }
            return true;
         }
         public void finished(TableEvent event)
         {
            System.out.println ("Done");
            synchronized (this)
            {
               notifyAll();
            }
         }
      };
      synchronized (tabListener)
      {
         Router.init ();
         System.out.println ("Sent getDenseTable call...");
         tu.getDenseTable(target, oids, tabListener, null, null, null);
         tabListener.wait ();
	 if (tableEmpty == true)
	 {
	    //try again
            Router.init ();
            tu.getDenseTable(target, oids, tabListener, null, null, null);
            tabListener.wait ();
	 }
         snmp.close();
	 transport.close ();
         System.out.println ("Snmp session closed.");
      }
      
      
      long t = System.currentTimeMillis ();
      Router.computeAllSPF();
      System.out.println ("Path computation time = " + 
	     (System.currentTimeMillis () - t));
      //Router.showDMatrix();
      /*
      
      String [] path = 
          Router.getRoute((Router) Router.allRouters.get(4), (Router)Router.allRouters.get(8));
      
      for (int i = 0; i < path.length; i++)
      {
         System.out.println (path [i]);
      }
      
      
      
      // creating PDU
      PDU pdu = new PDU();
      pdu.add(new VariableBinding(new OID(new int[] {1,3,6,1,2,1,1,1})));
      //pdu.add(new VariableBinding(new OID(new int[] {1,3,6,1,2,1,1,2})));
      
      pdu.setType(PDU.GETNEXT);
      // sending request
      ResponseListener listener = new ResponseListener() {
        public void onResponse(ResponseEvent event) {
          // Always cancel async request when response has been received
          // otherwise a memory leak is created! Not canceling a request
          // immediately can be useful when sending a request to a broadcast
          // address.
          ((Snmp)event.getSource()).cancel(event.getRequest(), this);
          System.out.println("Received response PDU is: "+event.getResponse());
        }
      };
 
      snmp.sendPDU(pdu, target,  null, listener);
      // Thread.sleep(15000);
    
       */
      //Thread.currentThread().join ();

   }
      
   public static void main (String args []) throws Exception
   {
      ReadOSPFDatabaseSNMP rodbs = new ReadOSPFDatabaseSNMP();
      //rodbs.read("155.98.39.30");
      if (args.length > 0)
      {
         System.out.println ("Starting snmp test");
         rodbs.read(args [0]);
      }
      else
      {
         System.out.println ("Syntax: java ReadOSPFDatabaseSNMP <ip-address>");
	 System.exit (1);
      }

      //Router.showDMatrix();
      /*Router.computeAllSPF();
      Router.showDMatrix();
      
      String [] path = 
          Router.getRoute((Router) Router.allRouters.get(4), (Router)Router.allRouters.get(8));
      
      for (int i = 0; i < path.length; i++)
      {
         System.out.println (path [i]);
      }
      */
   }
   

}
