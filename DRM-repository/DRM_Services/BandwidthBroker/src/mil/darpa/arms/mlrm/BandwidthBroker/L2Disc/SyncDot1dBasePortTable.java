package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;

import com.adventnet.snmp.studio.api.*;
import com.adventnet.utils.*;
import java.util.*;
import com.adventnet.snmp.beans.*;
import  java.applet.* ;


public class SyncDot1dBasePortTable
{
	//<Begin_Variable_Declarations>
	Communication comm = null;
	//<End_Variable_Declarations>

  
/**
 * Default constructor to instantiate this class.
 * This constructor can be used to get this class instance for creating any application.
 */
public SyncDot1dBasePortTable()
  {
         //<Begin_SyncDot1dBasePortTable> 
	comm = new Communication("SYNC");
	comm.loadMibs();

  
         //<End_SyncDot1dBasePortTable>
  } 
  
/**
 * Constructor to instantiate an applet instance of this class.
 * This constructor can be used for creating an applet.
 * 
 * @param applet an </code>Applet</code> instance.
 */
public SyncDot1dBasePortTable(Applet applet)
  {
         //<Begin_SyncDot1dBasePortTable_Applet> 
	comm = new Communication(applet,"SYNC");
	comm.loadMibs();

  
         //<End_SyncDot1dBasePortTable_Applet>
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
 * Gets the response from dot1dBasePortTable for the corresponding row index.
 * 
 * @param row an </code>int</code> value specifying the row index of the table.
 * 
 * @return a </code>String[]</code> for the row values of the table.
 */
public String[] getDot1dBasePortTableRow(int row)
  {
//<Begin_getDot1dBasePortTableRow_int> 
return comm.getRow(".1.3.6.1.2.1.17.1.4", row);
  
//<End_getDot1dBasePortTableRow_int>
  } 
  
/**
 * Returns the dot1dBasePortMtuExceededDiscards column values.
 * 
 * @return the response for dot1dBasePortMtuExceededDiscards in the form of </code>String[]</code> .
 */
public String[] getDot1dBasePortMtuExceededDiscards()
  {
//<Begin_getDot1dBasePortMtuExceededDiscards> 
return comm.getColumn("dot1dBasePortMtuExceededDiscards");
  
//<End_getDot1dBasePortMtuExceededDiscards>
  } 
  
/**
 * Returns the dot1dBasePortMtuExceededDiscards column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the dot1dBasePortMtuExceededDiscardscolumn.
 */
public long[] getDot1dBasePortMtuExceededDiscardsValue()
  {
//<Begin_getDot1dBasePortMtuExceededDiscardsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.17.1.4.1.5");
  
//<End_getDot1dBasePortMtuExceededDiscardsValue>
  } 
  
/**
 * Returns the dot1dBasePortCircuit column values.
 * 
 * @return the response for dot1dBasePortCircuit in the form of </code>String[]</code> .
 */
public String[] getDot1dBasePortCircuit()
  {
//<Begin_getDot1dBasePortCircuit> 
return comm.getColumn("dot1dBasePortCircuit");
  
//<End_getDot1dBasePortCircuit>
  } 
  
/**
 * Returns the dot1dBasePortDelayExceededDiscards column values.
 * 
 * @return the response for dot1dBasePortDelayExceededDiscards in the form of </code>String[]</code> .
 */
public String[] getDot1dBasePortDelayExceededDiscards()
  {
//<Begin_getDot1dBasePortDelayExceededDiscards> 
return comm.getColumn("dot1dBasePortDelayExceededDiscards");
  
//<End_getDot1dBasePortDelayExceededDiscards>
  } 
  
/**
 * Returns the dot1dBasePortDelayExceededDiscards column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the dot1dBasePortDelayExceededDiscardscolumn.
 */
public long[] getDot1dBasePortDelayExceededDiscardsValue()
  {
//<Begin_getDot1dBasePortDelayExceededDiscardsValue> 

return comm.getColumnValuesLong(".1.3.6.1.2.1.17.1.4.1.4");
  
//<End_getDot1dBasePortDelayExceededDiscardsValue>
  } 
  
/**
 * Returns the dot1dBasePort column values.
 * 
 * @return the response for dot1dBasePort in the form of </code>String[]</code> .
 */
public String[] getDot1dBasePort()
  {
//<Begin_getDot1dBasePort> 
return comm.getColumn("dot1dBasePort");
  
//<End_getDot1dBasePort>
  } 
  
/** 
 * Returns the dot1dBasePort column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the dot1dBasePortcolumn.
 */
public int[] getDot1dBasePortValue()
  {
//<Begin_getDot1dBasePortValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.17.1.4.1.1");
  
//<End_getDot1dBasePortValue>
  } 
  
/**
 * Returns the dot1dBasePortIfIndex column values.
 * 
 * @return the response for dot1dBasePortIfIndex in the form of </code>String[]</code> .
 */
public String[] getDot1dBasePortIfIndex()
  {
//<Begin_getDot1dBasePortIfIndex> 
return comm.getColumn("dot1dBasePortIfIndex");
  
//<End_getDot1dBasePortIfIndex>
  } 
  
/** 
 * Returns the dot1dBasePortIfIndex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the dot1dBasePortIfIndexcolumn.
 */
public int[] getDot1dBasePortIfIndexValue()
  {
//<Begin_getDot1dBasePortIfIndexValue> 

return comm.getColumnValuesInt(".1.3.6.1.2.1.17.1.4.1.2");
  
//<End_getDot1dBasePortIfIndexValue>
  } 
  
/**
 * Returns the columnNames of the table.
 * 
 * @return the columnnames in </code>String[]</code> form.
 */
public String[] getDot1dBasePortTableColumns()
  {
//<Begin_getDot1dBasePortTableColumns> 
return comm.getColumnNames(".1.3.6.1.2.1.17.1.4");
  
//<End_getDot1dBasePortTableColumns>
  } 
  
/**
 * Returns the response for the whole dot1dBasePortTable.
 * 
 * @return the response in </code>String[]</code> form.
 */
public String[][] getDot1dBasePortTable()
  {
//<Begin_getDot1dBasePortTable> 
return comm.getTable(".1.3.6.1.2.1.17.1.4");
  
//<End_getDot1dBasePortTable>
  } 
  
/**
 * Sends the request for dot1dBasePortTable Table OID which will update for any table change event.
 * To use this method in the application, first the application has to implement SnmpTableListener
 * and then register itself for receiving all fired table change events by implementing
 * the methods of SnmpTableListener.
 */
public void fetchDot1dBasePortTable()
  {
//<Begin_fetchDot1dBasePortTable> 
comm.fetchTableData(".1.3.6.1.2.1.17.1.4");
  
//<End_fetchDot1dBasePortTable>
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
