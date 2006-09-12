//$Id: LinkBean.java

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

/**
 * This is bean class for Link
 *
 */

class LinkBean extends TopBean implements Link
{
   LinkBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (Link.allAttr);
   }

}
