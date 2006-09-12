package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;

import java.util.*;
import com.adventnet.snmp.beans.*;
import com.adventnet.snmp.mibs.*;
import com.adventnet.snmp.snmp2.*;

public class SnmpVlanTable
{


   SnmpTarget target = null;
   private static SnmpOID rootOid = new SnmpOID(".1.3.6.1.4.1.9.9.46.1.3.1.1.2");
   private static boolean DEBUG = false;

    public SnmpVlanTable (String host, String community) {

	try {

	    target = new SnmpTarget();
	    target.setTargetHost(host);
	    target.setCommunity(community);
	    target.loadMibs("./mibs/IF-MIB ./mibs/IANAifType-MIB ./mibs/RFC1213-MIB ./mibs/SNMPv2-MIB ./mibs/CISCO-VTP-MIB ./mibs/CISCO-SMI ./mibs/SNMP-FRAMEWORK-MIB");
	    target.setSnmpOID(rootOid);
	} catch (Exception e) {
	    System.out.println("Error setting up VTP Vlan Table");
	    System.out.println(target.getErrorString());
	    e.printStackTrace();

	}

    }


    public String[] getVlans() {
	
	ArrayList vlans = new ArrayList();

	try {

	    while (true) {
		SnmpVarBind result = target.snmpGetNextVariableBinding();


		SnmpOID oid = result.getObjectID();

		if (!SnmpTarget.isInSubTree(rootOid, oid))
		    break;

		// now the VLAN id is the last in the index
		int [] oids = oid.toIntArray();
		vlans.add(String.valueOf(oids[oids.length-1]));
		if (DEBUG) {
		    System.out.println("OID: Value --> " + result.toString());
		    System.out.println("Vlan ID is: " + oids[oids.length - 1]);
		}
	    }


	    return ((String[])vlans.toArray(new String[] {}));


	} catch (Exception e) {
	    return null;
	}
    }

    public String getErrorString() {
	return (target.getErrorString());
    }

    public void closeResources() {
	target.releaseResources();
    }


	



} 
