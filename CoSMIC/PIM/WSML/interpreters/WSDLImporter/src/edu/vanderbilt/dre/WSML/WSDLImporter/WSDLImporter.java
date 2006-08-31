package edu.vanderbilt.dre.WSML.WSDLImporter;

import java.io.*;
import javax.wsdl.*;
import javax.wsdl.factory.*;
import javax.wsdl.xml.*;
import javax.xml.*;
import javax.xml.parsers.*;
import javax.xml.validation.*;
import org.w3c.dom.*;
import org.xml.sax.*;
import java.net.*;

public class WSDLImporter {

    public static void main (String[] args) {

        if (args == null || args.length < 2) {
            System.err.println("Usage: java edu.vanderbilt.dre.WSML.WSDLImporter <InputWSDL.wsdl> <OutputXme.xme>");
            return;
        }
        String wsdlURI = new String (args[0]);
        String outputFileName = new String (args[1]);
        try {
        	System.out.print ("Processing input WSDL file  " + wsdlURI + "...");
//            SchemaFactory sf
//                = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
//            URL schemaURL
//                = new URL ("file://./2003-02-11.xsd");
//            Schema wsdl = sf.newSchema (schemaURL);
            DocumentBuilderFactory
                dbf = DocumentBuilderFactory.newInstance();
            dbf.setIgnoringComments (true);
            dbf.setNamespaceAware (true);
            dbf.setValidating (false);
            // dbf.setSchema (wsdl);

            DocumentBuilder db = dbf.newDocumentBuilder();
            Document wsdlDoc = db.parse (wsdlURI);

            WSDLFactory factory = WSDLFactory.newInstance();
            WSDLReader reader = factory.newWSDLReader();
            reader.setFeature("javax.wsdl.verbose", false);
            reader.setFeature("javax.wsdl.importDocuments", true);

            Definition def = reader.readWSDL(wsdlDoc.getDocumentURI(),
                                             wsdlDoc);
            System.out.println ("done!");
            System.out.print ("Generating WSML model " + outputFileName + "...");
            XmlExportVisitor visitor
                = new XmlExportVisitor(def.getQName().getLocalPart(),
                                       outputFileName);
            visitor.visitDefinition(def);
            visitor.dump();
            System.out.println ("done!");
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        } catch (SAXException e) {
            e.printStackTrace();
        } catch (NullPointerException e) {
            e.printStackTrace();
        } catch (WSDLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
