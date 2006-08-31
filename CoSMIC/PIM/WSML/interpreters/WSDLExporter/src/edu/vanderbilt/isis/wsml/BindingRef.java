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
 * Domain specific class of <code>BindingRef</code>.
 */ 
public class BindingRef extends MgaObject
{
	// meta information
	public static final String META_TYPE = "BindingRef";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected BindingRef(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>BindingRef</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static BindingRef create(Service parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new BindingRef(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/* Attribute setters, getters */

	/* Associations */

	/*
	 * Asoociation with role name <code>srcPortBinding</code>.
	 */

	/**
	 * Sets the other end of the association with role name <code>srcPortBinding</code>.
	 * @param a The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setsrcPortBinding(PortBinding a)
		throws UdmException
	{
		setAssociation("srcPortBinding", a, UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other end of the association with role name <code>srcPortBinding</code>.
	 * @return The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public PortBinding getsrcPortBinding()
		throws UdmException
	{
		UdmPseudoObject result = getSingleAssociatedObject("srcPortBinding", UdmHelper.CLASS_FROM_TARGET);
		return (result == null) ? null : new PortBinding(result, getDiagram());
	}

	/*
	 * Asoociation with role name <code>ref</code>.
	 */

	/**
	 * Sets the other ends of the association with role name <code>ref</code>.
	 * @param a The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setref(Binding[] a)
		throws UdmException
	{
		setAssociation("ref", new UdmPseudoObjectContainer(a), UdmHelper.TARGET_FROM_PEER);
	}

	/**
	 * Returns the other ends of the association with role name <code>ref</code>.
	 * @return The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Binding[] getref()
		throws UdmException
	{
		UdmPseudoObjectContainer objs = getAssociation("ref", UdmHelper.TARGET_FROM_PEER);
		return (Binding[]) Utils.wrapWithSubclass(objs, Binding.class, getDiagram());
	}

}
