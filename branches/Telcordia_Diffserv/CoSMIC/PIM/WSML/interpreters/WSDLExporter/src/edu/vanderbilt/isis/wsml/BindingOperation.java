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

import edu.vanderbilt.isis.udm.*;

/**
 * Domain specific class of <code>BindingOperation</code>.
 */ 
public class BindingOperation extends MgaObject
{
	// meta information
	public static final String META_TYPE = "BindingOperation";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected BindingOperation(UdmPseudoObject upo, Diagram metaDiagram) 
		throws UdmException
	{
		super(upo, metaDiagram);
	}

	/**
	 * Returns the meta class.
	 * @return  The meta class
	 */ 
	UdmPseudoObject getMetaClass()
	{
		return metaClass;
	}

	/* Construction */

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>BindingOperation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static BindingOperation create(Binding parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new BindingOperation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/**
	 * Returns all the children of type <code>HeaderMessage<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public HeaderMessage[] getHeaderMessageChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, HeaderMessage.META_TYPE, HeaderMessage.META_TYPE_NS);
		HeaderMessage[] res = new HeaderMessage[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (HeaderMessage)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>HeaderPart<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public HeaderPart[] getHeaderPartChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, HeaderPart.META_TYPE, HeaderPart.META_TYPE_NS);
		HeaderPart[] res = new HeaderPart[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (HeaderPart)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>InputRef<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public InputRef[] getInputRefChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, InputRef.META_TYPE, InputRef.META_TYPE_NS);
		InputRef[] res = new InputRef[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (InputRef)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>OutputRef<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public OutputRef[] getOutputRefChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, OutputRef.META_TYPE, OutputRef.META_TYPE_NS);
		OutputRef[] res = new OutputRef[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (OutputRef)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>BodyParts<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public BodyParts[] getBodyPartsChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, BodyParts.META_TYPE, BodyParts.META_TYPE_NS);
		BodyParts[] res = new BodyParts[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (BodyParts)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>SOAPFaultExtension<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public SOAPFaultExtension[] getSOAPFaultExtensionChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, SOAPFaultExtension.META_TYPE, SOAPFaultExtension.META_TYPE_NS);
		SOAPFaultExtension[] res = new SOAPFaultExtension[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (SOAPFaultExtension)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>HeaderBinding<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public HeaderBinding[] getHeaderBindingChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, HeaderBinding.META_TYPE, HeaderBinding.META_TYPE_NS);
		HeaderBinding[] res = new HeaderBinding[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (HeaderBinding)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>BodyBinding<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public BodyBinding[] getBodyBindingChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, BodyBinding.META_TYPE, BodyBinding.META_TYPE_NS);
		BodyBinding[] res = new BodyBinding[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (BodyBinding)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>SOAPHeader<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public SOAPHeader[] getSOAPHeaderChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, SOAPHeader.META_TYPE, SOAPHeader.META_TYPE_NS);
		SOAPHeader[] res = new SOAPHeader[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (SOAPHeader)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>SOAPHeaderFault<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public SOAPHeaderFault[] getSOAPHeaderFaultChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, SOAPHeaderFault.META_TYPE, SOAPHeaderFault.META_TYPE_NS);
		SOAPHeaderFault[] res = new SOAPHeaderFault[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (SOAPHeaderFault)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>SOAPFault<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public SOAPFault[] getSOAPFaultChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, SOAPFault.META_TYPE, SOAPFault.META_TYPE_NS);
		SOAPFault[] res = new SOAPFault[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (SOAPFault)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>SOAPBody<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public SOAPBody[] getSOAPBodyChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, SOAPBody.META_TYPE, SOAPBody.META_TYPE_NS);
		SOAPBody[] res = new SOAPBody[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (SOAPBody)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>PartRef<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public PartRef[] getPartRefChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, PartRef.META_TYPE, PartRef.META_TYPE_NS);
		PartRef[] res = new PartRef[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (PartRef)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Return the child of type <code>SOAPOperation<code> of this container. 
	 * @return  The child
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public SOAPOperation getSOAPOperationChild()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, SOAPOperation.META_TYPE, SOAPOperation.META_TYPE_NS); 
		if (container.getLength() > 0)

			return (SOAPOperation)Utils.wrapWithSubclass(container.getAt(0), metaDiagram);
		return null;

	}

	/**
	 * Return the child of type <code>Documentation<code> of this container. 
	 * @return  The child
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Documentation getDocumentationChild()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Documentation.META_TYPE, Documentation.META_TYPE_NS); 
		if (container.getLength() > 0)

			return (Documentation)Utils.wrapWithSubclass(container.getAt(0), metaDiagram);
		return null;

	}

	/**
	 * Returns all the children of type <code>MessageRef<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public MessageRef[] getMessageRefChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, MessageRef.META_TYPE, MessageRef.META_TYPE_NS);
		MessageRef[] res = new MessageRef[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (MessageRef)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>FaultRef<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public FaultRef[] getFaultRefChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, FaultRef.META_TYPE, FaultRef.META_TYPE_NS);
		FaultRef[] res = new FaultRef[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (FaultRef)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/* Attribute setters, getters */

	/* Associations */

	/*
	 * Asoociation with role name <code>dstBindsOperation</code>.
	 */

	/**
	 * Sets the other ends of the association with role name <code>dstBindsOperation</code>.
	 * @param a The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setdstBindsOperation(BindsOperation[] a)
		throws UdmException
	{
		setAssociation("dstBindsOperation", new UdmPseudoObjectContainer(a), UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other ends of the association with role name <code>dstBindsOperation</code>.
	 * @return The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public BindsOperation[] getdstBindsOperation()
		throws UdmException
	{
		UdmPseudoObjectContainer objs = getAssociation("dstBindsOperation", UdmHelper.CLASS_FROM_TARGET);
		return (BindsOperation[]) Utils.wrapWithSubclass(objs, BindsOperation.class, getDiagram());
	}

}
