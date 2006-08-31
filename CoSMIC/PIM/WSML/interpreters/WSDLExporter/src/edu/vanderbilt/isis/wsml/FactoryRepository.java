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

import edu.vanderbilt.isis.udm.UdmException;

/**
 * Repository of domain specific factory classes.
 */ 
public class FactoryRepository 
{
	/**
	 * Returns an instance of the factory <code>RootFolderFileFactory</code>
	 * in diagram <code>WSML</code>.
	 * @return An instance of the factory <code>RootFolderFileFactory</code>.
	 * @throws UdmException If any Udm related exception occured
	 * @see edu.vanderbilt.isis.wsml.RootFolderFileFactory 
	 */ 
	public static edu.vanderbilt.isis.wsml.RootFolderFileFactory getWSMLRootFolderFileFactory() 
		 throws UdmException 
	{
		return new edu.vanderbilt.isis.wsml.RootFolderFileFactory();
	}

	/**
	 * Returns an instance of the factory <code>RootFolderStringFactory</code>
	 * in diagram <code>WSML</code>.
	 * @return An instance of the factory <code>RootFolderStringFactory</code>.
	 * @throws UdmException If any Udm related exception occured
	 * @see edu.vanderbilt.isis.wsml.RootFolderStringFactory 
	 */ 
	public static edu.vanderbilt.isis.wsml.RootFolderStringFactory getWSMLRootFolderStringFactory() 
		 throws UdmException 
	{
		return new edu.vanderbilt.isis.wsml.RootFolderStringFactory();
	}

}
