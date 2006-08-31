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
 * Domain specific class of <code>Part</code>.
 */ 
public class Part extends MgaObject
{
	// meta information
	public static final String META_TYPE = "Part";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected Part(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>Part</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Part create(Message parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Part(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/* Attribute setters, getters */

	/* Associations */

	/*
	 * Asoociation with role name <code>referedbyPartRef</code>.
	 */

	/**
	 * Sets the other end of the association with role name <code>referedbyPartRef</code>.
	 * @param a The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setreferedbyPartRef(PartRef a)
		throws UdmException
	{
		setAssociation("referedbyPartRef", a, UdmHelper.TARGET_FROM_PEER);
	}

	/**
	 * Returns the other end of the association with role name <code>referedbyPartRef</code>.
	 * @return The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public PartRef getreferedbyPartRef()
		throws UdmException
	{
		UdmPseudoObject result = getSingleAssociatedObject("referedbyPartRef", UdmHelper.TARGET_FROM_PEER);
		return (result == null) ? null : new PartRef(result, getDiagram());
	}

	/*
	 * Asoociation with role name <code>ref</code>.
	 */

	/**
	 * Sets the other ends of the association with role name <code>ref</code>.
	 * @param a The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setref(Referrable[] a)
		throws UdmException
	{
		setAssociation("ref", new UdmPseudoObjectContainer(a), UdmHelper.TARGET_FROM_PEER);
	}

	/**
	 * Returns the other ends of the association with role name <code>ref</code>.
	 * @return The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Referrable[] getref()
		throws UdmException
	{
		UdmPseudoObjectContainer objs = getAssociation("ref", UdmHelper.TARGET_FROM_PEER);
		return (Referrable[]) Utils.wrapWithSubclass(objs, Referrable.class, getDiagram());
	}

}
