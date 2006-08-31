/* Generated on Sat Jul 29 22:41:36 2006 */

/* This is a generalt file, do not modify its content.
 * Copyright (c) Vanderbilt University, 2000-2005
 * ALL RIGHTS RESERVED
 * Vanderbilt University disclaims all warranties with regard to this 
 * software, including all implied warranties of merchantability and 
 * fitness.  In no event shall Vanderbilt University be liable for any 
 * special, indirect or consequential damages or any damages whatsoever
 * resulting from loss of use, data or profits, whether in an action of
 * contract, negligence or other tortious action, arising out of or in 
 * connection with the use or performance of this software.	
 */

package edu.vanderbilt.isis.wsml;

import java.io.InputStream;

import edu.vanderbilt.isis.udm.Diagram;
import edu.vanderbilt.isis.udm.UdmException;
import edu.vanderbilt.isis.udm.UdmStringFactory;
import edu.vanderbilt.isis.udm.UdmHelper;

/**
 * Domain specific factory class for <code>RootFolder</code>.
 * <p>This factory allows only string and stream operations on the data network.
 * <p>This example opens an existing data network from a string
 * where the type of the root object is <code>RootFolder</code>.
 * <blockquote><pre>
 *         ....;
 *         String xmlString = "<...>";
 *        // open the data network
 *        RootFolderStringFactory gtf = FactoryRepository.getWSMLRootFolderStringFactory();
 *        RootFolder root = gtf.open(xmlString);
 * 
 *         // manipulate the data network
 *         ....;
 * 
 *         // check constraints
 *         gtf.checkConstraints();
 * 
 *         // close  and save the data network in a string
 *         String resul = gtf.save();
 *         ....;
 * </pre></blockquote>
 */ 
public class RootFolderStringFactory
{
	// resource information
	private static final String packagePath = "/edu/vanderbilt/isis/meta/";
	private static final String xmlMetaFile =".._.._.._paradigms_WSML.xml";
	private static final String xsdMetaFile ="WSML.xsd";
	private static final String metaName ="WSML";
	// the wrapped string factory instance
	private UdmStringFactory factory;

	/**
	 * Constructor.
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public RootFolderStringFactory()
		 throws UdmException 
	{
		factory = new UdmStringFactory(xmlMetaFile, xsdMetaFile, metaName, packagePath){
			protected void loadDiagram() throws UdmException {
				metaDiagram =
					new Diagram(
						xmlMetaFile, edu.vanderbilt.isis.meta.WSML_udm_xml.getString());
			}
		};
		initializeMetaClasses();
	}

	/**
	 * Initializes the meta classes.
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	private void initializeMetaClasses() 
		 throws UdmException 
	{
		factory.getDiagram().getMetaClass(HeaderMessage.META_TYPE, HeaderMessage.META_TYPE_NS);
		factory.getDiagram().getMetaClass(HeaderPart.META_TYPE, HeaderPart.META_TYPE_NS);
		factory.getDiagram().getMetaClass(BodyParts.META_TYPE, BodyParts.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SOAPFaultExtension.META_TYPE, SOAPFaultExtension.META_TYPE_NS);
		factory.getDiagram().getMetaClass(HeaderBinding.META_TYPE, HeaderBinding.META_TYPE_NS);
		factory.getDiagram().getMetaClass(BodyBinding.META_TYPE, BodyBinding.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SchemaBuiltins.META_TYPE, SchemaBuiltins.META_TYPE_NS);
		factory.getDiagram().getMetaClass(AtomicType.META_TYPE, AtomicType.META_TYPE_NS);
		factory.getDiagram().getMetaClass(BindingRef.META_TYPE, BindingRef.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Message.META_TYPE, Message.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Binding.META_TYPE, Binding.META_TYPE_NS);
		factory.getDiagram().getMetaClass(BindsOperation.META_TYPE, BindsOperation.META_TYPE_NS);
		factory.getDiagram().getMetaClass(PortBinding.META_TYPE, PortBinding.META_TYPE_NS);
		factory.getDiagram().getMetaClass(BindingOperation.META_TYPE, BindingOperation.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Port.META_TYPE, Port.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Types.META_TYPE, Types.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Service.META_TYPE, Service.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Definitions.META_TYPE, Definitions.META_TYPE_NS);
		factory.getDiagram().getMetaClass(OperationRef.META_TYPE, OperationRef.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Messages.META_TYPE, Messages.META_TYPE_NS);
		factory.getDiagram().getMetaClass(PortType.META_TYPE, PortType.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Part.META_TYPE, Part.META_TYPE_NS);
		factory.getDiagram().getMetaClass(PartRef.META_TYPE, PartRef.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Import.META_TYPE, Import.META_TYPE_NS);
		factory.getDiagram().getMetaClass(BindingPortType.META_TYPE, BindingPortType.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Documentation.META_TYPE, Documentation.META_TYPE_NS);
		factory.getDiagram().getMetaClass(InputRef.META_TYPE, InputRef.META_TYPE_NS);
		factory.getDiagram().getMetaClass(OneWayOperation.META_TYPE, OneWayOperation.META_TYPE_NS);
		factory.getDiagram().getMetaClass(MessageRef.META_TYPE, MessageRef.META_TYPE_NS);
		factory.getDiagram().getMetaClass(RequestResponseOperation.META_TYPE, RequestResponseOperation.META_TYPE_NS);
		factory.getDiagram().getMetaClass(FaultRef.META_TYPE, FaultRef.META_TYPE_NS);
		factory.getDiagram().getMetaClass(FaultMessage.META_TYPE, FaultMessage.META_TYPE_NS);
		factory.getDiagram().getMetaClass(OutputMessage.META_TYPE, OutputMessage.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Fault.META_TYPE, Fault.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Output.META_TYPE, Output.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SolicitResponseOperation.META_TYPE, SolicitResponseOperation.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Input.META_TYPE, Input.META_TYPE_NS);
		factory.getDiagram().getMetaClass(InputMessage.META_TYPE, InputMessage.META_TYPE_NS);
		factory.getDiagram().getMetaClass(NotificationOperation.META_TYPE, NotificationOperation.META_TYPE_NS);
		factory.getDiagram().getMetaClass(OutputRef.META_TYPE, OutputRef.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SchemaReference.META_TYPE, SchemaReference.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Attribute.META_TYPE, Attribute.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Element.META_TYPE, Element.META_TYPE_NS);
		factory.getDiagram().getMetaClass(Schema.META_TYPE, Schema.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SchemaImport.META_TYPE, SchemaImport.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SOAPOperation.META_TYPE, SOAPOperation.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SOAPHeader.META_TYPE, SOAPHeader.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SOAPHeaderFault.META_TYPE, SOAPHeaderFault.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SOAPFault.META_TYPE, SOAPFault.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SOAPBody.META_TYPE, SOAPBody.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SOAPBinding.META_TYPE, SOAPBinding.META_TYPE_NS);
		factory.getDiagram().getMetaClass(SOAPAddress.META_TYPE, SOAPAddress.META_TYPE_NS);
		factory.getDiagram().getMetaClass(RootFolder.META_TYPE, RootFolder.META_TYPE_NS);
		// load XSDs
		UdmHelper.StoreXsd("WSML.xsd", edu.vanderbilt.isis.meta.WSML_xsd.getString());
	}

	/**
	 * Creates a new data network with root object of type <code>RootFolder</code> in a string.
	 * @return  The root object of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public RootFolder create()
		 throws UdmException
	{
		RootFolder root =
			new RootFolder(
				factory.createNewDataNetwork(RootFolder.META_TYPE, RootFolder.META_TYPE_NS)
				, factory.getDiagram());
		return root;
	}

	/**
	 * Opens an existing data network from a string specified by the parameter.
	 * @param  xmlString The string containing the xml.
	 * @return  The root object of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public RootFolder open(String xmlString) 
		 throws UdmException
	{
		RootFolder root = 
			new RootFolder(
				factory.openExistingDataNetwork(xmlString)
				, factory.getDiagram());
		return root;
	}

	/**
	 * Opens an existing data network from the stream specified by the parameter.
	 * @param  xmlStream The stream that contains the xml
	 * @return  The root object of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public RootFolder open(InputStream xmlStream) 
		 throws UdmException
	{
		RootFolder root = 
			new RootFolder(
				factory.openExistingDataNetworkFromStream(xmlStream)
				, factory.getDiagram());
		return root;
	}

	/**
	 * Closes the data network without updating it and unloads the diagram.
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void close() 
		 throws UdmException
	{
		factory.closeNoUpdate();
		UdmHelper.ClearXsdStorage();
		factory.unloadDiagram();
	}
	/**
	 * Closes and updates the data network by saving the changes in the data network.
	 * @return  The data network in a string
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public String save() 
		 throws UdmException
	{
		factory.closeWithUpdate();
		String result = factory.saveAs();
		UdmHelper.ClearXsdStorage();
		factory.unloadDiagram();
		return result;
	}

	/**
	 * Closes and updates the data network, saves the changes in a stream,
	 * and unloads the diagram.
	 * @return  The data network in a stream
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public InputStream saveAsStream() 
		 throws UdmException
	{
		factory.closeWithUpdate();
		InputStream result = factory.saveAsStream();
		UdmHelper.ClearXsdStorage();
		factory.unloadDiagram();
		return result;
	}

	/**
	 * Calls the constraint checker.
	 * @return  The result of the constriant evaluation
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public String checkConstraints() 
		 throws UdmException
	{
		return (factory.getDataNetwork().checkConstraints());
	}

}
