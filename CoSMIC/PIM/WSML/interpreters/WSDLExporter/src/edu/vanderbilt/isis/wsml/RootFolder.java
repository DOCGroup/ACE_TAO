/* Generated on Sat Jul 29 22:41:37 2006 */

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
 * Domain specific class of <code>RootFolder</code>.
 */ 
public class RootFolder extends UdmPseudoObject
{
	// meta information
	public static final String META_TYPE = "RootFolder";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected RootFolder(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>RootFolder</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static RootFolder create(RootFolder parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new RootFolder(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/**
	 * Returns all the children of type <code>SchemaBuiltins<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public SchemaBuiltins[] getSchemaBuiltinsChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, SchemaBuiltins.META_TYPE, SchemaBuiltins.META_TYPE_NS);
		SchemaBuiltins[] res = new SchemaBuiltins[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (SchemaBuiltins)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>Definitions<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Definitions[] getDefinitionsChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Definitions.META_TYPE, Definitions.META_TYPE_NS);
		Definitions[] res = new Definitions[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (Definitions)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>Schema<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Schema[] getSchemaChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Schema.META_TYPE, Schema.META_TYPE_NS);
		Schema[] res = new Schema[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (Schema)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Returns all the children of type <code>RootFolder<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public RootFolder[] getRootFolderChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, RootFolder.META_TYPE, RootFolder.META_TYPE_NS);
		RootFolder[] res = new RootFolder[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (RootFolder)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/* Attribute setters, getters */

	/**
	 * Attribute for <code>name</code>.
	 */
	public static final String name = "name";

	/**
	 * Sets the value of the attribute <code>name</code> to a value specified by the parameter.
	 * @param _v The new value of the attribute
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setname(String _v)
		throws UdmException 
	{
		setStringVal(name, _v);
	}

	/**
	 * Returns the value of the attribute <code>name</code>.
	 * @return  The value
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public String getname()
		throws UdmException 
	{
		return getStringVal(name);
	}

	/* Associations */

}
