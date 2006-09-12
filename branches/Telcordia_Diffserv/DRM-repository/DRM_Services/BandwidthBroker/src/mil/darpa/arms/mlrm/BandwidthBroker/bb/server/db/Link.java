//$Id: Link.java


package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

/**
 * This class represents Links
 *
 */

public interface Link extends Top
{
    public static final Attribute attrAPtpID = new NumberAttribute ("aPtpID");
    public static final Attribute attrZPtpID  = new NumberAttribute ("zPtpID");

   static final Attribute []  allAttr = {attrAPtpID, attrZPtpID };

}




