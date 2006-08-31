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
 * Domain specific class of <code>RequestResponseOperation</code>.
 */ 
public class RequestResponseOperation extends Operation
{
	// meta information
	public static final String META_TYPE = "RequestResponseOperation";
	public static final String META_TYPE_NS = "";
	private static UdmPseudoObject metaClass;

	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected RequestResponseOperation(UdmPseudoObject upo, Diagram metaDiagram) 
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
	 * @return  An instance of the class <code>RequestResponseOperation</code>
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static RequestResponseOperation create(PortType parent) 
		throws UdmException 
	{
		Diagram metaDiagram = parent.getDiagram();
		return new RequestResponseOperation(parent.createObject(META_TYPE, META_TYPE_NS), metaDiagram);
	}

	/* Accessing children */

	/**
	 * Returns all the children of type <code>Fault<code> of this container. 
	 * @return  The children in an array
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Fault[] getFaultChildren()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Fault.META_TYPE, Fault.META_TYPE_NS);
		Fault[] res = new Fault[container.getLength()];
		for (int i=0; i < container.getLength(); i++) 
		{
			res[i] = (Fault)Utils.wrapWithSubclass(container.getAt(i), metaDiagram);
		}
		return res;
	}

	/**
	 * Return the child of type <code>Output<code> of this container. 
	 * @return  The child
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Output getOutputChild()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Output.META_TYPE, Output.META_TYPE_NS); 
		if (container.getLength() > 0)

			return (Output)Utils.wrapWithSubclass(container.getAt(0), metaDiagram);
		return null;

	}

	/**
	 * Return the child of type <code>Input<code> of this container. 
	 * @return  The child
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public Input getInputChild()
		throws UdmException 
	{
		UdmPseudoObjectContainer container = getChildren(null, Input.META_TYPE, Input.META_TYPE_NS); 
		if (container.getLength() > 0)

			return (Input)Utils.wrapWithSubclass(container.getAt(0), metaDiagram);
		return null;

	}

	/* Attribute setters, getters */

	/**
	 * Attribute for <code>parameterOrder</code>.
	 */
	public static final String parameterOrder = "parameterOrder";

	/**
	 * Sets the value of the attribute <code>parameterOrder</code> to a value specified by the parameter.
	 * @param _v The new value of the attribute
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setparameterOrder(String _v)
		throws UdmException 
	{
		setStringVal(parameterOrder, _v);
	}

	/**
	 * Returns the value of the attribute <code>parameterOrder</code>.
	 * @return  The value
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public String getparameterOrder()
		throws UdmException 
	{
		return getStringVal(parameterOrder);
	}

	/* Associations */

}
