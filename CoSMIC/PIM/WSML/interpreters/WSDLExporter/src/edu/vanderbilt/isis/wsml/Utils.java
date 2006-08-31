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
 * Utility class for diagram <code>WSML</code>.
 */ 
public class Utils
{
	/**
	 * Generates a domain specific object from the object specified by the parameter.
	 * @param  obj The generic udm object
	 * @param  diagram The diagram of the data network that contains <code>obj</code>
	 * @return  The domain specific object
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static UdmPseudoObject wrapWithSubclass(UdmPseudoObject obj, Diagram diagram)
		 throws UdmException
	{
		String type = obj.getType();
		if (HeaderMessage.META_TYPE.equals(type))
		{
			return new HeaderMessage(obj, diagram);
		}
		else
		if (HeaderPart.META_TYPE.equals(type))
		{
			return new HeaderPart(obj, diagram);
		}
		else
		if (BodyParts.META_TYPE.equals(type))
		{
			return new BodyParts(obj, diagram);
		}
		else
		if (SOAPFaultExtension.META_TYPE.equals(type))
		{
			return new SOAPFaultExtension(obj, diagram);
		}
		else
		if (HeaderBinding.META_TYPE.equals(type))
		{
			return new HeaderBinding(obj, diagram);
		}
		else
		if (BodyBinding.META_TYPE.equals(type))
		{
			return new BodyBinding(obj, diagram);
		}
		else
		if (SchemaBuiltins.META_TYPE.equals(type))
		{
			return new SchemaBuiltins(obj, diagram);
		}
		else
		if (AtomicType.META_TYPE.equals(type))
		{
			return new AtomicType(obj, diagram);
		}
		else
		if (BindingRef.META_TYPE.equals(type))
		{
			return new BindingRef(obj, diagram);
		}
		else
		if (Message.META_TYPE.equals(type))
		{
			return new Message(obj, diagram);
		}
		else
		if (Binding.META_TYPE.equals(type))
		{
			return new Binding(obj, diagram);
		}
		else
		if (BindsOperation.META_TYPE.equals(type))
		{
			return new BindsOperation(obj, diagram);
		}
		else
		if (PortBinding.META_TYPE.equals(type))
		{
			return new PortBinding(obj, diagram);
		}
		else
		if (BindingOperation.META_TYPE.equals(type))
		{
			return new BindingOperation(obj, diagram);
		}
		else
		if (Port.META_TYPE.equals(type))
		{
			return new Port(obj, diagram);
		}
		else
		if (Types.META_TYPE.equals(type))
		{
			return new Types(obj, diagram);
		}
		else
		if (Service.META_TYPE.equals(type))
		{
			return new Service(obj, diagram);
		}
		else
		if (Definitions.META_TYPE.equals(type))
		{
			return new Definitions(obj, diagram);
		}
		else
		if (OperationRef.META_TYPE.equals(type))
		{
			return new OperationRef(obj, diagram);
		}
		else
		if (Messages.META_TYPE.equals(type))
		{
			return new Messages(obj, diagram);
		}
		else
		if (PortType.META_TYPE.equals(type))
		{
			return new PortType(obj, diagram);
		}
		else
		if (Part.META_TYPE.equals(type))
		{
			return new Part(obj, diagram);
		}
		else
		if (PartRef.META_TYPE.equals(type))
		{
			return new PartRef(obj, diagram);
		}
		else
		if (Import.META_TYPE.equals(type))
		{
			return new Import(obj, diagram);
		}
		else
		if (BindingPortType.META_TYPE.equals(type))
		{
			return new BindingPortType(obj, diagram);
		}
		else
		if (Documentation.META_TYPE.equals(type))
		{
			return new Documentation(obj, diagram);
		}
		else
		if (InputRef.META_TYPE.equals(type))
		{
			return new InputRef(obj, diagram);
		}
		else
		if (OneWayOperation.META_TYPE.equals(type))
		{
			return new OneWayOperation(obj, diagram);
		}
		else
		if (MessageRef.META_TYPE.equals(type))
		{
			return new MessageRef(obj, diagram);
		}
		else
		if (RequestResponseOperation.META_TYPE.equals(type))
		{
			return new RequestResponseOperation(obj, diagram);
		}
		else
		if (FaultRef.META_TYPE.equals(type))
		{
			return new FaultRef(obj, diagram);
		}
		else
		if (FaultMessage.META_TYPE.equals(type))
		{
			return new FaultMessage(obj, diagram);
		}
		else
		if (OutputMessage.META_TYPE.equals(type))
		{
			return new OutputMessage(obj, diagram);
		}
		else
		if (Fault.META_TYPE.equals(type))
		{
			return new Fault(obj, diagram);
		}
		else
		if (Output.META_TYPE.equals(type))
		{
			return new Output(obj, diagram);
		}
		else
		if (SolicitResponseOperation.META_TYPE.equals(type))
		{
			return new SolicitResponseOperation(obj, diagram);
		}
		else
		if (Input.META_TYPE.equals(type))
		{
			return new Input(obj, diagram);
		}
		else
		if (InputMessage.META_TYPE.equals(type))
		{
			return new InputMessage(obj, diagram);
		}
		else
		if (NotificationOperation.META_TYPE.equals(type))
		{
			return new NotificationOperation(obj, diagram);
		}
		else
		if (OutputRef.META_TYPE.equals(type))
		{
			return new OutputRef(obj, diagram);
		}
		else
		if (SchemaReference.META_TYPE.equals(type))
		{
			return new SchemaReference(obj, diagram);
		}
		else
		if (Attribute.META_TYPE.equals(type))
		{
			return new Attribute(obj, diagram);
		}
		else
		if (Element.META_TYPE.equals(type))
		{
			return new Element(obj, diagram);
		}
		else
		if (Schema.META_TYPE.equals(type))
		{
			return new Schema(obj, diagram);
		}
		else
		if (SchemaImport.META_TYPE.equals(type))
		{
			return new SchemaImport(obj, diagram);
		}
		else
		if (SOAPOperation.META_TYPE.equals(type))
		{
			return new SOAPOperation(obj, diagram);
		}
		else
		if (SOAPHeader.META_TYPE.equals(type))
		{
			return new SOAPHeader(obj, diagram);
		}
		else
		if (SOAPHeaderFault.META_TYPE.equals(type))
		{
			return new SOAPHeaderFault(obj, diagram);
		}
		else
		if (SOAPFault.META_TYPE.equals(type))
		{
			return new SOAPFault(obj, diagram);
		}
		else
		if (SOAPBody.META_TYPE.equals(type))
		{
			return new SOAPBody(obj, diagram);
		}
		else
		if (SOAPBinding.META_TYPE.equals(type))
		{
			return new SOAPBinding(obj, diagram);
		}
		else
		if (SOAPAddress.META_TYPE.equals(type))
		{
			return new SOAPAddress(obj, diagram);
		}
		else
		if (RootFolder.META_TYPE.equals(type))
		{
			return new RootFolder(obj, diagram);
		}
		else
		{
			throw new UdmException("Unexpected type: '" + type + "'", true);
		}
	}

	/**
	 * Generates an array of domain specific objects from the objects specified by the parameter.
	 * @param  objs The generic udm objects
	 * @param  arrayClass The type of the array class
	 * @param  diagram The diagram of the data network that contains <code>obj</code>
	 * @return  The array of domain specific objects
	 * @throws  UdmException If any Udm related exception occured
	 */ 
	public static UdmPseudoObject[] wrapWithSubclass(UdmPseudoObjectContainer objs, Class arrayClass, Diagram diagram) 
		 throws UdmException 
	{
		if (!UdmPseudoObject.class.isAssignableFrom(arrayClass)) 
		{
			throw new IllegalArgumentException("" + arrayClass + " is not a subtype of "+ UdmPseudoObject.class.getName());
		}
		UdmPseudoObject[] res = (UdmPseudoObject[]) java.lang.reflect.Array.newInstance(arrayClass, objs.getLength());
		for (int i = 0; i < res.length; i++) 
		{
			UdmPseudoObject obj = objs.getAt(i);
			if (obj != null) 
			{
				res[i] = wrapWithSubclass(obj, diagram);
			}
		}
		return res;
	}
}
