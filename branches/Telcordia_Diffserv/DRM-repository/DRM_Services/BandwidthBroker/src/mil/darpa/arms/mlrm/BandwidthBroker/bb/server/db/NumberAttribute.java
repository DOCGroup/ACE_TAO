//$Id: NumberAttribute.java
/*
 * NumberAttribute.java - class for holding Number attribute 
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Technology Drive
 * Piscataway, NJ 08854-4157
 *
 *
 */


package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

import java.util.*;

/**
 * This class holds attribute name-value pairs 
 *
 */

public class NumberAttribute extends Attribute
{
   public NumberAttribute (String name)
   {
      super (name);
   }

   public String toDbString (String value)
   {
      return value;
   }

   public boolean equals (Attribute a)
   {
      if (a instanceof NumberAttribute)
      // bit dubious but for sake of hashtable
         return this.name.equals (a.name);
      else
         return false;
   }
}
