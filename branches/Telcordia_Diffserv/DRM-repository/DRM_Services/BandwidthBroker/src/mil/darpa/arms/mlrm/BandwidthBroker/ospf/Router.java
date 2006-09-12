package mil.darpa.arms.mlrm.BandwidthBroker.ospf;

import java.io.BufferedReader;
import java.util.*;

public class Router
{
   static Hashtable reverseLookup = new Hashtable ();

   public static ArrayList allRouters = new ArrayList ();

   String name;

   Hashtable inBounds;

   public static void init ()
   {
      allRouters = new ArrayList ();
      reverseLookup = new Hashtable ();
   }
   public Router(String name) throws Exception {
      this.name = name;
      inBounds = new Hashtable ();
      if (allRouters.contains (this))
         throw new Exception ();
      allRouters.add (this);
   }

   private static class Inbound
   {
      String id;

      String addr;

      int metric;

      Router from;

      Inbound(String id, String addr, int metric, Router from) {
         this.id = id;
         this.addr = addr;
         this.metric = metric;
         this.from = from;
      }

      public void show()
      {
         System.out.println (id + ":" + addr + ":" + metric);
      }
   }

   public void addLink(String id, String addr, int metric) throws Exception
   {
      // System.out.println (id + ":" + addr + ":" + metric);
      if (id == null || addr == null)
         return;
      Router farEnd = (Router) reverseLookup.remove (id);
      Inbound i = new Inbound (id, addr, metric, farEnd);

      if (farEnd != null)
      {
         // System.out.println ("Got far end");
         inBounds.put (farEnd, i);
         Inbound ifar = (Inbound) farEnd.inBounds.remove (id);
         ifar.from = this;
         farEnd.inBounds.put (this, ifar);
      }
      else
      {
         // System.out.println ("no far end");
         inBounds.put (id, i);
         reverseLookup.put (id, this);
      }

   }
   
   static void showDMatrix()
   {
      for (int i = 0; i < allRouters.size (); i++)
      {
         Router r = (Router) allRouters.get (i);
         System.out.println ("Printing inbounds for " + r.name);
         Enumeration e = r.inBounds.keys ();
         Router farEnd;
         while (e.hasMoreElements ())
         {
            Object o = e.nextElement();
            farEnd = null;
            if (o instanceof Router)
               farEnd = (Router) o;
            else if (o instanceof String)
            {
               System.out.println ("Unresolved LSID: " + o);
               continue;
            }
            Inbound in = (Inbound) r.inBounds.get (farEnd);
            System.out.print (farEnd.name + ":");
            in.show ();
         }
      }
   }

   public static int distance(Router s, Router d)
   {
      // System.out.println ("Findind distance between " + s.name +" and " +
      // d.name);
      Inbound i = (Inbound) d.inBounds.get (s);
      if (i == null)
         return -1;
      else
      {
         // System.out.println (s.name + "->" + d.name + ":" + i.metric);
         return i.metric;
      }

   }

   public static void computeAllSPF()
   {
      int N = allRouters.size ();
      for (int k = 0; k < N; k++)
      {
         Router hop = (Router) allRouters.get (k);
         for (int j = 0; j < N; j++)
         {
            Router d = (Router) allRouters.get (j);
            for (int i = 0; i < N; i++)
            {
               if (i == j || i == k || k == j)
                  continue;
               Router s = (Router) allRouters.get (i);
               int dij = distance (s, d);
               int dik = distance (s, hop);
               int dkj = distance (hop, d);
               if (dik == -1 || dkj == -1)
                  continue;
               if (dij == -1 || (dij > (dik + dkj)))
               {
                  // System.out.println (s.name + "->" + d.name + "through " +
                  // hop.name);
                  Inbound hopi = (Inbound) d.inBounds.get (hop);
                  d.inBounds.put (s, new Inbound (hopi.id, hopi.addr,
                        dik + dkj, hopi.from));
               }

            }

         }
      }

   }

   public boolean equals(Object o)
   {
      try
      {
         Router r = (Router) o;
         return r.name.equals (name);
      }
      catch (ClassCastException e)
      {

      }
      return false;
   }

   public static String[] getRoute(Router s, Router d)
   {
      Vector route = new Vector ();

      while (true)
      {
         Inbound i = (Inbound) d.inBounds.get (s);
	 if (i == null)
	    break;
         route.add (0, i.addr);
         if (i.from == s)
            break;
         d = i.from;
      }
      String[] rc = new String[route.size ()];
      route.toArray (rc);
      return rc;

   }
}
