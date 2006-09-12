//$Id: Inet4Subnet.java
// This software is governed by Government Purpose Rights
// Contract No: NBCHC030132
// Telcordia Technologies, Inc.
// 1 Telcordia Drive
// Piscataway, NJ 08854-4157

// InetSubnet.java

// This class represents IPv4 subnet


// Revision history

// 03-12-04        Shree	Initial version
//                              
//

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server;

import java.util.*;

public class Inet4Subnet
{
   // expects ipAddress as a.b.c.d
   // expects mask  a.b.c.d (e.g. 255.255.255.0)

   long ipAddress;
   long mask;

   public Inet4Subnet (String ipAddress, String mask) throws Exception
   {
      this.ipAddress = stringToInt (ipAddress);
      this.mask = stringToInt (mask);
   }

   boolean isInSubnet (String addr)
   {
      try
      {
         return isInSubnet ((int) stringToInt (addr));
      }
      catch (Exception e)
      {
      }
      return false;
   }

   boolean isInSubnet (int addr)
   {
      long t = 0;
      t = (t | addr) & 0xffffffffL;

      if ((t & mask) == (ipAddress & mask))
         return true;

      return false;
   }

   public static long stringToInt (String address) throws Exception
   {
      long addr = 0;

      StringTokenizer st = new StringTokenizer (address, ".");

      for (int i = 0; i < 4; i++)
      {
         if (st.hasMoreTokens ())
         {
            addr = addr << 8;

            int a = Integer.parseInt (st.nextToken ());
            if (a < 256)
            {
               addr += a;
               continue;
            }
         }
         throw new Exception ();
      }
      return addr;
   }

   public static boolean areInSameSubnet (String ipA, String maskA,
                                          String ipB, String maskB)
   {
      try
      {
         Inet4Subnet a = new Inet4Subnet (ipA, maskA);
         Inet4Subnet b = new Inet4Subnet (ipB, maskB);
   
         return ((a.ipAddress & a.mask) == (b.ipAddress & b.mask));
      }
      catch (Exception e)
      {
      }
      return false;
   }
}
