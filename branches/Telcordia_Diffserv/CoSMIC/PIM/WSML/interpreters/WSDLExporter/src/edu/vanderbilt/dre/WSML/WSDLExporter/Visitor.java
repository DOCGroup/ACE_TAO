package edu.vanderbilt.dre.WSML.WSDLExporter;

import edu.vanderbilt.isis.wsml.*;
import edu.vanderbilt.isis.udm.*;

public interface Visitor
{
  public void visitRootFolder (RootFolder folder) throws UdmException;
  public void visitSchemaBuiltins (SchemaBuiltins builtins) throws UdmException;
  public void visitDefinitions (Definitions def) throws UdmException;
  public void visitImport (Import imp) throws UdmException;
  public void visitTypes (Types types) throws UdmException;
  public void visitSchema (Schema schema) throws UdmException;
  public void visitMessages (Messages messages) throws UdmException;
  public void visitMessage (Message message) throws UdmException;
  public void visitPart (Part part) throws UdmException;
  public void visitPortType (PortType portType) throws UdmException;
  public void visitRequestResponseOperation (RequestResponseOperation oper)
                               throws UdmException;
  public void visitSolicitResponseOperation (SolicitResponseOperation oper)
                  throws UdmException;
  public void visitOneWayOperation (OneWayOperation oper)
                  throws UdmException;
  public void visitNotificationOperation (NotificationOperation oper)
                  throws UdmException;
  public void visitBinding (Binding binding) throws UdmException;
  public void visitSOAPBinding (SOAPBinding soapBinding) throws UdmException;
  public void visitBindingOperation (BindingOperation bindingOp) 
                                      throws UdmException;
  public void visitSOAPOperation (SOAPOperation soapOperation) 
          throws UdmException;
  public void visitSOAPHeader (SOAPHeader soapHeader) throws UdmException;
  public void visitSOAPHeaderFault (SOAPHeaderFault soapHeaderFault) 
      throws UdmException;
  public void visitSOAPBody (SOAPBody soapBody) throws UdmException;
  public void visitSOAPFault (SOAPFault soapFault) throws UdmException;
  public void visitService (Service service)  throws UdmException;
  public void visitPort (Port port) throws UdmException;
  public void visitSOAPAddress (SOAPAddress addr) throws UdmException;
}
