package mil.darpa.arms.mlrm.BandwidthBroker.ospf;
import java.io.*;

public class ReadOSPFDatabase
{
   private String lastLine = null;
   private BufferedReader br;
   
   void initReader (String dbFile) throws IOException
   {
       br = new BufferedReader (new FileReader (dbFile));  
   }
   
   String peekNextLine () throws IOException
   {
      if (lastLine != null)
         return lastLine;
      else
         return (lastLine = br.readLine());
   }
   
   String readNextLine () throws IOException
   {
      String rc;
      if (lastLine != null)
      {
         rc = lastLine;
         lastLine = null;         
      }
      else
         rc = br.readLine();
      
      return rc;
   }
 
   public void read (String dbFile) throws Exception
   {
      initReader (dbFile);
      while (true)
      {
         String line = peekNextLine ();
          
         if (line == null)
            break;
         if (line.indexOf ("Router Link States (Area") > 0)
            System.out.println ("Router Database start");
         if (line.indexOf ("Net Link States (Area") > 0)
         {
            System.out.println ("Link Database start");
            break;            
         }

         if (line.indexOf("LS age") > 0)
         {            
            createRouter (br);
            continue;
         }           
         readNextLine ();
      }
/*      while (true)
      {
         String line = peekNextLine();
         
         if (line == null)
            break;

         if (line.indexOf("LS age") > 0)
         {
            createLink (br);
         }                    
      }
  */    
      
   }
   
   public void createRouter (BufferedReader br) throws Exception
   {
      String routerID = null;
      String linkID = null;
      String linkAddr = null;
      String metric = null;
      Router r = null;
      
      int offset;
      while (true)
      {
         String lsAge = readNextLine();
         String line = peekNextLine();
         if (line == null)
            break;
         if (line.indexOf("LS age") > 0)
            break;
         if (line.indexOf("show ip ospf database network") > 0)
            break;
         if ((offset = line.indexOf ("Advertising Router: ")) > 0)
         {
            routerID = line.substring(offset + 20).trim();
            r = new Router (routerID);
         }
         if ((offset = line.indexOf ("(Link ID) Designated Router address: ")) > 0)
         {
            linkID = line.substring(offset + 37).trim(); 
         }
         if ((offset = line.indexOf ("(Link Data) Router Interface address: ")) > 0)
         {
            linkAddr = line.substring(offset + 37).trim(); 
         }
         if ((offset = line.indexOf ("Metric: ")) > 0)
         {
            metric = line.substring(offset + 8).trim();
            try
            {
               r.addLink (linkID, linkAddr, Integer.parseInt (metric));
            }
            catch (Exception e)
            {
            
            }
            linkID = linkAddr = metric = null;
         }
         
            
         //System.out.println (line);
      }
      System.out.println ("Created Router " + routerID);
   }
   
   public void createLink (BufferedReader br) throws IOException
   {
      while (true)
      {
         String lsAge = readNextLine();
         String line = peekNextLine();
         if (line == null)
            break;
         if (line.indexOf("LS age") > 0)
            break;         
      }
      System.out.println ("Created Link");
   }
      
   public static void main (String args []) throws Exception
   {
      ReadOSPFDatabase rodb;
      
      rodb = new ReadOSPFDatabase ();
      rodb.read(args [0]);
      //Router.showDMatrix();
      Router.computeAllSPF();
      Router.showDMatrix();
      
      String [] path = 
          Router.getRoute((Router) Router.allRouters.get(4), (Router)Router.allRouters.get(8));
      
      for (int i = 0; i < path.length; i++)
      {
         System.out.println (path [i]);
      }
   }

}
