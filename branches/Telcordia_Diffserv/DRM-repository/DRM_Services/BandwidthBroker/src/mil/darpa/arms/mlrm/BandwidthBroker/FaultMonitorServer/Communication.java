package mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer;

import com.adventnet.utils.*;
import  com.adventnet.snmp.studio.api.*;
import  java.applet.*;
import  java.awt.*;
import  com.adventnet.snmp.beans.*;
import  com.adventnet.snmp.ui.SnmpTableModel;


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
 * Default constructor to be used for creating TrapApplication.
 * 
 * @param port an </code>int</code> value specifying the portNo. on which the traps have to
 * be received. 
 */
public Communication(int port)
  {
         //<Begin_Communication_int> 
	operations = new HLOperations(port);

  
         //<End_Communication_int>
  } 
  
/**
 * Default constructor to be used for creating TrapApplet.
 * 
 * @param port an </code>int</code> value specifying the portNo. on which the traps have to
 * be received.
 *
 * @param applet an </code>Applet</code> instance.
 */
public Communication(int port,Applet applet)
  {
         //<Begin_Communication_int_Applet> 
	operations = new HLOperations(port,applet);

  
         //<End_Communication_int_Applet>
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
 * Adds a Listener to the list of listeners that is notified each time a trap 
 * notification arrives. The Listeners will be notified of all Trap Events from any SNMP
 * node at any point of time.So the application for receiving trap should implement TrapListener
 * and register itself to this Communication instance for receiving trap.
 * 
 * @param tl a </code>TrapListener</code> object instance.
 */
public void addTrapListener(TrapListener tl)
  {
         //<Begin_addTrapListener_TrapListener> 
	operations.addTrapListener(tl);

  
         //<End_addTrapListener_TrapListener>
  } 
  
/**
 * Removes the existing TrapListener instance from the list of Listeners.
 * 
 * @param tl a </code>TrapListener</code> object instance.
 */
public void removeTrapListener(TrapListener tl)
  {
         //<Begin_removeTrapListener_TrapListener> 
	operations.removeTrapListener(tl);

  
         //<End_removeTrapListener_TrapListener>
  } 
  
/**
 * Adds a ParserListener to the list of listeners that is notified each time 
 * a trap notification arrives.
 * The Listeners will be notified of all Trap Events from any SNMP
 * node at any point of time.So the application for receiving parsed trap event should 
 * implement TrapParserListener and register itself to this Communication instance 
 * for receiving parsed trap events.
 * 
 * @param tpl a </code>TrapParserListener</code> object instance.
 */
public void addTrapParserListener(TrapParserListener tpl)
  {
         //<Begin_addTrapParserListener_TrapParserListener> 
	operations.addParserListener(tpl);

  
         //<End_addTrapParserListener_TrapParserListener>
  } 
  
/**
 * Removes the existing TrapParserListener instance from the list of Listeners.
 * 
 * @param tpl a </code>TrapParserListener</code> object instance.
 */
public void removeTrapParserListener(TrapParserListener tpl)
  {
         //<Begin_removeTrapParserListener_TrapParserListener> 
	operations.removeParserListener(tpl);

  
         //<End_removeTrapParserListener_TrapParserListener>
  } 
  
/** 
 * For setting the portNumber after instantiating this class using default constructor.
 * This method will be used for setting the portNumber if the constructor used is 
 * Communication()/Communication(Applet applet)
 *
 * @param port an </code>int</code> value specifying the port number on which traps
 * will be received.
 */
public void setRemotePort(int port)
  {
         //<Begin_setRemotePort_int> 
	operations.setRemotePort(port);

  
         //<End_setRemotePort_int>
  } 
  
/** 
 * This method will open a v3 Entry for receiving v3 TrapEvents.
 *
 * @param userName - userName.
 *
 * @param engine_ID - Engine ID of the usm user entry to be created.
 *
 * @param auth_Protocol - The Authentication Protocol.The MD5 protocol, is 21 and for SHA protocol is 22.
 *
 * @param auth_Password - authentication passowrd for SNMPv3 authentication as string
 *
 * @param priv_Password - the SNMPv3 privacy password. The privacy password contains only
 * the password and not the localised key.
 * 
 */
public void openv3Entry(String userName, String engineID, String authProtocol, String authPassword, String privPassword)
  {
         //<Begin_openv3Entry_String_String_String_String_String> 
	operations.openv3Entry(userName, engineID, authProtocol, authPassword, privPassword);

  
         //<End_openv3Entry_String_String_String_String_String>
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
 * To send a get request for the the SNMP variable for the Object Identifier passed.
 * It will return the request id which may be used for mapping in case of 
 * receiving multiple responses.
 *
 * @param oid a </code>String</code> value for the oid. 
 *
 * @return an </code>int</code> value for the requestID.
 */
public int  sendGet(String oid)
  {
         //<Begin_sendGet_String> 
	return operations.sendGet(oid);
  
         //<End_sendGet_String>
  } 
  
/**
 * To send a getNext request for the the SNMP variable for the Object Identifier passed.
 * It will request to get the value of the next oid to that of passed and will return the
 * request id which may be used for mapping in case of receiving multiple responses.
 *
 * @param oid a </code>String</code> value for the oid. 
 *
 * @return an </code>int</code> value for the requestID.
 */
public int  sendGetNext(String oid)
  {
         //<Begin_sendGetNext_String> 
	return operations.sendGetNext(oid);
  
         //<End_sendGetNext_String>
  } 
  
/**
 * To send a getNextList request for the the SNMP variable for the list of Object Identifier passed.
 * It will request to get the value of the next oids to that of passed ones and will return the
 * request id which may be used for mapping in case of receiving multiple responses.
 *
 * @param oid a </code>String</code> value for the oid. 
 *
 * @return an </code>int</code> value for the requestID.
 */
public int  sendGetNextList(String[] oids)
  {
         //<Begin_sendGetNextList_String[]> 
	return operations.sendGetNextList(oids);
  
         //<End_sendGetNextList_String[]>
  } 
  
/**
 * To send a getBulk request for the the SNMP variable for the Object Identifier passed.
 * It will request to get the response in bulk form for the passed oid and will return the
 * request id which may be used for mapping in case of receiving multiple responses.
 *
 * @param oid a </code>String</code> value for the oid. 
 *
 * @return an </code>int</code> value for the requestID.
 */
public int  sendGetBulk(String oid)
  {
         //<Begin_sendGetBulk_String> 
	return operations.sendGetBulk(oid);
  
         //<End_sendGetBulk_String>
  } 
  
/**
 * To send a getBulkList request for the the SNMP variables for the list of Object Identifier passed.
 * It will request to get the response in bulk form for the passed oids and will return the
 * request id which may be used for mapping in case of receiving multiple responses.
 *
 * @param oid a </code>String</code> value for the oid.
 * 
 * @return an </code>int</code> value for the requestID.
 */
public int  sendGetBulkList(String[] oids)
  {
         //<Begin_sendGetBulkList_String[]> 
	return operations.sendGetBulkList(oids);
  
         //<End_sendGetBulkList_String[]>
  } 
  
/**
 * To send a set request for the the SNMP variable for the Object Identifier passed.
 * It will return the request id which may be used for mapping in case of receiving multiple responses.
 *
 * @param oid a </code>String</code> value for the oid.
 * 
 * @param value a </code>String</code> value to be set.
 *
 * @return an </code>int</code> value for the requestID.
 */
public  int sendSet(String oid, String value)
  {
         //<Begin_sendSet_String_String> 
	return operations.sendSet(oid, value);
  
         //<End_sendSet_String_String>
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
	operations.loadMibModule("./mibs/IF-MIB ./mibs/IANAifType-MIB ./mibs/RFC1213-MIB ./mibs/SNMPv2-MIB");
  
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
 * Adds a Listener to the list that is notified each time a response arrives.
 * The Listeners will receive Result Events from any or all SNMP requests. Targeting 
 * responses to specific listeners, requires using RequestEvent and the setDestination 
 * method while creating the request.
 *
 * @param rl a </code>ResultListener</code> object instance.
 */public void addResultListener(ResultListener client)
  {
         //<Begin_addResultListener_ResultListener> 
	operations.addResultListener(client);
  
         //<End_addResultListener_ResultListener>
  } 
  
/**
 * Removes the specific ResultListener from the list that of listeners.
 *
 * @param client a </code>ResultListener</code> object instance.
 */public void removeResultListener(ResultListener client)
  {
         //<Begin_removeResultListener_ResultListener> 
	operations.removeResultListener(client);
  
         //<End_removeResultListener_ResultListener>
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
 * Sends a multiGet request, for the ObjectIDs passed.
 * This will send a single request for all the scalar oids passed as argument and 
 * will return an int value as the requestID for the request sent.
 * 
 * @param oids a </code>String[]</code> specifying the list of oids.
 * 
 * @return an </code>int</code> value as the request ID which may be used during receiving
 * response in case of multiple request sent.
 */
public int sendGetList(String oids[])
  {
//<Begin_sendGetList_String[]> 
return operations.sendGetList(oids);

  
//<End_sendGetList_String[]>
  } 
  
/**
 * Returns the a Table model instance for the OID specified.
 * This method returns a tablemodel instance which can be used for building SnmpTable related
 * ui applications.
 * 
 * @return a </code>SnmpTableModel</code> instance for the ObjectID specified.
 */
public SnmpTableModel getTableModel(String tableOID)
  {
//<Begin_getTableModel_String> 
return operations.getTableModel(tableOID);

  
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
public SnmpTableModel getColumnModel(String tableOID[])
  {
//<Begin_getColumnModel_String[]> 
return operations.getColumnModel(tableOID);

  
//<End_getColumnModel_String[]>
  } 
  
/**
 * Sets the TrapParsing functionality.
 * For applications receiving parsedTrap events this method has to be called after 
 * instantiating Communication class with portNumber as argument. 
 * Before adding the TrapParserListener this method should be called.
 */
public void setTrapParser() 
  {
         //<Begin_setTrapParser> 
	operations.setParser();
  
         //<End_setTrapParser>
  } 
  
/**
 * For creating the parser file taking all TrapParsing parameters which 
 * will be used for parsing trapEvents before receiving.
 * This method will take the full path of the .parser file where
 * it has to be created, and all parsing parameters based on which the file 
 * will be created.
 *
 * @param parserFile a </code>String</code> value specifying the full path of the .parser file,
 * to be generated for any TrapParser application.
 *
 * @param parsingParams a </code>String[]</code> containing all trap parsing parameters
 * @param color a </code>Color</code> object.
 */
 public void setParsingParameters(String parserFile, String[] parsingParams, Color color) 
  {
         //<Begin_setParsingParameters_String_String[]_Color> 
	operations.setParsingParameters(parserFile, parsingParams, color);
  
         //<End_setParsingParameters_String_String[]_Color>
  } 
  
/**
 * To parse the trapEvent and fire it again.
 * This method will be used inside the implemented method for receiving TrapEvent.
 * There it will take the trapEvent as argument, parse it and fire
 * again so that the parsed trap event can be received by TrapParserListener implemented
 * method.
 *
 * @param event a </code>TrapEvent</code> to be parsed.
 */
public void parseTrapEvent(TrapEvent event) 
  {
         //<Begin_parseTrapEvent_TrapEvent> 
	operations.parseEvtAndFire(event);
  
         //<End_parseTrapEvent_TrapEvent>
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
 * Determines whether debug output during receiving any trapEvent will be printed or not.
 * 
 * @param debug a </code>boolean</code> value which will decide whether
 * the trap debug output will be printed in the console or not.
 */
public void setDebugForTrap(boolean debug) 
  {
         //<Begin_setDebugForTrap_boolean> 
	operations.setTrapDebug(debug);
  
         //<End_setDebugForTrap_boolean>
  } 
  
/**
 * Returns the error string associated with trap receiver if the port is already in use.
 * This method should be called in the application before start receiving trap.
 * 
 * @return a </code>String</code> value specifying the error message.
 */
public String getTrapError()
  {
         //<Begin_getTrapError> 
	return operations.getTrapError();
  
         //<End_getTrapError>
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
