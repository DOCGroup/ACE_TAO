/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = FILENAME
//    Property_Evaluator.java
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu> 
//   
// ========================================================================


import org.omg.CORBA.*;
import CosTrading.*;
import CosTradingDynamic.*;
import TAO_Sequences.*;
import TAO_VR.*;

/**
  * This is a Java port of the TAO_Property_Evaluator class in the TAO
  * trading service implementation.
  */

public class Property_Evaluator
{
  private Property[] props_;
  private boolean supports_dp_ = true;
  
  /**
    * Construct an instance of TAO_Property_Evaluator that operates on
    * a PropertySeq, <properties>.
    */
  Property_Evaluator (Property[] properties)
    {
      this.props_ = properties;
    }

  /**
    * Construct an instance of TAO_Property_Evaluator that operates on
    * an <offer>.
    */
  public Property_Evaluator (Offer offer)
    {
      this.props_ = offer.properties;
    }

  /**
    * Disable or enable the evaluation of dynamic properties.
    */
  public void allow_dynamic_properties (boolean toggle)
    {
      this.supports_dp_ = toggle;
    }

  /**
    * Returns true if the property at index <index> is dynamic. Returns a
    * 0 when the index is out of bounds.    
    */
  public boolean is_dynamic_property (int index)
    {
      boolean return_value = false;
      int num_properties = this.props_.length;
      
      if (index >= 0 && index < num_properties)
	return_value = Property_Evaluator.is_dynamic_property (this.props_[index]);
      
      return return_value;
    }	 

  public static boolean is_dynamic_property (Property prop)
    {
      boolean return_value = false;

      Any value = prop.value;
      TypeCode type = value.type ();

      // Compare the type of the Any contents with the typecode
      // for a dynamic property structure. 
      if (type.equal (DynamicPropHelper.type ()))
	return_value = true;

      return return_value;
    }
  
  /**
    * Returns value of the property whose index is <index>. If the
    * property at that index is dynamic and the trader supports dynamic 
    * properties, then the property_value method will obtain the value
    * of the dynamic property using the evalDP method on the
    * CosTradingDynamic::DynamicPropEval interface, passing on a
    * CosTradingDynamic::DPEvalFailure exception on failure. If the
    * property index is undefined, the method returns a null pointer.
    */
  public Any property_value (int index, ORB orb)
    throws DPEvalFailure
    {
      Any return_value = null;
      int num_properties = this.props_.length;
      
      if (index >= 0 && index < num_properties)
	{
	  return_value =
	    Property_Evaluator.property_value (this.props_[index], orb);
	}

      return return_value;
    }

  public static Any property_value (Property prop, ORB orb)
    throws DPEvalFailure
    {
      Any prop_value = null;
      
      if (! Property_Evaluator.is_dynamic_property (prop))
	prop_value = prop.value;
      else
	{
	  DynamicProp dp_struct =
	    DynamicPropHelper.extract (prop.value);
	  String prop_name = prop.name;
	  
	  // This is because of the bug in TAO:
	  // Now we extract the object reference from the dynamic
	  // property structure.
	  org.omg.CORBA.Object obj =
	    orb.string_to_object (dp_struct.eval_if);
	  DynamicPropEval dp_eval = DynamicPropEvalHelper.narrow (obj);

	  if (dp_eval != null)
	    {
	      prop_value = dp_eval.evalDP (prop_name,
					   dp_struct.returned_type,
					   dp_struct.extra_info);
	    }
	  else
	    {
	      throw new DPEvalFailure (prop_name,
				       dp_struct.returned_type,
				       dp_struct.extra_info);	      
	    }
	}

      return prop_value;
    }
  
  /**
    * Returns the type of the property whose index is <index>. If the
    * property is dynamic and the trader supports dynamic properties,
    * then the method returns the <returned_type> field of the
    * CosTradingDynamic::DynamicProp struct associated with the
    * property name. If the index is out of bounds, the method returns 
    * a null pointer (that is, 0).
    */
  
  public TypeCode property_type (int index)
    {
      TypeCode prop_type = null;

      if (! this.is_dynamic_property (index))
	prop_type = this.props_[index].value.type ();
      else
	{
	  Any value = this.props_[index].value;
	  DynamicProp dp_struct = DynamicPropHelper.extract (value);

	  prop_type = dp_struct.returned_type;
	}

      return prop_type;
    }

  public String property_to_string (int index, ORB orb)
    throws DPEvalFailure
    {
      String return_value = null;
      Any any_value = this.property_value (index, orb);
      TypeCode type = any_value.type ();
      int kind = type.kind ().value ();
      
      if (kind == TCKind._tk_null)
	return_value = "Null";

      else if (kind == TCKind._tk_void)
	return_value = "Void";
      
      else if (kind == TCKind._tk_short)
	return_value = String.valueOf (any_value.extract_short ());

      else if (kind == TCKind._tk_long)
	return_value = String.valueOf (any_value.extract_long ());

      else if (kind == TCKind._tk_ushort)
	return_value = String.valueOf (any_value.extract_ushort ());

      else if (kind == TCKind._tk_ulong)
	return_value = String.valueOf (any_value.extract_ulong ());

      else if (kind == TCKind._tk_float)
	return_value = String.valueOf (any_value.extract_float ());

      else if (kind == TCKind._tk_double)
	return_value = String.valueOf (any_value.extract_double ());

      else if (kind == TCKind._tk_boolean)
	return_value = String.valueOf (any_value.extract_boolean ());

      else if (kind  == TCKind._tk_char)
	return_value = String.valueOf (any_value.extract_char ());

      else if (kind == TCKind._tk_string)
	return_value = any_value.extract_string ();

      else if (type.equal (StringSeqHelper.type ()))
	{
	  System.out.println ("Printing string sequence.");
	  String[] str_seq = StringSeqHelper.extract (any_value);	  

	  for (int length = str_seq.length, i = 0; i < length; i++)
	    return_value = str_seq[i] + " ";	 
	}

      else if (type.equal (ULongSeqHelper.type ()))
	{
	  System.out.println ("Printing ulong sequence.");
	  int[] ulong_seq = ULongSeqHelper.extract (any_value);	  

	  for (int length = ulong_seq.length, i = 0; i < length; i++)
	    return_value = ulong_seq[i] + " ";
	}

      else
	System.out.println ("TCKind" + kind);

      return return_value;
    }
}
