package edu.vanderbilt.dre.WSML.WSDLImporter;

import java.io.*;
import javax.wsdl.*;
import javax.wsdl.extensions.*;
import javax.wsdl.extensions.soap.*;
import javax.wsdl.extensions.schema.*;
import javax.xml.*;
import javax.xml.bind.*;
import javax.xml.parsers.*;
import javax.xml.namespace.*;
import java.util.*;
import edu.vanderbilt.isis.gme.xme.*;
import org.w3c.dom.*;
import org.w3c.dom.Element;
import org.w3c.dom.ls.*;

public class XmlExportVisitor implements Visitor {
    private ObjectFactory objFactory = new ObjectFactory();
    private DocumentBuilderFactory dbf;
    private DocumentBuilder db;
    private DOMImplementation dbimpl;
    private DOMImplementationLS dbimplLS;
    private DocumentType docType;
    private Document doc;
    private JAXBContext jc;
    private Marshaller marshaller;
    private LSSerializer writer;
    private LSOutput output;
    private org.w3c.dom.Element docElement;
    private DocumentFragment docFragment;
    private Folder rootFolder;

    private Model currModel = null;
    private Stack<Model> modelStack = new Stack<Model>();

    private String currNs = null;
    private Stack<String> nsStack = new Stack<String>();

    private Operation currOp = null;
    private Reference currRef = null;
    private Message currMessage = null;

    private int objCounter = 0;

    private Map<QName, Object> typeMap = new HashMap<QName, Object>();

    private Map<QName, Object> elementMap = new HashMap<QName, Object>();

    private Map<QName, Model> messageMap = new HashMap<QName, Model>();

    private Map<QName, Model> portTypeMap = new HashMap<QName, Model>();

    private Map<QName, Model> bindingMap = new HashMap<QName, Model>();

    private Map<QName, Model> serviceMap = new HashMap<QName, Model>();

    private Map<Operation, Model> operMap = new HashMap<Operation, Model>();

    private Map<Input, Model> inputMap = new HashMap<Input, Model>();

    private Map<Output, Model> outputMap = new HashMap<Output, Model>();

    private Map<Fault, Model> faultMap = new HashMap<Fault, Model>();

    private Map<QName, Reference> partMap = new HashMap<QName, Reference>();

    private int xPos = 12;
    private int yPos = 12;

    // QNames for top-level Schema elements
    private static final QName SCHEMA
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "schema");
    private static final QName COMPLEXTYPE
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "complexType");
    private static final QName ELEMENT
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "element");
    private static final QName ATTRIBUTE
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "attribute");
    private static final QName SIMPLETYPE
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "simpleType");
    private static final QName ANY
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "any");
    private static final QName ANYATTRIBUTE
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "anyAttribute");
    private static final QName GROUP
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "group");
    private static final QName ATTRIBUTEGROUP
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "attributeGroup");
    private static final QName KEY
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "key");
    private static final QName KEYREF
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "keyref");
    private static final QName UNIQUE
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "unique");
    private static final QName ANNOTATION
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "annotation");
    private static final QName DOCUMENTATION
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "documentation");
    private static final QName APPINFO
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "appinfo");
    private static final QName NOTATION
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "notation");
    private static final QName INCLUDE
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "include");
    private static final QName IMPORT
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "import");
    private static final QName REDEFINE
        = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI, "redefine");

    private enum GmeKind {
        Model (0x65),
        Atom(0x66),
        Reference(0x67),
        Connection(0x68),
        Folder(0x6a);

        private int value;

        GmeKind (int value) {
            this.value = value;
        }

        public String toString() {
            return this.name();
        }

        public int value () {
            return this.value;
        }

    }

    public XmlExportVisitor(String defName, String outputFileName) {
        try {
            this.dbf = DocumentBuilderFactory.newInstance();
            this.db = dbf.newDocumentBuilder();
            this.dbimpl = db.getDOMImplementation();
            this.dbimplLS =
                (DOMImplementationLS)this.dbimpl.getFeature ("LS", "3.0");

            this.docType = this.dbimpl.createDocumentType("project",
                                                     null,
                                                     "mga.dtd");
            this.doc = this.dbimpl.createDocument (null, "project", docType);
            this.jc = JAXBContext.newInstance("edu.vanderbilt.isis.gme.xme");
            this.marshaller = this.jc.createMarshaller();

            FileOutputStream outFile = new FileOutputStream (outputFileName);
            this.writer = this.dbimplLS.createLSSerializer();
            this.output = this.dbimplLS.createLSOutput();
            this.output.setEncoding ("UTF-8");
            this.output.setByteStream (outFile);

            this.docElement = this.doc.getDocumentElement();
            this.docElement.setAttribute ("metaname", "WSML");

            Name name = this.objFactory.createName();
            name.setContent (defName);

            this.docFragment = this.doc.createDocumentFragment();
            this.marshaller.marshal (name, this.docFragment);
            this.docElement.appendChild (this.docFragment);

            edu.vanderbilt.isis.gme.xme.Comment
                comment = this.objFactory.createComment();
            comment.setContent ("Imported from WSDL on "
                                + new Date().toString());
            this.marshaller.marshal (comment, this.docFragment);
            this.docElement.appendChild (this.docFragment);

            Author author = this.objFactory.createAuthor();
            author.setContent ("edu.vanderbilt.dre.WSML.WSDLImporter");
            this.marshaller.marshal (author, this.docFragment);
            this.docElement.appendChild (this.docFragment);

            this.rootFolder = this.objFactory.createFolder();
            this.rootFolder.setKind("RootFolder");
            this.rootFolder.setId(createGmeId(GmeKind.Folder));
            this.rootFolder.setName (this.setElementName (defName));

            // Create the SchemaBuiltins model
            Model schemaBuiltins = this.createSchemaBuiltins();
            this.rootFolder.getRegnodeOrConstraintOrFolder().add(schemaBuiltins);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (ParserConfigurationException e) {
            e.printStackTrace();

        } catch (LSException e) {
            e.printStackTrace();

        } catch (DOMException e) {
            e.printStackTrace();
        } catch (MarshalException e) {
            e.printStackTrace();
        } catch (JAXBException e) {
            e.printStackTrace();
        }
    }

    public void dump () {
        try {
            this.marshaller.marshal (this.rootFolder, this.docFragment);
            this.docElement.appendChild (this.docFragment);
            this.writer.write (this.doc, this.output);

        } catch (JAXBException e) {
            e.printStackTrace();
        }

    }

    public void push (Model ele) {
        this.modelStack.push (this.currModel);
        this.currModel = ele;
    }

    public void pop () {
        this.currModel = this.modelStack.pop();
    }

    public void pushNamespace (String ns) {
        this.nsStack.push (this.currNs);
        this.currNs = ns;
    }

    public void popNamespace() {
        this.currNs = this.nsStack.pop();
    }


    public String createGmeId (GmeKind kind) {
        return String.format ("id-%04x-%08x", kind.value(), ++objCounter);
    }

    public Model createGmeModel (String name, String kind, String role) {
        Model model = this.objFactory.createModel();
        model.setId (this.createGmeId(GmeKind.Model));
        model.setName (this.setElementName(name));
        model.setKind (kind);
        return model;
    }

    public Reference createGmeReference (String name, String kind,
                                         String role) {
        Reference ref = this.objFactory.createReference();
        ref.setId (this.createGmeId(GmeKind.Reference));
        ref.setName(this.setElementName(name));
        ref.setKind(kind);
        ref.setRole(role);
        return ref;
    }

    public Connection createGmeConnection (String name, String kind,
                                           String role) {
        Connection conn = this.objFactory.createConnection();
        conn.setId (this.createGmeId(GmeKind.Connection));
        conn.setName(this.setElementName(name));
        conn.setKind(kind);
        conn.setRole(role);
        return conn;
    }

    public Connpoint createGmeConnpoint (String role) {
        Connpoint conn = this.objFactory.createConnpoint();
        conn.setRole(role);
        return conn;
    }

    public Atom createGmeAtom (String name, String kind, String role) {
        Atom atom = this.objFactory.createAtom();
        atom.setId (this.createGmeId(GmeKind.Atom));
        atom.setName (this.setElementName(name));
        atom.setKind (kind);
        atom.setRole (role);
        return atom;
    }

    public Attribute createGmeAttribute (String kind, String status,
                                         String value) {
        Attribute attr = this.objFactory.createAttribute();
        attr.setKind(kind);
        if (status != null)
            attr.setStatus(status);
        Value val = this.objFactory.createValue();
        val.setContent (value);
        attr.setValue(val);
        return attr;
    }

    public Regnode createGmeRegnode (String name,
                                     String status,
                                     boolean isOpaque) {
        Regnode regNode = this.objFactory.createRegnode();
        regNode.setName (name);
        if (status != null)
            regNode.setStatus(status);
        if (isOpaque)
            regNode.setIsopaque ("yes");
        return regNode;
    }

    public Regnode createPartRegs (int x, int y) {
        // PartRegs registry node
        Regnode partregs = this.createGmeRegnode ("PartRegs", "undefined",
                                                  false);
        Value value = this.objFactory.createValue();
        value.setContent("");
        partregs.setValue (value);

        // ServiceDefinition registry node
        Regnode service = this.createGmeRegnode ("ServiceDefinition",
                                                 "undefined", false);
        service.setValue(value);
        partregs.getRegnode().add (service);

        // Position registry node
        Regnode pos = this.createGmeRegnode ("Position", null, true);
        Value posValue = this.objFactory.createValue();
        posValue.setContent(String.format ("%d,%d", x, y));
        pos.setValue(posValue);
        service.getRegnode().add(pos);

        return partregs;
    }

    public Regnode createAutoRouterPref (String value) {
        // autorouterpref registry node
        Regnode autorouterpref = this.createGmeRegnode ("autorouterPref",
                                                        null,
                                                        true);
        Value val = this.objFactory.createValue();
        val.setContent(value);
        autorouterpref.setValue (val);

        return autorouterpref;
    }

    public Name setElementName (String name) {
        Name n = this.objFactory.createName();
        n.setContent (name);
        return n;
    }

    public Model createSchemaBuiltins() {
        String [][] builtinTypes = {
            {"XsString", "string"},
            {"XsBoolean", "boolean"},
            {"XsFloat", "float"},
            {"XsDouble","double"},
            {"XsDecimal", "decimal"},
            {"XsDuration", "duration"},
            {"XsDateTime", "dateTime"},
            {"XsTime", "time"},
            {"XsDate", "date"},
            {"XsGYearMonth", "gYearMonth"},
            {"XsGYear", "gYear"},
            {"XsGMonthDay", "gMonthDay"},
            {"XsGDay", "gDay"},
            {"XsGMonth", "gMonth"},
            {"XsHexBinary", "hexBinary"},
            {"XsBase64Binary", "base64Binary"},
            {"XsAnyUri", "anyURI"},
            {"XsQName", "QName"},
            {"XsNotation", "NOTATION"},
            {"XsNormalizedString", "normalizedString"},
            {"XsToken", "token"},
            {"XsLanguage", "language"},
            {"XsIdRefs","IDREFS"},
            {"XsEntities","ENTITIES"},
            {"XsNmToken", "NMTOKEN"},
            {"XsNmTokens","NMTOKENS"},
            {"XsName", "name"},
            {"XsNcName","NCName"},
            {"XsId","ID"},
            {"XsIdRef","IDREF"},
            {"XsEntity","ENTITY"},
            {"XsInteger","integer"},
            {"XsNonPositiveInteger","nonPositiveInteger"},
            {"XsNegativeInteger","negativeInteger"},
            {"XsLong","long"},
            {"XsInt","int"},
            {"XsShort","short"},
            {"XsByte","byte"},
            {"XsNonNegativeInteger","nonNegativeInteger"},
            {"XsUnsignedLong","unsignedLong"},
            {"XsUnsignedInt","unsignedInt"},
            {"XsUnsignedShort","unsignedShort"},
            {"XsUnsignedByte","unsignedByte"},
            {"XsPositiveInteger", "positiveInteger"},
        };
        Model model = this.createGmeModel ("SchemaBuiltins", "SchemaBuiltins",
                                           null);
        for (int i = 0; i < builtinTypes.length; ++i) {
            Atom atom = this.createGmeAtom (builtinTypes[i][1],
                                            "AtomicType",
                                            "AtomicType");
            Attribute attr = this.createGmeAttribute("typeCode", 
                                                     null, 
                                                     builtinTypes[i][1]);
            atom.getRegnodeOrConstraintOrAttribute().add(attr);
            QName qname = new QName (XMLConstants.W3C_XML_SCHEMA_NS_URI,
                                     builtinTypes[i][1]);
            this.typeMap.put (qname, atom);
            Regnode partRegs = this.createPartRegs(xPos, yPos);
            atom.getRegnodeOrConstraintOrAttribute().add(partRegs);
            model.getRegnodeOrConstraintOrAttribute().add(atom);
            xPos += 115;
            // Wrap around to the next row
            // (based on a screen of width 1600x1200 with GME maximized)
            if (xPos > 1084) {
                xPos = 12;
                yPos += 115;
            }
        }
        xPos = yPos = 12;  // Reset values to the global default
        return model;
    }

    public void visitDefinition (Definition definition) {

        // Create the Definitions model
        String defName = definition.getQName().getLocalPart();
        Model def = this.createGmeModel(defName, "Definitions", null);
        String targetNamespace = definition.getTargetNamespace();
        boolean popNamespace = false;
        if (targetNamespace != null) {
            this.pushNamespace(targetNamespace);
            popNamespace = true;
            Attribute attr = this.createGmeAttribute("targetNamespace",
                                                     null,
                                                     targetNamespace);
            def.getRegnodeOrConstraintOrAttribute().add (attr);
        }
        this.typeMap.put (definition.getQName(), def);
        this.push (def);

        // Create the different namespace attributes
        Map namespaces = definition.getNamespaces();
        Iterator keys = namespaces.keySet().iterator();
        while (keys.hasNext()) {
            String prefix = (String)keys.next();
            String uri = (String)namespaces.get(prefix);
            Atom ns =
                this.createGmeAtom ("xmlns" +
                                    (prefix == "" ? prefix : ":" + prefix),
                                    "Attribute", "Attribute");
            Regnode partregs = this.createPartRegs (xPos, yPos);
            ns.getRegnodeOrConstraintOrAttribute().add(partregs);
            xPos += 115;
            // Wrap around to the next row (based on a screen of width
            // 1600x1200 with GME maximized)
            if (xPos > 1084) {
                xPos = 12;
                yPos += 115;
            }
            Attribute value = this.createGmeAttribute ("Value",
                                                       null,
                                                       uri);
            ns.getRegnodeOrConstraintOrAttribute().add(value);
            this.currModel.getRegnodeOrConstraintOrAttribute().add(ns);
        }

        Map imports = definition.getImports();
        Iterator iter = imports.values().iterator();
        while (iter.hasNext()) {
            List importList = (List)iter.next();
            Iterator listIter = importList.iterator();
            while (listIter.hasNext()) {
                Import imp = (Import)listIter.next();
                this.visitImport (imp);
            }
        }

        Types types = definition.getTypes();
        if (types != null) {
            this.visitTypes (types);
        } else {
            System.out.println ("Encountered null type definition in " +
                                definition.getQName());
        }

        xPos += 115;
        // Wrap around to the next row (based on a screen of width
        // 1600x1200 with GME maximized)
        if (xPos > 1084) {
            xPos = 12;
            yPos += 115;
        }

        Map messages = definition.getMessages();
        if (!messages.isEmpty()) {
            Model messagesModel = this.createGmeModel ("Messages", "Messages",
                                                  "Messages");
            Regnode partregs = this.createPartRegs (xPos, yPos);
            messagesModel.getRegnodeOrConstraintOrAttribute().add(partregs);
            def.getRegnodeOrConstraintOrAttribute().add(messagesModel);
            iter = messages.values().iterator();
            while (iter.hasNext()) {
                Message msg = (Message)iter.next();
                this.push(messagesModel);
                this.visitMessage (msg);
                this.pop();
                xPos += 115;
                // Wrap around to the next row (based on a screen of width
                // 1600x1200 with GME maximized)
                if (xPos > 1084) {
                    xPos = 12;
                    yPos += 115;
                }
            }
        }

        Map portTypes = definition.getPortTypes();
        iter = portTypes.values().iterator();
        while (iter.hasNext()) {
            PortType portType = (PortType)iter.next();
            this.visitPortType (portType);
        }

        Map bindings = definition.getBindings();
        iter = bindings.values().iterator();
        while (iter.hasNext()) {
            Binding binding = (Binding)iter.next();
            this.visitBinding (binding);
        }

        Map services = definition.getServices();
        iter= services.values().iterator();
        while (iter.hasNext()) {
            Service service = (Service)iter.next();
            this.visitService (service);
        }
        this.pop();

        if (popNamespace)
            this.popNamespace();

        if (this.currModel == null) {
            this.rootFolder.getRegnodeOrConstraintOrFolder().add(def);
        } else {
            this.currModel.getRegnodeOrConstraintOrAttribute().add(def);
        }
    }

    public void visitImport (Import imp) {
        Model importModel = this.createGmeModel ("import", "Import", "Import");
        Regnode partregs = this.createPartRegs (xPos, yPos);
        importModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(importModel);
        String namespaceName = imp.getNamespaceURI();
        if (namespaceName == null) {
            System.out.println ("Error: namespace attribute of <wsdl:import>"
                                + "cannot be null!");
            System.exit(1);
        }
        Attribute namespace = this.createGmeAttribute ("namespace",
                                                       null,
                                                       namespaceName);
        importModel.getRegnodeOrConstraintOrAttribute().add(namespace);
        String locationName = imp.getLocationURI();
        if (locationName == null) {
            System.out.println ("Error: location attribute of <wsdl:import>"
                                + "cannot be null!");
            System.exit(1);
        }
        Attribute location = this.createGmeAttribute ("location",
                                                      null,
                                                      locationName);
        importModel.getRegnodeOrConstraintOrAttribute().add(location);

        Definition importedDef = imp.getDefinition();
        this.push(importModel);
        this.visitDefinition (importedDef);
        this.pop();
    }

    public void visitTypes (Types types) {
        Model typeModel = this.createGmeModel ("Types", "Types", "Types");
        this.currModel.getRegnodeOrConstraintOrAttribute().add (typeModel);
        Regnode partregs = this.createPartRegs (xPos, yPos);
        typeModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        List extElements = types.getExtensibilityElements();
        Iterator iterator = extElements.iterator();
        while (iterator.hasNext()) {
            ExtensibilityElement elem = (ExtensibilityElement)iterator.next();
            if (elem instanceof Schema) {
                Schema schema = (Schema)elem;
                this.push (typeModel);
                this.visitSchema(schema);
                this.pop();
            }
        }
    }

    public void visitSchema (Schema schema) {
        boolean popNamespace = false;
        org.w3c.dom.Element schemaEle = schema.getElement();
        String schemaName = schemaEle.getNodeName();
        Model schemaModel = this.createGmeModel(schemaName, "Schema", "Schema");
        this.currModel.getRegnodeOrConstraintOrAttribute().add (schemaModel);
        xPos = yPos= 12;
        Regnode partregs = this.createPartRegs (xPos, yPos);
        schemaModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        NamedNodeMap attrs = schemaEle.getAttributes();
        for (int i = 0; i < attrs.getLength(); ++i) {
            Node attrNode = attrs.item (i);
            String attrName = attrNode.getNodeName();
            String attrValue = attrNode.getNodeValue();
            if (attrNode.getLocalName().equals ("targetNamespace")) {
                Attribute attr = this.createGmeAttribute (attrName,
                                                          null,
                                                          attrValue);
                schemaModel.getRegnodeOrConstraintOrAttribute().add(attr);
                this.pushNamespace(attrValue);
                popNamespace = true;
                continue;
            }
            Atom atom = this.createGmeAtom (attrName, "Attribute", "Attribute");
            Attribute attrVal = this.createGmeAttribute ("Value", null,
                                                         attrValue);
            atom.getRegnodeOrConstraintOrAttribute().add(attrVal);
            Regnode atomregs = this.createPartRegs (xPos, yPos);
            xPos += 115;
            // Wrap around to the next row (based on a screen of width
            // 1600x1200 with GME maximized)
            if (xPos > 1084) {
                xPos = 12;
                yPos += 115;
            }
            atom.getRegnodeOrConstraintOrAttribute().add(atomregs);
            schemaModel.getRegnodeOrConstraintOrAttribute().add(atom);
        }

        xPos = yPos = 12;

        List includes = schema.getIncludes();
        Iterator iter = includes.iterator();
        while (iter.hasNext()) {
            SchemaReference schemaRef = (SchemaReference)iter.next();
            Model schemaRefModel = this.createGmeModel ("include",
                                                        "SchemaReference",
                                                        "SchemaReference");
            schemaModel.getRegnodeOrConstraintOrAttribute().add(schemaRefModel);
            this.push (schemaRefModel);
            this.visitSchemaReference (schemaRef);
            this.pop();
        }

        Map imports = schema.getImports();
        Iterator values = imports.values().iterator();
        while (values.hasNext()) {
            List importList = (List)values.next();
            iter = importList.iterator();
            while (iter.hasNext()) {
                SchemaImport schemaImport = (SchemaImport)iter.next();
                this.push (schemaModel);
                this.visitSchemaImport(schemaImport);
                this.pop();
            }
        }

        List redefines = schema.getRedefines();
        iter = redefines.iterator();
        while (iter.hasNext()) {
            SchemaReference schemaRef = (SchemaReference)iter.next();
            Model schemaRefModel = this.createGmeModel ("redefine",
                                                        "SchemaReference",
                                                        "SchemaReference");
            schemaModel.getRegnodeOrConstraintOrAttribute().add(schemaRefModel);
            this.push (schemaRefModel);
            this.visitSchemaReference (schemaRef);
            this.pop();
        }

        NodeList children = schemaEle.getChildNodes();
        int orderCount = 0;
        for (int i = 0; i < children.getLength(); ++i) {
            Node child = children.item(i);
            if (child instanceof Element) {
                org.w3c.dom.Element childEle = (org.w3c.dom.Element)child;
                this.visitElement (childEle, schemaModel, 0, orderCount++);
            }
        }

        if (popNamespace)
            this.popNamespace();
    }

    public void visitSchemaImport (SchemaImport schemaImport) {
        Model schemaImpModel = this.createGmeModel ("import",
                                                    "SchemaImport",
                                                    "SchemaImport");
        this.currModel.getRegnodeOrConstraintOrAttribute(). add(schemaImpModel);
        String id = schemaImport.getId();
        if (id != null) {
            Attribute attr = this.createGmeAttribute ("id", null, id);
            schemaImpModel.getRegnodeOrConstraintOrAttribute().add(attr);
        }
        String schemaLocation = schemaImport.getSchemaLocationURI();
        if (schemaLocation == null) {
            System.out.println ("Error: schemaLocation attribute of "
                                + this.currModel.getName().getContent()
                                + " is null!");
            System.exit(1);
        }
        Attribute attr = this.createGmeAttribute ("schemaLocation", null,
                                                  schemaLocation);
        schemaImpModel.getRegnodeOrConstraintOrAttribute().add(attr);

        String namespace = schemaImport.getNamespaceURI();
        if (schemaLocation == null) {
            System.out.println ("Error: schemaLocation attribute of "
                                + this.currModel.getName().getContent()
                                + " is null!");
            System.exit(1);
        }
        attr = this.createGmeAttribute ("namespace", null, namespace);
        schemaImpModel.getRegnodeOrConstraintOrAttribute().add(attr);
        Schema schema = schemaImport.getReferencedSchema();
        this.push(schemaImpModel);
        this.visitSchema(schema);
        this.pop();
    }

    public void visitSchemaReference (SchemaReference schemaReference) {
        String id = schemaReference.getId();
        if (id != null) {
            Attribute attr = this.createGmeAttribute ("id", null, id);
            this.currModel.getRegnodeOrConstraintOrAttribute().add(attr);
        }
        String schemaLocation = schemaReference.getSchemaLocationURI();
        if (schemaLocation == null) {
            System.out.println ("Error: schemaLocation attribute of "
                                + this.currModel.getName().getContent()
                                + " is null!");
            System.exit(1);
        }
        Attribute attr = this.createGmeAttribute ("schemaLocation", null,
                                                  schemaLocation);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(attr);
        Schema schema = schemaReference.getReferencedSchema();
        this.visitSchema(schema);
    }

    public void visitElement (org.w3c.dom.Element visitedEle, Model parent,
                              int level, int order) {
        String eleName = visitedEle.getNodeName();
        Model model = this.createGmeModel (eleName, "Element", "Element");
        parent.getRegnodeOrConstraintOrAttribute().add (model);
        xPos = yPos = 12;
        Regnode partregs = this.createPartRegs (xPos, yPos);
        model.getRegnodeOrConstraintOrAttribute().add(partregs);
        Attribute orderAttr = this.createGmeAttribute("Order", null, 
                                                      Integer.toString(order));
        model.getRegnodeOrConstraintOrAttribute().add(orderAttr);
        NamedNodeMap attrs = visitedEle.getAttributes();
        for (int i = 0; i < attrs.getLength(); ++i) {
            Node attrNode = attrs.item (i);
            String attrName = attrNode.getNodeName();
            String attrValue = attrNode.getNodeValue();
            Atom atom = this.createGmeAtom (attrName, "Attribute", "Attribute");
            Attribute attrVal = this.createGmeAttribute ("Value", null,
                                                         attrValue);
            atom.getRegnodeOrConstraintOrAttribute().add(attrVal);
            Regnode atomregs = this.createPartRegs (xPos, yPos);
            xPos += 115;
            // Wrap around to the next row (based on a screen of width
            // 1600x1200 with GME maximized)
            if (xPos > 1084) {
                xPos = 12;
                yPos += 115;
            }
            atom.getRegnodeOrConstraintOrAttribute().add(atomregs);
            model.getRegnodeOrConstraintOrAttribute().add(atom);
            xPos = yPos = 12;
        }
        String localName = visitedEle.getLocalName();
        String namespaceURI = visitedEle.getNamespaceURI();
        QName qname = new QName (namespaceURI, localName);
        if (qname.equals (COMPLEXTYPE)
            || qname.equals (SIMPLETYPE)
            || qname.equals (ELEMENT)) {
            Attribute attr = this.createGmeAttribute ("schemaType",
                                                      null, localName);
            model.getRegnodeOrConstraintOrAttribute().add(attr);
            if (visitedEle.getAttributeNode("name") != null) {
                String name = visitedEle.getAttribute ("name");
                model.setName(this.setElementName(name));
                // Add top-level elements of schema to the list of elements that
                // can be referenced
                if (level == 0) {
                    QName eleQName = new QName (this.currNs, name);
                    if (qname.equals (ELEMENT)) {
//                        System.out.println ("Adding " + eleQName
//                                            + " to the elementMap");
                        this.elementMap.put (eleQName, model);
                    } else {
//                        System.out.println ("Adding " + eleQName
//                                            + " to the typeMap");
                        this.typeMap.put (eleQName, model);

                    }
                }
            }
        } else if (qname.equals (ANY) || qname.equals (ANYATTRIBUTE)
                   || qname.equals (ANNOTATION)
                   || qname.equals (DOCUMENTATION)
                   || qname.equals (APPINFO)
                   || qname.equals (INCLUDE)
                   || qname.equals (IMPORT)
                   || qname.equals (ATTRIBUTE)
                   || qname.equals (GROUP)
                   || qname.equals (ATTRIBUTEGROUP)
                   || qname.equals (KEY)
                   || qname.equals (KEYREF)
                   || qname.equals (UNIQUE)
                   || qname.equals (NOTATION)
                   || qname.equals (REDEFINE)) {
            Attribute attr = this.createGmeAttribute ("schemaType",
                                                      null, localName);
            model.getRegnodeOrConstraintOrAttribute().add(attr);
        } else {
            Attribute attr = this.createGmeAttribute ("schemaType",
                                                      null, "local");
            model.getRegnodeOrConstraintOrAttribute().add(attr);
        }

        int newLevel = level + 1;
        NodeList children = visitedEle.getChildNodes();
        int orderCount = 0;
        for (int i = 0; i < children.getLength(); ++i) {
            Node child = children.item (i);
            if (child instanceof Element) {
                org.w3c.dom.Element childEle = (org.w3c.dom.Element)child;
                this.visitElement (childEle, model, newLevel, orderCount++);
            }
        }
    }

    public void visitMessage (Message message) {
        String localName = message.getQName().getLocalPart();
        Model msgModel = this.createGmeModel(localName, "Message", "Message");
        Regnode partregs = this.createPartRegs(xPos, yPos);
        msgModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(msgModel);
        this.messageMap.put (message.getQName(), msgModel);
        Map parts = message.getParts();
        Iterator partIter = parts.values().iterator();
        xPos = yPos = 12;
        while (partIter.hasNext()) {
            Part part = (Part)partIter.next();
            this.push(msgModel);
            this.visitPart (part);
            this.pop();
        }
    }

    public void visitPart (Part part) {
        String localName = part.getName();
        Reference ref = this.createGmeReference (localName, "Part", "Part");
        Regnode partregs = this.createPartRegs(xPos, yPos);
        ref.getRegnodeOrConstraintOrAttribute().add(partregs);
        Object referred = null;
        QName referredName = part.getTypeName();
        if (referredName != null) {
            referred = this.typeMap.get(referredName);
        } else {
            referredName = part.getElementName();
            referred = this.elementMap.get(referredName);
        }
        if (referred == null) {
            System.out.println ("Error: Unknown type referred by part "
                                + referredName);
            System.exit(1);
        }
        ref.setReferred(referred);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(ref);
        String messageName = this.currModel.getName().getContent();
        QName msgPartQName = new QName (this.currNs, messageName + localName);
        this.partMap.put (msgPartQName, ref);
    }

    public void visitPortType (PortType portType) {
        String localName = portType.getQName().getLocalPart();
//        System.out.println ("Adding OperationType " + portType.getQName());
        Model portTypeModel = this.createGmeModel (localName, "PortType",
                                                   "PortType");
        Regnode partregs = this.createPartRegs(xPos, yPos);
        portTypeModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(portTypeModel);
        this.portTypeMap.put (portType.getQName(), portTypeModel);
        List operations = portType.getOperations();
        Iterator opIter = operations.iterator();
        xPos = yPos = 12;
        while (opIter.hasNext()) {
            Operation op = (Operation)opIter.next();
            this.push(portTypeModel);
            this.visitOperation(op);
            this.pop();
        }
    }

    public void visitOperation (Operation oper) {
        String localName = oper.getName();
//        System.out.println ("Adding Operation " + localName);
        OperationType style = oper.getStyle();
        String kind = null;
        String role = null;
        if (style == null) {
            System.out.println ("Error: Unknown OperationStyle for operation "
                                + localName);
            System.exit(1);
        }
        if (style.equals (OperationType.ONE_WAY)) {
            kind = role = "OneWayOperation";
        } else if (style.equals(OperationType.REQUEST_RESPONSE)) {
            kind = role = "RequestResponseOperation";
        } else if (style.equals(OperationType.SOLICIT_RESPONSE)) {
            kind = role = "SolicitResponseOperation";
        } else if (style.equals(OperationType.NOTIFICATION)) {
            kind = role = "NotificationOperation";
        } else {
            System.out.println ("Error: Unknown OperationStyle for operation"
                                + localName);
            System.exit(1);
        }
        Model operModel = this.createGmeModel(localName, kind, role);
        Regnode partregs = this.createPartRegs(xPos, yPos);
        operModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        StringBuilder params = new StringBuilder();
        if (style.equals(OperationType.REQUEST_RESPONSE)
            || style.equals(OperationType.SOLICIT_RESPONSE)) {
            List parameterOrdering = oper.getParameterOrdering();
            if (parameterOrdering != null) {
                Iterator paramIter = parameterOrdering.iterator();
                while (paramIter.hasNext()) {
                    params.append((String)paramIter.next());
                    params.append(" ");
                }
            }
        }
        Attribute parameterOrder
            = this.createGmeAttribute("parameterOrder", null,
                                      params.toString().trim());
        operModel.getRegnodeOrConstraintOrAttribute().add(parameterOrder);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(operModel);
        this.operMap.put(oper, operModel);
        Input input = oper.getInput();
        Output output = oper.getOutput();
        Map faults = oper.getFaults();
        if (style.equals (OperationType.ONE_WAY)) {
            this.push(operModel);
            if (input.getName() == null) {
                input.setName (localName);
            }
            this.visitInput(input);
            this.pop();
        } else if (style.equals(OperationType.NOTIFICATION)) {
            this.push(operModel);
            if (output.getName() == null) {
                output.setName (localName);
            }
            this.visitOutput(output);
            this.pop();
        } else {
            this.push(operModel);
            if (style.equals(OperationType.REQUEST_RESPONSE)) {
                if (input.getName() == null)
                    input.setName (localName+"Request");
                if (output.getName() == null)
                    output.setName (localName+"Response");
            } else if (style.equals(OperationType.SOLICIT_RESPONSE)) {
                if (output.getName() == null)
                    output.setName (localName+"Solicit");
                if (input.getName() == null)
                    input.setName (localName+"Response");
            }
            this.visitInput(input);
            this.visitOutput(output);
            Iterator faultIter = faults.values().iterator();
            while (faultIter.hasNext()) {
                Fault fault = (Fault)faultIter.next();
                this.visitFault (fault);
            }
            this.pop();
        }
    }

    public void visitInput (Input input) {
        String localName = input.getName();
        Model inputModel = this.createGmeModel (localName, "Input", "Input");
        Regnode partregs = this.createPartRegs(xPos, yPos);
        inputModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(inputModel);
        this.inputMap.put(input, inputModel);
        // Create the message reference
        Message refMessage = input.getMessage();
        Model refMsgModel = this.messageMap.get (refMessage.getQName());
        Reference inputMsgRef
            = this.createGmeReference (refMessage.getQName().getLocalPart(),
                                       "MessageRef", "MessageRef");
        xPos += 168;
        partregs = this.createPartRegs(xPos, yPos);
        inputMsgRef.getRegnodeOrConstraintOrAttribute().add(partregs);
        inputMsgRef.setReferred(refMsgModel);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(inputMsgRef);

        Connection inputMsgConn = this.createGmeConnection ("InputMessage",
                                                            "InputMessage",
                                                            "InputMessage");
        Regnode autoRouter = this.createAutoRouterPref("Ew");
        inputMsgConn.getRegnodeOrConstraintOrConnpoint().add(autoRouter);
        Connpoint src = this.createGmeConnpoint ("src");
        src.setTarget(inputModel);
        inputMsgConn.getRegnodeOrConstraintOrConnpoint().add(src);
        Connpoint dst = this.createGmeConnpoint ("dst");
        dst.setTarget(inputMsgRef);
        inputMsgConn.getRegnodeOrConstraintOrConnpoint().add(dst);

        this.currModel.getRegnodeOrConstraintOrAttribute().add(inputMsgConn);

    }



    public void visitOutput (Output output) {
        String localName = output.getName();
        Model outputModel = this.createGmeModel (localName, "Output", "Output");
        Regnode partregs = this.createPartRegs(xPos, yPos);
        outputModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(outputModel);
        this.outputMap.put(output, outputModel);
        // Create the message reference
        Message refMessage = output.getMessage();
        Model refMsgModel = this.messageMap.get (refMessage.getQName());
        Reference outputMsgRef
            = this.createGmeReference (refMessage.getQName().getLocalPart(),
                                       "MessageRef", "MessageRef");
        xPos += 168;
        partregs = this.createPartRegs(xPos, yPos);
        outputMsgRef.getRegnodeOrConstraintOrAttribute().add(partregs);
        outputMsgRef.setReferred(refMsgModel);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(outputMsgRef);

        Connection outputMsgConn = this.createGmeConnection ("OutputMessage",
                                                             "OutputMessage",
                                                             "OutputMessage");
        Regnode autoRouter = this.createAutoRouterPref("Ew");
        outputMsgConn.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        Connpoint src = this.createGmeConnpoint ("src");
        src.setTarget(outputModel);
        outputMsgConn.getRegnodeOrConstraintOrConnpoint().add(src);
        Connpoint dst = this.createGmeConnpoint ("dst");
        dst.setTarget(outputMsgRef);
        outputMsgConn.getRegnodeOrConstraintOrConnpoint().add(dst);

        this.currModel.getRegnodeOrConstraintOrAttribute().add(outputMsgConn);

    }
    public void visitFault (Fault fault) {
        String localName = fault.getName();
        Model faultModel = this.createGmeModel (localName, "Fault", "Fault");
        Regnode partregs = this.createPartRegs(xPos, yPos);
        faultModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(faultModel);
        this.faultMap.put (fault, faultModel);
        // Create the message reference
        Message refMessage = fault.getMessage();
        Model refMsgModel = this.messageMap.get (refMessage.getQName());
        Reference faultMsgRef
            = this.createGmeReference (refMessage.getQName().getLocalPart(),
                                       "MessageRef", "MessageRef");
        xPos += 168;
        partregs = this.createPartRegs(xPos, yPos);
        faultMsgRef.getRegnodeOrConstraintOrAttribute().add(partregs);
        faultMsgRef.setReferred(refMsgModel);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(faultMsgRef);

        Connection faultMsgConn = this.createGmeConnection ("FaultMessage",
                                                            "FaultMessage",
                                                            "FaultMessage");
        Regnode autoRouter = this.createAutoRouterPref("Ew");
        faultMsgConn.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        Connpoint src = this.createGmeConnpoint ("src");
        src.setTarget(faultModel);
        faultMsgConn.getRegnodeOrConstraintOrConnpoint().add(src);
        Connpoint dst = this.createGmeConnpoint ("dst");
        dst.setTarget(faultMsgRef);
        faultMsgConn.getRegnodeOrConstraintOrConnpoint().add(dst);

        this.currModel.getRegnodeOrConstraintOrAttribute().add(faultMsgConn);

    }

    public void visitBinding (Binding binding) {
        String localName = binding.getQName().getLocalPart();
        Model bindingModel = this.createGmeModel (localName, "Binding",
                                                  "Binding");
        Regnode partregs = this.createPartRegs (xPos, yPos);
        bindingModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(bindingModel);
        this.bindingMap.put(binding.getQName(), bindingModel);
        PortType bindingPortType = binding.getPortType();
        Model refPortType = this.portTypeMap.get(bindingPortType.getQName());
        if (refPortType == null) {
            System.out.println ("Error: No portType named" +
                                bindingPortType.getQName() + "found.");
            System.exit(1);
        }

        Connection portType = this.createGmeConnection ("BindingPortType",
                                                        "BindingPortType",
                                                        "BindingPortType");
        Regnode autoRouter = this.createAutoRouterPref("We");
        portType.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        Connpoint src = this.createGmeConnpoint ("src");
        src.setTarget(bindingModel);
        portType.getRegnodeOrConstraintOrConnpoint().add(src);
        Connpoint dst = this.createGmeConnpoint ("dst");
        dst.setTarget(refPortType);
        portType.getRegnodeOrConstraintOrConnpoint().add(dst);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(portType);

        List extElements = binding.getExtensibilityElements();
        Iterator iterator = extElements.iterator();
        while (iterator.hasNext()) {
            ExtensibilityElement elem = (ExtensibilityElement)iterator.next();
            if (elem instanceof SOAPBinding) {
                SOAPBinding soapBinding = (SOAPBinding)elem;
                this.push (bindingModel);
                xPos = yPos = 12;
                this.visitSOAPBinding(soapBinding);
                this.pop();
            }
        }

        List bindingOperations = binding.getBindingOperations();
        iterator = bindingOperations.iterator();
        while (iterator.hasNext()) {
            BindingOperation bindOp = (BindingOperation)iterator.next();
            this.push(bindingModel);
            this.visitBindingOperation(bindOp);
            this.pop();
        }
    }


    public void visitSOAPBinding (SOAPBinding soapBinding) {
        String name = soapBinding.getElementType().getLocalPart();
        Atom binding = this.createGmeAtom (name, "SOAPBinding", "SOAPBinding");
        this.currModel.getRegnodeOrConstraintOrAttribute().add(binding);
        Regnode partregs = this.createPartRegs (xPos, yPos);
        binding.getRegnodeOrConstraintOrAttribute().add(partregs);

        Boolean req = soapBinding.getRequired();
        if (req != null) {
            Attribute required
                = this.createGmeAttribute("required", null,
                                          req.toString());
            binding.getRegnodeOrConstraintOrAttribute().add(required);
        }

        String attrVal = soapBinding.getStyle();
        if (attrVal != null) {
            Attribute style = this.createGmeAttribute("style", null, attrVal);
            binding.getRegnodeOrConstraintOrAttribute().add(style);
        }

        attrVal = soapBinding.getTransportURI();
        if (attrVal != null) {
            Attribute transport = this.createGmeAttribute ("transport", null,
                                                           attrVal);
            binding.getRegnodeOrConstraintOrAttribute().add(transport);
        }
    }

    public void visitBindingOperation (BindingOperation bindingOperation) {
        String localName = bindingOperation.getName();
        Model bindOp = this.createGmeModel (localName, "BindingOperation",
                                            "BindingOperation");
        this.currModel.getRegnodeOrConstraintOrAttribute().add (bindOp);
        Regnode partregs = this.createPartRegs (xPos, yPos);
        bindOp.getRegnodeOrConstraintOrAttribute().add(partregs);

        List extElements = bindingOperation.getExtensibilityElements();
        Iterator iterator = extElements.iterator();
        while (iterator.hasNext()) {
            ExtensibilityElement elem = (ExtensibilityElement)iterator.next();
            if (elem instanceof SOAPOperation) {
                SOAPOperation soapOperation = (SOAPOperation)elem;
                xPos = yPos = 12;
                this.push (bindOp);
                this.visitSOAPOperation(soapOperation);
                this.pop();
            }
        }

        Operation op = bindingOperation.getOperation();
        // The current BindingOperation that is being visited.  Used by the
        // Binding* functions to retrieve otherwise impossible to retrieve
        // information.
        this.currOp = op;
        Model operationRefModel = this.operMap.get(op);
        if (operationRefModel == null) {
            System.out.println ("Error: No operation model named" +
                                op.getName() + "found.");
            System.exit(1);
        }

        // Create the OperationRef
        Reference operationRef
            = this.createGmeReference (op.getName(),
                                       "OperationRef", "OperationRef");
        xPos += 168;
        partregs = this.createPartRegs(xPos, yPos);
        operationRef.getRegnodeOrConstraintOrAttribute().add(partregs);
        operationRef.setReferred(operationRefModel);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(operationRef);

        // Create the BindsOperation connection
        Connection bindsOperation = this.createGmeConnection ("BindsOperation",
                                                              "BindsOperation",
                                                              "BindsOperation");
        Regnode autoRouter = this.createAutoRouterPref ("Ew");
        bindsOperation.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        Connpoint src = this.createGmeConnpoint ("src");
        src.setTarget(bindOp);
        bindsOperation.getRegnodeOrConstraintOrConnpoint().add(src);
        Connpoint dst = this.createGmeConnpoint ("dst");
        dst.setTarget(operationRef);
        bindsOperation.getRegnodeOrConstraintOrConnpoint().add(dst);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(bindsOperation);

        BindingInput bindInput = bindingOperation.getBindingInput();
        if (bindInput != null) {
            this.push(bindOp);
            this.visitBindingInput (bindInput);
            this.pop();
        }
        BindingOutput bindOutput = bindingOperation.getBindingOutput();
        if (bindOutput != null) {
            this.push(bindOp);
            this.visitBindingOutput (bindOutput);
            this.pop();
        }
        Map bindingFaults = bindingOperation.getBindingFaults();
        iterator = bindingFaults.values().iterator();
        while (iterator.hasNext()) {
            BindingFault bindFault = (BindingFault)iterator.next();
            this.push(bindOp);
            this.visitBindingFault(bindFault);
            this.pop();
        }

        // currOp is not guaranteed to have valid values after this point.
        // So make sure we crash if these fields are accesssed again.
        this.currOp = null;

    }

    public void visitBindingInput (BindingInput bindingInput) {
        String localName = bindingInput.getName();
        Input input = this.currOp.getInput();
        if (localName == null) {
            localName = input.getName();
        }
        Model inputRefModel = this.inputMap.get(input);
        if (inputRefModel == null) {
            System.out.println ("Error: No input model named" +
                                input.getName() + "found.");
            System.exit(1);
        }

        String refName = inputRefModel.getName().getContent();
        // Create the inputRef
        Reference inputRef
            = this.createGmeReference (refName, "InputRef", "InputRef");
        xPos += 168;
        yPos += 115;
        Regnode partregs = this.createPartRegs(xPos, yPos);
        inputRef.getRegnodeOrConstraintOrAttribute().add(partregs);
        inputRef.setReferred(inputRefModel);
        this.currRef = inputRef;
        this.currMessage = input.getMessage();
        this.currModel.getRegnodeOrConstraintOrAttribute().add(inputRef);
        List extElements = bindingInput.getExtensibilityElements();
        Iterator iterator = extElements.iterator();
        while (iterator.hasNext()) {
            ExtensibilityElement elem = (ExtensibilityElement)iterator.next();
            if (elem instanceof SOAPBody) {
                SOAPBody soapBody = (SOAPBody)elem;
                this.visitSOAPBody(soapBody);
            } else if (elem instanceof SOAPHeader) {
                SOAPHeader soapHeader = (SOAPHeader)elem;
                this.visitSOAPHeader(soapHeader);
            }
        }
        this.currRef = null;
        this.currMessage = null;
    }


    public void visitBindingOutput (BindingOutput bindingOutput) {
        String localName = bindingOutput.getName();
        Output output = this.currOp.getOutput();
        if (localName == null) {
            localName = output.getName();
        }
        Model outputRefModel = this.outputMap.get(output);
        if (outputRefModel == null) {
            System.out.println ("Error: No output model named " +
                                output.getName() + " found.");
            System.exit(1);
        }

        String refName = outputRefModel.getName().getContent();
        // Create the outputRef
        Reference outputRef
            = this.createGmeReference (refName, "OutputRef", "OutputRef");
        xPos += 168;
        yPos += 115;
        Regnode partregs = this.createPartRegs(xPos, yPos);
        outputRef.getRegnodeOrConstraintOrAttribute().add(partregs);
        outputRef.setReferred(outputRefModel);
        this.currRef = outputRef;
        this.currMessage = output.getMessage();
        this.currModel.getRegnodeOrConstraintOrAttribute().add(outputRef);
        List extElements = bindingOutput.getExtensibilityElements();
        Iterator iterator = extElements.iterator();
        while (iterator.hasNext()) {
            ExtensibilityElement elem = (ExtensibilityElement)iterator.next();
            if (elem instanceof SOAPBody) {
                SOAPBody soapBody = (SOAPBody)elem;
                this.visitSOAPBody(soapBody);
            } else if (elem instanceof SOAPHeader) {
                SOAPHeader soapHeader = (SOAPHeader)elem;
                this.visitSOAPHeader(soapHeader);
            }
        }
        // currRef is no longer valid
        this.currRef = null;
        this.currMessage = null;
    }

    public void visitBindingFault (BindingFault bindingFault) {
        String localName = bindingFault.getName();
        List extElements = bindingFault.getExtensibilityElements();
        Iterator iterator = extElements.iterator();
        while (iterator.hasNext()) {
            ExtensibilityElement elem = (ExtensibilityElement)iterator.next();
            if (elem instanceof SOAPFault) {
                SOAPFault soapFault = (SOAPFault)elem;
                Fault fault = this.currOp.getFault(soapFault.getName());
                if (localName == null) {
                    localName = fault.getName();
                }
                Model faultRefModel = this.faultMap.get(fault);
                if (faultRefModel == null) {
                    System.out.println ("Error: No fault model named" +
                                        fault.getName() + "found.");
                    System.exit(1);
                }
                String refName = faultRefModel.getName().getContent();
                // Create the faultRef
                Reference faultRef
                    = this.createGmeReference (refName,
                                               "FaultRef",
                                               "FaultRef");
                xPos += 168;
                yPos += 115;
                Regnode partregs = this.createPartRegs(xPos, yPos);
                faultRef.getRegnodeOrConstraintOrAttribute().add(partregs);
                faultRef.setReferred(faultRefModel);
                this.currRef = faultRef;
                this.currMessage = fault.getMessage();
                this.currModel.getRegnodeOrConstraintOrAttribute().add(faultRef);
                this.visitSOAPFault(soapFault);
            }
        }
        this.currRef = null;
        this.currMessage = null;
    }


    public void visitSOAPBody (SOAPBody soapBody) {
        String localName = soapBody.getElementType().getLocalPart();
        Atom body = this.createGmeAtom (localName, "SOAPBody",
                                        "SOAPBody");
        this.currModel.getRegnodeOrConstraintOrAttribute().add(body);
        Regnode partregs = this.createPartRegs (xPos, yPos);
        body.getRegnodeOrConstraintOrAttribute().add(partregs);

        Boolean req = soapBody.getRequired();
        if (req != null) {
            Attribute required
                = this.createGmeAttribute("required", null, req.toString());
            body.getRegnodeOrConstraintOrAttribute().add(required);
        }

        String attrVal = soapBody.getUse();
        if (attrVal != null) {
            Attribute use = this.createGmeAttribute("use", null, attrVal);
            body.getRegnodeOrConstraintOrAttribute().add(use);
        }

        attrVal = soapBody.getNamespaceURI();
        if (attrVal != null) {
            Attribute namespace = this.createGmeAttribute ("namespace",
                                                           null, attrVal);
            body.getRegnodeOrConstraintOrAttribute().add(namespace);
        }

        List encodingStyles = soapBody.getEncodingStyles();
        if (encodingStyles != null) {
            Iterator iter = encodingStyles.iterator();
            StringBuilder sb = new StringBuilder();
            while (iter.hasNext()) {
                sb.append((String)iter.next());
                sb.append(" ");
            }
            Attribute encodingStyle
                = this.createGmeAttribute ("encodingStyle", null,
                                           sb.toString().trim());
            body.getRegnodeOrConstraintOrAttribute().add(encodingStyle);
        }


        QName refMsgName = this.currMessage.getQName();
        // Create the PartRef reference
        List parts = soapBody.getParts();
        if (parts != null) {
            Iterator iter = parts.iterator();
            while (iter.hasNext()) {
                String partName = refMsgName.getLocalPart()
                    + (String)iter.next();
                Reference part = this.partMap.get(partName);
                if (part == null) {
                    System.out.println ("Error: No part model named " +
                                        partName + " found.");
                    System.exit(1);
                }
                Reference partRef
                    = this.createGmeReference (part.getName().getContent(),
                                               "PartRef", "PartRef");
                xPos += 168;
                partregs = this.createPartRegs(xPos, yPos);
                partRef.getRegnodeOrConstraintOrAttribute().add(partregs);
                partRef.setReferred(part);
                this.currModel.getRegnodeOrConstraintOrAttribute().add(partRef);

                // Create the BodyParts connection
                Connection bodyPartsConn
                    = this.createGmeConnection ("BodyParts", "BodyParts",
                                                "BodyParts");
                Regnode autoRouter = this.createAutoRouterPref("Ew");
                bodyPartsConn.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

                Connpoint src = this.createGmeConnpoint ("src");
                src.setTarget(body);
                bodyPartsConn.getRegnodeOrConstraintOrConnpoint().add(src);
                Connpoint dst = this.createGmeConnpoint ("dst");
                dst.setTarget(partRef);
                bodyPartsConn.getRegnodeOrConstraintOrConnpoint().add(dst);
                this.currModel.getRegnodeOrConstraintOrAttribute().add(bodyPartsConn);
            }
        }

        // Create the BodyBinding connection
        Connection bodyBinding = this.createGmeConnection ("BodyBinding",
                                                           "BodyBinding",
                                                           "BodyBinding");
        Regnode autoRouter = this.createAutoRouterPref("Ew");
        bodyBinding.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        Connpoint src = this.createGmeConnpoint ("src");
        src.setTarget(this.currRef);
        bodyBinding.getRegnodeOrConstraintOrConnpoint().add(src);
        Connpoint dst = this.createGmeConnpoint ("dst");
        dst.setTarget(body);
        bodyBinding.getRegnodeOrConstraintOrConnpoint().add(dst);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(bodyBinding);
    }

    public void visitSOAPHeader (SOAPHeader soapHeader) {
        String localName = soapHeader.getElementType().getLocalPart();
        Model header = this.createGmeModel (localName, "SOAPHeader",
                                            "SOAPHeader");
        this.currModel.getRegnodeOrConstraintOrAttribute().add(header);
        Regnode partregs = this.createPartRegs (xPos, yPos);
        header.getRegnodeOrConstraintOrAttribute().add(partregs);

        Boolean req = soapHeader.getRequired();
        if (req != null) {
            Attribute required
                = this.createGmeAttribute("required", null, req.toString());
            header.getRegnodeOrConstraintOrAttribute().add(required);
        }

        String attrVal = soapHeader.getUse();
        if (attrVal != null) {
            Attribute use = this.createGmeAttribute("use", null, attrVal);
            header.getRegnodeOrConstraintOrAttribute().add(use);
        }

        attrVal = soapHeader.getNamespaceURI();
        if (attrVal != null) {
            Attribute namespace = this.createGmeAttribute ("namespace",
                                                           null, attrVal);
            header.getRegnodeOrConstraintOrAttribute().add(namespace);
        }

        List encodingStyles = soapHeader.getEncodingStyles();
        if (encodingStyles != null) {
            Iterator iter = encodingStyles.iterator();
            StringBuilder sb = new StringBuilder();
            while (iter.hasNext()) {
                sb.append((String)iter.next());
                sb.append(" ");
            }
            Attribute encodingStyle
                = this.createGmeAttribute ("encodingStyle", null,
                                           sb.toString().trim());
            header.getRegnodeOrConstraintOrAttribute().add(encodingStyle);
        }

        // Create the message reference
        QName refMessageName = soapHeader.getMessage();
        Model refMsgModel = this.messageMap.get (refMessageName);
        Reference messageRef
            = this.createGmeReference (refMessageName.getLocalPart(),
                                       "MessageRef", "MessageRef");
        xPos += 168;
        partregs = this.createPartRegs(xPos, yPos);
        messageRef.getRegnodeOrConstraintOrAttribute().add(partregs);
        messageRef.setReferred(refMsgModel);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(messageRef);

        // Create the HeaderMessage connection
        Connection headerMsgConn = this.createGmeConnection ("HeaderMessage",
                                                             "HeaderMessage",
                                                             "HeaderMessage");
        Regnode autoRouter = this.createAutoRouterPref("Ew");
        headerMsgConn.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        Connpoint src = this.createGmeConnpoint ("src");
        src.setTarget(header);
        headerMsgConn.getRegnodeOrConstraintOrConnpoint().add(src);
        Connpoint dst = this.createGmeConnpoint ("dst");
        dst.setTarget(messageRef);
        headerMsgConn.getRegnodeOrConstraintOrConnpoint().add(dst);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(headerMsgConn);

        // Create the part reference
        String partName = refMessageName.getLocalPart() + soapHeader.getPart();
        Reference part = this.partMap.get(partName);
        if (part == null) {
            System.out.println ("Error: No part model named " +
                                partName + " found.");
            System.exit(1);
        }

        Reference partRef
            = this.createGmeReference (part.getName().getContent(),
                                       "PartRef", "PartRef");
        xPos += 168;
        partregs = this.createPartRegs(xPos, yPos);
        partRef.getRegnodeOrConstraintOrAttribute().add(partregs);
        partRef.setReferred(part);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(partRef);


        // Create the HeaderPart connection
        Connection headerPartConn = this.createGmeConnection ("HeaderPart",
                                                              "HeaderPart",
                                                              "HeaderPart");
        autoRouter = this.createAutoRouterPref("Ew");
        headerPartConn.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        src = this.createGmeConnpoint ("src");
        src.setTarget(header);
        headerPartConn.getRegnodeOrConstraintOrConnpoint().add(src);
        dst = this.createGmeConnpoint ("dst");
        dst.setTarget(partRef);
        headerPartConn.getRegnodeOrConstraintOrConnpoint().add(dst);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(headerPartConn);

        // Create the HeaderBinding connection
        Connection headerBinding = this.createGmeConnection ("HeaderBinding",
                                                             "HeaderBinding",
                                                             "HeaderBinding");
        autoRouter = this.createAutoRouterPref("Ew");
        headerBinding.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        src = this.createGmeConnpoint ("src");
        src.setTarget(this.currRef);
        headerBinding.getRegnodeOrConstraintOrConnpoint().add(src);
        dst = this.createGmeConnpoint ("dst");
        dst.setTarget(header);
        headerBinding.getRegnodeOrConstraintOrConnpoint().add(dst);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(headerBinding);

        List headerFaults = soapHeader.getSOAPHeaderFaults();
        Iterator iter = headerFaults.iterator();
        while (iter.hasNext()) {
            SOAPHeaderFault soapHeaderFault = (SOAPHeaderFault)iter.next();
            this.visitSOAPHeaderFault(soapHeaderFault);
        }
    }

    public void visitSOAPHeaderFault (SOAPHeaderFault soapHeaderFault) {
        String localName = soapHeaderFault.getElementType().getLocalPart();
        Atom headerFault = this.createGmeAtom (localName, "SOAPHeaderFault",
                                               "SOAPHeaderFault");
        this.currModel.getRegnodeOrConstraintOrAttribute().add(headerFault);
        Regnode partregs = this.createPartRegs (xPos, yPos);
        headerFault.getRegnodeOrConstraintOrAttribute().add(partregs);

        Boolean req = soapHeaderFault.getRequired();
        if (req != null) {
            Attribute required
                = this.createGmeAttribute("required", null, req.toString());
            headerFault.getRegnodeOrConstraintOrAttribute().add(required);
        }

        String attrVal = soapHeaderFault.getUse();
        if (attrVal != null) {
            Attribute use = this.createGmeAttribute("use", null, attrVal);
            headerFault.getRegnodeOrConstraintOrAttribute().add(use);
        }

        attrVal = soapHeaderFault.getNamespaceURI();
        if (attrVal != null) {
            Attribute namespace
                = this.createGmeAttribute ("namespace", null, attrVal);
            headerFault.getRegnodeOrConstraintOrAttribute().add(namespace);
        }

        List encodingStyles = soapHeaderFault.getEncodingStyles();
        if (encodingStyles != null) {

            Iterator iter = encodingStyles.iterator();
            StringBuilder sb = new StringBuilder();
            while (iter.hasNext()) {
                sb.append((String)iter.next());
                sb.append(" ");
            }
            Attribute encodingStyle
                = this.createGmeAttribute ("encodingStyle", null,
                                           sb.toString().trim());
            headerFault.getRegnodeOrConstraintOrAttribute().add(encodingStyle);
        }

        // Create the message reference
        QName refMessageName = soapHeaderFault.getMessage();
        Model refMsgModel = this.messageMap.get (refMessageName);
        Reference messageRef
            = this.createGmeReference (refMessageName.getLocalPart(),
                                       "MessageRef", "MessageRef");
        xPos += 168;
        partregs = this.createPartRegs(xPos, yPos);
        messageRef.getRegnodeOrConstraintOrAttribute().add(partregs);
        messageRef.setReferred(refMsgModel);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(messageRef);

        // Create the HeaderMessage connection
        Connection headerMsgConn = this.createGmeConnection ("HeaderMessage",
                                                             "HeaderMessage",
                                                             "HeaderMessage");
        Regnode autoRouter = this.createAutoRouterPref("Ew");
        headerMsgConn.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        Connpoint src = this.createGmeConnpoint ("src");
        src.setTarget(headerFault);
        headerMsgConn.getRegnodeOrConstraintOrConnpoint().add(src);
        Connpoint dst = this.createGmeConnpoint ("dst");
        dst.setTarget(messageRef);
        headerMsgConn.getRegnodeOrConstraintOrConnpoint().add(dst);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(headerMsgConn);

        // Create the part reference
        String partName = refMessageName.getLocalPart() + soapHeaderFault.getPart();
        Reference part = this.partMap.get(partName);
        if (part == null) {
            System.out.println ("Error: No part model named " +
                                partName + " found.");
            System.exit(1);
        }

        Reference partRef
            = this.createGmeReference (part.getName().getContent(),
                                       "PartRef", "PartRef");
        xPos += 168;
        partregs = this.createPartRegs(xPos, yPos);
        partRef.getRegnodeOrConstraintOrAttribute().add(partregs);
        partRef.setReferred(part);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(partRef);


        // Create the HeaderPart connection
        Connection headerPartConn = this.createGmeConnection ("HeaderPart",
                                                              "HeaderPart",
                                                              "HeaderPart");
        autoRouter = this.createAutoRouterPref("Ew");
        headerPartConn.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        src = this.createGmeConnpoint ("src");
        src.setTarget(headerFault);
        headerPartConn.getRegnodeOrConstraintOrConnpoint().add(src);
        dst = this.createGmeConnpoint ("dst");
        dst.setTarget(partRef);
        headerPartConn.getRegnodeOrConstraintOrConnpoint().add(dst);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(headerPartConn);

        // Create the HeaderBinding connection
        Connection headerBinding = this.createGmeConnection ("HeaderBinding",
                                                             "HeaderBinding",
                                                             "HeaderBinding");
        autoRouter = this.createAutoRouterPref("Ew");
        headerBinding.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        src = this.createGmeConnpoint ("src");
        src.setTarget(this.currRef);
        headerBinding.getRegnodeOrConstraintOrConnpoint().add(src);
        dst = this.createGmeConnpoint ("dst");
        dst.setTarget(headerFault);
        headerBinding.getRegnodeOrConstraintOrConnpoint().add(dst);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(headerBinding);
    }

    public void visitSOAPOperation (SOAPOperation soapOperation) {

        String name = soapOperation.getElementType().getLocalPart();
        Atom operation = this.createGmeAtom (name, "SOAPOperation",
                                             "SOAPOperation");
        this.currModel.getRegnodeOrConstraintOrAttribute().add(operation);
        Regnode partregs = this.createPartRegs (xPos, yPos);
        operation.getRegnodeOrConstraintOrAttribute().add(partregs);

        Boolean req = soapOperation.getRequired();
        if (req != null) {
            Attribute required = this.createGmeAttribute("required",
                                                         null,
                                                         req.toString());
            operation.getRegnodeOrConstraintOrAttribute().add(required);
        }

        String attrVal = soapOperation.getStyle();
        if (attrVal != null) {
            Attribute style = this.createGmeAttribute("style", null, attrVal);
            operation.getRegnodeOrConstraintOrAttribute().add(style);
        }

        attrVal = soapOperation.getSoapActionURI();
        if (attrVal != null) {
            Attribute soapAction = this.createGmeAttribute ("soapAction",
                                                            null, attrVal);
            operation.getRegnodeOrConstraintOrAttribute().add(soapAction);
        }
    }

    public void visitSOAPFault (SOAPFault soapFault) {
        String localName = soapFault.getName();
        Atom fault = this.createGmeAtom (localName, "SOAPFault",
                                         "SOAPFault");
        this.currModel.getRegnodeOrConstraintOrAttribute().add(fault);
        Regnode partregs = this.createPartRegs (xPos, yPos);
        fault.getRegnodeOrConstraintOrAttribute().add(partregs);

        Boolean req = soapFault.getRequired();
        if (req != null) {
            Attribute required
                = this.createGmeAttribute("required", null, req.toString());
            fault.getRegnodeOrConstraintOrAttribute().add(required);
        }

        String attrVal = soapFault.getUse();
        if (attrVal != null) {
            Attribute use = this.createGmeAttribute("use", null, attrVal);
            fault.getRegnodeOrConstraintOrAttribute().add(use);
        }

        attrVal = soapFault.getNamespaceURI();
        if (attrVal != null) {
            Attribute namespace
                = this.createGmeAttribute ("namespace", null, attrVal);
            fault.getRegnodeOrConstraintOrAttribute().add(namespace);
        }

        List encodingStyles = soapFault.getEncodingStyles();
        if (encodingStyles != null) {
            Iterator iter = encodingStyles.iterator();
            StringBuilder sb = new StringBuilder();
            while (iter.hasNext()) {
                sb.append((String)iter.next());
                sb.append(" ");
            }
            Attribute encodingStyle
                = this.createGmeAttribute ("encodingStyle", null,
                                           sb.toString().trim());
            fault.getRegnodeOrConstraintOrAttribute().add(encodingStyle);
        }

        // Create the SOAPFaultExtension connection
        Connection faultExtension
            = this.createGmeConnection ("SOAPFaultExtension",
                                        "SOAPFaultExtension",
                                        "SOAPFaultExtension");
        Regnode autoRouter = this.createAutoRouterPref("Ew");
        faultExtension.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        Connpoint src = this.createGmeConnpoint ("src");
        src.setTarget(this.currRef);
        faultExtension.getRegnodeOrConstraintOrConnpoint().add(src);
        Connpoint dst = this.createGmeConnpoint ("dst");
        dst.setTarget(fault);
        faultExtension.getRegnodeOrConstraintOrConnpoint().add(dst);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(faultExtension);
    }


    public void visitService (Service service) {
        String localName = service.getQName().getLocalPart();
        Model serviceModel = this.createGmeModel(localName, "Service",
                                                 "Service");
        Regnode partregs = this.createPartRegs (xPos, yPos);
        serviceModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        Map ports = service.getPorts();
        Iterator iter = ports.values().iterator();
        while (iter.hasNext()) {
            Port port = (Port)iter.next();
            this.push(serviceModel);
            this.visitPort(port);
            this.pop();
        }
        this.currModel.getRegnodeOrConstraintOrAttribute().add(serviceModel);
        this.serviceMap.put(service.getQName(), serviceModel);
    }

    public void visitPort (Port port) {
        String localName = port.getName();
        Model portModel = this.createGmeModel (localName, "Port", "Port");
        Regnode partregs = this.createPartRegs(xPos, yPos);
        portModel.getRegnodeOrConstraintOrAttribute().add(partregs);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(portModel);

        QName bindingName = port.getBinding().getQName();
        Model bindingModel = this.bindingMap.get(bindingName);
        if (bindingModel == null) {
            System.out.println ("Error: No binding model named " +
                                bindingName + " found.");
            System.exit(1);
        }
        Reference bindingRef
            = this.createGmeReference (bindingName.getLocalPart(),
                                       "BindingRef", "BindingRef");
        xPos += 168;
        partregs = this.createPartRegs(xPos, yPos);
        bindingRef.getRegnodeOrConstraintOrAttribute().add(partregs);
        bindingRef.setReferred(bindingModel);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(bindingRef);
        // Create the PortBinding connection
        Connection portBinding = this.createGmeConnection ("PortBinding",
                                                           "PortBinding",
                                                           "PortBinding");
        Regnode autoRouter = this.createAutoRouterPref("Ew");
        portBinding.getRegnodeOrConstraintOrConnpoint().add(autoRouter);

        Connpoint src = this.createGmeConnpoint ("src");
        src.setTarget(portModel);
        portBinding.getRegnodeOrConstraintOrConnpoint().add(src);
        Connpoint dst = this.createGmeConnpoint ("dst");
        dst.setTarget(bindingRef);
        portBinding.getRegnodeOrConstraintOrConnpoint().add(dst);
        this.currModel.getRegnodeOrConstraintOrAttribute().add(portBinding);

        List extElements = port.getExtensibilityElements();
        Iterator iterator = extElements.iterator();
        while (iterator.hasNext()) {
            ExtensibilityElement elem = (ExtensibilityElement)iterator.next();
            if (elem instanceof SOAPAddress) {
                SOAPAddress address = (SOAPAddress)elem;
                this.push (portModel);
                this.visitSOAPAddress(address);
                this.pop();
            }
        }
    }


    public void visitSOAPAddress (SOAPAddress soapAddress) {
        String localName = soapAddress.getElementType().getLocalPart();
        Atom address = this.createGmeAtom (localName, "SOAPAddress",
                                           "SOAPAddress");
        this.currModel.getRegnodeOrConstraintOrAttribute().add(address);
        xPos = yPos = 12;
        Regnode partregs = this.createPartRegs (xPos, yPos);
        address.getRegnodeOrConstraintOrAttribute().add(partregs);

        Boolean req = soapAddress.getRequired();
        if (req != null) {
            Attribute required
                = this.createGmeAttribute("required", null, req.toString());
            address.getRegnodeOrConstraintOrAttribute().add(required);
        }

        String attrVal = soapAddress.getLocationURI();
        if (attrVal != null) {
            Attribute location = this.createGmeAttribute("location", null,
                                                         attrVal);
            address.getRegnodeOrConstraintOrAttribute().add(location);
        }
    }
}
