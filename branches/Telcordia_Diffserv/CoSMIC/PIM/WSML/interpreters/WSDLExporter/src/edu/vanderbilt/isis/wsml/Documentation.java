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
 * Domain specific class of <code>Documentation</code>.
 */ 
public class Documentation extends MgaObject
{
	// meta information
	public static final String META_TYPE = "Documentation";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected Documentation(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>Documentation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Documentation create(Operation parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Documentation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Documentation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Documentation create(Binding parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Documentation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Documentation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Documentation create(BindingOperation parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Documentation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Documentation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Documentation create(Port parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Documentation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Documentation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Documentation create(Types parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Documentation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Documentation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Documentation create(Definitions parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Documentation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Documentation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Documentation create(PortType parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Documentation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Documentation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Documentation create(Import parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Documentation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Documentation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Documentation create(Fault parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Documentation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Documentation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Documentation create(Output parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Documentation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/**
	 * Creates an instance of the class in the container specified by the parameter. 
	 * @param  parent The parent container
	 * @return  An instance of the class <code>Documentation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static Documentation create(Input parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new Documentation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

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
	 * Attribute for <code>Mixed</code>.
	 */
	public static final String Mixed = "Mixed";

	/**
	 * Sets the value of the attribute <code>Mixed</code> to a value specified by the parameter.
	 * @param _v The new value of the attribute
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setMixed(boolean _v)
		throws UdmException 
	{
		setBooleanVal(Mixed, _v);
	}

	/**
	 * Returns the value of the attribute <code>Mixed</code>.
	 * @return  The value
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public boolean getMixed()
		throws UdmException 
	{
		return getBooleanVal(Mixed);
	}

	/**
	 * Attribute for <code>DocText</code>.
	 */
	public static final String DocText = "DocText";

	/**
	 * Sets the value of the attribute <code>DocText</code> to a value specified by the parameter.
	 * @param _v The new value of the attribute
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setDocText(String _v)
		throws UdmException 
	{
		setStringVal(DocText, _v);
	}

	/**
	 * Returns the value of the attribute <code>DocText</code>.
	 * @return  The value
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public String getDocText()
		throws UdmException 
	{
		return getStringVal(DocText);
	}

	/* Associations */

}
