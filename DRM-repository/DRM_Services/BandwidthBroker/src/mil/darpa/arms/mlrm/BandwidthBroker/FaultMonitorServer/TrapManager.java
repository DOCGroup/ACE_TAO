package mil.darpa.arms.mlrm.BandwidthBroker.FaultMonitorServer;

import com.adventnet.snmp.studio.api.*;
import  com.adventnet.snmp.beans.*;
import  java.applet.*;
import  java.awt.*;


public class TrapManager
{
	//<Begin_Variable_Declarations>
	Communication comm = null;
	//<End_Variable_Declarations>

  
/**
 * Default constructor to instantiate TrapManager.
 * This constructor can be used to get the TrapManager instance for creating any application.
 * to receive trap, at the port specified as argument.
 *
 * @param port an </code>int</code> value specifying the port number.
 */
public TrapManager(int port)
  {
//<Begin_TrapManager_int> 
comm = new Communication(port);

  
//<End_TrapManager_int>
  } 
  
/**
 * Applet constructor to instantiate TrapManager.
 * This constructor can be used to get the TrapManager instance for creating an applet.
 * to receive trap, at the port specified as argument.
 *
 * @param port an </code>int</code> value specifying the port number.
 * 
 * @param applet an </code>Applet</code> instance.*/
public TrapManager(int port,Applet applet)
  {
//<Begin_TrapManager_int_Applet> 
comm = new Communication(port,applet);

  
//<End_TrapManager_int_Applet>
  } 
  
/**
 * Add a listener to the list that's notified each time when ever a trapEvent arrives.
 * TrapApplication using this class instance to be notified about the trap events have to implement
 * </code>TrapListener</code> interface and pass this reference as argument through this method to set 
 * in this class instance as a listener.
 * 
 * @param tl a </code>TrapListener</code> instance reference.
 */
public void addTrapListener(TrapListener tl)
  {
         //<Begin_addTrapListener_TrapListener> 
	comm.addTrapListener(tl);
  
         //<End_addTrapListener_TrapListener>
  } 
  public void removeTrapListener(TrapListener tl)
  {
         //<Begin_removeTrapListener_TrapListener> 
	comm.removeTrapListener(tl);
  
         //<End_removeTrapListener_TrapListener>
  } 
  
/**
 * Add a listener to the list that's notified each time when ever a parsed trap Event arrives.
 * TrapApplication using this class instance to be notified about the trap events after parsing have to implement
 * </code>TrapParserListener</code> interface and pass this reference as argument through this method to set 
 * in this class instance as a listener.
 * 
 * @param tpl a </code>TrapParserListener</code> instance reference.
 */
public void addTrapParserListener(TrapParserListener tpl)
  {
         //<Begin_addTrapParserListener_TrapParserListener> 
	comm.addTrapParserListener(tpl);

  
         //<End_addTrapParserListener_TrapParserListener>
  } 
  
/**
 * Removes the listener from the list of listeners.
 * 
 * @param tpl a </code>TrapParserListener</code> instance reference.
 */
public void removeTrapParserListener(TrapParserListener tpl)
  {
         //<Begin_removeTrapParserListener_TrapParserListener> 
	comm.removeTrapParserListener(tpl);

  
         //<End_removeTrapParserListener_TrapParserListener>
  } 
  public void setRemotePort(int port)
  {
         //<Begin_setRemotePort_int> 
	comm.setRemotePort(port);
  
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
	comm.openv3Entry(userName, engineID, authProtocol, authPassword, privPassword);

  
         //<End_openv3Entry_String_String_String_String_String>
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
	return comm.getTrapError();
  
         //<End_getTrapError>
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
	comm.setTrapParser();
  
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
 *
 * @param color a </code>Color</code> object.
 */
 public void setParsingParameters(String parserFile, String[] parsingParams, Color color) 
  {
         //<Begin_setParsingParameters_String_String[]_Color> 
	comm.setParsingParameters(parserFile, parsingParams, color);
  
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
	comm.parseTrapEvent(event);
  
         //<End_parseTrapEvent_TrapEvent>
  } 
  
/**
 * Determines whether debug output during receiving any trapEvent will be printed or not.
 * 
 * @param debug a </code>boolean</code> value which will decide whether
 * the trap debug output will be printed in the console or not.
 */
public void setDebug(boolean debug) 
  {
         //<Begin_setDebug_boolean> 
	comm.setDebugForTrap(debug);
  
         //<End_setDebug_boolean>
  } 

} 
