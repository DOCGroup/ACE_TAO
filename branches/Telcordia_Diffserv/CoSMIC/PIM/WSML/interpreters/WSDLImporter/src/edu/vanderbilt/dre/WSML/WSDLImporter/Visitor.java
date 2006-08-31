package edu.vanderbilt.dre.WSML.WSDLImporter;

import javax.wsdl.*;
import javax.wsdl.extensions.soap.*;
import javax.wsdl.extensions.schema.*;

public interface Visitor
{
  public void visitBinding (Binding binding);
  public void visitBindingFault (BindingFault bindingFault);
  public void visitBindingInput (BindingInput bindingInput);
  public void visitBindingOperation (BindingOperation bindingOperation);
  public void visitBindingOutput (BindingOutput bindingOutput);
  public void visitDefinition (Definition definition);
  public void visitFault (Fault fault);
  public void visitImport (Import imports);
  public void visitInput (Input input);
  public void visitMessage (Message message);
  public void visitOperation (Operation operation);
  public void visitOutput (Output output);
  public void visitPart (Part part);
  public void visitPort (Port port);
  public void visitPortType (PortType portType);
  public void visitService (Service service);
  public void visitTypes (Types types);
  public void visitSOAPAddress (SOAPAddress soapAddress);
  public void visitSOAPBinding (SOAPBinding soapBinding);
  public void visitSOAPBody (SOAPBody soapBody);
  public void visitSOAPFault (SOAPFault soapFault);
  public void visitSOAPHeader (SOAPHeader soapHeader);
  public void visitSOAPHeaderFault (SOAPHeaderFault soapHeaderFault);
  public void visitSOAPOperation (SOAPOperation soapOperation);
  public void visitSchema (Schema schema);
  public void visitSchemaImport (SchemaImport schemaImport);
  public void visitSchemaReference (SchemaReference schemaReference);
}
