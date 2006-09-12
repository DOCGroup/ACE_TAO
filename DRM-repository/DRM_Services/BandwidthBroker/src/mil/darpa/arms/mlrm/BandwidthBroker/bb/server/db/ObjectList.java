//$Id: ObjectList.java
/*
 * ObjectList.java - class for list of objects that are returned by
 * a find query
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
 * This class represents list of objects returned by a query
 *
 */
public class ObjectList
{
   private Vector list = new Vector ();
   /**
    * Adds an object to the list
    *
    */
   public void add (Top obj)
   {
      list.add (obj);
   }

   /**
    * removes an object from the list
    *
    */
   public void remove (Top obj)
   {
      list.remove (obj);
   }

   /**
    * gets an object at a specified index;
    */
   public Top get (int index)
   {
      return (Top) list.get (index);
   }

   /**
    * returns the size of object list
    */
   public int size ()
   {
      return list.size ();
   }

}
