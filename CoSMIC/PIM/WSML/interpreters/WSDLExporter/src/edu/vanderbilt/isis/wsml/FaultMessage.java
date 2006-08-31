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
 * Domain specific class of <code>FaultMessage</code>.
 */ 
public class FaultMessage extends MgaObject
{
	// meta information
	public static final String META_TYPE = "FaultMessage";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected FaultMessage(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>FaultMessage</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static FaultMessage create(Operation parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new FaultMessage(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/* Attribute setters, getters */

	/* Associations */

	/*
	 * Asoociation with role name <code>dstFaultMessage</code>.
	 */
	/**
	 * Sets the end of the association with role name <code>dstFaultMessage</code>.
	 * @param a The end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setdstFaultMessage(MessageRef a)
		throws UdmException
	{
		UdmPseudoObjectContainer container = new UdmPseudoObjectContainer(1);
		container.setAt(0, a);
		setAssociation("dstFaultMessage", container, UdmHelper.TARGET_FROM_CLASS);
	}

	/**
	 * Returns the end of the association with role name <code>dstFaultMessage</code>.
	 * @return The end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public MessageRef getdstFaultMessage()
		throws UdmException
	{
		UdmPseudoObjectContainer container = getAssociation("dstFaultMessage", UdmHelper.TARGET_FROM_CLASS);
		if (container.getLength() > 0)

			return (MessageRef) Utils.wrapWithSubclass(container.getAt(0), metaDiagram);
		return null;

	}

	/*
	 * Asoociation with role name <code>srcFaultMessage</code>.
	 */
	/**
	 * Sets the end of the association with role name <code>srcFaultMessage</code>.
	 * @param a The end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setsrcFaultMessage(Fault a)
		throws UdmException
	{
		UdmPseudoObjectContainer container = new UdmPseudoObjectContainer(1);
		container.setAt(0, a);
		setAssociation("srcFaultMessage", container, UdmHelper.TARGET_FROM_CLASS);
	}

	/**
	 * Returns the end of the association with role name <code>srcFaultMessage</code>.
	 * @return The end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Fault getsrcFaultMessage()
		throws UdmException
	{
		UdmPseudoObjectContainer container = getAssociation("srcFaultMessage", UdmHelper.TARGET_FROM_CLASS);
		if (container.getLength() > 0)

			return (Fault) Utils.wrapWithSubclass(container.getAt(0), metaDiagram);
		return null;

	}

}
