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
 * Domain specific class of <code>Schema</code>.
 */ 
public class Schema extends MgaObject
{
	// meta information
	public static final String META_TYPE = "Schema";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected Schema(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>Schema</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Schema create(Types parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Schema(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Schema</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Schema create(SchemaReference parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Schema(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Schema</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Schema create(RootFolder parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Schema(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/**
	 * Returns all the children of type <code>SchemaReference<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public SchemaReference[] getSchemaReferenceChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, SchemaReference.META_TYPE, SchemaReference.META_TYPE_NS);
		SchemaReference[] res = new SchemaReference[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (SchemaReference)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>SchemaImport<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public SchemaImport[] getSchemaImportChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, SchemaImport.META_TYPE, SchemaImport.META_TYPE_NS);
		SchemaImport[] res = new SchemaImport[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (SchemaImport)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
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

	/**
	 * Returns all the children of type <code>Element<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Element[] getElementChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Element.META_TYPE, Element.META_TYPE_NS);
		Element[] res = new Element[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (Element)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
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
