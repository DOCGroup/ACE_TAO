//$Id: activeVLANBean.java

package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

/**
 * This is bean class for activeVLAN
 *
 */

class activeVLANBean extends TopBean implements activeVLAN
{
   activeVLANBean (java.sql.Connection c, String table, String id)
   {
      super (c, table, id);
      addToAllAttributes (activeVLAN.allAttr);
   }

}
