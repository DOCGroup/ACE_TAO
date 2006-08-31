package edu.vanderbilt.dre.WSML.WSDLExporter;


import edu.vanderbilt.isis.wsml.Binding;
import edu.vanderbilt.isis.wsml.BindingOperation;
import edu.vanderbilt.isis.wsml.Import;
import edu.vanderbilt.isis.wsml.Message;
import edu.vanderbilt.isis.wsml.Operation;
import edu.vanderbilt.isis.wsml.Part;
import edu.vanderbilt.isis.wsml.Port;
import edu.vanderbilt.isis.wsml.PortType;
import edu.vanderbilt.isis.wsml.SOAPBinding;
import edu.vanderbilt.isis.wsml.SOAPBody;
import edu.vanderbilt.isis.wsml.SOAPFault;
import edu.vanderbilt.isis.wsml.SOAPHeader;
import edu.vanderbilt.isis.wsml.SOAPHeaderFault;
import edu.vanderbilt.isis.wsml.SOAPOperation;
import edu.vanderbilt.isis.wsml.Service;
import edu.vanderbilt.isis.wsml.Types;
import edu.vanderbilt.isis.wsml.Element;
import edu.vanderbilt.isis.wsml.Input;
import edu.vanderbilt.isis.wsml.Output;
import edu.vanderbilt.isis.wsml.Fault;

import edu.vanderbilt.isis.wsml.*;
import edu.vanderbilt.isis.udm.*;

import javax.xml.*;
import javax.xml.parsers.*;
import javax.xml.namespace.*;

import javax.wsdl.*;
import javax.wsdl.factory.*;
import javax.wsdl.xml.*;
import javax.wsdl.extensions.*;

import java.io.*;
import java.util.*;
import org.w3c.dom.*;


public class WSDLExportVisitor implements Visitor {

    private WSDLFactory factory = null;

    private FileOutputStream outputFile = null;

    private Definition def = null;

    private Object currParent = null;
    
    private Stack<Context> contextStack = new Stack<Context>();
    
    private Stack<Object> parentStack = new Stack<Object>();

    private Map<String, String> defNsMap = new HashMap<String,String>();

    private Map<String, String> schemaNsMap = new HashMap<String, String>();

    private Map<String, String> revSchemaNsMap = new HashMap<String, String>();

    private Map<Referrable, QName> typeMap = new HashMap<Referrable, QName>();

    private Map<Message, javax.wsdl.Message> messageMap = 
        new HashMap<Message, javax.wsdl.Message>();

    private Map<Part, javax.wsdl.Part> partMap = 
        new HashMap<Part, javax.wsdl.Part>();
    
    private Map<PortType, javax.wsdl.PortType> portTypeMap = 
        new HashMap<PortType, javax.wsdl.PortType>();

    private Map<Operation, javax.wsdl.Operation> operationMap =
        new HashMap<Operation, javax.wsdl.Operation>();
    
    private Map<Binding, javax.wsdl.Binding> bindingMap = 
        new HashMap<Binding, javax.wsdl.Binding>();
    
    private static final String W3C_SOAP_NS_URI = 
        "http://schemas.xmlsoap.org/wsdl/soap/";
    
    private ElementComparator eleComparator = new ElementComparator();
    
    private void push(Object parent) {
        this.parentStack.push (this.currParent);
        this.currParent = parent;
    }
    
    private void pop() {
        this.currParent = this.parentStack.pop();
    }

    private void pushContext (Definition def, Object currParent) {
        this.contextStack.push(new Context(def, currParent));
    }
    private void popContext () {
        Context context = this.contextStack.pop();
        this.def = context.def;
        this.currParent = context.currParent;
    }
    
    private class Context {
        private Definition def = null;
        private Object currParent = null;
        
        public Context (Definition def, Object currParent) {
            this.def = def;
            this.currParent = currParent;
        }
    }
    private class ElementComparator implements Comparator<Element> {
        public int compare (Element ele1, Element ele2) {
            long firstOrder = 0;
            long secondOrder = 0;
            try {
                firstOrder = ele1.getOrder();
                secondOrder = ele2.getOrder();
            } catch (UdmException e) {
                e.printStackTrace();
            }
            if (firstOrder < secondOrder) { 
                return -1;
            } else if (firstOrder == secondOrder) {
                return 0;
            }
            return 1;
        }
    }

    public WSDLExportVisitor (WSDLFactory factory, String outputFileName) {
        try {
            this.factory = factory;
            this.outputFile = new FileOutputStream (outputFileName);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        }
    }

    public void dump () {
        try {
            WSDLWriter writer = this.factory.newWSDLWriter();
            writer.writeWSDL (this.def, outputFile);
        } catch (WSDLException e) {
            e.printStackTrace();
        }
    }

    public void visitRootFolder (RootFolder folder) throws UdmException {
        for (SchemaBuiltins builtins: folder.getSchemaBuiltinsChildren()) {
            this.visitSchemaBuiltins (builtins);
        }
        for (Definitions wsmlDef : folder.getDefinitionsChildren()) {
            this.visitDefinitions (wsmlDef);
        }

    }

    public void visitSchemaBuiltins(SchemaBuiltins builtins)
            throws UdmException {
        for (AtomicType type : builtins.getAtomicTypeChildren()) {
            this.typeMap.put(type, new QName(XMLConstants.W3C_XML_SCHEMA_NS_URI,
                                             type.gettypeCode()));
        }
    }

    public void visitDefinitions(Definitions def) throws UdmException {
        this.def = this.factory.newDefinition();
        String localName = def.getname();
        String targetNamespace = def.gettargetNamespace();
        this.def.setTargetNamespace(targetNamespace);
        QName defQName = new QName (targetNamespace, localName);
        this.def.setQName(defQName);
        for (Attribute attr : def.getAttributeChildren()) {
            String attrName = attr.getname();
            String prefix;
            if (attrName.equals ("xmlns")) {
                prefix = null;
            } else if (attrName.startsWith("xmlns")) {
                int localIndex = attrName.lastIndexOf(":");
                prefix = attrName.substring(localIndex+1);
            } else {
                System.err.println ("Ignoring unknown extensibility attribute "
                                    + attrName + " of Definition " + localName);
                continue;
            }
            String namespace = attr.getValue();
            this.defNsMap.put (prefix, namespace);
            this.revSchemaNsMap.put (namespace, prefix);
            this.def.addNamespace (prefix, namespace);
        }

        for (Import imp : def.getImportChildren()) {
            this.visitImport(imp);
        }

        Types types = def.getTypesChild();
        this.visitTypes(types);

        Messages messages = def.getMessagesChild();
        this.visitMessages(messages);
        
        for (PortType portType : def.getPortTypeChildren()) {
            this.visitPortType (portType);
        }
        
        for (Binding binding : def.getBindingChildren()) {
            this.visitBinding(binding);
        }
        
        for (Service service : def.getServiceChildren()) {
            this.visitService(service);
        }
    }

    public void visitImport(Import imp) throws UdmException {
        javax.wsdl.Import wsdlImport = this.def.createImport();
        wsdlImport.setLocationURI(imp.getlocation());
        wsdlImport.setNamespaceURI(imp.getnamespace());
        this.def.addImport(wsdlImport);
        Definitions def = imp.getDefinitionsChild();
        this.pushContext(this.def, this.currParent);
        this.visitDefinitions(def);
        this.popContext();
    }

    public void visitTypes(Types types) throws UdmException {
        javax.wsdl.Types defTypes = this.def.createTypes();
        this.def.setTypes(defTypes);
        for (Schema schema : types.getSchemaChildren()) {
            this.visitSchema (schema);
        }
    }

    public void visitSchema(Schema schema) throws UdmException {
        String localName = schema.getname();
        for (Attribute attr : schema.getAttributeChildren()) {
            String attrName = attr.getname();
            if (attrName.equals ("xmlns")) {
                this.schemaNsMap.put(XMLConstants.DEFAULT_NS_PREFIX,
                                     attr.getValue());
            } else if (attrName.startsWith("xmlns")) {
                int localIndex = attrName.lastIndexOf(':');
                if (localIndex != -1) {
                    String nsPrefix = attrName.substring(localIndex+1);
                    String namespace = attr.getValue();
                    this.schemaNsMap.put(nsPrefix, namespace);
                    this.revSchemaNsMap.put (namespace, nsPrefix);
                }
            }
        }
        org.w3c.dom.Element docElement = null;
        try {
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            DOMImplementation domImpl = db.getDOMImplementation();
            Document doc =
                domImpl.createDocument(this.getSchemaNamespace(localName),
                                       localName,
                                       null);
            docElement = doc.getDocumentElement();
            for (Attribute attr : schema.getAttributeChildren()) {
                String attrName = attr.getname();
                if (attrName.startsWith("xmlns")) {
                    docElement.setAttributeNS(XMLConstants.XMLNS_ATTRIBUTE_NS_URI,
                                              attr.getname(), attr.getValue());
                } else {
                    docElement.setAttributeNS(this.getSchemaNamespace(attrName),
                                              attrName, attr.getValue());
                }
            }
            docElement.setAttribute("targetNamespace",
                                    schema.gettargetNamespace());
            Element[] childList = schema.getElementChildren(); 
            Arrays.sort (childList, eleComparator);
            for (Element ele : childList) {
                this.visitElement (doc, ele, docElement, 0);
            }
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        }
        QName elementType = this.createSchemaQName (localName);
        try {
            ExtensionRegistry registry = this.def.getExtensionRegistry();
            javax.wsdl.extensions.schema.Schema wsdlSchema =
                (javax.wsdl.extensions.schema.Schema)
                registry.createExtension(javax.wsdl.Types.class,
                                         elementType);
            wsdlSchema.setElement(docElement);
            this.def.getTypes().addExtensibilityElement(wsdlSchema);
        } catch (WSDLException e) {
            e.printStackTrace();
        }
    }

    private void visitElement (Document doc, Element modelEle, 
                               org.w3c.dom.Element parent,
                               int level)
        throws UdmException {
        String qName = modelEle.getname();
        String eleName = qName;
        String schemaType = modelEle.getschemaType();
        if (schemaType.equals ("complexType")
            || schemaType.equals("simpleType")
            || schemaType.equals("element")) {
            eleName =
                this.revSchemaNsMap.get(XMLConstants.W3C_XML_SCHEMA_NS_URI);
            eleName += ":" + schemaType;
            if (level == 0) {
                this.typeMap.put(modelEle, 
                                 new QName (this.def.getTargetNamespace(),
                                            qName));    
            }
        }
        org.w3c.dom.Element domEle = doc.createElement(eleName);
        for (Attribute attr : modelEle.getAttributeChildren()) {
            String attrName = attr.getname();
            domEle.setAttributeNS (this.getSchemaNamespace(attrName),
                                   attrName,
                                   attr.getValue());
        }
        parent.appendChild(domEle);
        int newLevel = level + 1;
        Element[] childList = modelEle.getElementChildren(); 
        Arrays.sort (childList, eleComparator);
        for (Element child : childList) {
            this.visitElement(doc, child, domEle, newLevel);
        }
    }

    private QName createSchemaQName (String qName) {
        String namespace = this.getSchemaNamespace (qName);
        String localName = null;
        int localIndex = qName.indexOf(':');
        if (localIndex == -1) {
            localName = qName;
        } else {
            localName = qName.substring(localIndex+1, qName.length());
        }
        return new QName(namespace, localName);
    }

    private String getSchemaNamespace (String qName) {
        int localIndex = qName.indexOf(':');
        String prefix = null;
        if (localIndex == -1) {
            prefix = XMLConstants.DEFAULT_NS_PREFIX;
        } else {
            prefix = qName.substring(0, localIndex);
        }
        String name = this.schemaNsMap.get(prefix);
        if (name == null) {
            name = this.defNsMap.get(prefix);
            if (name == null) {
                name = XMLConstants.NULL_NS_URI;
            }
        }
        return name;
    }


    public void visitMessages(Messages messages) throws UdmException {
        for (Message message : messages.getMessageChildren()) {
            this.visitMessage(message);
        }
    }

    public void visitMessage(Message message) throws UdmException {
        javax.wsdl.Message wsdlMessage = this.def.createMessage();
        wsdlMessage.setUndefined(false);
        QName msgQName = new QName (this.def.getTargetNamespace(),
                                    message.getname());
        wsdlMessage.setQName(msgQName);
        this.def.addMessage(wsdlMessage);
        this.messageMap.put(message, wsdlMessage);
        for (Part part : message.getPartChildren()) {
            this.push(wsdlMessage);
            this.visitPart (part);
            this.pop();
        }
    }

    public void visitPart(Part part) throws UdmException {
        Referrable[] referred = part.getref();
        for (Referrable ref : referred) {
            javax.wsdl.Part wsdlPart = this.def.createPart();
            wsdlPart.setName(part.getname());
            this.partMap.put(part, wsdlPart);
            if (ref instanceof Element) {
                Element refEle = (Element)ref;
                if (refEle.getschemaType().equals("element")) {
                    wsdlPart.setElementName(this.typeMap.get(ref));
                } else {
                    wsdlPart.setTypeName(this.typeMap.get(ref));
                }
            } else {
                wsdlPart.setTypeName(this.typeMap.get(ref));
            }
            javax.wsdl.Message parent = (javax.wsdl.Message)this.currParent;
            parent.addPart(wsdlPart);
        }
    }

    public void visitPortType(PortType portType) throws UdmException {
        javax.wsdl.PortType wsdlPortType = this.def.createPortType();
        wsdlPortType.setQName(new QName (this.def.getTargetNamespace(),
                                         portType.getname()));
        wsdlPortType.setUndefined(false);
        this.portTypeMap.put(portType, wsdlPortType);
        this.def.addPortType(wsdlPortType);
        for (RequestResponseOperation 
                oper : portType.getRequestResponseOperationChildren()) {
            this.push(wsdlPortType);
            this.visitRequestResponseOperation(oper);
            this.pop();
        }
        for (SolicitResponseOperation 
                oper : portType.getSolicitResponseOperationChildren()) {
            this.push(wsdlPortType);
            this.visitSolicitResponseOperation(oper);
            this.pop();
        }
        for (OneWayOperation
                oper : portType.getOneWayOperationChildren()) {
            this.push(wsdlPortType);
            this.visitOneWayOperation(oper);
            this.pop();
        }
        for (NotificationOperation 
                oper : portType.getNotificationOperationChildren()) {
            this.push(wsdlPortType);
            this.visitNotificationOperation(oper);
            this.pop();
        }
    }

    public void visitRequestResponseOperation(RequestResponseOperation oper)
        throws UdmException {
        javax.wsdl.Operation wsdlOperation = this.def.createOperation();
        wsdlOperation.setName(oper.getname());
        wsdlOperation.setUndefined(false);
        wsdlOperation.setStyle(OperationType.REQUEST_RESPONSE);
        this.operationMap.put(oper, wsdlOperation);
        Input input = oper.getInputChild();
        for (InputMessage inputMessage : input.getdstInputMessage()) {
            for (Message message : inputMessage.getdstInputMessage().getref()) {
                javax.wsdl.Input wsdlInput = this.def.createInput();
                wsdlInput.setName(input.getname());
                wsdlInput.setMessage(this.messageMap.get(message));
                wsdlOperation.setInput(wsdlInput);
            }
        }
        Output output = oper.getOutputChild();
        for (OutputMessage outputMessage : output.getdstOutputMessage()) {
            for (Message message : outputMessage.getdstOutputMessage().getref()) {
                javax.wsdl.Output wsdlOutput = this.def.createOutput();
                wsdlOutput.setName(output.getname());
                wsdlOutput.setMessage(this.messageMap.get(message));
                wsdlOperation.setOutput(wsdlOutput);
            }
        }
        for (Fault fault : oper.getFaultChildren()) {
            for (FaultMessage faultMessage : fault.getdstFaultMessage()) {
                for (Message message : faultMessage.getdstFaultMessage().getref()) {
                    javax.wsdl.Fault wsdlFault = this.def.createFault();
                    wsdlFault.setName(fault.getname());
                    wsdlFault.setMessage(this.messageMap.get(message));
                    wsdlOperation.addFault(wsdlFault);
                }
            }
        }
        javax.wsdl.PortType parent = (javax.wsdl.PortType)this.currParent;
        parent.addOperation(wsdlOperation);
    }
    
    public void visitNotificationOperation(NotificationOperation oper) 
             throws UdmException {
        javax.wsdl.Operation wsdlOperation = this.def.createOperation();
        wsdlOperation.setName(oper.getname());
        wsdlOperation.setUndefined(false);
        wsdlOperation.setStyle(OperationType.NOTIFICATION);
        this.operationMap.put(oper, wsdlOperation);
        Output output = oper.getOutputChild();
        for (OutputMessage outputMessage : output.getdstOutputMessage()) {
            for (Message message : outputMessage.getdstOutputMessage().getref()) {
                javax.wsdl.Output wsdlOutput = this.def.createOutput();
                wsdlOutput.setName(output.getname());
                wsdlOutput.setMessage(this.messageMap.get(message));
                wsdlOperation.setOutput(wsdlOutput);
            }
        }
        javax.wsdl.PortType parent = (javax.wsdl.PortType)this.currParent;
        parent.addOperation(wsdlOperation);
    }

    public void visitOneWayOperation(OneWayOperation oper) 
        throws UdmException {
        javax.wsdl.Operation wsdlOperation = this.def.createOperation();
        wsdlOperation.setName(oper.getname());
        wsdlOperation.setUndefined(false);
        wsdlOperation.setStyle(OperationType.ONE_WAY);
        this.operationMap.put(oper, wsdlOperation);
        Input input = oper.getInputChild();
        for (InputMessage inputMessage : input.getdstInputMessage()) {
            for (Message message : inputMessage.getdstInputMessage().getref()) {
                javax.wsdl.Input wsdlInput = this.def.createInput();
                wsdlInput.setName(input.getname());
                wsdlInput.setMessage(this.messageMap.get(message));
                wsdlOperation.setInput(wsdlInput);
            }
        }
        javax.wsdl.PortType parent = (javax.wsdl.PortType)this.currParent;
        parent.addOperation(wsdlOperation);
    }

    public void visitSolicitResponseOperation(SolicitResponseOperation oper)
         throws UdmException {
        javax.wsdl.Operation wsdlOperation = this.def.createOperation();
        wsdlOperation.setName(oper.getname());
        wsdlOperation.setUndefined(false);
        wsdlOperation.setStyle(OperationType.SOLICIT_RESPONSE);
        this.operationMap.put(oper, wsdlOperation);
        Input input = oper.getInputChild();
        for (InputMessage inputMessage : input.getdstInputMessage()) {
            for (Message message : inputMessage.getdstInputMessage().getref()) {
                javax.wsdl.Input wsdlInput = this.def.createInput();
                wsdlInput.setName(input.getname());
                wsdlInput.setMessage(this.messageMap.get(message));
                wsdlOperation.setInput(wsdlInput);
            }
        }
        Output output = oper.getOutputChild();
        for (OutputMessage outputMessage : output.getdstOutputMessage()) {
            for (Message message : outputMessage.getdstOutputMessage().getref()) {
                javax.wsdl.Output wsdlOutput = this.def.createOutput();
                wsdlOutput.setName(output.getname());
                wsdlOutput.setMessage(this.messageMap.get(message));
                wsdlOperation.setOutput(wsdlOutput);
            }
        }
        for (Fault fault : oper.getFaultChildren()) {
            for (FaultMessage faultMessage : fault.getdstFaultMessage()) {
                for (Message message : faultMessage.getdstFaultMessage().getref()) {
                    javax.wsdl.Fault wsdlFault = this.def.createFault();
                    wsdlFault.setName(fault.getname());
                    wsdlFault.setMessage(this.messageMap.get(message));
                    wsdlOperation.addFault(wsdlFault);
                }
            }
        }
        javax.wsdl.PortType parent = (javax.wsdl.PortType)this.currParent;
        parent.addOperation(wsdlOperation);
    }

    public void visitBinding(Binding binding) throws UdmException {
        javax.wsdl.Binding wsdlBinding = this.def.createBinding();
        wsdlBinding.setQName(new QName (this.def.getTargetNamespace(),
                                        binding.getname()));
        wsdlBinding.setUndefined(false);
        this.def.addBinding(wsdlBinding);
        this.bindingMap.put(binding, wsdlBinding);
        for (BindingPortType bPortType : binding.getdstBindingPortType()) {
            PortType portType = bPortType.getdstBindingPortType();
            wsdlBinding.setPortType(this.portTypeMap.get(portType));
        }
        SOAPBinding soapBinding = binding.getSOAPBindingChild();
        this.push(wsdlBinding);
        this.visitSOAPBinding (soapBinding);
        this.pop();
                
        for (BindingOperation bindOp : binding.getBindingOperationChildren()) {
            this.push(wsdlBinding);
            this.visitBindingOperation(bindOp);
            this.pop();
        }
    }

    public void visitSOAPBinding(SOAPBinding soapBinding) throws UdmException {
        javax.wsdl.Binding wsdlBinding = (javax.wsdl.Binding)this.currParent;
        try {
            QName elementType = new QName (W3C_SOAP_NS_URI, "binding");
            ExtensionRegistry registry = this.def.getExtensionRegistry();
            javax.wsdl.extensions.soap.SOAPBinding wsdlSoapBinding
            = (javax.wsdl.extensions.soap.SOAPBinding)
            registry.createExtension(javax.wsdl.Binding.class,
                                     elementType);
            wsdlSoapBinding.setRequired(soapBinding.getrequired());
            wsdlSoapBinding.setStyle(soapBinding.getstyle());
            wsdlSoapBinding.setTransportURI(soapBinding.gettransport());
            wsdlSoapBinding.setElementType(elementType);
            wsdlBinding.addExtensibilityElement(wsdlSoapBinding);
        } catch (WSDLException e) {
            e.printStackTrace();
        }
    }

    public void visitBindingOperation(BindingOperation bindingOp) 
        throws UdmException {
        javax.wsdl.BindingOperation wsdlBindOp = 
            this.def.createBindingOperation();
        wsdlBindOp.setName(bindingOp.getname());
        SOAPOperation soapOperation = bindingOp.getSOAPOperationChild();
        
        this.push(wsdlBindOp);
        this.visitSOAPOperation(soapOperation);
        this.pop();
        
        for (BindsOperation bindsOp : bindingOp.getdstBindsOperation()) {
            OperationRef opRef = bindsOp.getdstBindsOperation();
            for (Operation oper : opRef.getref()) {
                wsdlBindOp.setOperation(this.operationMap.get(oper));
            }
        }
        
        for (InputRef inputRef : bindingOp.getInputRefChildren()) {
            for (Input input : inputRef.getref()) {
                javax.wsdl.BindingInput bindInput = this.def.createBindingInput();
                bindInput.setName(input.getname());    
                wsdlBindOp.setBindingInput(bindInput);
                for (BodyBinding bodyBinding : inputRef.getdstBodyBinding()) {
                    SOAPBody soapBody = bodyBinding.getdstBodyBinding();
                    this.push(bindInput);
                    this.visitSOAPBody(soapBody);
                    this.pop();
                }
                for (HeaderBinding headerBinding : inputRef.getdstHeaderBinding()) {
                    SOAPHeaders soapHeaders = 
                        headerBinding.getdstHeaderBinding();
                    if (soapHeaders instanceof SOAPHeader) {
                        SOAPHeader soapHeader = (SOAPHeader)soapHeaders; 
                        this.push(bindInput);
                        this.visitSOAPHeader(soapHeader);
                        this.pop();
                    } else if (soapHeaders instanceof SOAPHeaderFault) {
                        SOAPHeaderFault soapHeaderFault = 
                            (SOAPHeaderFault)soapHeaders;
                        this.push(bindInput);
                        this.visitSOAPHeaderFault(soapHeaderFault);
                        this.pop();
                    }
                }

            }
        }
        for (OutputRef outputRef : bindingOp.getOutputRefChildren()) {
            for (Output output : outputRef.getref()) {
                javax.wsdl.BindingOutput bindOutput = 
                    this.def.createBindingOutput();
                bindOutput.setName(output.getname());
                wsdlBindOp.setBindingOutput(bindOutput);
                for (BodyBinding bodyBinding : outputRef.getdstBodyBinding()) {
                    SOAPBody soapBody = bodyBinding.getdstBodyBinding();
                    this.push(bindOutput);
                    this.visitSOAPBody(soapBody);
                    this.pop();
                }
                for (HeaderBinding headerBinding : outputRef.getdstHeaderBinding()) {
                    SOAPHeaders soapHeaders = 
                        headerBinding.getdstHeaderBinding();
                    if (soapHeaders instanceof SOAPHeader) {
                        SOAPHeader soapHeader = (SOAPHeader)soapHeaders; 
                        this.push(bindOutput);
                        this.visitSOAPHeader(soapHeader);
                        this.pop();
                    } else if (soapHeaders instanceof SOAPHeaderFault) {
                        SOAPHeaderFault soapHeaderFault = 
                            (SOAPHeaderFault)soapHeaders;
                        this.push(bindOutput);
                        this.visitSOAPHeaderFault(soapHeaderFault);
                        this.pop();
                    }
                }
            }
        }
        
        for (FaultRef faultRef : bindingOp.getFaultRefChildren()) {
            for (Fault fault : faultRef.getref()) {
                javax.wsdl.BindingFault bindFault = this.def.createBindingFault();
                bindFault.setName(fault.getname());
                wsdlBindOp.addBindingFault(bindFault);
                for (SOAPFaultExtension faultExt : 
                    faultRef.getdstSOAPFaultExtension()) {
                    SOAPFault soapFault = faultExt.getdstSOAPFaultExtension();
                    this.push(bindFault);
                    this.visitSOAPFault (soapFault);
                    this.pop();
                }
            }
        }
        javax.wsdl.Binding binding = (javax.wsdl.Binding)this.currParent;
        binding.addBindingOperation(wsdlBindOp);
    }

    public void visitSOAPOperation(SOAPOperation soapOperation) throws UdmException {
        try {
            QName elementType = new QName (W3C_SOAP_NS_URI, "operation");
            ExtensionRegistry registry = this.def.getExtensionRegistry();
            javax.wsdl.extensions.soap.SOAPOperation wsdlSoapOper = 
                (javax.wsdl.extensions.soap.SOAPOperation)
                registry.createExtension(javax.wsdl.BindingOperation.class,
                                         elementType);
            wsdlSoapOper.setElementType(elementType);
            wsdlSoapOper.setRequired(soapOperation.getrequired());
            wsdlSoapOper.setSoapActionURI(soapOperation.getsoapAction());
            wsdlSoapOper.setStyle(soapOperation.getstyle());
            javax.wsdl.BindingOperation bindOp = 
                (javax.wsdl.BindingOperation)this.currParent;
            bindOp.addExtensibilityElement(wsdlSoapOper);
        } catch (WSDLException e) {
            e.printStackTrace();
        }
    }

    public void visitSOAPBody(SOAPBody soapBody) throws UdmException {
        BodyBinding bodyBinding = soapBody.getsrcBodyBinding();
        IORef ioref = bodyBinding.getsrcBodyBinding();
        Class parentClass = null;
        javax.wsdl.BindingInput wsdlInput = null;
        javax.wsdl.BindingOutput wsdlOutput = null;
        if (ioref instanceof InputRef) {
            wsdlInput = (javax.wsdl.BindingInput)this.currParent;
            parentClass = javax.wsdl.BindingInput.class;
        } else if (ioref instanceof OutputRef) {
            wsdlOutput = (javax.wsdl.BindingOutput)this.currParent;
            parentClass = javax.wsdl.BindingOutput.class;
        }
        try {
            QName elementType = new QName (W3C_SOAP_NS_URI, "body");
            ExtensionRegistry registry = this.def.getExtensionRegistry();
            javax.wsdl.extensions.soap.SOAPBody wsdlSoapBody =  
                (javax.wsdl.extensions.soap.SOAPBody) 
                registry.createExtension(parentClass, elementType);
            wsdlSoapBody.setElementType(elementType);
            wsdlSoapBody.setRequired(soapBody.getrequired());
            String styles = soapBody.getencodingStyle();
            List encodingStyles = Arrays.asList(styles.split(" "));
            wsdlSoapBody.setEncodingStyles(encodingStyles);
            wsdlSoapBody.setNamespaceURI(soapBody.getnamespace());
            wsdlSoapBody.setUse(soapBody.getuse());
            for (BodyParts bodyParts : soapBody.getdstBodyParts()) {
                PartRef partRef = bodyParts.getdstBodyParts();
                List<javax.wsdl.Part> parts = 
                    new ArrayList<javax.wsdl.Part>();
                for (Part part : partRef.getref()) {
                    parts.add(this.partMap.get(part));
                }
                wsdlSoapBody.setParts(parts);
            }
            if (wsdlInput != null) {
                wsdlInput.addExtensibilityElement(wsdlSoapBody);
            } else if (wsdlOutput != null) {
                wsdlOutput.addExtensibilityElement(wsdlSoapBody);
            }
        } catch (WSDLException e) {
            e.printStackTrace();
        }
        
    }

    public void visitSOAPHeader(SOAPHeader soapHeader) throws UdmException {
        for (HeaderBinding headerBinding : soapHeader.getsrcHeaderBinding()) {
            IORef ioref = headerBinding.getsrcHeaderBinding();
            Class parentClass = null;
            javax.wsdl.BindingInput wsdlInput = null;
            javax.wsdl.BindingOutput wsdlOutput = null;
            if (ioref instanceof InputRef) {
                wsdlInput = (javax.wsdl.BindingInput)this.currParent;
                parentClass = javax.wsdl.BindingInput.class;
            } else if (ioref instanceof OutputRef) {
                wsdlOutput = (javax.wsdl.BindingOutput)this.currParent;
                parentClass = javax.wsdl.BindingOutput.class;
            }
            try {
                QName elementType = new QName (W3C_SOAP_NS_URI, "header");
                ExtensionRegistry registry = this.def.getExtensionRegistry();
                javax.wsdl.extensions.soap.SOAPHeader wsdlSoapHeader=  
                    (javax.wsdl.extensions.soap.SOAPHeader) 
                    registry.createExtension(parentClass, elementType);
                wsdlSoapHeader.setElementType(elementType);
                wsdlSoapHeader.setRequired(soapHeader.getrequired());
                String styles = soapHeader.getencodingStyle();
                List encodingStyles = Arrays.asList(styles.split(" "));
                wsdlSoapHeader.setEncodingStyles(encodingStyles);
                wsdlSoapHeader.setNamespaceURI(soapHeader.getnamespace());
                wsdlSoapHeader.setUse(soapHeader.getuse());
                for (HeaderPart headerPart : soapHeader.getdstHeaderPart()) {
                    PartRef partRef = headerPart.getdstHeaderPart();
                    for (Part part : partRef.getref()) {
                        wsdlSoapHeader.setPart(this.partMap.get(part).getName());
                    }
                }
                for (HeaderMessage hdrMsg : soapHeader.getdstHeaderMessage()) {
                    MessageRef messageRef = hdrMsg.getdstHeaderMessage();
                    for (Message message : messageRef.getref()) {
                        javax.wsdl.Message wsdlMessage = 
                            this.messageMap.get(message);
                        wsdlSoapHeader.setMessage(wsdlMessage.getQName());
                    }
                }
                if (wsdlInput != null) {
                    wsdlInput.addExtensibilityElement(wsdlSoapHeader);
                } else if (wsdlOutput != null) {
                    wsdlOutput.addExtensibilityElement(wsdlSoapHeader);
                }
            } catch (WSDLException e) {
                e.printStackTrace();
            }
        }
    }

    public void visitSOAPHeaderFault(SOAPHeaderFault soapHeaderFault) throws UdmException {
        // TODO Auto-generated method stub
        
    }

    public void visitSOAPFault(SOAPFault soapFault) throws UdmException {
        javax.wsdl.BindingFault bindFault =  
            (javax.wsdl.BindingFault)this.currParent;
        try {
            QName elementType = new QName (W3C_SOAP_NS_URI, "fault");
            ExtensionRegistry registry = this.def.getExtensionRegistry();
            javax.wsdl.extensions.soap.SOAPFault wsdlSoapFault =  
                (javax.wsdl.extensions.soap.SOAPFault) 
                registry.createExtension(javax.wsdl.BindingFault.class,
                                         elementType);
            wsdlSoapFault.setElementType(elementType);
            wsdlSoapFault.setRequired(soapFault.getrequired());
            String styles = soapFault.getencodingStyle();
            List encodingStyles = Arrays.asList(styles.split(" "));
            wsdlSoapFault.setEncodingStyles(encodingStyles);
            wsdlSoapFault.setNamespaceURI(soapFault.getnamespace());
            wsdlSoapFault.setUse(soapFault.getuse());
            SOAPFaultExtension faultExt = soapFault.getsrcSOAPFaultExtension();
            FaultRef faultRef = faultExt.getsrcSOAPFaultExtension();
            for (Fault fault : faultRef.getref()) {
                wsdlSoapFault.setName(fault.getname());
            }
            bindFault.addExtensibilityElement(wsdlSoapFault);
        } catch (WSDLException e) {
            e.printStackTrace();
        }
    }

    public void visitService(Service service) throws UdmException {
        javax.wsdl.Service wsdlService = this.def.createService();
        wsdlService.setQName(new QName (this.def.getTargetNamespace(),
                                        service.getname()));
        for (Port port : service.getPortChildren()) {
            this.push(wsdlService);
            this.visitPort (port);
            this.pop();
        }
        this.def.addService(wsdlService);
    }

    public void visitPort(Port port) throws UdmException {
        javax.wsdl.Port wsdlPort = this.def.createPort();
        wsdlPort.setName(port.getname());
        for (PortBinding portBinding : port.getdstPortBinding()) {
            BindingRef bindingRef = portBinding.getdstPortBinding();
            for (Binding binding : bindingRef.getref()) {
                wsdlPort.setBinding(this.bindingMap.get(binding));
            }
        }
        SOAPAddress soapAddress = port.getSOAPAddressChild();
        this.push(wsdlPort);
        this.visitSOAPAddress(soapAddress);
        this.pop();
        javax.wsdl.Service wsdlService = (javax.wsdl.Service)this.currParent;
        wsdlService.addPort(wsdlPort);
    }

    public void visitSOAPAddress(SOAPAddress addr) throws UdmException {
        javax.wsdl.Port wsdlPort = (javax.wsdl.Port)this.currParent;
        try {
            QName elementType = new QName (W3C_SOAP_NS_URI, "address");
            ExtensionRegistry registry = this.def.getExtensionRegistry();
            javax.wsdl.extensions.soap.SOAPAddress wsdlSoapAddr =  
                (javax.wsdl.extensions.soap.SOAPAddress) 
                registry.createExtension(javax.wsdl.Port.class, elementType);
            wsdlSoapAddr.setElementType(elementType);
            wsdlSoapAddr.setLocationURI(addr.getlocation());
            wsdlSoapAddr.setRequired(addr.getrequired());
            wsdlPort.addExtensibilityElement(wsdlSoapAddr);
        } catch (WSDLException e) {
            e.printStackTrace();
        }
    }
}
