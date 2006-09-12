//$Id: AttributeList.java
/*
 * AttributeList.java - The class for holding list of attributes
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
 * This class holds a list of attributes
 * objects
 *
 */
public class AttributeList
{
   private Vector list = new Vector ();


   /**
    * Adds an attribute to the list
    *
    */
   public void add (Attribute name)
   {
      list.add (name);
   }


   /**
    * removes an attribute from the list
    *
    */
   public void remove (Attribute name)
   {
      list.remove (name);
   }

   /**
    * gets an attribute at a specified index;
    */
   public Attribute get (int index)
   {
      return (Attribute) list.get (index);
   }

   /**
    * returns the size of attribute list
    */
   public int size ()
   {
      return list.size ();
   }
}
