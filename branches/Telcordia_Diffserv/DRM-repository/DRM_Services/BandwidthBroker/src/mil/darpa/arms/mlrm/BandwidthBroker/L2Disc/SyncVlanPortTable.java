package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;

import com.adventnet.snmp.studio.api.*;
import com.adventnet.utils.*;
import java.util.*;
import com.adventnet.snmp.beans.*;
import  java.applet.* ;


public class SyncVlanPortTable
{
	//<Begin_Variable_Declarations>
	Communication comm = null;
	//<End_Variable_Declarations>

  
/**
 * Default constructor to instantiate this class.
 * This constructor can be used to get this class instance for creating any application.
 */
public SyncVlanPortTable()
  {
         //<Begin_SyncVlanPortTable> 
	comm = new Communication("SYNC");
	comm.loadMibs();

  
         //<End_SyncVlanPortTable>
  } 
  
/**
 * Constructor to instantiate an applet instance of this class.
 * This constructor can be used for creating an applet.
 * 
 * @param applet an </code>Applet</code> instance.
 */
public SyncVlanPortTable(Applet applet)
  {
         //<Begin_SyncVlanPortTable_Applet> 
	comm = new Communication(applet,"SYNC");
	comm.loadMibs();

  
         //<End_SyncVlanPortTable_Applet>
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
 * Gets the response from vlanPortTable for the corresponding row index.
 * 
 * @param row an </code>int</code> value specifying the row index of the table.
 * 
 * @return a </code>String[]</code> for the row values of the table.
 */
public String[] getVlanPortTableRow(int row)
  {
//<Begin_getVlanPortTableRow_int> 
return comm.getRow(".1.3.6.1.4.1.9.5.1.9.3", row);
  
//<End_getVlanPortTableRow_int>
  } 
  
/**
 * Returns the vlanPortIslPriorityVlans column values.
 * 
 * @return the response for vlanPortIslPriorityVlans in the form of </code>String[]</code> .
 */
public String[] getVlanPortIslPriorityVlans()
  {
//<Begin_getVlanPortIslPriorityVlans> 
return comm.getColumn("vlanPortIslPriorityVlans");
  
//<End_getVlanPortIslPriorityVlans>
  } 
  
/**
 * Returns the vlanPort column values.
 * 
 * @return the response for vlanPort in the form of </code>String[]</code> .
 */
public String[] getVlanPort()
  {
//<Begin_getVlanPort> 
return comm.getColumn("vlanPort");
  
//<End_getVlanPort>
  } 
  
/** 
 * Returns the vlanPort column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vlanPortcolumn.
 */
public int[] getVlanPortValue()
  {
//<Begin_getVlanPortValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.9.3.1.2");
  
//<End_getVlanPortValue>
  } 
  
/**
 * Returns the vlanPortAuxiliaryVlan column values.
 * 
 * @return the response for vlanPortAuxiliaryVlan in the form of </code>String[]</code> .
 */
public String[] getVlanPortAuxiliaryVlan()
  {
//<Begin_getVlanPortAuxiliaryVlan> 
return comm.getColumn("vlanPortAuxiliaryVlan");
  
//<End_getVlanPortAuxiliaryVlan>
  } 
  
/** 
 * Returns the vlanPortAuxiliaryVlan column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vlanPortAuxiliaryVlancolumn.
 */
public int[] getVlanPortAuxiliaryVlanValue()
  {
//<Begin_getVlanPortAuxiliaryVlanValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.9.3.1.12");
  
//<End_getVlanPortAuxiliaryVlanValue>
  } 
  
/**
 * Returns the vlanPortAdminStatus column values.
 * 
 * @return the response for vlanPortAdminStatus in the form of </code>String[]</code> .
 */
public String[] getVlanPortAdminStatus()
  {
//<Begin_getVlanPortAdminStatus> 
return comm.getColumn("vlanPortAdminStatus");
  
//<End_getVlanPortAdminStatus>
  } 
  
/** 
 * Returns the vlanPortAdminStatus column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vlanPortAdminStatuscolumn.
 */
public int[] getVlanPortAdminStatusValue()
  {
//<Begin_getVlanPortAdminStatusValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.9.3.1.10");
  
//<End_getVlanPortAdminStatusValue>
  } 
  
/**
 * Returns the vlanPortIslVlansAllowed column values.
 * 
 * @return the response for vlanPortIslVlansAllowed in the form of </code>String[]</code> .
 */
public String[] getVlanPortIslVlansAllowed()
  {
//<Begin_getVlanPortIslVlansAllowed> 
return comm.getColumn("vlanPortIslVlansAllowed");
  
//<End_getVlanPortIslVlansAllowed>
  } 
  
/**
 * Returns the vlanPortIslOperStatus column values.
 * 
 * @return the response for vlanPortIslOperStatus in the form of </code>String[]</code> .
 */
public String[] getVlanPortIslOperStatus()
  {
//<Begin_getVlanPortIslOperStatus> 
return comm.getColumn("vlanPortIslOperStatus");
  
//<End_getVlanPortIslOperStatus>
  } 
  
/** 
 * Returns the vlanPortIslOperStatus column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vlanPortIslOperStatuscolumn.
 */
public int[] getVlanPortIslOperStatusValue()
  {
//<Begin_getVlanPortIslOperStatusValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.9.3.1.8");
  
//<End_getVlanPortIslOperStatusValue>
  } 
  
/**
 * Returns the vlanPortOperStatus column values.
 * 
 * @return the response for vlanPortOperStatus in the form of </code>String[]</code> .
 */
public String[] getVlanPortOperStatus()
  {
//<Begin_getVlanPortOperStatus> 
return comm.getColumn("vlanPortOperStatus");
  
//<End_getVlanPortOperStatus>
  } 
  
/** 
 * Returns the vlanPortOperStatus column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vlanPortOperStatuscolumn.
 */
public int[] getVlanPortOperStatusValue()
  {
//<Begin_getVlanPortOperStatusValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.9.3.1.11");
  
//<End_getVlanPortOperStatusValue>
  } 
  
/**
 * Returns the vlanPortIslAdminStatus column values.
 * 
 * @return the response for vlanPortIslAdminStatus in the form of </code>String[]</code> .
 */
public String[] getVlanPortIslAdminStatus()
  {
//<Begin_getVlanPortIslAdminStatus> 
return comm.getColumn("vlanPortIslAdminStatus");
  
//<End_getVlanPortIslAdminStatus>
  } 
  
/** 
 * Returns the vlanPortIslAdminStatus column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vlanPortIslAdminStatuscolumn.
 */
public int[] getVlanPortIslAdminStatusValue()
  {
//<Begin_getVlanPortIslAdminStatusValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.9.3.1.7");
  
//<End_getVlanPortIslAdminStatusValue>
  } 
  
/**
 * Returns the vlanPortModule column values.
 * 
 * @return the response for vlanPortModule in the form of </code>String[]</code> .
 */
public String[] getVlanPortModule()
  {
//<Begin_getVlanPortModule> 
return comm.getColumn("vlanPortModule");
  
//<End_getVlanPortModule>
  } 
  
/** 
 * Returns the vlanPortModule column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vlanPortModulecolumn.
 */
public int[] getVlanPortModuleValue()
  {
//<Begin_getVlanPortModuleValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.9.3.1.1");
  
//<End_getVlanPortModuleValue>
  } 
  
/**
 * Returns the vlanPortVlan column values.
 * 
 * @return the response for vlanPortVlan in the form of </code>String[]</code> .
 */
public String[] getVlanPortVlan()
  {
//<Begin_getVlanPortVlan> 
return comm.getColumn("vlanPortVlan");
  
//<End_getVlanPortVlan>
  } 
  
/** 
 * Returns the vlanPortVlan column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vlanPortVlancolumn.
 */
public int[] getVlanPortVlanValue()
  {
//<Begin_getVlanPortVlanValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.9.3.1.3");
  
//<End_getVlanPortVlanValue>
  } 
  
/**
 * Returns the vlanPortSwitchLevel column values.
 * 
 * @return the response for vlanPortSwitchLevel in the form of </code>String[]</code> .
 */
public String[] getVlanPortSwitchLevel()
  {
//<Begin_getVlanPortSwitchLevel> 
return comm.getColumn("vlanPortSwitchLevel");
  
//<End_getVlanPortSwitchLevel>
  } 
  
/** 
 * Returns the vlanPortSwitchLevel column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the vlanPortSwitchLevelcolumn.
 */
public int[] getVlanPortSwitchLevelValue()
  {
//<Begin_getVlanPortSwitchLevelValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.9.3.1.6");
  
//<End_getVlanPortSwitchLevelValue>
  } 
  
/**
 * Returns the columnNames of the table.
 * 
 * @return the columnnames in </code>String[]</code> form.
 */
public String[] getVlanPortTableColumns()
  {
//<Begin_getVlanPortTableColumns> 
return comm.getColumnNames(".1.3.6.1.4.1.9.5.1.9.3");
  
//<End_getVlanPortTableColumns>
  } 
  
/**
 * Returns the response for the whole vlanPortTable.
 * 
 * @return the response in </code>String[]</code> form.
 */
public String[][] getVlanPortTable()
  {
//<Begin_getVlanPortTable> 
return comm.getTable(".1.3.6.1.4.1.9.5.1.9.3");
  
//<End_getVlanPortTable>
  } 
  
/**
 * Sends the request for vlanPortTable Table OID which will update for any table change event.
 * To use this method in the application, first the application has to implement SnmpTableListener
 * and then register itself for receiving all fired table change events by implementing
 * the methods of SnmpTableListener.
 */
public void fetchVlanPortTable()
  {
//<Begin_fetchVlanPortTable> 
comm.fetchTableData(".1.3.6.1.4.1.9.5.1.9.3");
  
//<End_fetchVlanPortTable>
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
