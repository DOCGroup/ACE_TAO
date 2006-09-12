//$Id: FaultSubscriptions.java


package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

/**
 * This class represents FaultSubscriptions
 *
 */

public interface FaultSubscriptions extends Top
{
    public static final Attribute attrEventSinkName = new StringAttribute ("eventSinkName");
    public static final Attribute attrEventSinkType = new NumberAttribute ("eventSinkType");
    public static final Attribute attrCookie = new StringAttribute ("cookie");
    public static final Attribute attrSwitchID  = new NumberAttribute ("switchID");
    public static final Attribute attrEventType = new StringAttribute ("eventType");

   static final Attribute []  allAttr = {attrEventSinkName, attrEventSinkType, attrCookie, attrSwitchID, attrEventType};

}




