//$Id: FaultSubscriptionsHome.java

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;
/**
 * This home class for FaultSubscriptions
 *
 */

public class FaultSubscriptionsHome extends TopHome
{
   /**
    * Constructor
    */
   public FaultSubscriptionsHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "FaultSubscriptions");
      homeBean = new FaultSubscriptionsBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new FaultSubscriptionsBean (getConnection (), getTable (), id);
   }

   public FaultSubscriptions narrow (Top t)
   {
      return (FaultSubscriptions) t;
   }
}
