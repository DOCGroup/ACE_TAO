package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;

import com.adventnet.snmp.studio.api.*;
import com.adventnet.utils.*;
import java.util.*;
import com.adventnet.snmp.beans.*;
import  java.applet.* ;


public class SyncIfTable
{
	//<Begin_Variable_Declarations>
	Communication comm = null;
	//<End_Variable_Declarations>

  
/**
 * Default constructor to instantiate this class.
 * This constructor can be used to get this class instance for creating any application.
 */
public SyncIfTable()
  {
         //<Begin_SyncIfTable> 
	comm = new Communication("SYNC");
	comm.loadMibs();

  
         //<End_SyncIfTable>
  } 
  
/**
 * Constructor to instantiate an applet instance of this class.
 * This constructor can be used for creating an applet.
 * 
 * @param applet an </code>Applet</code> instance.
 */
public SyncIfTable(Applet applet)
  {
         //<Begin_SyncIfTable_Applet> 
	comm = new Communication(applet,"SYNC");
	comm.loadMibs();

  
         //<End_SyncIfTable_Applet>
  } 
  
/**
 * Adds the LogClient reference to this class to redirect the log message.
 * Application using this class instance if needs to catch the log/err/dbg message has
 * to implement </code>LogManager</code> interface and pass it as argument through this 
 * method to set in this class instance.
 *
 * @param client a </code>LogInterface</code> reference.
 */
public void addLogClient(LogInterface client)
  {
         //<Begin_addLogClient_LogInterface> 
	comm.addLogClient(client);
  
         //<End_addLogClient_LogInterface>
  } 
  
/**
 * Add a listener to the list that's notified each time when ever a change is made to the 
 * SNMP Table occurs.
 * Application using this class instance if needs to be informed about any table change has to implement
 * </code>SnmpTableListener</code> interface and pass it as argument through this method
 * to set in this class instance as a listener.
 * 
 * @param stl a </code>SnmpTableListener</code> instance reference.
 */
public void addSnmpTableListener(SnmpTableListener stl)
  {
//<Begin_addSnmpTableListener_SnmpTableListener> 
comm.addSnmpTableListener(stl);
  
//<End_addSnmpTableListener_SnmpTableListener>
  } 
  
/**
 * Removes the SnmpTablelistener instance from the list of listeners.
 * 
 * @param stl a </code>SnmpTableListener</code> instance reference.
 */
public void removeSnmpTableListener(SnmpTableListener stl)
  {
//<Begin_removeSnmpTableListener_SnmpTableListener> 
comm.removeSnmpTableListener(stl);
  
//<End_removeSnmpTableListener_SnmpTableListener>
  } 
  
/**
 * Gets the response from ifTable for the corresponding row index.
 * 
 * @param row an </code>int</code> value specifying the row index of the table.
 * 
 * @return a </code>String[]</code> for the row values of the table.
 */
public String[] getIfTableRow(int row)
  {
//<Begin_getIfTableRow_int> 
return comm.getRow(".1.3.6.1.2.1.2.2", row);
  
//<End_getIfTableRow_int>
  } 
  
/**
 * Returns the ifDescr column values.
 * 
 * @return the response for ifDescr in the form of </code>String[]</code> .
 */
public String[] getIfDescr()
  {
//<Begin_getIfDescr> 
return comm.getColumn("ifDescr");
  
//<End_getIfDescr>
  } 
  
/**
 * Returns the ifInUnknownProtos column values.
 * 
 * @return the response for ifInUnknownProtos in the form of </code>String[]</code> .
 */
public String[] getIfInUnknownProtos()
  {
//<Begin_getIfInUnknownProtos> 
return comm.getColumn("ifInUnknownProtos");
  
//<End_getIfInUnknownProtos>
  } 
  
/**
 * Returns the ifInUnknownProtos column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifInUnknownProtoscolumn.
 */
public long[] getIfInUnknownProtosValue()
  {
//<Begin_getIfInUnknownProtosValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.15");
  
//<End_getIfInUnknownProtosValue>
  } 
  
/**
 * Returns the ifInOctets column values.
 * 
 * @return the response for ifInOctets in the form of </code>String[]</code> .
 */
public String[] getIfInOctets()
  {
//<Begin_getIfInOctets> 
return comm.getColumn("ifInOctets");
  
//<End_getIfInOctets>
  } 
  
/**
 * Returns the ifInOctets column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifInOctetscolumn.
 */
public long[] getIfInOctetsValue()
  {
//<Begin_getIfInOctetsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.10");
  
//<End_getIfInOctetsValue>
  } 
  
/**
 * Returns the ifOutErrors column values.
 * 
 * @return the response for ifOutErrors in the form of </code>String[]</code> .
 */
public String[] getIfOutErrors()
  {
//<Begin_getIfOutErrors> 
return comm.getColumn("ifOutErrors");
  
//<End_getIfOutErrors>
  } 
  
/**
 * Returns the ifOutErrors column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifOutErrorscolumn.
 */
public long[] getIfOutErrorsValue()
  {
//<Begin_getIfOutErrorsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.20");
  
//<End_getIfOutErrorsValue>
  } 
  
/**
 * Returns the ifType column values.
 * 
 * @return the response for ifType in the form of </code>String[]</code> .
 */
public String[] getIfType()
  {
//<Begin_getIfType> 
return comm.getColumn("ifType");
  
//<End_getIfType>
  } 
  
/** 
 * Returns the ifType column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the ifTypecolumn.
 */
public int[] getIfTypeValue()
  {
//<Begin_getIfTypeValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.2.2.1.3");
  
//<End_getIfTypeValue>
  } 
  
/**
 * Returns the ifSpeed column values.
 * 
 * @return the response for ifSpeed in the form of </code>String[]</code> .
 */
public String[] getIfSpeed()
  {
//<Begin_getIfSpeed> 
return comm.getColumn("ifSpeed");
  
//<End_getIfSpeed>
  } 
  
/**
 * Returns the ifSpeed column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifSpeedcolumn.
 */
public long[] getIfSpeedValue()
  {
//<Begin_getIfSpeedValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.5");
  
//<End_getIfSpeedValue>
  } 
  
/**
 * Returns the ifInUcastPkts column values.
 * 
 * @return the response for ifInUcastPkts in the form of </code>String[]</code> .
 */
public String[] getIfInUcastPkts()
  {
//<Begin_getIfInUcastPkts> 
return comm.getColumn("ifInUcastPkts");
  
//<End_getIfInUcastPkts>
  } 
  
/**
 * Returns the ifInUcastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifInUcastPktscolumn.
 */
public long[] getIfInUcastPktsValue()
  {
//<Begin_getIfInUcastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.11");
  
//<End_getIfInUcastPktsValue>
  } 
  
/**
 * Returns the ifOutQLen column values.
 * 
 * @return the response for ifOutQLen in the form of </code>String[]</code> .
 */
public String[] getIfOutQLen()
  {
//<Begin_getIfOutQLen> 
return comm.getColumn("ifOutQLen");
  
//<End_getIfOutQLen>
  } 
  
/**
 * Returns the ifOutQLen column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifOutQLencolumn.
 */
public long[] getIfOutQLenValue()
  {
//<Begin_getIfOutQLenValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.21");
  
//<End_getIfOutQLenValue>
  } 
  
/**
 * Returns the ifInDiscards column values.
 * 
 * @return the response for ifInDiscards in the form of </code>String[]</code> .
 */
public String[] getIfInDiscards()
  {
//<Begin_getIfInDiscards> 
return comm.getColumn("ifInDiscards");
  
//<End_getIfInDiscards>
  } 
  
/**
 * Returns the ifInDiscards column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifInDiscardscolumn.
 */
public long[] getIfInDiscardsValue()
  {
//<Begin_getIfInDiscardsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.13");
  
//<End_getIfInDiscardsValue>
  } 
  
/**
 * Returns the ifMtu column values.
 * 
 * @return the response for ifMtu in the form of </code>String[]</code> .
 */
public String[] getIfMtu()
  {
//<Begin_getIfMtu> 
return comm.getColumn("ifMtu");
  
//<End_getIfMtu>
  } 
  
/** 
 * Returns the ifMtu column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the ifMtucolumn.
 */
public int[] getIfMtuValue()
  {
//<Begin_getIfMtuValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.2.2.1.4");
  
//<End_getIfMtuValue>
  } 
  
/**
 * Returns the ifOperStatus column values.
 * 
 * @return the response for ifOperStatus in the form of </code>String[]</code> .
 */
public String[] getIfOperStatus()
  {
//<Begin_getIfOperStatus> 
return comm.getColumn("ifOperStatus");
  
//<End_getIfOperStatus>
  } 
  
/** 
 * Returns the ifOperStatus column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the ifOperStatuscolumn.
 */
public int[] getIfOperStatusValue()
  {
//<Begin_getIfOperStatusValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.2.2.1.8");
  
//<End_getIfOperStatusValue>
  } 
  
/**
 * Returns the ifOutOctets column values.
 * 
 * @return the response for ifOutOctets in the form of </code>String[]</code> .
 */
public String[] getIfOutOctets()
  {
//<Begin_getIfOutOctets> 
return comm.getColumn("ifOutOctets");
  
//<End_getIfOutOctets>
  } 
  
/**
 * Returns the ifOutOctets column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifOutOctetscolumn.
 */
public long[] getIfOutOctetsValue()
  {
//<Begin_getIfOutOctetsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.16");
  
//<End_getIfOutOctetsValue>
  } 
  
/**
 * Returns the ifInErrors column values.
 * 
 * @return the response for ifInErrors in the form of </code>String[]</code> .
 */
public String[] getIfInErrors()
  {
//<Begin_getIfInErrors> 
return comm.getColumn("ifInErrors");
  
//<End_getIfInErrors>
  } 
  
/**
 * Returns the ifInErrors column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifInErrorscolumn.
 */
public long[] getIfInErrorsValue()
  {
//<Begin_getIfInErrorsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.14");
  
//<End_getIfInErrorsValue>
  } 
  
/**
 * Returns the ifPhysAddress column values.
 * 
 * @return the response for ifPhysAddress in the form of </code>String[]</code> .
 */
public String[] getIfPhysAddress()
  {
//<Begin_getIfPhysAddress> 
return comm.getColumn("ifPhysAddress");
  
//<End_getIfPhysAddress>
  } 
  
/**
 * Returns the ifOutUcastPkts column values.
 * 
 * @return the response for ifOutUcastPkts in the form of </code>String[]</code> .
 */
public String[] getIfOutUcastPkts()
  {
//<Begin_getIfOutUcastPkts> 
return comm.getColumn("ifOutUcastPkts");
  
//<End_getIfOutUcastPkts>
  } 
  
/**
 * Returns the ifOutUcastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifOutUcastPktscolumn.
 */
public long[] getIfOutUcastPktsValue()
  {
//<Begin_getIfOutUcastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.17");
  
//<End_getIfOutUcastPktsValue>
  } 
  
/**
 * Returns the ifOutDiscards column values.
 * 
 * @return the response for ifOutDiscards in the form of </code>String[]</code> .
 */
public String[] getIfOutDiscards()
  {
//<Begin_getIfOutDiscards> 
return comm.getColumn("ifOutDiscards");
  
//<End_getIfOutDiscards>
  } 
  
/**
 * Returns the ifOutDiscards column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifOutDiscardscolumn.
 */
public long[] getIfOutDiscardsValue()
  {
//<Begin_getIfOutDiscardsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.19");
  
//<End_getIfOutDiscardsValue>
  } 
  
/**
 * Returns the ifLastChange column values.
 * 
 * @return the response for ifLastChange in the form of </code>String[]</code> .
 */
public String[] getIfLastChange()
  {
//<Begin_getIfLastChange> 
return comm.getColumn("ifLastChange");
  
//<End_getIfLastChange>
  } 
  
/**
 * Returns the ifLastChange column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifLastChangecolumn.
 */
public long[] getIfLastChangeValue()
  {
//<Begin_getIfLastChangeValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.9");
  
//<End_getIfLastChangeValue>
  } 
  
/**
 * Returns the ifIndex column values.
 * 
 * @return the response for ifIndex in the form of </code>String[]</code> .
 */
public String[] getIfIndex()
  {
//<Begin_getIfIndex> 
return comm.getColumn("ifIndex");
  
//<End_getIfIndex>
  } 
  
/** 
 * Returns the ifIndex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the ifIndexcolumn.
 */
public int[] getIfIndexValue()
  {
//<Begin_getIfIndexValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.2.2.1.1");
  
//<End_getIfIndexValue>
  } 
  
/**
 * Returns the ifAdminStatus column values.
 * 
 * @return the response for ifAdminStatus in the form of </code>String[]</code> .
 */
public String[] getIfAdminStatus()
  {
//<Begin_getIfAdminStatus> 
return comm.getColumn("ifAdminStatus");
  
//<End_getIfAdminStatus>
  } 
  
/** 
 * Returns the ifAdminStatus column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the ifAdminStatuscolumn.
 */
public int[] getIfAdminStatusValue()
  {
//<Begin_getIfAdminStatusValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.2.2.1.7");
  
//<End_getIfAdminStatusValue>
  } 
  
/**
 * Returns the ifSpecific column values.
 * 
 * @return the response for ifSpecific in the form of </code>String[]</code> .
 */
public String[] getIfSpecific()
  {
//<Begin_getIfSpecific> 
return comm.getColumn("ifSpecific");
  
//<End_getIfSpecific>
  } 
  
/**
 * Returns the ifOutNUcastPkts column values.
 * 
 * @return the response for ifOutNUcastPkts in the form of </code>String[]</code> .
 */
public String[] getIfOutNUcastPkts()
  {
//<Begin_getIfOutNUcastPkts> 
return comm.getColumn("ifOutNUcastPkts");
  
//<End_getIfOutNUcastPkts>
  } 
  
/**
 * Returns the ifOutNUcastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifOutNUcastPktscolumn.
 */
public long[] getIfOutNUcastPktsValue()
  {
//<Begin_getIfOutNUcastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.18");
  
//<End_getIfOutNUcastPktsValue>
  } 
  
/**
 * Returns the ifInNUcastPkts column values.
 * 
 * @return the response for ifInNUcastPkts in the form of </code>String[]</code> .
 */
public String[] getIfInNUcastPkts()
  {
//<Begin_getIfInNUcastPkts> 
return comm.getColumn("ifInNUcastPkts");
  
//<End_getIfInNUcastPkts>
  } 
  
/**
 * Returns the ifInNUcastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifInNUcastPktscolumn.
 */
public long[] getIfInNUcastPktsValue()
  {
//<Begin_getIfInNUcastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.2.2.1.12");
  
//<End_getIfInNUcastPktsValue>
  } 
  
/**
 * Returns the columnNames of the table.
 * 
 * @return the columnnames in </code>String[]</code> form.
 */
public String[] getIfTableColumns()
  {
//<Begin_getIfTableColumns> 
return comm.getColumnNames(".1.3.6.1.2.1.2.2");
  
//<End_getIfTableColumns>
  } 
  
/**
 * Returns the response for the whole ifTable.
 * 
 * @return the response in </code>String[]</code> form.
 */
public String[][] getIfTable()
  {
//<Begin_getIfTable> 
return comm.getTable(".1.3.6.1.2.1.2.2");
  
//<End_getIfTable>
  } 
  
/**
 * Sends the request for ifTable Table OID which will update for any table change event.
 * To use this method in the application, first the application has to implement SnmpTableListener
 * and then register itself for receiving all fired table change events by implementing
 * the methods of SnmpTableListener.
 */
public void fetchIfTable()
  {
//<Begin_fetchIfTable> 
comm.fetchTableData(".1.3.6.1.2.1.2.2");
  
//<End_fetchIfTable>
  } 
  
/**
 * Set the AgentParams instance to this class.
 * AgentParams is the class instance which contains all agent related parameters.So before 
 * sending any query this instance should be set in this class using setAgentParams method.
 * 
 * @param ap an </code>AgentParams</code> class instance
 *
 * @see AgentParams
 */
public void setAgentParams(AgentParams ap)
  {
         //<Begin_setAgentParams_AgentParams> 
	comm.setAgentParams(ap);

  
         //<End_setAgentParams_AgentParams>
  } 
  
/**
 * Returns a Communication instance reference.
 * This method will be used for getting acces to the advanced SnmpOperations which can be
 * done using Communication reference.
 * 
 * @return a </code>Communication</code> class reference.
 */
public Communication getCommunication()
  {
         //<Begin_getCommunication> 
	return comm;

  
         //<End_getCommunication>
  } 
  
/**
 * Returns the error string associated with the request if the request fails or times out.
 * This method should be called in the application if response for any request comes to be null
 * 
 * @return a </code>String</code> value specifying the error message.
 */
public String getErrorString()
  {
         //<Begin_getErrorString> 
	return comm.getErrorString();

  
         //<End_getErrorString>
  } 
  
/**
 * Determines whether debug output will be printed.
 * Setting true in this method will print the debug message in the console.
 *
 * @param debug a </code>boolean</code> value, which will decide whether the debug message
 * has to be printed or not.
 */
public void setDebug(boolean debug) 
  {
         //<Begin_setDebug_boolean> 
	comm.setDebug(debug);
  
         //<End_setDebug_boolean>
  } 
  
/**
 * Closes all existing class resources.
 * This method should be used before closing any application.
 */
public void closeResources() 
  {
         //<Begin_closeResources> 
	comm.closeResources();
  
         //<End_closeResources>
  } 

} 
