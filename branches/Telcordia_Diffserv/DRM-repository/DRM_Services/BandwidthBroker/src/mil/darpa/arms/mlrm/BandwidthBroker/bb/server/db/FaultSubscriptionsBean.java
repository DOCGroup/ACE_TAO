//$Id: FaultSubscriptionsBean.java

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

/**
 * This is bean class for FaultSubscriptions
 *
 */

class FaultSubscriptionsBean extends TopBean implements FaultSubscriptions
{
   FaultSubscriptionsBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (FaultSubscriptions.allAttr);
   }

}
