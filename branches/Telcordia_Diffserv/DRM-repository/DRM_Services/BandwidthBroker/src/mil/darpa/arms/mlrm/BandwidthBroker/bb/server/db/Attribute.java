//$Id: Attribute.java
/*
 * Attribute.java - class for holding attribute 
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Technology Drive
 * Piscataway, NJ 08854-4157
 *
 */


package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

import java.util.*;

/**
 * This class holds attribute name-value pairs 
 *
 */

abstract public class Attribute
{
   String name;
   String value = null;

   public Attribute (String name)
   {
      this.name = name;
   }

   public String toString ()
   {
      return name;
   }

   abstract public String toDbString (String value);

   public boolean equals (Attribute a)
   {
      // bit dubious but for sake of hashtable
      return this.name.equals (a.name);
   }

   public String getValue ()
   {
      return value;
   }

   public void setValue (String value)
   {
      this.value = value;
   }

   public String getDbValue ()
   {
      return toDbString (value);
   }
}
