
#include "BGML/IDL_Util.h"
#include "BGML/TypeEmitter.h"
#include "Uml.h"


std::string
IDL_Util::component_name (PICML::OperationRef& op_ref)
{
	PICML::OperationBase op_base = PICML::OperationBase::Cast (op_ref.ref ());
	PICML::TwowayOperation op = PICML::TwowayOperation::Cast (op_base);

	PICML::ComponentOperation op_end = op_ref.dstComponentOperation();

	PICML::Component parent;

	if (op_end == Udm::null)
	{
		// Obtain the object parent
		PICML::HasOperations has_op = op.HasOperations_parent();
		PICML::Object object = PICML::Object::Cast (has_op);
		std::vector<std::string> return_set;
		std::set<PICML::RequiredRequestPort> receptacle_set = object.referedbyRequiredRequestPort();
		for (std::set<PICML::RequiredRequestPort>::iterator iter = receptacle_set.begin ();
		     iter != receptacle_set.end ();
			 iter ++)
		{
			// Return the first component name; all names will be the same
			parent = iter->Component_parent();
			break;
		}
	}
	else
	{
		// Return the Component Associated with this
		PICML::CompRef component_ref = op_end.dstComponentOperation_end();
		parent = component_ref.ref ();
	}

	std::string name;
	parent.GetStrValue ("name", name);
	return name;
}

std::string
IDL_Util::dependant_idls (PICML::TwowayOperation& op)
{
	// Obtain the object parent
	PICML::HasOperations has_op = op.HasOperations_parent();
	PICML::Object object = PICML::Object::Cast (has_op);
	std::vector<std::string> return_set;
	std::set<PICML::RequiredRequestPort> receptacle_set = object.referedbyRequiredRequestPort();

	for (std::set<PICML::RequiredRequestPort>::iterator iter = receptacle_set.begin ();
	iter != receptacle_set.end ();
	iter ++)
	{
		// Return the first component name; all names will be the same
		PICML::Component component = iter->Component_parent ();

		// Get the File parent
		Udm::Object some_parent = component.parent ();
		while (Udm::IsDerivedFrom (some_parent.type(), PICML::Package::meta))
		{
			PICML::Package temp = PICML::Package::Cast (some_parent);
			some_parent = temp.parent ();
		}

		PICML::File file_parent = PICML::File::Cast (some_parent);

		// Get the dependant IDL files included by this file
		std::set<PICML::FileRef> file_refs = file_parent.FileRef_children ();
		std::string return_set;

		if (file_refs.size ())
		{
			// Add each of the dependant file names
			for (std::set<PICML::FileRef>::iterator iter = file_refs.begin ();
			     iter != file_refs.end ();
				 iter ++)
				 {
					 std::string name;
					 iter->GetStrValue ("name", name);
					 return_set.append (name);
					 return_set.append ("_stub ");
				 }
		}
		else
		{
			// Add this file to the dependant list
			std::string name;
			file_parent.GetStrValue ("name", name);
			return_set.append (name);
			return_set.append ("_stub ");

		}

		// Return at the first iteration
		return return_set;
	}

	// Return null string
	return 0;

}

/*
* Convention for genrating the Operation signatures for the types
* 0: Return type
* 1: In argument
* 2: Inout argument
* 3: Out argument
*/

void IDL_Util::return_type_signature (PICML::MemberType& mem_type,
									  std::string &operation_name,
									  std::string &package,
									  int type)
{
	std::string kindName = mem_type.type().name();
	std::string scope_name;
	mem_type.GetStrValue ("name", scope_name);

	/// Append the package name
	if (package.size ())
		scope_name = package + "::" + scope_name;

	switch (type)
	{
	case 0:
		{
			std::string& return_type =
				ReturnTypeEmitter::generate_type_defn (kindName, scope_name);
			operation_name.append (return_type);
			break;
		}
	case 1:
		{
			std::string& in_param_name =
				InTypeEmitter::generate_type_defn (kindName, scope_name);
			operation_name.append (in_param_name);
			break;
		}
	case 2:
		{
			std::string& inout_type =
				InoutTypeEmitter::generate_type_defn (kindName, scope_name);
			operation_name.append (inout_type);
			break;
		}
	case 3:
		{
			std::string& out_type =
				OutTypeEmitter::generate_type_defn (kindName, scope_name);
			operation_name.append (out_type);
			break;
		}
	}
}

std::string IDL_Util::operation_name (PICML::TwowayOperation& op)
{
	std::string op_name;
	op.GetStrValue ("name", op_name);
	return op_name;

}

std::string
IDL_Util::scope_name (PICML::TwowayOperation &two_way)
{
	PICML::NamedType type_parent = two_way.HasOperations_parent ();
	PICML::Package package_parent = type_parent.Package_parent ();
	std::string parent_name;

	if (package_parent != Udm::null)
	{
		// Appending the scope name
		package_parent.GetStrValue ("name", parent_name);
	}

	return parent_name;
}

std::vector<std::string>
IDL_Util::argument_list (PICML::TwowayOperation& op)
{
	std::vector<std::string> arg_list;
	/////////////////////////////////////////////////////////////////////
	// Operation consists of: "return_type" operation_name (arg_list)"
	// The arg list inturn can be any of the following: in, out, in out
	// Ignore the Return and Exception types for now. Need not consider them
	// part of the operation signature we are looking for
	//////////////////////////////////////////////////////////////////////


	/// Operation parameters
	std::set<PICML::InoutParameter> inout_param_set = op.InoutParameter_children();
	std::set<PICML::OutParameter> out_param_set = op.OutParameter_children ();
	std::set<PICML::InParameter> in_param_set = op.InParameter_children ();

	// Identify the scope name
	std::string scope_name = IDL_Util::scope_name (op);

	if (in_param_set.size () != 0)
	{
		std::string temp;
		bool first_iter = true;
		for (std::set<PICML::InParameter>::iterator in_iter = in_param_set.begin ();
		in_iter != in_param_set.end ();
		in_iter ++)
		{
			std::string temp;
			PICML::MemberType mem_type = in_iter->ref ();
			IDL_Util::return_type_signature (mem_type, temp, scope_name, 1);
			arg_list.push_back (temp);
			first_iter = false;
		}
	}

	if (out_param_set.size () != 0)
	{
		std::string temp;
		bool first_iter = true;
		for (std::set<PICML::OutParameter>::iterator out_iter = out_param_set.begin ();
		out_iter != out_param_set.end ();
		out_iter ++)
		{
			PICML::MemberType mem_type = out_iter->ref ();
			IDL_Util::return_type_signature (mem_type, temp, scope_name, 2);
			arg_list.push_back (temp);
			first_iter = false;
		}
	}

	if (inout_param_set.size () != 0)
	{
		std::string temp;
		bool first_iter = true;
		for (std::set<PICML::InoutParameter>::iterator in_out_iter = inout_param_set.begin ();
		in_out_iter != inout_param_set.end ();
		in_out_iter ++)
		{
			PICML::MemberType mem_type = in_out_iter->ref ();
			IDL_Util::return_type_signature (mem_type, temp, scope_name, 3);
			arg_list.push_back (temp);
			first_iter = false;
		}
	}

	return arg_list;
}
