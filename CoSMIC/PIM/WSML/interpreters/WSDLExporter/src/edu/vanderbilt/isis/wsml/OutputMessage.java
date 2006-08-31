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
 * Domain specific class of <code>OutputMessage</code>.
 */ 
public class OutputMessage extends MgaObject
{
	// meta information
	public static final String META_TYPE = "OutputMessage";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected OutputMessage(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>OutputMessage</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static OutputMessage create(Operation parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new OutputMessage(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/* Attribute setters, getters */

	/* Associations */

	/*
	 * Asoociation with role name <code>dstOutputMessage</code>.
	 */
	/**
	 * Sets the end of the association with role name <code>dstOutputMessage</code>.
	 * @param a The end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setdstOutputMessage(MessageRef a)
		throws UdmException
	{
		UdmPseudoObjectContainer container = new UdmPseudoObjectContainer(1);
		container.setAt(0, a);
		setAssociation("dstOutputMessage", container, UdmHelper.TARGET_FROM_CLASS);
	}

	/**
	 * Returns the end of the association with role name <code>dstOutputMessage</code>.
	 * @return The end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public MessageRef getdstOutputMessage()
		throws UdmException
	{
		UdmPseudoObjectContainer container = getAssociation("dstOutputMessage", UdmHelper.TARGET_FROM_CLASS);
		if (container.getLength() > 0)

			return (MessageRef) Utils.wrapWithSubclass(container.getAt(0), metaDiagram);
		return null;

	}

	/*
	 * Asoociation with role name <code>srcOutputMessage</code>.
	 */
	/**
	 * Sets the end of the association with role name <code>srcOutputMessage</code>.
	 * @param a The end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setsrcOutputMessage(Output a)
		throws UdmException
	{
		UdmPseudoObjectContainer container = new UdmPseudoObjectContainer(1);
		container.setAt(0, a);
		setAssociation("srcOutputMessage", container, UdmHelper.TARGET_FROM_CLASS);
	}

	/**
	 * Returns the end of the association with role name <code>srcOutputMessage</code>.
	 * @return The end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Output getsrcOutputMessage()
		throws UdmException
	{
		UdmPseudoObjectContainer container = getAssociation("srcOutputMessage", UdmHelper.TARGET_FROM_CLASS);
		if (container.getLength() > 0)

			return (Output) Utils.wrapWithSubclass(container.getAt(0), metaDiagram);
		return null;

	}

}
