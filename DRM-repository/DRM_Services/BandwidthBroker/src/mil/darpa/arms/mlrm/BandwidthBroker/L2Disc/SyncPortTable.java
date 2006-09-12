package mil.darpa.arms.mlrm.BandwidthBroker.L2Disc;

import com.adventnet.snmp.studio.api.*;
import com.adventnet.utils.*;
import java.util.*;
import com.adventnet.snmp.beans.*;
import  java.applet.* ;


public class SyncPortTable
{
	//<Begin_Variable_Declarations>
	Communication comm = null;
	//<End_Variable_Declarations>

  
/**
 * Default constructor to instantiate this class.
 * This constructor can be used to get this class instance for creating any application.
 */
public SyncPortTable()
  {
         //<Begin_SyncPortTable> 
	comm = new Communication("SYNC");
	comm.loadMibs();

  
         //<End_SyncPortTable>
  } 
  
/**
 * Constructor to instantiate an applet instance of this class.
 * This constructor can be used for creating an applet.
 * 
 * @param applet an </code>Applet</code> instance.
 */
public SyncPortTable(Applet applet)
  {
         //<Begin_SyncPortTable_Applet> 
	comm = new Communication(applet,"SYNC");
	comm.loadMibs();

  
         //<End_SyncPortTable_Applet>
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
 * Gets the response from portTable for the corresponding row index.
 * 
 * @param row an </code>int</code> value specifying the row index of the table.
 * 
 * @return a </code>String[]</code> for the row values of the table.
 */
public String[] getPortTableRow(int row)
  {
//<Begin_getPortTableRow_int> 
return comm.getRow(".1.3.6.1.4.1.9.5.1.4.1", row);
  
//<End_getPortTableRow_int>
  } 
  
/**
 * Returns the portName column values.
 * 
 * @return the response for portName in the form of </code>String[]</code> .
 */
public String[] getPortName()
  {
//<Begin_getPortName> 
return comm.getColumn("portName");
  
//<End_getPortName>
  } 
  
/**
 * Returns the portOperStatus column values.
 * 
 * @return the response for portOperStatus in the form of </code>String[]</code> .
 */
public String[] getPortOperStatus()
  {
//<Begin_getPortOperStatus> 
return comm.getColumn("portOperStatus");
  
//<End_getPortOperStatus>
  } 
  
/** 
 * Returns the portOperStatus column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portOperStatuscolumn.
 */
public int[] getPortOperStatusValue()
  {
//<Begin_getPortOperStatusValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.6");
  
//<End_getPortOperStatusValue>
  } 
  
/**
 * Returns the portAdditionalOperStatus column values.
 * 
 * @return the response for portAdditionalOperStatus in the form of </code>String[]</code> .
 */
public String[] getPortAdditionalOperStatus()
  {
//<Begin_getPortAdditionalOperStatus> 
return comm.getColumn("portAdditionalOperStatus");
  
//<End_getPortAdditionalOperStatus>
  } 
  
/**
 * Returns the portAdditionalStatus column values.
 * 
 * @return the response for portAdditionalStatus in the form of </code>String[]</code> .
 */
public String[] getPortAdditionalStatus()
  {
//<Begin_getPortAdditionalStatus> 
return comm.getColumn("portAdditionalStatus");
  
//<End_getPortAdditionalStatus>
  } 
  
/** 
 * Returns the portAdditionalStatus column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portAdditionalStatuscolumn.
 */
public int[] getPortAdditionalStatusValue()
  {
//<Begin_getPortAdditionalStatusValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.8");
  
//<End_getPortAdditionalStatusValue>
  } 
  
/**
 * Returns the portInlinePowerDetect column values.
 * 
 * @return the response for portInlinePowerDetect in the form of </code>String[]</code> .
 */
public String[] getPortInlinePowerDetect()
  {
//<Begin_getPortInlinePowerDetect> 
return comm.getColumn("portInlinePowerDetect");
  
//<End_getPortInlinePowerDetect>
  } 
  
/** 
 * Returns the portInlinePowerDetect column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portInlinePowerDetectcolumn.
 */
public int[] getPortInlinePowerDetectValue()
  {
//<Begin_getPortInlinePowerDetectValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.24");
  
//<End_getPortInlinePowerDetectValue>
  } 
  
/**
 * Returns the portMacControlPauseReceiveFrames column values.
 * 
 * @return the response for portMacControlPauseReceiveFrames in the form of </code>String[]</code> .
 */
public String[] getPortMacControlPauseReceiveFrames()
  {
//<Begin_getPortMacControlPauseReceiveFrames> 
return comm.getColumn("portMacControlPauseReceiveFrames");
  
//<End_getPortMacControlPauseReceiveFrames>
  } 
  
/**
 * Returns the portMacControlPauseReceiveFrames column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the portMacControlPauseReceiveFramescolumn.
 */
public long[] getPortMacControlPauseReceiveFramesValue()
  {
//<Begin_getPortMacControlPauseReceiveFramesValue> 

return comm.getColumnValuesLong(".1.3.6.1.4.1.9.5.1.4.1.1.20");
  
//<End_getPortMacControlPauseReceiveFramesValue>
  } 
  
/**
 * Returns the portType column values.
 * 
 * @return the response for portType in the form of </code>String[]</code> .
 */
public String[] getPortType()
  {
//<Begin_getPortType> 
return comm.getColumn("portType");
  
//<End_getPortType>
  } 
  
/** 
 * Returns the portType column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portTypecolumn.
 */
public int[] getPortTypeValue()
  {
//<Begin_getPortTypeValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.5");
  
//<End_getPortTypeValue>
  } 
  
/**
 * Returns the portMacControlReceiveFrames column values.
 * 
 * @return the response for portMacControlReceiveFrames in the form of </code>String[]</code> .
 */
public String[] getPortMacControlReceiveFrames()
  {
//<Begin_getPortMacControlReceiveFrames> 
return comm.getColumn("portMacControlReceiveFrames");
  
//<End_getPortMacControlReceiveFrames>
  } 
  
/**
 * Returns the portMacControlReceiveFrames column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the portMacControlReceiveFramescolumn.
 */
public long[] getPortMacControlReceiveFramesValue()
  {
//<Begin_getPortMacControlReceiveFramesValue> 

return comm.getColumnValuesLong(".1.3.6.1.4.1.9.5.1.4.1.1.18");
  
//<End_getPortMacControlReceiveFramesValue>
  } 
  
/**
 * Returns the portModuleIndex column values.
 * 
 * @return the response for portModuleIndex in the form of </code>String[]</code> .
 */
public String[] getPortModuleIndex()
  {
//<Begin_getPortModuleIndex> 
return comm.getColumn("portModuleIndex");
  
//<End_getPortModuleIndex>
  } 
  
/** 
 * Returns the portModuleIndex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portModuleIndexcolumn.
 */
public int[] getPortModuleIndexValue()
  {
//<Begin_getPortModuleIndexValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.1");
  
//<End_getPortModuleIndexValue>
  } 
  
/**
 * Returns the portMacControlTransmitFrames column values.
 * 
 * @return the response for portMacControlTransmitFrames in the form of </code>String[]</code> .
 */
public String[] getPortMacControlTransmitFrames()
  {
//<Begin_getPortMacControlTransmitFrames> 
return comm.getColumn("portMacControlTransmitFrames");
  
//<End_getPortMacControlTransmitFrames>
  } 
  
/**
 * Returns the portMacControlTransmitFrames column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the portMacControlTransmitFramescolumn.
 */
public long[] getPortMacControlTransmitFramesValue()
  {
//<Begin_getPortMacControlTransmitFramesValue> 

return comm.getColumnValuesLong(".1.3.6.1.4.1.9.5.1.4.1.1.17");
  
//<End_getPortMacControlTransmitFramesValue>
  } 
  
/**
 * Returns the portMacControlUnknownProtocolFrames column values.
 * 
 * @return the response for portMacControlUnknownProtocolFrames in the form of </code>String[]</code> .
 */
public String[] getPortMacControlUnknownProtocolFrames()
  {
//<Begin_getPortMacControlUnknownProtocolFrames> 
return comm.getColumn("portMacControlUnknownProtocolFrames");
  
//<End_getPortMacControlUnknownProtocolFrames>
  } 
  
/**
 * Returns the portMacControlUnknownProtocolFrames column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the portMacControlUnknownProtocolFramescolumn.
 */
public long[] getPortMacControlUnknownProtocolFramesValue()
  {
//<Begin_getPortMacControlUnknownProtocolFramesValue> 

return comm.getColumnValuesLong(".1.3.6.1.4.1.9.5.1.4.1.1.21");
  
//<End_getPortMacControlUnknownProtocolFramesValue>
  } 
  
/**
 * Returns the portOperRxFlowControl column values.
 * 
 * @return the response for portOperRxFlowControl in the form of </code>String[]</code> .
 */
public String[] getPortOperRxFlowControl()
  {
//<Begin_getPortOperRxFlowControl> 
return comm.getColumn("portOperRxFlowControl");
  
//<End_getPortOperRxFlowControl>
  } 
  
/** 
 * Returns the portOperRxFlowControl column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portOperRxFlowControlcolumn.
 */
public int[] getPortOperRxFlowControlValue()
  {
//<Begin_getPortOperRxFlowControlValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.14");
  
//<End_getPortOperRxFlowControlValue>
  } 
  
/**
 * Returns the portAdminSpeed column values.
 * 
 * @return the response for portAdminSpeed in the form of </code>String[]</code> .
 */
public String[] getPortAdminSpeed()
  {
//<Begin_getPortAdminSpeed> 
return comm.getColumn("portAdminSpeed");
  
//<End_getPortAdminSpeed>
  } 
  
/** 
 * Returns the portAdminSpeed column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portAdminSpeedcolumn.
 */
public int[] getPortAdminSpeedValue()
  {
//<Begin_getPortAdminSpeedValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.9");
  
//<End_getPortAdminSpeedValue>
  } 
  
/**
 * Returns the portCrossIndex column values.
 * 
 * @return the response for portCrossIndex in the form of </code>String[]</code> .
 */
public String[] getPortCrossIndex()
  {
//<Begin_getPortCrossIndex> 
return comm.getColumn("portCrossIndex");
  
//<End_getPortCrossIndex>
  } 
  
/** 
 * Returns the portCrossIndex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portCrossIndexcolumn.
 */
public int[] getPortCrossIndexValue()
  {
//<Begin_getPortCrossIndexValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.3");
  
//<End_getPortCrossIndexValue>
  } 
  
/**
 * Returns the portAdminRxFlowControl column values.
 * 
 * @return the response for portAdminRxFlowControl in the form of </code>String[]</code> .
 */
public String[] getPortAdminRxFlowControl()
  {
//<Begin_getPortAdminRxFlowControl> 
return comm.getColumn("portAdminRxFlowControl");
  
//<End_getPortAdminRxFlowControl>
  } 
  
/** 
 * Returns the portAdminRxFlowControl column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portAdminRxFlowControlcolumn.
 */
public int[] getPortAdminRxFlowControlValue()
  {
//<Begin_getPortAdminRxFlowControlValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.13");
  
//<End_getPortAdminRxFlowControlValue>
  } 
  
/**
 * Returns the portEntPhysicalIndex column values.
 * 
 * @return the response for portEntPhysicalIndex in the form of </code>String[]</code> .
 */
public String[] getPortEntPhysicalIndex()
  {
//<Begin_getPortEntPhysicalIndex> 
return comm.getColumn("portEntPhysicalIndex");
  
//<End_getPortEntPhysicalIndex>
  } 
  
/** 
 * Returns the portEntPhysicalIndex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portEntPhysicalIndexcolumn.
 */
public int[] getPortEntPhysicalIndexValue()
  {
//<Begin_getPortEntPhysicalIndexValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.25");
  
//<End_getPortEntPhysicalIndexValue>
  } 
  
/**
 * Returns the portSpantreeFastStart column values.
 * 
 * @return the response for portSpantreeFastStart in the form of </code>String[]</code> .
 */
public String[] getPortSpantreeFastStart()
  {
//<Begin_getPortSpantreeFastStart> 
return comm.getColumn("portSpantreeFastStart");
  
//<End_getPortSpantreeFastStart>
  } 
  
/** 
 * Returns the portSpantreeFastStart column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portSpantreeFastStartcolumn.
 */
public int[] getPortSpantreeFastStartValue()
  {
//<Begin_getPortSpantreeFastStartValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.12");
  
//<End_getPortSpantreeFastStartValue>
  } 
  
/**
 * Returns the portMacControlPauseTransmitFrames column values.
 * 
 * @return the response for portMacControlPauseTransmitFrames in the form of </code>String[]</code> .
 */
public String[] getPortMacControlPauseTransmitFrames()
  {
//<Begin_getPortMacControlPauseTransmitFrames> 
return comm.getColumn("portMacControlPauseTransmitFrames");
  
//<End_getPortMacControlPauseTransmitFrames>
  } 
  
/**
 * Returns the portMacControlPauseTransmitFrames column values in it's actual form.
 *
 * @return a </code>long[]</code> for the as response for 
 * the portMacControlPauseTransmitFramescolumn.
 */
public long[] getPortMacControlPauseTransmitFramesValue()
  {
//<Begin_getPortMacControlPauseTransmitFramesValue> 

return comm.getColumnValuesLong(".1.3.6.1.4.1.9.5.1.4.1.1.19");
  
//<End_getPortMacControlPauseTransmitFramesValue>
  } 
  
/**
 * Returns the portCrossGroupIndex column values.
 * 
 * @return the response for portCrossGroupIndex in the form of </code>String[]</code> .
 */
public String[] getPortCrossGroupIndex()
  {
//<Begin_getPortCrossGroupIndex> 
return comm.getColumn("portCrossGroupIndex");
  
//<End_getPortCrossGroupIndex>
  } 
  
/** 
 * Returns the portCrossGroupIndex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portCrossGroupIndexcolumn.
 */
public int[] getPortCrossGroupIndexValue()
  {
//<Begin_getPortCrossGroupIndexValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.7");
  
//<End_getPortCrossGroupIndexValue>
  } 
  
/**
 * Returns the portDuplex column values.
 * 
 * @return the response for portDuplex in the form of </code>String[]</code> .
 */
public String[] getPortDuplex()
  {
//<Begin_getPortDuplex> 
return comm.getColumn("portDuplex");
  
//<End_getPortDuplex>
  } 
  
/** 
 * Returns the portDuplex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portDuplexcolumn.
 */
public int[] getPortDuplexValue()
  {
//<Begin_getPortDuplexValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.10");
  
//<End_getPortDuplexValue>
  } 
  
/**
 * Returns the portIndex column values.
 * 
 * @return the response for portIndex in the form of </code>String[]</code> .
 */
public String[] getPortIndex()
  {
//<Begin_getPortIndex> 
return comm.getColumn("portIndex");
  
//<End_getPortIndex>
  } 
  
/** 
 * Returns the portIndex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portIndexcolumn.
 */
public int[] getPortIndexValue()
  {
//<Begin_getPortIndexValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.2");
  
//<End_getPortIndexValue>
  } 
  
/**
 * Returns the portOperTxFlowControl column values.
 * 
 * @return the response for portOperTxFlowControl in the form of </code>String[]</code> .
 */
public String[] getPortOperTxFlowControl()
  {
//<Begin_getPortOperTxFlowControl> 
return comm.getColumn("portOperTxFlowControl");
  
//<End_getPortOperTxFlowControl>
  } 
  
/** 
 * Returns the portOperTxFlowControl column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portOperTxFlowControlcolumn.
 */
public int[] getPortOperTxFlowControlValue()
  {
//<Begin_getPortOperTxFlowControlValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.16");
  
//<End_getPortOperTxFlowControlValue>
  } 
  
/**
 * Returns the portErrDisableTimeOutEnable column values.
 * 
 * @return the response for portErrDisableTimeOutEnable in the form of </code>String[]</code> .
 */
public String[] getPortErrDisableTimeOutEnable()
  {
//<Begin_getPortErrDisableTimeOutEnable> 
return comm.getColumn("portErrDisableTimeOutEnable");
  
//<End_getPortErrDisableTimeOutEnable>
  } 
  
/** 
 * Returns the portErrDisableTimeOutEnable column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portErrDisableTimeOutEnablecolumn.
 */
public int[] getPortErrDisableTimeOutEnableValue()
  {
//<Begin_getPortErrDisableTimeOutEnableValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.26");
  
//<End_getPortErrDisableTimeOutEnableValue>
  } 
  
/**
 * Returns the portLinkFaultStatus column values.
 * 
 * @return the response for portLinkFaultStatus in the form of </code>String[]</code> .
 */
public String[] getPortLinkFaultStatus()
  {
//<Begin_getPortLinkFaultStatus> 
return comm.getColumn("portLinkFaultStatus");
  
//<End_getPortLinkFaultStatus>
  } 
  
/** 
 * Returns the portLinkFaultStatus column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portLinkFaultStatuscolumn.
 */
public int[] getPortLinkFaultStatusValue()
  {
//<Begin_getPortLinkFaultStatusValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.22");
  
//<End_getPortLinkFaultStatusValue>
  } 
  
/**
 * Returns the portAdminTxFlowControl column values.
 * 
 * @return the response for portAdminTxFlowControl in the form of </code>String[]</code> .
 */
public String[] getPortAdminTxFlowControl()
  {
//<Begin_getPortAdminTxFlowControl> 
return comm.getColumn("portAdminTxFlowControl");
  
//<End_getPortAdminTxFlowControl>
  } 
  
/** 
 * Returns the portAdminTxFlowControl column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portAdminTxFlowControlcolumn.
 */
public int[] getPortAdminTxFlowControlValue()
  {
//<Begin_getPortAdminTxFlowControlValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.15");
  
//<End_getPortAdminTxFlowControlValue>
  } 
  
/**
 * Returns the portIfIndex column values.
 * 
 * @return the response for portIfIndex in the form of </code>String[]</code> .
 */
public String[] getPortIfIndex()
  {
//<Begin_getPortIfIndex> 
return comm.getColumn("portIfIndex");
  
//<End_getPortIfIndex>
  } 
  
/** 
 * Returns the portIfIndex column values in it's actual form.
 *
 * @return a </code>int[]</code> for the as response for 
 * the portIfIndexcolumn.
 */
public int[] getPortIfIndexValue()
  {
//<Begin_getPortIfIndexValue> 

return comm.getColumnValuesInt(".1.3.6.1.4.1.9.5.1.4.1.1.11");
  
//<End_getPortIfIndexValue>
  } 
  
/**
 * Returns the columnNames of the table.
 * 
 * @return the columnnames in </code>String[]</code> form.
 */
public String[] getPortTableColumns()
  {
//<Begin_getPortTableColumns> 
return comm.getColumnNames(".1.3.6.1.4.1.9.5.1.4.1");
  
//<End_getPortTableColumns>
  } 
  
/**
 * Returns the response for the whole portTable.
 * 
 * @return the response in </code>String[]</code> form.
 */
public String[][] getPortTable()
  {
//<Begin_getPortTable> 
return comm.getTable(".1.3.6.1.4.1.9.5.1.4.1");
  
//<End_getPortTable>
  } 
  
/**
 * Sends the request for portTable Table OID which will update for any table change event.
 * To use this method in the application, first the application has to implement SnmpTableListener
 * and then register itself for receiving all fired table change events by implementing
 * the methods of SnmpTableListener.
 */
public void fetchPortTable()
  {
//<Begin_fetchPortTable> 
comm.fetchTableData(".1.3.6.1.4.1.9.5.1.4.1");
  
//<End_fetchPortTable>
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
