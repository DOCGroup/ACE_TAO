//$Id: L3Interface.java,
/*
 * L3Interface.java - class for LSR (label swicthed routers)
 *
 * The DARPA ARMS PROGRAM
 * This software is governed by Government Purpose Rights
 * Contract No: NBCHC030132
 * Telcordia Technologies, Inc.
 * 1 Technology Drive
 * Piscataway, NJ 08854-4157
 *
 *
 */


package mil.darpa.arms.mlrm.BandwidthBroker.bb.server.db;

/**
 * This class represents L3Interface
 *
 */

public interface L3Interface extends CTP
{
   public static final Attribute attrIPAddress = new StringAttribute ("ipAddress");
   public static final Attribute attrIPMask = new StringAttribute ("ipMask");
   public static final Attribute attrVlanID = new NumberAttribute ("vlanID");
   public static final Attribute attrPTPID = new NumberAttribute ("ptpID");
   public static final Attribute attrLinkID = new NumberAttribute ("linkID");
   public static final Attribute attrRouterID = new NumberAttribute ("routerID");
   static final Attribute []  allAttr = {attrIPAddress, attrIPMask, 
                                         attrVlanID, attrPTPID, 
					 attrLinkID, attrRouterID};

}
