//
///* Generated on Sat Jul 29 22:41:36 2006 */
//
///* This is a generalt file, do not modify its content.
// * Copyright (c) Vanderbilt University, 2000-2005
// * ALL RIGHTS RESERVED
// * Vanderbilt University disclaims all warranties with regard to this 
// * software, including all implied warranties of merchantability and 
// * fitness.  In no event shall Vanderbilt University be liable for any 
// * special, indirect or consequential damages or any damages whatsoever
// * resulting from loss of use, data or profits, whether in an action of
// * contract, negligence or other tortious action, arising out of or in 
// * connection with the use or performance of this software.	
// */
//
//package test;
//
//import edu.vanderbilt.isis.wsml.FactoryRepository;
//import java.io.InputStream;
//import edu.vanderbilt.isis.udm.UdmException;
//
///**
// * Test.
// */ 
//public class TestExample 
//{
//
//	/**
//	 * Creates a new data network in a file with root object <code>RootFolder</code> 
//	 * in diagram <code>WSML</code>.
//	 * @throws UdmException
//	 */
//	public void testCreateInFile_RootFolder_WSML() 
//		throws UdmException 
//	{
//		// open the data network
//		edu.vanderbilt.isis.wsml.RootFolderFileFactory gtf 
//			= FactoryRepository.getWSMLRootFolderFileFactory();
//		edu.vanderbilt.isis.wsml.RootFolder root 
//			= gtf.create("RootFolder-newmodel.xml");
//		
//		// manipulate the data network
//		//...;
//		
//		// check constraints
//		gtf.checkConstraints();
//		
//		// close the data network without saving the changes
//		gtf.close();
//		
//	}
//
//	/**
//	 * Opens an existing data network from a file with root object <code>RootFolder</code> 
//	 * in diagram <code>WSML</code>.
//	 * @throws UdmException
//	 */
//	public void testOpenFromFile_RootFolder_WSML() 
//		throws UdmException 
//	{
//		// open the data network
//		edu.vanderbilt.isis.wsml.RootFolderFileFactory gtf 
//			= FactoryRepository.getWSMLRootFolderFileFactory();
//		edu.vanderbilt.isis.wsml.RootFolder root 
//			= gtf.open("RootFolder-existingmodel.xml");
//		
//		// manipulate the data network
//		//...;
//		
//		// check constraints
//		gtf.checkConstraints();
//		
//		// close and save the data network
//		gtf.save();
//		
//	}
//
//	/**
//	 * Creates a new data network in a string with root object <code>RootFolder</code> 
//	 * in diagram <code>WSML</code>.
//	 * @throws UdmException
//	 */
//	public void testCreateInString_RootFolder_WSML() 
//		throws UdmException 
//	{
//		// open the data network
//		edu.vanderbilt.isis.wsml.RootFolderStringFactory gtf 
//			= FactoryRepository.getWSMLRootFolderStringFactory();
//		edu.vanderbilt.isis.wsml.RootFolder root 
//			= gtf.create();
//		
//		// manipulate the data network
//		//...;
//		
//		// check constraints
//		gtf.checkConstraints();
//		
//		// close the data network without saving the changes
//		gtf.close();
//		
//	}
//
//	/**
//	 * Opens an existing data network from a string with root object <code>RootFolder</code> 
//	 * in diagram <code>WSML</code>.
//	 * @throws UdmException
//	 */
//	public void testOpenFromString_RootFolder_WSML() 
//		throws UdmException 
//	{
//		String xmlString = "<...>";
//		// open the data network
//		edu.vanderbilt.isis.wsml.RootFolderStringFactory gtf 
//			= FactoryRepository.getWSMLRootFolderStringFactory();
//		edu.vanderbilt.isis.wsml.RootFolder root 
//			= gtf.open(xmlString);
//		
//		// manipulate the data network
//		//...;
//		
//		// check constraints
//		gtf.checkConstraints();
//		
//		// close and save the data network
//		String result = gtf.save();
//		System.out.println(result);
//	}
//
//	/**
//	 * Opens an existing data network from a stream with root object <code>RootFolder</code> 
//	 * in diagram <code>WSML</code>.
//	 * @throws UdmException
//	 */
//	public void testOpenFromStream_RootFolder_WSML(InputStream in) 
//		throws UdmException 
//	{
//		// open the data network
//		edu.vanderbilt.isis.wsml.RootFolderStringFactory gtf 
//			= FactoryRepository.getWSMLRootFolderStringFactory();
//		edu.vanderbilt.isis.wsml.RootFolder root 
//			= gtf.open(in);
//		//edu.vanderbilt.isis.wsml.RootFolderFileFactory gtf 
//			//= FactoryRepository.getWSMLRootFolderFileFactory();
//		//edu.vanderbilt.isis.wsml.RootFolder root 
//			//= gtf.open(in);
//		
//		// manipulate the data network
//		//...;
//		
//		// check constraints
//		gtf.checkConstraints();
//		
//		// close and save the data network
//		InputStream result = gtf.saveAsStream();
//		
//	}
//}
