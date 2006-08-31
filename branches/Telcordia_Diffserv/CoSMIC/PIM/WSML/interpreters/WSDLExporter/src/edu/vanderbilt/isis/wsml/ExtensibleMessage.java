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
 * Domain specific class of <code>ExtensibleMessage</code>.
 */ 
public abstract class ExtensibleMessage extends ExtensibilityElement
{
	// meta information
	/**
	 * Constructor.
	 * @param  upo The object that helps the initialization of the instance 
	 * @param  metaDiagram The diagram of the data network
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	protected ExtensibleMessage(UdmPseudoObject upo, Diagram metaDiagram) 
		throws UdmException
	{
		super(upo, metaDiagram);
	}

	/* Accessing children */

	/* Attribute setters, getters */

	/**
	 * Attribute for <code>use</code>.
	 */
	public static final String use = "use";

	/**
	 * Sets the value of the attribute <code>use</code> to a value specified by the parameter.
	 * @param _v The new value of the attribute
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setuse(String _v)
		throws UdmException 
	{
		setStringVal(use, _v);
	}

	/**
	 * Returns the value of the attribute <code>use</code>.
	 * @return  The value
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public String getuse()
		throws UdmException 
	{
		return getStringVal(use);
	}

	/**
	 * Attribute for <code>encodingStyle</code>.
	 */
	public static final String encodingStyle = "encodingStyle";

	/**
	 * Sets the value of the attribute <code>encodingStyle</code> to a value specified by the parameter.
	 * @param _v The new value of the attribute
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setencodingStyle(String _v)
		throws UdmException 
	{
		setStringVal(encodingStyle, _v);
	}

	/**
	 * Returns the value of the attribute <code>encodingStyle</code>.
	 * @return  The value
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public String getencodingStyle()
		throws UdmException 
	{
		return getStringVal(encodingStyle);
	}

	/**
	 * Attribute for <code>namespace</code>.
	 */
	public static final String namespace = "namespace";

	/**
	 * Sets the value of the attribute <code>namespace</code> to a value specified by the parameter.
	 * @param _v The new value of the attribute
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public void setnamespace(String _v)
		throws UdmException 
	{
		setStringVal(namespace, _v);
	}

	/**
	 * Returns the value of the attribute <code>namespace</code>.
	 * @return  The value
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public String getnamespace()
		throws UdmException 
	{
		return getStringVal(namespace);
	}

	/* Associations */

}
