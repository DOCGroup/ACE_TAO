package mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer;

import com.adventnet.snmp.studio.api.*;
import com.adventnet.utils.*;
import java.util.*;
import com.adventnet.snmp.beans.*;
import  java.applet.* ;
import com.adventnet.snmp.ui.SnmpTableModel;


public class SyncIfXTable
{
	//<Begin_Variable_Declarations>
	Communication comm = null;
	//<End_Variable_Declarations>

  
/**
 * Default constructor to instantiate this class.
 * This constructor can be used to get this class instance for creating any application.
 */
public SyncIfXTable()
  {
         //<Begin_SyncIfXTable> 
	comm = new Communication("SYNC");
	comm.loadMibs();

  
         //<End_SyncIfXTable>
  } 
  
/**
 * Constructor to instantiate an applet instance of this class.
 * This constructor can be used for creating an applet.
 * 
 * @param applet an </code>Applet</code> instance.
 */
public SyncIfXTable(Applet applet)
  {
         //<Begin_SyncIfXTable_Applet> 
	comm = new Communication(applet,"SYNC");
	comm.loadMibs();

  
         //<End_SyncIfXTable_Applet>
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
 * Gets the response from ifXTable for the corresponding row index.
 * 
 * @param row an </code>int</code> value specifying the row index of the table.
 * 
 * @return a </code>String[]</code> for the row values of the table.
 */
public String[] getIfXTableRow(int row)
  {
//<Begin_getIfXTableRow_int> 
return comm.getRow(".1.3.6.1.2.1.31.1.1", row);
  
//<End_getIfXTableRow_int>
  } 
  
/**
 * Returns the ifName column values.
 * 
 * @return the response for ifName in the form of </code>String[]</code> .
 */
public String[] getIfName()
  {
//<Begin_getIfName> 
return comm.getColumn("ifName");
  
//<End_getIfName>
  } 
  
/**
 * Returns the ifHCOutUcastPkts column values.
 * 
 * @return the response for ifHCOutUcastPkts in the form of </code>String[]</code> .
 */
public String[] getIfHCOutUcastPkts()
  {
//<Begin_getIfHCOutUcastPkts> 
return comm.getColumn("ifHCOutUcastPkts");
  
//<End_getIfHCOutUcastPkts>
  } 
  
/**
 * Returns the ifHCOutUcastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifHCOutUcastPktscolumn.
 */
public long[] getIfHCOutUcastPktsValue()
  {
//<Begin_getIfHCOutUcastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.11");
  
//<End_getIfHCOutUcastPktsValue>
  } 
  
/**
 * Returns the ifAlias column values.
 * 
 * @return the response for ifAlias in the form of </code>String[]</code> .
 */
public String[] getIfAlias()
  {
//<Begin_getIfAlias> 
return comm.getColumn("ifAlias");
  
//<End_getIfAlias>
  } 
  
/**
 * Returns the ifHCInOctets column values.
 * 
 * @return the response for ifHCInOctets in the form of </code>String[]</code> .
 */
public String[] getIfHCInOctets()
  {
//<Begin_getIfHCInOctets> 
return comm.getColumn("ifHCInOctets");
  
//<End_getIfHCInOctets>
  } 
  
/**
 * Returns the ifHCInOctets column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifHCInOctetscolumn.
 */
public long[] getIfHCInOctetsValue()
  {
//<Begin_getIfHCInOctetsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.6");
  
//<End_getIfHCInOctetsValue>
  } 
  
/**
 * Returns the ifHighSpeed column values.
 * 
 * @return the response for ifHighSpeed in the form of </code>String[]</code> .
 */
public String[] getIfHighSpeed()
  {
//<Begin_getIfHighSpeed> 
return comm.getColumn("ifHighSpeed");
  
//<End_getIfHighSpeed>
  } 
  
/**
 * Returns the ifHighSpeed column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifHighSpeedcolumn.
 */
public long[] getIfHighSpeedValue()
  {
//<Begin_getIfHighSpeedValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.15");
  
//<End_getIfHighSpeedValue>
  } 
  
/**
 * Returns the ifOutMulticastPkts column values.
 * 
 * @return the response for ifOutMulticastPkts in the form of </code>String[]</code> .
 */
public String[] getIfOutMulticastPkts()
  {
//<Begin_getIfOutMulticastPkts> 
return comm.getColumn("ifOutMulticastPkts");
  
//<End_getIfOutMulticastPkts>
  } 
  
/**
 * Returns the ifOutMulticastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifOutMulticastPktscolumn.
 */
public long[] getIfOutMulticastPktsValue()
  {
//<Begin_getIfOutMulticastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.4");
  
//<End_getIfOutMulticastPktsValue>
  } 
  
/**
 * Returns the ifInBroadcastPkts column values.
 * 
 * @return the response for ifInBroadcastPkts in the form of </code>String[]</code> .
 */
public String[] getIfInBroadcastPkts()
  {
//<Begin_getIfInBroadcastPkts> 
return comm.getColumn("ifInBroadcastPkts");
  
//<End_getIfInBroadcastPkts>
  } 
  
/**
 * Returns the ifInBroadcastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifInBroadcastPktscolumn.
 */
public long[] getIfInBroadcastPktsValue()
  {
//<Begin_getIfInBroadcastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.3");
  
//<End_getIfInBroadcastPktsValue>
  } 
  
/**
 * Returns the ifHCOutBroadcastPkts column values.
 * 
 * @return the response for ifHCOutBroadcastPkts in the form of </code>String[]</code> .
 */
public String[] getIfHCOutBroadcastPkts()
  {
//<Begin_getIfHCOutBroadcastPkts> 
return comm.getColumn("ifHCOutBroadcastPkts");
  
//<End_getIfHCOutBroadcastPkts>
  } 
  
/**
 * Returns the ifHCOutBroadcastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifHCOutBroadcastPktscolumn.
 */
public long[] getIfHCOutBroadcastPktsValue()
  {
//<Begin_getIfHCOutBroadcastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.13");
  
//<End_getIfHCOutBroadcastPktsValue>
  } 
  
/**
 * Returns the ifHCInMulticastPkts column values.
 * 
 * @return the response for ifHCInMulticastPkts in the form of </code>String[]</code> .
 */
public String[] getIfHCInMulticastPkts()
  {
//<Begin_getIfHCInMulticastPkts> 
return comm.getColumn("ifHCInMulticastPkts");
  
//<End_getIfHCInMulticastPkts>
  } 
  
/**
 * Returns the ifHCInMulticastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifHCInMulticastPktscolumn.
 */
public long[] getIfHCInMulticastPktsValue()
  {
//<Begin_getIfHCInMulticastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.8");
  
//<End_getIfHCInMulticastPktsValue>
  } 
  
/**
 * Returns the ifHCOutOctets column values.
 * 
 * @return the response for ifHCOutOctets in the form of </code>String[]</code> .
 */
public String[] getIfHCOutOctets()
  {
//<Begin_getIfHCOutOctets> 
return comm.getColumn("ifHCOutOctets");
  
//<End_getIfHCOutOctets>
  } 
  
/**
 * Returns the ifHCOutOctets column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifHCOutOctetscolumn.
 */
public long[] getIfHCOutOctetsValue()
  {
//<Begin_getIfHCOutOctetsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.10");
  
//<End_getIfHCOutOctetsValue>
  } 
  
/**
 * Returns the ifHCInUcastPkts column values.
 * 
 * @return the response for ifHCInUcastPkts in the form of </code>String[]</code> .
 */
public String[] getIfHCInUcastPkts()
  {
//<Begin_getIfHCInUcastPkts> 
return comm.getColumn("ifHCInUcastPkts");
  
//<End_getIfHCInUcastPkts>
  } 
  
/**
 * Returns the ifHCInUcastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifHCInUcastPktscolumn.
 */
public long[] getIfHCInUcastPktsValue()
  {
//<Begin_getIfHCInUcastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.7");
  
//<End_getIfHCInUcastPktsValue>
  } 
  
/**
 * Returns the ifConnectorPresent column values.
 * 
 * @return the response for ifConnectorPresent in the form of </code>String[]</code> .
 */
public String[] getIfConnectorPresent()
  {
//<Begin_getIfConnectorPresent> 
return comm.getColumn("ifConnectorPresent");
  
//<End_getIfConnectorPresent>
  } 
  
/** 
 * Returns the ifConnectorPresent column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the ifConnectorPresentcolumn.
 */
public int[] getIfConnectorPresentValue()
  {
//<Begin_getIfConnectorPresentValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.31.1.1.1.17");
  
//<End_getIfConnectorPresentValue>
  } 
  
/**
 * Returns the ifLinkUpDownTrapEnable column values.
 * 
 * @return the response for ifLinkUpDownTrapEnable in the form of </code>String[]</code> .
 */
public String[] getIfLinkUpDownTrapEnable()
  {
//<Begin_getIfLinkUpDownTrapEnable> 
return comm.getColumn("ifLinkUpDownTrapEnable");
  
//<End_getIfLinkUpDownTrapEnable>
  } 
  
/** 
 * Returns the ifLinkUpDownTrapEnable column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the ifLinkUpDownTrapEnablecolumn.
 */
public int[] getIfLinkUpDownTrapEnableValue()
  {
//<Begin_getIfLinkUpDownTrapEnableValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.31.1.1.1.14");
  
//<End_getIfLinkUpDownTrapEnableValue>
  } 
  
/**
 * Returns the ifOutBroadcastPkts column values.
 * 
 * @return the response for ifOutBroadcastPkts in the form of </code>String[]</code> .
 */
public String[] getIfOutBroadcastPkts()
  {
//<Begin_getIfOutBroadcastPkts> 
return comm.getColumn("ifOutBroadcastPkts");
  
//<End_getIfOutBroadcastPkts>
  } 
  
/**
 * Returns the ifOutBroadcastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifOutBroadcastPktscolumn.
 */
public long[] getIfOutBroadcastPktsValue()
  {
//<Begin_getIfOutBroadcastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.5");
  
//<End_getIfOutBroadcastPktsValue>
  } 
  
/**
 * Returns the ifHCInBroadcastPkts column values.
 * 
 * @return the response for ifHCInBroadcastPkts in the form of </code>String[]</code> .
 */
public String[] getIfHCInBroadcastPkts()
  {
//<Begin_getIfHCInBroadcastPkts> 
return comm.getColumn("ifHCInBroadcastPkts");
  
//<End_getIfHCInBroadcastPkts>
  } 
  
/**
 * Returns the ifHCInBroadcastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifHCInBroadcastPktscolumn.
 */
public long[] getIfHCInBroadcastPktsValue()
  {
//<Begin_getIfHCInBroadcastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.9");
  
//<End_getIfHCInBroadcastPktsValue>
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
 * Returns the ifInMulticastPkts column values.
 * 
 * @return the response for ifInMulticastPkts in the form of </code>String[]</code> .
 */
public String[] getIfInMulticastPkts()
  {
//<Begin_getIfInMulticastPkts> 
return comm.getColumn("ifInMulticastPkts");
  
//<End_getIfInMulticastPkts>
  } 
  
/**
 * Returns the ifInMulticastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifInMulticastPktscolumn.
 */
public long[] getIfInMulticastPktsValue()
  {
//<Begin_getIfInMulticastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.2");
  
//<End_getIfInMulticastPktsValue>
  } 
  
/**
 * Returns the ifHCOutMulticastPkts column values.
 * 
 * @return the response for ifHCOutMulticastPkts in the form of </code>String[]</code> .
 */
public String[] getIfHCOutMulticastPkts()
  {
//<Begin_getIfHCOutMulticastPkts> 
return comm.getColumn("ifHCOutMulticastPkts");
  
//<End_getIfHCOutMulticastPkts>
  } 
  
/**
 * Returns the ifHCOutMulticastPkts column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the ifHCOutMulticastPktscolumn.
 */
public long[] getIfHCOutMulticastPktsValue()
  {
//<Begin_getIfHCOutMulticastPktsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.31.1.1.1.12");
  
//<End_getIfHCOutMulticastPktsValue>
  } 
  
/**
 * Returns the ifPromiscuousMode column values.
 * 
 * @return the response for ifPromiscuousMode in the form of </code>String[]</code> .
 */
public String[] getIfPromiscuousMode()
  {
//<Begin_getIfPromiscuousMode> 
return comm.getColumn("ifPromiscuousMode");
  
//<End_getIfPromiscuousMode>
  } 
  
/** 
 * Returns the ifPromiscuousMode column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the ifPromiscuousModecolumn.
 */
public int[] getIfPromiscuousModeValue()
  {
//<Begin_getIfPromiscuousModeValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.31.1.1.1.16");
  
//<End_getIfPromiscuousModeValue>
  } 
  
/**
 * Returns the columnNames of the table.
 * 
 * @return the columnnames in </code>String[]</code> form.
 */
public String[] getIfXTableColumns()
  {
//<Begin_getIfXTableColumns> 
return comm.getColumnNames(".1.3.6.1.2.1.31.1.1");
  
//<End_getIfXTableColumns>
  } 
  
/**
 * Returns the response for the whole ifXTable.
 * 
 * @return the response in </code>String[]</code> form.
 */
public String[][] getIfXTable()
  {
//<Begin_getIfXTable> 
return comm.getTable(".1.3.6.1.2.1.31.1.1");
  
//<End_getIfXTable>
  } 
  
/**
 * Sends the request for ifXTable Table OID which will update for any table change event.
 * To use this method in the application, first the application has to implement SnmpTableListener
 * and then register itself for receiving all fired table change events by implementing
 * the methods of SnmpTableListener.
 */
public void fetchIfXTable()
  {
//<Begin_fetchIfXTable> 
comm.fetchTableData(".1.3.6.1.2.1.31.1.1");
  
//<End_fetchIfXTable>
  } 
  
/**
 * Returns the ifXTable Table model.
 * This method returns a tablemodel instance which can be used for building SnmpTable related
 * ui applications.
 * 
 * @return a </code>SnmpTableModel</code> instance.
 */
public SnmpTableModel getIfXTableModel()
  {
//<Begin_getIfXTableModel> 
return comm.getTableModel(".1.3.6.1.2.1.31.1.1");
  
//<End_getIfXTableModel>
  } 
  
/**
 * Returns the Table model for the columnar OID.
 * This method returns a tablemodel instance which can be used for building SnmpTable related
 * ui applications for specific columns.
 * 
 * @return a </code>SnmpTableModel</code> instance.
 */
public SnmpTableModel getTableModel(String oid)
  {
//<Begin_getTableModel_String> 
return comm.getTableModel(".1.3.6.1.2.1.31.1.1");
  
//<End_getTableModel_String>
  } 
  
/**
 * Returns the TableModel for the passed columnar oids.
 * 
 * @param oid a </code>String[]</code> for the list of column oids. SnmpTableModel has to be 
 * constructed based on these column oids
 *
 * @return a </code>SnmpTableModel</code> instance.
 */
public SnmpTableModel getIfXTableColumnModel(String[] oid)
  {
//<Begin_getIfXTableColumnModel_String[]> 
return comm.getColumnModel(oid);
  
//<End_getIfXTableColumnModel_String[]>
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
