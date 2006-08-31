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
 * Domain specific class of <code>MessageRef</code>.
 */ 
public class MessageRef extends MgaObject
{
	// meta information
	public static final String META_TYPE = "MessageRef";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected MessageRef(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>MessageRef</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static MessageRef create(Operation parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new MessageRef(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>MessageRef</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static MessageRef create(BindingOperation parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new MessageRef(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/* Attribute setters, getters */

	/* Associations */

	/*
	 * Asoociation with role name <code>ref</code>.
	 */

	/**
	 * Sets the other ends of the association with role name <code>ref</code>.
	 * @param a The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setref(Message[] a)
		throws UdmException
	{
		setAssociation("ref", new UdmPseudoObjectContainer(a), UdmHelper.TARGET_FROM_PEER);
	}

	/**
	 * Returns the other ends of the association with role name <code>ref</code>.
	 * @return The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Message[] getref()
		throws UdmException
	{
		UdmPseudoObjectContainer objs = getAssociation("ref", UdmHelper.TARGET_FROM_PEER);
		return (Message[]) Utils.wrapWithSubclass(objs, Message.class, getDiagram());
	}

	/*
	 * Asoociation with role name <code>srcFaultMessage</code>.
	 */

	/**
	 * Sets the other end of the association with role name <code>srcFaultMessage</code>.
	 * @param a The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setsrcFaultMessage(FaultMessage a)
		throws UdmException
	{
		setAssociation("srcFaultMessage", a, UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other end of the association with role name <code>srcFaultMessage</code>.
	 * @return The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public FaultMessage getsrcFaultMessage()
		throws UdmException
	{
		UdmPseudoObject result = getSingleAssociatedObject("srcFaultMessage", UdmHelper.CLASS_FROM_TARGET);
		return (result == null) ? null : new FaultMessage(result, getDiagram());
	}

	/*
	 * Asoociation with role name <code>srcOutputMessage</code>.
	 */

	/**
	 * Sets the other end of the association with role name <code>srcOutputMessage</code>.
	 * @param a The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setsrcOutputMessage(OutputMessage a)
		throws UdmException
	{
		setAssociation("srcOutputMessage", a, UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other end of the association with role name <code>srcOutputMessage</code>.
	 * @return The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public OutputMessage getsrcOutputMessage()
		throws UdmException
	{
		UdmPseudoObject result = getSingleAssociatedObject("srcOutputMessage", UdmHelper.CLASS_FROM_TARGET);
		return (result == null) ? null : new OutputMessage(result, getDiagram());
	}

	/*
	 * Asoociation with role name <code>srcInputMessage</code>.
	 */

	/**
	 * Sets the other end of the association with role name <code>srcInputMessage</code>.
	 * @param a The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setsrcInputMessage(InputMessage a)
		throws UdmException
	{
		setAssociation("srcInputMessage", a, UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other end of the association with role name <code>srcInputMessage</code>.
	 * @return The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public InputMessage getsrcInputMessage()
		throws UdmException
	{
		UdmPseudoObject result = getSingleAssociatedObject("srcInputMessage", UdmHelper.CLASS_FROM_TARGET);
		return (result == null) ? null : new InputMessage(result, getDiagram());
	}

	/*
	 * Asoociation with role name <code>srcHeaderMessage</code>.
	 */

	/**
	 * Sets the other end of the association with role name <code>srcHeaderMessage</code>.
	 * @param a The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setsrcHeaderMessage(HeaderMessage a)
		throws UdmException
	{
		setAssociation("srcHeaderMessage", a, UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other end of the association with role name <code>srcHeaderMessage</code>.
	 * @return The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public HeaderMessage getsrcHeaderMessage()
		throws UdmException
	{
		UdmPseudoObject result = getSingleAssociatedObject("srcHeaderMessage", UdmHelper.CLASS_FROM_TARGET);
		return (result == null) ? null : new HeaderMessage(result, getDiagram());
	}

}
