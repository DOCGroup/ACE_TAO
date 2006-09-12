package mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer;

import com.adventnet.snmp.studio.api.*;
import com.adventnet.utils.*;
import com.adventnet.snmp.beans.*;
import  java.applet.* ;


public class AsyncIfXTable
{
	//<Begin_Variable_Declarations>
	Communication comm = null;
	//<End_Variable_Declarations>

  
/**
 * Default constructor to instantiate this class.
 * This constructor can be used to get this class instance for creating any application.
 */
public AsyncIfXTable()
  {
         //<Begin_AsyncIfXTable> 
	comm = new Communication("ASYNC");
	comm.loadMibs();

  
         //<End_AsyncIfXTable>
  } 
  
/**
 * Constructor to instantiate an applet instance of this class.
 * This constructor can be used for creating an applet.
 * 
 * @param applet an </code>Applet</code> instance.
 */
public AsyncIfXTable(Applet applet)
  {
         //<Begin_AsyncIfXTable_Applet> 
	comm = new Communication(applet,"ASYNC");
	comm.loadMibs();

  
         //<End_AsyncIfXTable_Applet>
  } 
  
/**
 * Adds a Listener to the list that is notified each time a response arrives.
 * The Listeners will receive Result Events from any or all SNMP requests. Targeting 
 * responses to specific listeners, requires using RequestEvent and the setDestination 
 * method while creating the request.
 *
 * @param rl a </code>ResultListener</code> object instance.
 */
public void addResultListener(ResultListener rl)
  {
         //<Begin_addResultListener_ResultListener> 
	comm.addResultListener(rl);
  
         //<End_addResultListener_ResultListener>
  } 
  
/**
 * Removes the ResultListener from the list of listeners.
 *
 * @param rl a </code>ResultListener</code> object instance.
 */
public void removeResultListener(ResultListener rl)
  {
         //<Begin_removeResultListener_ResultListener> 
	comm.removeResultListener(rl);
  
         //<End_removeResultListener_ResultListener>
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
 * Returns the error string associated with the request if the request fails or times out.
 * This method should be called in the application if the request ID for any request is -1 
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
