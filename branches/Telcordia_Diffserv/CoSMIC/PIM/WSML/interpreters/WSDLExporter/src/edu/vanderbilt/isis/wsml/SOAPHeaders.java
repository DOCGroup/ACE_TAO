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
 * Domain specific class of <code>SOAPHeaders</code>.
 */ 
public abstract class SOAPHeaders extends ExtensibleMessage
{
	// meta information
	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected SOAPHeaders(UdmPseudoObject upo, Diagram metaDiagram) 
		throws UdmException
	{
		super(upo, metaDiagram);
	}

	/* Accessing children */

	/* Attribute setters, getters */

	/* Associations */

	/*
	 * Asoociation with role name <code>srcHeaderBinding</code>.
	 */

	/**
	 * Sets the other ends of the association with role name <code>srcHeaderBinding</code>.
	 * @param a The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setsrcHeaderBinding(HeaderBinding[] a)
		throws UdmException
	{
		setAssociation("srcHeaderBinding", new UdmPseudoObjectContainer(a), UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other ends of the association with role name <code>srcHeaderBinding</code>.
	 * @return The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public HeaderBinding[] getsrcHeaderBinding()
		throws UdmException
	{
		UdmPseudoObjectContainer objs = getAssociation("srcHeaderBinding", UdmHelper.CLASS_FROM_TARGET);
		return (HeaderBinding[]) Utils.wrapWithSubclass(objs, HeaderBinding.class, getDiagram());
	}

	/*
	 * Asoociation with role name <code>dstHeaderPart</code>.
	 */

	/**
	 * Sets the other ends of the association with role name <code>dstHeaderPart</code>.
	 * @param a The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setdstHeaderPart(HeaderPart[] a)
		throws UdmException
	{
		setAssociation("dstHeaderPart", new UdmPseudoObjectContainer(a), UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other ends of the association with role name <code>dstHeaderPart</code>.
	 * @return The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public HeaderPart[] getdstHeaderPart()
		throws UdmException
	{
		UdmPseudoObjectContainer objs = getAssociation("dstHeaderPart", UdmHelper.CLASS_FROM_TARGET);
		return (HeaderPart[]) Utils.wrapWithSubclass(objs, HeaderPart.class, getDiagram());
	}

	/*
	 * Asoociation with role name <code>dstHeaderMessage</code>.
	 */

	/**
	 * Sets the other ends of the association with role name <code>dstHeaderMessage</code>.
	 * @param a The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setdstHeaderMessage(HeaderMessage[] a)
		throws UdmException
	{
		setAssociation("dstHeaderMessage", new UdmPseudoObjectContainer(a), UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other ends of the association with role name <code>dstHeaderMessage</code>.
	 * @return The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public HeaderMessage[] getdstHeaderMessage()
		throws UdmException
	{
		UdmPseudoObjectContainer objs = getAssociation("dstHeaderMessage", UdmHelper.CLASS_FROM_TARGET);
		return (HeaderMessage[]) Utils.wrapWithSubclass(objs, HeaderMessage.class, getDiagram());
	}

}
