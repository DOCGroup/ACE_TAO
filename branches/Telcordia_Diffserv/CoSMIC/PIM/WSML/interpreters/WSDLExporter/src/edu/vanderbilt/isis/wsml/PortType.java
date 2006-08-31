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
 * Domain specific class of <code>PortType</code>.
 */ 
public class PortType extends MgaObject
{
	// meta information
	public static final String META_TYPE = "PortType";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected PortType(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>PortType</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static PortType create(Definitions parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new PortType(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/**
	 * Returns all the children of type <code>OneWayOperation<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public OneWayOperation[] getOneWayOperationChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, OneWayOperation.META_TYPE, OneWayOperation.META_TYPE_NS);
		OneWayOperation[] res = new OneWayOperation[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (OneWayOperation)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>RequestResponseOperation<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public RequestResponseOperation[] getRequestResponseOperationChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, RequestResponseOperation.META_TYPE, RequestResponseOperation.META_TYPE_NS);
		RequestResponseOperation[] res = new RequestResponseOperation[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (RequestResponseOperation)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>SolicitResponseOperation<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public SolicitResponseOperation[] getSolicitResponseOperationChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, SolicitResponseOperation.META_TYPE, SolicitResponseOperation.META_TYPE_NS);
		SolicitResponseOperation[] res = new SolicitResponseOperation[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (SolicitResponseOperation)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>NotificationOperation<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public NotificationOperation[] getNotificationOperationChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, NotificationOperation.META_TYPE, NotificationOperation.META_TYPE_NS);
		NotificationOperation[] res = new NotificationOperation[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (NotificationOperation)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
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
	 * Asoociation with role name <code>srcBindingPortType</code>.
	 */

	/**
	 * Sets the other end of the association with role name <code>srcBindingPortType</code>.
	 * @param a The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setsrcBindingPortType(BindingPortType a)
		throws UdmException
	{
		setAssociation("srcBindingPortType", a, UdmHelper.CLASS_FROM_TARGET);
	}

	/**
	 * Returns the other end of the association with role name <code>srcBindingPortType</code>.
	 * @return The other end of the association
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public BindingPortType getsrcBindingPortType()
		throws UdmException
	{
		UdmPseudoObject result = getSingleAssociatedObject("srcBindingPortType", UdmHelper.CLASS_FROM_TARGET);
		return (result == null) ? null : new BindingPortType(result, getDiagram());
	}

}
