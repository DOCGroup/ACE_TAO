package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;

import com.adventnet.snmp.studio.api.*;
import com.adventnet.utils.*;
import java.util.*;
import com.adventnet.snmp.beans.*;
import  java.applet.* ;


public class SyncDot1dStpPortTable
{
	//<Begin_Variable_Declarations>
	Communication comm = null;
	//<End_Variable_Declarations>

  
/**
 * Default constructor to instantiate this class.
 * This constructor can be used to get this class instance for creating any application.
 */
public SyncDot1dStpPortTable()
  {
         //<Begin_SyncDot1dStpPortTable> 
	comm = new Communication("SYNC");
	comm.loadMibs();

  
         //<End_SyncDot1dStpPortTable>
  } 
  
/**
 * Constructor to instantiate an applet instance of this class.
 * This constructor can be used for creating an applet.
 * 
 * @param applet an </code>Applet</code> instance.
 */
public SyncDot1dStpPortTable(Applet applet)
  {
         //<Begin_SyncDot1dStpPortTable_Applet> 
	comm = new Communication(applet,"SYNC");
	comm.loadMibs();

  
         //<End_SyncDot1dStpPortTable_Applet>
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
 * Gets the response from dot1dStpPortTable for the corresponding row index.
 * 
 * @param row an </code>int</code> value specifying the row index of the table.
 * 
 * @return a </code>String[]</code> for the row values of the table.
 */
public String[] getDot1dStpPortTableRow(int row)
  {
//<Begin_getDot1dStpPortTableRow_int> 
return comm.getRow(".1.3.6.1.2.1.17.2.15", row);
  
//<End_getDot1dStpPortTableRow_int>
  } 
  
/**
 * Returns the dot1dStpPortDesignatedPort column values.
 * 
 * @return the response for dot1dStpPortDesignatedPort in the form of </code>String[]</code> .
 */
public String[] getDot1dStpPortDesignatedPort()
  {
//<Begin_getDot1dStpPortDesignatedPort> 
return comm.getColumn("dot1dStpPortDesignatedPort");
  
//<End_getDot1dStpPortDesignatedPort>
  } 
  
/**
 * Returns the dot1dStpPortDesignatedCost column values.
 * 
 * @return the response for dot1dStpPortDesignatedCost in the form of </code>String[]</code> .
 */
public String[] getDot1dStpPortDesignatedCost()
  {
//<Begin_getDot1dStpPortDesignatedCost> 
return comm.getColumn("dot1dStpPortDesignatedCost");
  
//<End_getDot1dStpPortDesignatedCost>
  } 
  
/** 
 * Returns the dot1dStpPortDesignatedCost column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the dot1dStpPortDesignatedCostcolumn.
 */
public int[] getDot1dStpPortDesignatedCostValue()
  {
//<Begin_getDot1dStpPortDesignatedCostValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.17.2.15.1.7");
  
//<End_getDot1dStpPortDesignatedCostValue>
  } 
  
/**
 * Returns the dot1dStpPortDesignatedBridge column values.
 * 
 * @return the response for dot1dStpPortDesignatedBridge in the form of </code>String[]</code> .
 */
public String[] getDot1dStpPortDesignatedBridge()
  {
//<Begin_getDot1dStpPortDesignatedBridge> 
return comm.getColumn("dot1dStpPortDesignatedBridge");
  
//<End_getDot1dStpPortDesignatedBridge>
  } 
  
/**
 * Returns the dot1dStpPortPathCost column values.
 * 
 * @return the response for dot1dStpPortPathCost in the form of </code>String[]</code> .
 */
public String[] getDot1dStpPortPathCost()
  {
//<Begin_getDot1dStpPortPathCost> 
return comm.getColumn("dot1dStpPortPathCost");
  
//<End_getDot1dStpPortPathCost>
  } 
  
/** 
 * Returns the dot1dStpPortPathCost column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the dot1dStpPortPathCostcolumn.
 */
public int[] getDot1dStpPortPathCostValue()
  {
//<Begin_getDot1dStpPortPathCostValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.17.2.15.1.5");
  
//<End_getDot1dStpPortPathCostValue>
  } 
  
/**
 * Returns the dot1dStpPortForwardTransitions column values.
 * 
 * @return the response for dot1dStpPortForwardTransitions in the form of </code>String[]</code> .
 */
public String[] getDot1dStpPortForwardTransitions()
  {
//<Begin_getDot1dStpPortForwardTransitions> 
return comm.getColumn("dot1dStpPortForwardTransitions");
  
//<End_getDot1dStpPortForwardTransitions>
  } 
  
/**
 * Returns the dot1dStpPortForwardTransitions column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the dot1dStpPortForwardTransitionscolumn.
 */
public long[] getDot1dStpPortForwardTransitionsValue()
  {
//<Begin_getDot1dStpPortForwardTransitionsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.17.2.15.1.10");
  
//<End_getDot1dStpPortForwardTransitionsValue>
  } 
  
/**
 * Returns the dot1dStpPortPriority column values.
 * 
 * @return the response for dot1dStpPortPriority in the form of </code>String[]</code> .
 */
public String[] getDot1dStpPortPriority()
  {
//<Begin_getDot1dStpPortPriority> 
return comm.getColumn("dot1dStpPortPriority");
  
//<End_getDot1dStpPortPriority>
  } 
  
/** 
 * Returns the dot1dStpPortPriority column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the dot1dStpPortPrioritycolumn.
 */
public int[] getDot1dStpPortPriorityValue()
  {
//<Begin_getDot1dStpPortPriorityValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.17.2.15.1.2");
  
//<End_getDot1dStpPortPriorityValue>
  } 
  
/**
 * Returns the dot1dStpPortDesignatedRoot column values.
 * 
 * @return the response for dot1dStpPortDesignatedRoot in the form of </code>String[]</code> .
 */
public String[] getDot1dStpPortDesignatedRoot()
  {
//<Begin_getDot1dStpPortDesignatedRoot> 
return comm.getColumn("dot1dStpPortDesignatedRoot");
  
//<End_getDot1dStpPortDesignatedRoot>
  } 
  
/**
 * Returns the dot1dStpPortState column values.
 * 
 * @return the response for dot1dStpPortState in the form of </code>String[]</code> .
 */
public String[] getDot1dStpPortState()
  {
//<Begin_getDot1dStpPortState> 
return comm.getColumn("dot1dStpPortState");
  
//<End_getDot1dStpPortState>
  } 
  
/** 
 * Returns the dot1dStpPortState column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the dot1dStpPortStatecolumn.
 */
public int[] getDot1dStpPortStateValue()
  {
//<Begin_getDot1dStpPortStateValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.17.2.15.1.3");
  
//<End_getDot1dStpPortStateValue>
  } 
  
/**
 * Returns the dot1dStpPort column values.
 * 
 * @return the response for dot1dStpPort in the form of </code>String[]</code> .
 */
public String[] getDot1dStpPort()
  {
//<Begin_getDot1dStpPort> 
return comm.getColumn("dot1dStpPort");
  
//<End_getDot1dStpPort>
  } 
  
/** 
 * Returns the dot1dStpPort column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the dot1dStpPortcolumn.
 */
public int[] getDot1dStpPortValue()
  {
//<Begin_getDot1dStpPortValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.17.2.15.1.1");
  
//<End_getDot1dStpPortValue>
  } 
  
/**
 * Returns the dot1dStpPortEnable column values.
 * 
 * @return the response for dot1dStpPortEnable in the form of </code>String[]</code> .
 */
public String[] getDot1dStpPortEnable()
  {
//<Begin_getDot1dStpPortEnable> 
return comm.getColumn("dot1dStpPortEnable");
  
//<End_getDot1dStpPortEnable>
  } 
  
/** 
 * Returns the dot1dStpPortEnable column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the dot1dStpPortEnablecolumn.
 */
public int[] getDot1dStpPortEnableValue()
  {
//<Begin_getDot1dStpPortEnableValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.17.2.15.1.4");
  
//<End_getDot1dStpPortEnableValue>
  } 
  
/**
 * Returns the columnNames of the table.
 * 
 * @return the columnnames in </code>String[]</code> form.
 */
public String[] getDot1dStpPortTableColumns()
  {
//<Begin_getDot1dStpPortTableColumns> 
return comm.getColumnNames(".1.3.6.1.2.1.17.2.15");
  
//<End_getDot1dStpPortTableColumns>
  } 
  
/**
 * Returns the response for the whole dot1dStpPortTable.
 * 
 * @return the response in </code>String[]</code> form.
 */
public String[][] getDot1dStpPortTable()
  {
//<Begin_getDot1dStpPortTable> 
return comm.getTable(".1.3.6.1.2.1.17.2.15");
  
//<End_getDot1dStpPortTable>
  } 
  
/**
 * Sends the request for dot1dStpPortTable Table OID which will update for any table change event.
 * To use this method in the application, first the application has to implement SnmpTableListener
 * and then register itself for receiving all fired table change events by implementing
 * the methods of SnmpTableListener.
 */
public void fetchDot1dStpPortTable()
  {
//<Begin_fetchDot1dStpPortTable> 
comm.fetchTableData(".1.3.6.1.2.1.17.2.15");
  
//<End_fetchDot1dStpPortTable>
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
