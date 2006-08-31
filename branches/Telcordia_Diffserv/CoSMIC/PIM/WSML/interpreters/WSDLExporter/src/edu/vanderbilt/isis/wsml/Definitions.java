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
 * Domain specific class of <code>Definitions</code>.
 */ 
public class Definitions extends MgaObject
{
	// meta information
	public static final String META_TYPE = "Definitions";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected Definitions(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>Definitions</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Definitions create(Import parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Definitions(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Definitions</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Definitions create(RootFolder parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Definitions(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/**
	 * Returns all the children of type <code>Binding<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Binding[] getBindingChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Binding.META_TYPE, Binding.META_TYPE_NS);
		Binding[] res = new Binding[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (Binding)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Return the child of type <code>Types<code> of this container. 
	 * @return  The child
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Types getTypesChild()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Types.META_TYPE, Types.META_TYPE_NS); 
		if (container.getLength() > 0)

			return (Types)Utils.wrapWithSubclass(container.getAt(0), metaDiagram);
		return null;

	}

	/**
	 * Returns all the children of type <code>Service<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Service[] getServiceChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Service.META_TYPE, Service.META_TYPE_NS);
		Service[] res = new Service[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (Service)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Return the child of type <code>Messages<code> of this container. 
	 * @return  The child
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Messages getMessagesChild()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Messages.META_TYPE, Messages.META_TYPE_NS); 
		if (container.getLength() > 0)

			return (Messages)Utils.wrapWithSubclass(container.getAt(0), metaDiagram);
		return null;

	}

	/**
	 * Returns all the children of type <code>PortType<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public PortType[] getPortTypeChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, PortType.META_TYPE, PortType.META_TYPE_NS);
		PortType[] res = new PortType[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (PortType)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>Import<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Import[] getImportChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Import.META_TYPE, Import.META_TYPE_NS);
		Import[] res = new Import[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (Import)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>BindingPortType<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public BindingPortType[] getBindingPortTypeChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, BindingPortType.META_TYPE, BindingPortType.META_TYPE_NS);
		BindingPortType[] res = new BindingPortType[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (BindingPortType)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
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

	/**
	 * Returns all the children of type <code>Attribute<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Attribute[] getAttributeChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Attribute.META_TYPE, Attribute.META_TYPE_NS);
		Attribute[] res = new Attribute[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (Attribute)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/* Attribute setters, getters */

	/**
	 * Attribute for <code>targetNamespace</code>.
	 */
	public static final String targetNamespace = "targetNamespace";

	/**
	 * Sets the value of the attribute <code>targetNamespace</code> to a value specified by the parameter.
	 * @param _v The new value of the attribute
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void settargetNamespace(String _v)
		throws UdmException 
	{
		setStringVal(targetNamespace, _v);
	}

	/**
	 * Returns the value of the attribute <code>targetNamespace</code>.
	 * @return  The value
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public String gettargetNamespace()
		throws UdmException 
	{
		return getStringVal(targetNamespace);
	}

	/* Associations */

}
