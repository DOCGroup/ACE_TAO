//$Id: Top.java
/*
 * Top.java - The base class for all the network entities
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Telcordia Drive
 * Piscataway, NJ 08854-4157
 *
 */


package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

/**
 * This class is a base class for all the network objects
 *
 */

public interface Top
{
   // override in sub-classes
   public static Attribute ID = new StringAttribute ("id"); 
   // override in sub-classes
   static Attribute []  allAttr = {ID};

   /**
    * Returns a hashtable of all the attributes values of the object
    * the name of the attribute is the key in hashtable
    */
   public AttributeValues getAttributes () throws CDBException;
   
   /**
    * Returns a hashtable of attributes values of the object for 
    * attributes specified in AttributeList
    * the name of the attribute is the key in hashtable
    */
   public AttributeValues getAttributes (AttributeList aList) throws CDBException;

   public void setAttributes (AttributeValues values) throws CDBException;
   
   public Attribute getAttributeByName (String name);
   
   public String getID ();

}
