//$Id: SwitchBean.java
/*
 * SwitchBean.java - bean class for Switchs
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

/**
 * This is bean class for L2 Switches
 *
 */

class SwitchBean extends MEBean implements Switch
{
   SwitchBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (Switch.allAttr);
      //allAttributes.add (isProviderEdge);
   }


   public L2Interface [] getL2Interfaces (AttributeValues av) throws CDBException
   {
      if (av == null)
         av = new AttributeValues ();
      av.addNameValue (L2Interface.attrSwitchID, getID ());

      L2InterfaceHome l3PTPHome = new L2InterfaceHome (getConnection ());
      ObjectList ol = l3PTPHome.findByAttributes (av);

      L2Interface [] rc = new L2Interface [ol.size ()];
      for (int i = 0; i < rc.length; i++)
      {
          rc [i] = (L2Interface) ol.get (i);
      }
      return rc;
      
   }
}
