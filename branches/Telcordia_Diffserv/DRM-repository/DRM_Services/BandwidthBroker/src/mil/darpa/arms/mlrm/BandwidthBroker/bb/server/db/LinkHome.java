//$Id: LinkHome.java

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;
/**
 * This home class for Link
 *
 */

public class LinkHome extends TopHome
{
   /**
    * Constructor
    */
   public LinkHome (java.sql.Connection c)
   {
      // defines the table
      super (c, "Link");
      homeBean = new LinkBean (c, getTable (), null);
   }

   public Top getInstance (String id)
   {
      return new LinkBean (getConnection (), getTable (), id);
   }

   public Link narrow (Top t)
   {
      return (Link) t;
   }
}
