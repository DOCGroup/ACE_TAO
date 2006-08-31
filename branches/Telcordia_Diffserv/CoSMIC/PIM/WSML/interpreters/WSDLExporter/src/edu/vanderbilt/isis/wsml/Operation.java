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
 * Domain specific class of <code>Operation</code>.
 */ 
public abstract class Operation extends MgaObject
{
	// meta information
	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected Operation(UdmPseudoObject upo, Diagram metaDiagram) 
		throws UdmException
	{
		super(upo, metaDiagram);
	}

	/* Accessing children */

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
	 * Returns all the children of type <code>FaultMessage<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public FaultMessage[] getFaultMessageChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, FaultMessage.META_TYPE, FaultMessage.META_TYPE_NS);
		FaultMessage[] res = new FaultMessage[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (FaultMessage)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>OutputMessage<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public OutputMessage[] getOutputMessageChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, OutputMessage.META_TYPE, OutputMessage.META_TYPE_NS);
		OutputMessage[] res = new OutputMessage[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (OutputMessage)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>InputMessage<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public InputMessage[] getInputMessageChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, InputMessage.META_TYPE, InputMessage.META_TYPE_NS);
		InputMessage[] res = new InputMessage[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (InputMessage)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/* Attribute setters, getters */

	/* Associations */

	/*
	 * Asoociation with role name <code>referedbyOperationRef</code>.
	 */

	/**
	 * Sets the other end of the association with role name <code>referedbyOperationRef</code>.
	 * @param a The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setreferedbyOperationRef(OperationRef a)
		throws UdmException
	{
		setAssociation("referedbyOperationRef", a, UdmHelper.TARGET_FROM_PEER);
	}

	/**
	 * Returns the other end of the association with role name <code>referedbyOperationRef</code>.
	 * @return The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public OperationRef getreferedbyOperationRef()
		throws UdmException
	{
		UdmPseudoObject result = getSingleAssociatedObject("referedbyOperationRef", UdmHelper.TARGET_FROM_PEER);
		return (result == null) ? null : new OperationRef(result, getDiagram());
	}

}
