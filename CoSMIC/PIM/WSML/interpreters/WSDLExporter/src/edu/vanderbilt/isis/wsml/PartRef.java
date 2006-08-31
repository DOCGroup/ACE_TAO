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
 * Domain specific class of <code>PartRef</code>.
 */ 
public class PartRef extends MgaObject
{
	// meta information
	public static final String META_TYPE = "PartRef";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected PartRef(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>PartRef</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static PartRef create(BindingOperation parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new PartRef(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
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
	public void setref(Part[] a)
		throws UdmException
	{
		setAssociation("ref", new UdmPseudoObjectContainer(a), UdmHelper.TARGET_FROM_PEER);
	}

	/**
	 * Returns the other ends of the association with role name <code>ref</code>.
	 * @return The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Part[] getref()
		throws UdmException
	{
		UdmPseudoObjectContainer objs = getAssociation("ref", UdmHelper.TARGET_FROM_PEER);
		return (Part[]) Utils.wrapWithSubclass(objs, Part.class, getDiagram());
	}

	/*
	 * Asoociation with role name <code>srcHeaderPart</code>.
	 */

	/**
	 * Sets the other end of the association with role name <code>srcHeaderPart</code>.
	 * @param a The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setsrcHeaderPart(HeaderPart a)
		throws UdmException
	{
		setAssociation("srcHeaderPart", a, UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other end of the association with role name <code>srcHeaderPart</code>.
	 * @return The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public HeaderPart getsrcHeaderPart()
		throws UdmException
	{
		UdmPseudoObject result = getSingleAssociatedObject("srcHeaderPart", UdmHelper.CLASS_FROM_TARGET);
		return (result == null) ? null : new HeaderPart(result, getDiagram());
	}

	/*
	 * Asoociation with role name <code>srcBodyParts</code>.
	 */

	/**
	 * Sets the other ends of the association with role name <code>srcBodyParts</code>.
	 * @param a The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setsrcBodyParts(BodyParts[] a)
		throws UdmException
	{
		setAssociation("srcBodyParts", new UdmPseudoObjectContainer(a), UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other ends of the association with role name <code>srcBodyParts</code>.
	 * @return The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public BodyParts[] getsrcBodyParts()
		throws UdmException
	{
		UdmPseudoObjectContainer objs = getAssociation("srcBodyParts", UdmHelper.CLASS_FROM_TARGET);
		return (BodyParts[]) Utils.wrapWithSubclass(objs, BodyParts.class, getDiagram());
	}

}
