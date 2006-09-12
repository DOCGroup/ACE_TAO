//$Id: RouterBean.java
/*
 * RouterBean.java - bean class for Routers
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
 * This is bean class for Routers
 *
 */

class RouterBean extends MEBean implements Router
{
   RouterBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (Router.allAttr);
      //allAttributes.add (isProviderEdge);
   }

    public L3Interface [] getL3Interfaces (AttributeValues av) throws CDBException
   {
      if (av == null)
         av = new AttributeValues ();
      av.addNameValue (L3Interface.attrRouterID, getID ());

      L3InterfaceHome l3PTPHome = new L3InterfaceHome (getConnection ());
      ObjectList ol = l3PTPHome.findByAttributes (av);

      L3Interface [] rc = new L3Interface [ol.size ()];
      for (int i = 0; i < rc.length; i++)
      {
          rc [i] = (L3Interface) ol.get (i);
      }
      return rc;
      
   }
}
