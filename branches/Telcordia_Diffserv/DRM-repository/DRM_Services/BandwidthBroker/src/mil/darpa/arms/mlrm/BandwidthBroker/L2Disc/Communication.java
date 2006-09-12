package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;

import com.adventnet.utils.*;
import  com.adventnet.snmp.studio.api.*;
import  java.applet.*;
import  com.adventnet.snmp.beans.*;


public class Communication
{
	//<Begin_Variable_Declarations>
	HLOperations operations = null;
	boolean isApplet = false;
	Applet appletInstance = null;
	//<End_Variable_Declarations>

  
/**
 * Default constructor to be used for application.
 * As this constructor does not take the communication type as argument, hence the 
 * communication type has to be set soon after instantiating this class using this 
 * constructor using the setOperationType method.
 */
public Communication()
  {
         //<Begin_Communication> 
	isApplet = false; 
	appletInstance = null;

  
         //<End_Communication>
  } 
  
/**
 * Constructor to be used for applet creation.
 * As this constructor doe not take the communication type as argument, hence the 
 * communication type has to be set soon after instantiating this class using this 
 * constructor using the setOperationType method
 *
 * @param applet an </code>Applet</code> instance.
 */
public Communication(Applet applet)
  {
         //<Begin_Communication_Applet> 
	isApplet = true; 
	appletInstance = applet;

  
         //<End_Communication_Applet>
  } 
  
/**
 * Constructor to be used for creating applications which will take the communication type
 * as argument.Here no need to set the communication type again.
 *
 * @param communicationType a </code>String</code> value specifying the communicationType.
 * It can be either SCALAR/TABLE/ASYNC
 */
public Communication(String communicationType)
  {
         //<Begin_Communication_String> 
	operations = new HLOperations(communicationType);

  
         //<End_Communication_String>
  } 
  
/**
 * Constructor to be used for creating applets which will take the communication type
 * as argument.Here no need to set the communication type again.
 *
 * @param applet an </code>Applet</code> instance.
 *
 * @param communicationType a </code>String</code> value specifying the communicationType.
 * It can be either SCALAR/TABLE/ASYNC
 */
public Communication(Applet applet,String communicationType)
  {
         //<Begin_Communication_Applet_String> 
	operations = new HLOperations(applet,communicationType);

  
         //<End_Communication_Applet_String>
  } 
  
/**
 * This method will be called after instantiating this Communication class using the 
 * default constructor without any argument.
 * This will set the operation type i.e. SCALAR/TABLE/ASYNC in the Communication instance.
 * Without setting the operationType, the response can't be received for any request sent.
 * So before sending any request or start using the methods the operationType should be 
 * set if the constructor used is default constructor to instantiate this class i.e. 
 * Communication()/Communication(Applet applet).
 *
 * @param communicationType a </code>String</code> value specifying the communication type for this instance.
 */
public void setOperationType(String communicationType)
  {
         //<Begin_setOperationType_String> 

	if(isApplet)
	{
		operations = new HLOperations(appletInstance, communicationType);
	}
	else
	{
		operations = new HLOperations(communicationType);
	} 
  
         //<End_setOperationType_String>
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
public void setAgentParams(AgentParams params)
  {
         //<Begin_setAgentParams_AgentParams> 
	operations.setAgentParams(params);
  
         //<End_setAgentParams_AgentParams>
  } 
  
/** 
 * Gets the SNMP variable identified by the specified Object Identifier.
 * This will return the value in string form for the oid passed.
 *
 * @param oid a </code>String</code> value specifying the numeric oid.
 *
 * @return a </code>String</code> response for the get request sent.
 */
public String get(String oid)
  {
         //<Begin_get_String> 
	return operations.get(oid);
  
         //<End_get_String>
  } 
  
/** 
 * Gets the SNMP variable for the next Object Identifier to that of specified.
 * Performs a getNext request for the oid passed and returns the value for the next oid to
 * that of the oid passed.
 *
 * @param oid a </code>String</code> value specifying the numeric oid.
 *
 * @return a </code>String</code> response for the getNext request sent.
 */
public String getNext(String oid)
  {
         //<Begin_getNext_String> 
	return operations.getNext(oid);
  
         //<End_getNext_String>
  } 
  
/** 
 * Queries the agent and returns the list of SNMP variables from the agent using the Object
 * Identifier list passed.This will return the response values for the next available 
 * oids to that of the oids passed.
 *
 * @param oids a </code>String[]</code> value specifying the numeric oids.
 *
 * @return a </code>String[]</code> as response for the getNextList request sent.
 */
public String[] getNextList(String[] oids)
  {
         //<Begin_getNextList_String[]> 
	return operations.getNextList(oids);
  
         //<End_getNextList_String[]>
  } 
  
/** 
 * Returns the bulk response to the request done for the oid.
 * This will return the response value for the next 'n' number of oids where 'n' is the 
 * maxRepeatitions value set, in the form of a two dimensional string array form.
 *
 * @param oid a </code>String</code> value specifying the numeric oid.
 *
 * @return a </code>String[][]</code> response for the getBulk request sent.
 * In this case the number of rows will be one and the number of columns will be same as the
 * maxRepetition value.
 */
public String[][] getBulk(String oid)
  {
         //<Begin_getBulk_String> 
	return operations.getBulk(oid);
  
         //<End_getBulk_String>
  } 
  
/** 
 * Returns the bulk response to the request done for the list of oids.
 * This will return the response values for the next 'n' number of oids(to each oid from 
 * the list)where 'n' is the maxRepeatitions value set, in the form of a two dimensional
 * string array form.
 *
 * @param oids a </code>String[]</code> value specifying the numeric oids.
 *
 * @return a </code>String[][]</code> response for the getBulkList request sent.
 * In this case the number of rows will be same as the length of the string[] passed as 
 * argument and the number of columns will be same as the maxRepetition value.
 */
public String[][] getBulkList(String[] oids)
  {
         //<Begin_getBulkList_String[]> 
	return operations.getBulkList(oids);
  
         //<End_getBulkList_String[]>
  } 
  
/**
 * Does an SNMP set to the oid passed using the value passed.
 * This method will set the value passed on the oid specified and return the response for 
 * the set value.
 * 
 * @param oid a </code>String</code> form of the numeric oid for which value has to be set.
 * 
 * @param value a </code>String</code> value to be set on the specified oid.
 *
 * @return the response after setting the value in </code>String</code> form.
 */
public  String set(String oid, String value)
  {
         //<Begin_set_String_String> 
	return operations.set(oid, value);
  
         //<End_set_String_String>
  } 
  
/**
 * Gets the SNMP variable identified by the specified Object Identifier in it's actual form.
 * This method functionality is almost same as that of get() method
 * ,the only difference is here the method return type is in the form of Object of it's actual
 * type.For ex. it may be either Long object or Integer object.
 *
 * @param oid a </code>String</code> value specifying the oid.
 *
 * @return a </code>Object</code> instance which may be either Integer/Long object.
 */
public Object getValue(String oid) 
  {
         //<Begin_getValue_String> 
	return operations.getValue(oid);
  
         //<End_getValue_String>
  } 
  
/**
 * Gets the data of the whole table for the table oid passed.
 * This method will take a table ObjectID as argument and return the whole table response in the form
 * of a two dimensional string array.
 * 
 * @param tableOID a </code>String</code> value specifying the tableOID.
 *
 * @return a </code>String[][]</code> as response for the tableID passed.
 */
public String[][] getTable(String tableOID)
  {
//<Begin_getTable_String> 
return operations.getTable(tableOID);
  
//<End_getTable_String>
  } 
  
/**
 * Returns the specified column values in the form of String[].
 *
 * @param columnOID a </code>String</code> value specifying the columnar OID.
 *
 * @return a </code>String[]</code> response for the columnar oid.
 * This array length will be same as the number of rows existing under that column.
 */
public String[] getColumn(String columnOID)
  {
//<Begin_getColumn_String> 
return operations.getColumn(columnOID);
  
//<End_getColumn_String>
  } 
  
/**
 * Returns the specified column values in it's actual type.
 *
 * @param columnOID a </code>String</code> value specifying the columnar OID.
 *
 * @return a </code>long[]</code> response for the columnar oid.
 * This array length will be same as the number of rows existing under that column.
 * This method is for SnmpManager internal usage.This should not be used as an API of 
 * Communication class
 */
public long[] getColumnValuesLong(String columnOID)
  {
//<Begin_getColumnValuesLong_String> 
return operations.getColumnValuesLong(columnOID);
  
//<End_getColumnValuesLong_String>
  } 
  
/**
 * Returns the specified column values in it's actual type.
 *
 * @param columnOID a </code>String</code> value specifying the columnar OID.
 *
 * @return a </code>int[]</code> response for the columnar oid.
 * This array length will be same as the number of rows existing under that column.
 * This method is for SnmpManager internal usage.This should not be used as an API of 
 * Communication class
 */
public int[] getColumnValuesInt(String columnOID)
  {
//<Begin_getColumnValuesInt_String> 
return operations.getColumnValuesInt(columnOID);
  
//<End_getColumnValuesInt_String>
  } 
  
/**
 * Returns the specified row's value for a table.
 * This method will take the table OID and the row index as argument and 
 * will return the row's value in a string array form.
 * 
 * @param tableOID a </code>String</code> value for the table OID in it's numeric form.
 * 
 * @param row an </code>int</code> index of the table for which the request has to be sent.
 *
 * @return the table row response in </code>String[]</code> the form.
 */
public String[] getRow(String tableOID, int row)
  {
//<Begin_getRow_String_int> 
return operations.getRow(tableOID, row);
  
//<End_getRow_String_int>
  } 
  
/**
 * Sends request for the table OID response. 
 * This method is an equivalent of asynchronous get method.It will onlly send the request
 * for the specified table OID and response has toe be recieved through the implemented 
 * methods of SnmpTableListener.Application which need the response of fetchtable request
 * has to implement SnmpTableListener and can catch the response in the implmented tableChanged()
 * method.Advantage of using this method than getTable() method is that it will make the
 * listener informed about any table change made for the specific table.
 * 
 * @param tableOID a </code>String</code> value specifying the table OID for which request has to be sent.
 */
public void fetchTableData(String tableOID)
  {
//<Begin_fetchTableData_String> 
operations.fetchTableData(tableOID);
  
//<End_fetchTableData_String>
  } 
  
/**
 * Return all the columnnames for the tableNode.
 * This method will take argument as the table OID and return all the 
 * existing column names inside the table in the form of a string array.
 * 
 * @param tableOID a </code>String</code> value for the table oid.
 * 
 * @return the columnnames in </code>String[]</code> form.
 */
public String[] getColumnNames(String tableOID)
  {
//<Begin_getColumnNames_String> 
return operations.getColumnNames(tableOID);
  
//<End_getColumnNames_String>
  } 
  
/**
 * This method will load the user selected Mib in this Communication instance.
 * Application created using Communication instance should call this method, soon after instantiating
 * this class. Because without loading Mib the responses for any request sent will be null.
 */
public void loadMibs() 
  {
         //<Begin_loadMibs> 
	operations.loadMibModule("./mibs/IF-MIB ./mibs/IANAifType-MIB ./mibs/RFC1213-MIB ./mibs/SNMPv2-MIB ./mibs/BRIDGE-MIB ./mibs/CISCO-VTP-MIB ./mibs/CISCO-SMI ./mibs/SNMP-FRAMEWORK-MIB ./mibs/CISCO-VLAN-MEMBERSHIP-MIB ./mibs/TOKEN-RING-RMON-MIB ./mibs/RFC1271-MIB ./mibs/Q-BRIDGE-MIB ./mibs/P-BRIDGE-MIB ./mibs/RMON2-MIB ./mibs/CISCO-STACK-MIB ./mibs/ENTITY-MIB");
  
         //<End_loadMibs>
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
	operations.addLogClient(client);
  
         //<End_addLogClient_LogInterface>
  } 
  
/**
 * Add a listener to the list that's notified each time when ever a change is made to the 
 * SNMP Table occurs.
 * Application using this class instance if needs to be informed about any table change has to implement
 * </code>SnmpTableListener</code> interface and pass it as argument through this method
 * to set in this class instance as a listener.
 * 
 * @param tableClient a </code>SnmpTableListener</code> instance reference.
 */
public void addSnmpTableListener(SnmpTableListener tableClient)
  {
//<Begin_addSnmpTableListener_SnmpTableListener> 
operations.addSnmpTableListener(tableClient);
  
//<End_addSnmpTableListener_SnmpTableListener>
  } 
  
/**
 * Removes the SnmpTableListener instance from the list of listeners.
 * 
 * @param tableClient a </code>SnmpTableListener</code> instance reference.
 */
public void removeSnmpTableListener(SnmpTableListener tableClient)
  {
//<Begin_removeSnmpTableListener_SnmpTableListener> 
operations.removeSnmpTableListener(tableClient);
  
//<End_removeSnmpTableListener_SnmpTableListener>
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
	return operations.getErrorString();
  
         //<End_getErrorString>
  } 
  
/**
 * Returns the response for all the apecified ObjectIDs in a single GET request.
 *
 * @param oids a </code>String[]</code> for the list of oids.
 *
 * @return the response in the form of </code>String[]</code> for all the oids passed.
 */ public String[] getList(String oids[])
  {
//<Begin_getList_String[]> 
return operations.getList(oids);

  
//<End_getList_String[]>
  } 
  
/**
 * Determines whether debug output will be printed or not.
 * 
 * @param debug a </code>boolean</code> value which will decide whether
 * the debug output will be printed in the console or not.
 */
public void setDebug(boolean debug) 
  {
         //<Begin_setDebug_boolean> 
	operations.setDebug(debug);
  
         //<End_setDebug_boolean>
  } 
  
/**
 * Gets the decoded form of the debug message.
 * This method will be used after getting the debug message through 
 * the implemented method dbg() of LogManager class.
 *
 * @param debugValue the </code>String</code> form of the debug message.
 *
 * @return after decoding the debug message in </code></code> String form.
 */
public String getDecodedMessage(String debugValue) 
  {
         //<Begin_getDecodedMessage_String> 
	return operations.decodeString(debugValue);
  
         //<End_getDecodedMessage_String>
  } 
  
/**
 * Closes all existing class resources.
 * This method should be used before closing any application.
 */
public void closeResources() 
  {
         //<Begin_closeResources> 
	operations.closeResources();
  
         //<End_closeResources>
  } 

} 
