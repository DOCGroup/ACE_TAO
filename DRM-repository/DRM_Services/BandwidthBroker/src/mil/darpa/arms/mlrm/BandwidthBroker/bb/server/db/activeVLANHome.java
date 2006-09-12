//$Id: activeVLANHome.java

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;
/**
 * This home class for activeVLAN
 *
 */

public class activeVLANHome extends TopHome
{
   /**
    * Constructor
    */
   public activeVLANHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "activeVLAN");
      homeBean = new activeVLANBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new activeVLANBean (getConnection (), getTable (), id);
   }

   public activeVLAN narrow (Top t)
   {
      return (activeVLAN) t;
   }
}
