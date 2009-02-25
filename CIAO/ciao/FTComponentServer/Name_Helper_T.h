#ifndef _NAME_HELPER_T_H_
#define _NAME_HELPER_T_H_

#include <stdexcept>
#include <orbsvcs/CosNamingC.h>
#include <ace/Env_Value_T.h>

class Name_Helper_Exception : public std::domain_error
{
 public:
  Name_Helper_Exception (const std::string & reason = "")
    : std::domain_error (reason) {}
};

template <typename T>
class Name_Helper_T 
{
 public:
  Name_Helper_T (CORBA::ORB_ptr orb, bool use_env_var = false)
    {
      CORBA::Object_var ns_obj;

      if (use_env_var)
	{
	  std::string ns_ior = ACE_Env_Value<std::string> ("NameServiceIOR", 
							   std::string ());

	  if (ns_ior.empty ())
	    {
	      throw Name_Helper_Exception ("Name_Helper_T - no NameServiceIOR env variable");
	    }

	  ns_obj = orb->string_to_object (ns_ior.c_str ());
	}
      else
	{
	  ns_obj = orb->resolve_initial_references ("NameService");
	}

      if (CORBA::is_nil (ns_obj.in ()))
	{
	  throw Name_Helper_Exception ("Name_Helper_T - could not resolve NameService");
	}

      ns_ = CosNaming::NamingContextExt::_narrow (ns_obj.in ());
	  
      if (CORBA::is_nil (ns_.in ()))
	{
	  throw Name_Helper_Exception ("Name_Helper_T - could not narrow to NameContextExt");
	}
    }

  void bind (std::string path, typename T::_ptr_type obj)
  {
    size_t last_slash = path.find_last_of ('/');

    std::string obj_name;
    std::string obj_path;

    if (last_slash != std::string::npos)
      {
	obj_path = path.substr (0, last_slash);
	obj_name = path.substr (last_slash + 1);
      }
    else
      {
	obj_name = path;
      }

    if (obj_name.empty ())
      throw Name_Helper_Exception ("Name_Helper_T::bind - no object name.");

    ACE_DEBUG ((LM_INFO, "path=%s, name=%s\n", obj_path.c_str (), obj_name.c_str ()));

    CosNaming::Name_var name;
    CosNaming::NamingContext_var nc;

    if (!obj_path.empty ())
      {
	name = ns_->to_name (obj_path.c_str ());

	nc = CosNaming::NamingContext::_narrow (ns_.in ());

	CosNaming::Name entry;
	entry.length (1);
      
	for (size_t i = 0;
	     i < name->length ();
	     ++i)
	  {
	    entry[0] = CosNaming::NameComponent (name[i]);
	    try
	      {
		nc = nc->bind_new_context (entry);
	      }
	    catch (CosNaming::NamingContext::AlreadyBound & ex)
	      {
		// if the entry is already there just go on
		nc = CosNaming::NamingContext::_narrow (nc->resolve (entry));
	      }
	  }
      }
    
    name = ns_->to_name (obj_name.c_str ());

    try 
      {
	nc->bind (name, obj);
      }
    catch (CosNaming::NamingContext::AlreadyBound & e)
      {
	nc->rebind (name, obj);
      }
  }

  void unbind (const std::string & path, size_t depth = 0)
  {
    std::string current_path = path;
    size_t current_depth = 0;
    CosNaming::Name_var name;

    if (path.empty ())
      throw Name_Helper_Exception ("Name_Helper_T::unbind - empty path");

    do
      {
	name = ns_->to_name (current_path.c_str ());

	CosNaming::NamingContext_var nc =
	  CosNaming::NamingContext::_narrow (
            ns_->resolve (name.in ()));
	
	bool no_more_entries = true;

	if (!CORBA::is_nil (nc.in ()))
	  {
	    CosNaming::BindingList_var bl;
	    CosNaming::BindingIterator_var bit;
	    nc->list (2, bl.out (), bit.out ());

	    if (bl->length () > 0)
	      no_more_entries = false;
	  }

	if (no_more_entries)
	  ns_->unbind (name.in ());

	if (current_depth < depth)
	  {
	    size_t pos = current_path.find_last_of ('/');

	    if (pos == std::string::npos)
	      break;

	    current_path = current_path.substr (0, pos);
	    ++current_depth;
	  }
      }
    while (depth >= current_depth);
  }

  typename T::_ptr_type resolve (const std::string & path)
    {
      CORBA::Object_var obj = ns_->resolve_str (path.c_str ());

      typename T::_var_type ret = T::_narrow (obj.in ());

      if (CORBA::is_nil (ret.in ()))
	  throw Name_Helper_Exception ("Name_Helper_T::resolve - could not narrow resolved obj");

      return ret._retn ();
    }

  std::string escape_dots (const std::string &path)
    {
      std::string esc_path (path);

      // replace all dots in the path with excaped dots for 
      // to_name conversion.
      for (size_t pos = esc_path.find ('.', 0); 
	   pos < esc_path.length ();
	   pos = esc_path.find ('.', pos))
	{
	  esc_path.replace (pos, 1, "\\.");
	  pos += 2;
	}

      return esc_path;
    }

 private:
  CORBA::ORB_var orb_;
  CosNaming::NamingContextExt_var ns_;
};

#endif /* _NAME_HELPER_T_H_ */
