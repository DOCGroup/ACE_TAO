package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;

import com.adventnet.snmp.studio.api.*;
import com.adventnet.utils.*;
import java.util.*;
import com.adventnet.snmp.beans.*;
import  java.applet.* ;


public class SyncVtpVlanTable
{
	//<Begin_Variable_Declarations>
	Communication comm = null;
	//<End_Variable_Declarations>

  
/**
 * Default constructor to instantiate this class.
 * This constructor can be used to get this class instance for creating any application.
 */
public SyncVtpVlanTable()
  {
         //<Begin_SyncVtpVlanTable> 
	comm = new Communication("SYNC");
	comm.loadMibs();

  
         //<End_SyncVtpVlanTable>
  } 
  
/**
 * Constructor to instantiate an applet instance of this class.
 * This constructor can be used for creating an applet.
 * 
 * @param applet an </code>Applet</code> instance.
 */
public SyncVtpVlanTable(Applet applet)
  {
         //<Begin_SyncVtpVlanTable_Applet> 
	comm = new Communication(applet,"SYNC");
	comm.loadMibs();

  
         //<End_SyncVtpVlanTable_Applet>
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
 * Gets the response from vtpVlanTable for the corresponding row index.
 * 
 * @param row an </code>int</code> value specifying the row index of the table.
 * 
 * @return a </code>String[]</code> for the row values of the table.
 */
public String[] getVtpVlanTableRow(int row)
  {
//<Begin_getVtpVlanTableRow_int> 
return comm.getRow(".1.3.6.1.4.1.9.9.46.1.3.1", row);
  
//<End_getVtpVlanTableRow_int>
  } 
  
/**
 * Returns the vtpVlanSteHopCount column values.
 * 
 * @return the response for vtpVlanSteHopCount in the form of </code>String[]</code> .
 */
public String[] getVtpVlanSteHopCount()
  {
//<Begin_getVtpVlanSteHopCount> 
return comm.getColumn("vtpVlanSteHopCount");
  
//<End_getVtpVlanSteHopCount>
  } 
  
/** 
 * Returns the vtpVlanSteHopCount column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanSteHopCountcolumn.
 */
public int[] getVtpVlanSteHopCountValue()
  {
//<Begin_getVtpVlanSteHopCountValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.15");
  
//<End_getVtpVlanSteHopCountValue>
  } 
  
/**
 * Returns the vtpVlanTranslationalVlan2 column values.
 * 
 * @return the response for vtpVlanTranslationalVlan2 in the form of </code>String[]</code> .
 */
public String[] getVtpVlanTranslationalVlan2()
  {
//<Begin_getVtpVlanTranslationalVlan2> 
return comm.getColumn("vtpVlanTranslationalVlan2");
  
//<End_getVtpVlanTranslationalVlan2>
  } 
  
/** 
 * Returns the vtpVlanTranslationalVlan2 column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanTranslationalVlan2column.
 */
public int[] getVtpVlanTranslationalVlan2Value()
  {
//<Begin_getVtpVlanTranslationalVlan2Value> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.12");
  
//<End_getVtpVlanTranslationalVlan2Value>
  } 
  
/**
 * Returns the vtpVlanTranslationalVlan1 column values.
 * 
 * @return the response for vtpVlanTranslationalVlan1 in the form of </code>String[]</code> .
 */
public String[] getVtpVlanTranslationalVlan1()
  {
//<Begin_getVtpVlanTranslationalVlan1> 
return comm.getColumn("vtpVlanTranslationalVlan1");
  
//<End_getVtpVlanTranslationalVlan1>
  } 
  
/** 
 * Returns the vtpVlanTranslationalVlan1 column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanTranslationalVlan1column.
 */
public int[] getVtpVlanTranslationalVlan1Value()
  {
//<Begin_getVtpVlanTranslationalVlan1Value> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.11");
  
//<End_getVtpVlanTranslationalVlan1Value>
  } 
  
/**
 * Returns the managementDomainIndex column values.
 * 
 * @return the response for managementDomainIndex in the form of </code>String[]</code> .
 */
public String[] getManagementDomainIndex()
  {
//<Begin_getManagementDomainIndex> 
return comm.getColumn("managementDomainIndex");
  
//<End_getManagementDomainIndex>
  } 
  
/** 
 * Returns the managementDomainIndex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the managementDomainIndexcolumn.
 */
public int[] getManagementDomainIndexValue()
  {
//<Begin_getManagementDomainIndexValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.2.1.1.1");
  
//<End_getManagementDomainIndexValue>
  } 
  
/**
 * Returns the vtpVlanBridgeType column values.
 * 
 * @return the response for vtpVlanBridgeType in the form of </code>String[]</code> .
 */
public String[] getVtpVlanBridgeType()
  {
//<Begin_getVtpVlanBridgeType> 
return comm.getColumn("vtpVlanBridgeType");
  
//<End_getVtpVlanBridgeType>
  } 
  
/** 
 * Returns the vtpVlanBridgeType column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanBridgeTypecolumn.
 */
public int[] getVtpVlanBridgeTypeValue()
  {
//<Begin_getVtpVlanBridgeTypeValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.13");
  
//<End_getVtpVlanBridgeTypeValue>
  } 
  
/**
 * Returns the vtpVlanBridgeNumber column values.
 * 
 * @return the response for vtpVlanBridgeNumber in the form of </code>String[]</code> .
 */
public String[] getVtpVlanBridgeNumber()
  {
//<Begin_getVtpVlanBridgeNumber> 
return comm.getColumn("vtpVlanBridgeNumber");
  
//<End_getVtpVlanBridgeNumber>
  } 
  
/** 
 * Returns the vtpVlanBridgeNumber column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanBridgeNumbercolumn.
 */
public int[] getVtpVlanBridgeNumberValue()
  {
//<Begin_getVtpVlanBridgeNumberValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.8");
  
//<End_getVtpVlanBridgeNumberValue>
  } 
  
/**
 * Returns the vtpVlanState column values.
 * 
 * @return the response for vtpVlanState in the form of </code>String[]</code> .
 */
public String[] getVtpVlanState()
  {
//<Begin_getVtpVlanState> 
return comm.getColumn("vtpVlanState");
  
//<End_getVtpVlanState>
  } 
  
/** 
 * Returns the vtpVlanState column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanStatecolumn.
 */
public int[] getVtpVlanStateValue()
  {
//<Begin_getVtpVlanStateValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.2");
  
//<End_getVtpVlanStateValue>
  } 
  
/**
 * Returns the vtpVlanName column values.
 * 
 * @return the response for vtpVlanName in the form of </code>String[]</code> .
 */
public String[] getVtpVlanName()
  {
//<Begin_getVtpVlanName> 
return comm.getColumn("vtpVlanName");
  
//<End_getVtpVlanName>
  } 
  
/**
 * Returns the vtpVlanIfIndex column values.
 * 
 * @return the response for vtpVlanIfIndex in the form of </code>String[]</code> .
 */
public String[] getVtpVlanIfIndex()
  {
//<Begin_getVtpVlanIfIndex> 
return comm.getColumn("vtpVlanIfIndex");
  
//<End_getVtpVlanIfIndex>
  } 
  
/** 
 * Returns the vtpVlanIfIndex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanIfIndexcolumn.
 */
public int[] getVtpVlanIfIndexValue()
  {
//<Begin_getVtpVlanIfIndexValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.18");
  
//<End_getVtpVlanIfIndexValue>
  } 
  
/**
 * Returns the vtpVlanIndex column values.
 * 
 * @return the response for vtpVlanIndex in the form of </code>String[]</code> .
 */
public String[] getVtpVlanIndex()
  {
//<Begin_getVtpVlanIndex> 
return comm.getColumn("vtpVlanIndex");
  
//<End_getVtpVlanIndex>
  } 
  
/** 
 * Returns the vtpVlanIndex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanIndexcolumn.
 */
public int[] getVtpVlanIndexValue()
  {
//<Begin_getVtpVlanIndexValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.1");
  
//<End_getVtpVlanIndexValue>
  } 
  
/**
 * Returns the vtpVlanDot10Said column values.
 * 
 * @return the response for vtpVlanDot10Said in the form of </code>String[]</code> .
 */
public String[] getVtpVlanDot10Said()
  {
//<Begin_getVtpVlanDot10Said> 
return comm.getColumn("vtpVlanDot10Said");
  
//<End_getVtpVlanDot10Said>
  } 
  
/**
 * Returns the vtpVlanStpType column values.
 * 
 * @return the response for vtpVlanStpType in the form of </code>String[]</code> .
 */
public String[] getVtpVlanStpType()
  {
//<Begin_getVtpVlanStpType> 
return comm.getColumn("vtpVlanStpType");
  
//<End_getVtpVlanStpType>
  } 
  
/** 
 * Returns the vtpVlanStpType column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanStpTypecolumn.
 */
public int[] getVtpVlanStpTypeValue()
  {
//<Begin_getVtpVlanStpTypeValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.9");
  
//<End_getVtpVlanStpTypeValue>
  } 
  
/**
 * Returns the vtpVlanType column values.
 * 
 * @return the response for vtpVlanType in the form of </code>String[]</code> .
 */
public String[] getVtpVlanType()
  {
//<Begin_getVtpVlanType> 
return comm.getColumn("vtpVlanType");
  
//<End_getVtpVlanType>
  } 
  
/** 
 * Returns the vtpVlanType column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanTypecolumn.
 */
public int[] getVtpVlanTypeValue()
  {
//<Begin_getVtpVlanTypeValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.3");
  
//<End_getVtpVlanTypeValue>
  } 
  
/**
 * Returns the vtpVlanMtu column values.
 * 
 * @return the response for vtpVlanMtu in the form of </code>String[]</code> .
 */
public String[] getVtpVlanMtu()
  {
//<Begin_getVtpVlanMtu> 
return comm.getColumn("vtpVlanMtu");
  
//<End_getVtpVlanMtu>
  } 
  
/** 
 * Returns the vtpVlanMtu column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanMtucolumn.
 */
public int[] getVtpVlanMtuValue()
  {
//<Begin_getVtpVlanMtuValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.5");
  
//<End_getVtpVlanMtuValue>
  } 
  
/**
 * Returns the vtpVlanAreHopCount column values.
 * 
 * @return the response for vtpVlanAreHopCount in the form of </code>String[]</code> .
 */
public String[] getVtpVlanAreHopCount()
  {
//<Begin_getVtpVlanAreHopCount> 
return comm.getColumn("vtpVlanAreHopCount");
  
//<End_getVtpVlanAreHopCount>
  } 
  
/** 
 * Returns the vtpVlanAreHopCount column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanAreHopCountcolumn.
 */
public int[] getVtpVlanAreHopCountValue()
  {
//<Begin_getVtpVlanAreHopCountValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.14");
  
//<End_getVtpVlanAreHopCountValue>
  } 
  
/**
 * Returns the vtpVlanParentVlan column values.
 * 
 * @return the response for vtpVlanParentVlan in the form of </code>String[]</code> .
 */
public String[] getVtpVlanParentVlan()
  {
//<Begin_getVtpVlanParentVlan> 
return comm.getColumn("vtpVlanParentVlan");
  
//<End_getVtpVlanParentVlan>
  } 
  
/** 
 * Returns the vtpVlanParentVlan column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanParentVlancolumn.
 */
public int[] getVtpVlanParentVlanValue()
  {
//<Begin_getVtpVlanParentVlanValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.10");
  
//<End_getVtpVlanParentVlanValue>
  } 
  
/**
 * Returns the vtpVlanTypeExt column values.
 * 
 * @return the response for vtpVlanTypeExt in the form of </code>String[]</code> .
 */
public String[] getVtpVlanTypeExt()
  {
//<Begin_getVtpVlanTypeExt> 
return comm.getColumn("vtpVlanTypeExt");
  
//<End_getVtpVlanTypeExt>
  } 
  
/**
 * Returns the vtpVlanRingNumber column values.
 * 
 * @return the response for vtpVlanRingNumber in the form of </code>String[]</code> .
 */
public String[] getVtpVlanRingNumber()
  {
//<Begin_getVtpVlanRingNumber> 
return comm.getColumn("vtpVlanRingNumber");
  
//<End_getVtpVlanRingNumber>
  } 
  
/** 
 * Returns the vtpVlanRingNumber column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanRingNumbercolumn.
 */
public int[] getVtpVlanRingNumberValue()
  {
//<Begin_getVtpVlanRingNumberValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.7");
  
//<End_getVtpVlanRingNumberValue>
  } 
  
/**
 * Returns the vtpVlanIsCRFBackup column values.
 * 
 * @return the response for vtpVlanIsCRFBackup in the form of </code>String[]</code> .
 */
public String[] getVtpVlanIsCRFBackup()
  {
//<Begin_getVtpVlanIsCRFBackup> 
return comm.getColumn("vtpVlanIsCRFBackup");
  
//<End_getVtpVlanIsCRFBackup>
  } 
  
/** 
 * Returns the vtpVlanIsCRFBackup column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vtpVlanIsCRFBackupcolumn.
 */
public int[] getVtpVlanIsCRFBackupValue()
  {
//<Begin_getVtpVlanIsCRFBackupValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.9.46.1.3.1.1.16");
  
//<End_getVtpVlanIsCRFBackupValue>
  } 
  
/**
 * Returns the columnNames of the table.
 * 
 * @return the columnnames in </code>String[]</code> form.
 */
public String[] getVtpVlanTableColumns()
  {
//<Begin_getVtpVlanTableColumns> 
return comm.getColumnNames(".1.3.6.1.4.1.9.9.46.1.3.1");
  
//<End_getVtpVlanTableColumns>
  } 
  
/**
 * Returns the response for the whole vtpVlanTable.
 * 
 * @return the response in </code>String[]</code> form.
 */
public String[][] getVtpVlanTable()
  {
//<Begin_getVtpVlanTable> 
return comm.getTable(".1.3.6.1.4.1.9.9.46.1.3.1");
  
//<End_getVtpVlanTable>
  } 
  
/**
 * Sends the request for vtpVlanTable Table OID which will update for any table change event.
 * To use this method in the application, first the application has to implement SnmpTableListener
 * and then register itself for receiving all fired table change events by implementing
 * the methods of SnmpTableListener.
 */
public void fetchVtpVlanTable()
  {
//<Begin_fetchVtpVlanTable> 
comm.fetchTableData(".1.3.6.1.4.1.9.9.46.1.3.1");
  
//<End_fetchVtpVlanTable>
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
