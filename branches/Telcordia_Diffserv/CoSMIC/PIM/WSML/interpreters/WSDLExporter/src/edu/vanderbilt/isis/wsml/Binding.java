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
 * Domain specific class of <code>Binding</code>.
 */ 
public class Binding extends MgaObject
{
	// meta information
	public static final String META_TYPE = "Binding";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected Binding(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>Binding</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Binding create(Definitions parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Binding(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/**
	 * Return the child of type <code>SOAPBinding<code> of this container. 
	 * @return  The child
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public SOAPBinding getSOAPBindingChild()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, SOAPBinding.META_TYPE, SOAPBinding.META_TYPE_NS); 
		if (container.getLength() > 0)

			return (SOAPBinding)Utils.wrapWithSubclass(container.getAt(0), metaDiagram);
		return null;

	}

	/**
	 * Returns all the children of type <code>BindsOperation<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public BindsOperation[] getBindsOperationChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, BindsOperation.META_TYPE, BindsOperation.META_TYPE_NS);
		BindsOperation[] res = new BindsOperation[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (BindsOperation)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>BindingOperation<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public BindingOperation[] getBindingOperationChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, BindingOperation.META_TYPE, BindingOperation.META_TYPE_NS);
		BindingOperation[] res = new BindingOperation[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (BindingOperation)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>OperationRef<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public OperationRef[] getOperationRefChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, OperationRef.META_TYPE, OperationRef.META_TYPE_NS);
		OperationRef[] res = new OperationRef[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (OperationRef)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
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

	/* Attribute setters, getters */

	/* Associations */

	/*
	 * Asoociation with role name <code>referedbyBindingRef</code>.
	 */

	/**
	 * Sets the other end of the association with role name <code>referedbyBindingRef</code>.
	 * @param a The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setreferedbyBindingRef(BindingRef a)
		throws UdmException
	{
		setAssociation("referedbyBindingRef", a, UdmHelper.TARGET_FROM_PEER);
	}

	/**
	 * Returns the other end of the association with role name <code>referedbyBindingRef</code>.
	 * @return The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public BindingRef getreferedbyBindingRef()
		throws UdmException
	{
		UdmPseudoObject result = getSingleAssociatedObject("referedbyBindingRef", UdmHelper.TARGET_FROM_PEER);
		return (result == null) ? null : new BindingRef(result, getDiagram());
	}

	/*
	 * Asoociation with role name <code>dstBindingPortType</code>.
	 */

	/**
	 * Sets the other ends of the association with role name <code>dstBindingPortType</code>.
	 * @param a The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setdstBindingPortType(BindingPortType[] a)
		throws UdmException
	{
		setAssociation("dstBindingPortType", new UdmPseudoObjectContainer(a), UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other ends of the association with role name <code>dstBindingPortType</code>.
	 * @return The other ends of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public BindingPortType[] getdstBindingPortType()
		throws UdmException
	{
		UdmPseudoObjectContainer objs = getAssociation("dstBindingPortType", UdmHelper.CLASS_FROM_TARGET);
		return (BindingPortType[]) Utils.wrapWithSubclass(objs, BindingPortType.class, getDiagram());
	}

}
