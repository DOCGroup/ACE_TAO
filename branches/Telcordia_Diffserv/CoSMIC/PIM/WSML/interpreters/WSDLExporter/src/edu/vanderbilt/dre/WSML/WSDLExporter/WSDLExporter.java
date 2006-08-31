package edu.vanderbilt.dre.WSML.WSDLExporter;


import javax.wsdl.*;
import javax.wsdl.factory.*;
import edu.vanderbilt.isis.wsml.*;
import edu.vanderbilt.isis.udm.*;

public class WSDLExporter {
    
    public static void main(String[] args) {
        if (args == null || args.length < 2) {
            System.err.println("Usage: java edu.vanderbilt.dre.WSML."
                                + "WSDLExporter <InputModel.xml> " 
                                + "<OutputWSDL.wsdl>");
            return;
        }
        String inputModel = new String(args[0]);
        String outputFile = new String(args[1]);
        
        try {
            System.out.print ("Processing input model " + inputModel + "...");
            // open the data network
            RootFolderFileFactory 
                gtf = FactoryRepository.getWSMLRootFolderFileFactory();
            RootFolder root = gtf.open(inputModel);
            
            WSDLFactory factory = WSDLFactory.newInstance();
            WSDLExportVisitor 
                visitor = new WSDLExportVisitor(factory, outputFile);
            visitor.visitRootFolder(root);
            
            // check constraints
            gtf.checkConstraints();
            
            // close and save the data network
            gtf.save();
            System.out.println ("done.");
            System.out.print ("Creating WSDL file " + outputFile + "...");
            visitor.dump();
            System.out.println ("done.");
            
        } catch (WSDLException e) {
            e.printStackTrace();
        } catch (UdmException e) {
            e.printStackTrace();
        }
    }
}
