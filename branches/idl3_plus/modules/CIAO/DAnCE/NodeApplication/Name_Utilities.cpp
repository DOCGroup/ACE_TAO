// $Id$

#include "Name_Utilities.h"

#include "ace/Auto_Ptr.h"
#include "ace/SString.h"
#include "Logger/Log_Macros.h"

namespace DAnCE
{
  bool
  Name_Utilities::write_ior (const ACE_TCHAR *file,
                             const char *ior)
  {
    FILE* ior_output_file_ = ACE_OS::fopen (file, "w");
    if (ior_output_file_)
      {
        ACE_OS::fprintf (ior_output_file_,
                         "%s",
                         ior);
        ACE_OS::fclose (ior_output_file_);
        return true;
      }
    return false;
  }
  
  bool
  Name_Utilities::bind_object (const char *name,
                               CORBA::Object_ptr obj,
                               CosNaming::NamingContext_ptr ctx)
  {
    DANCE_TRACE ("Name_Utilities::bind_object");

    if (CORBA::is_nil (ctx))
      {
        DANCE_ERROR ((LM_WARNING, DLINFO "Name_Utilities::bind_object - "
                      "Provided naming context is nil, component %s will not be registered.",
                      name));
        return false;
      }
    
    try
      {
        CosNaming::Name nm;
        
        Name_Utilities::build_name (name, nm);
        
        if (nm.length () == 0)
          {
            DANCE_ERROR ((LM_WARNING, DLINFO "Name_Utilities::bind_object - "
                          "build_name resulted in an invalid name for string %C\n",
                          name));
            return false;
          }
        
        Name_Utilities::bind_context (nm, ctx);
        
        try
          {
            ctx->bind (nm, obj);
          }
        catch (const CosNaming::NamingContext::AlreadyBound &)
          {
            DANCE_ERROR ((LM_WARNING, DLINFO "Name_Utilities::bind_object - "
                          "Name %C already bound, rebinding....\n",
                          name));
            ctx->rebind (nm, obj);
          }
      }
    catch (const CORBA::Exception &ex)
      {
        DANCE_ERROR ((LM_ERROR, DLINFO "Name_Utilities::bind_object - "
                      "Caught CORBA exception while attempting to bind name %C: %C\n",
                      name, ex._info ().c_str ()));
        return false;
      }
    catch (...)
      {
        DANCE_ERROR ((LM_ERROR, DLINFO "Name_Utilities::bind_object - "
                      "Caught unknown C++ exception while attemptint to bind name %C\n",
                      name));
        return false;
      }
    
    return true;
  }
  
  void
  Name_Utilities::bind_context (CosNaming::Name &nm,
                                CosNaming::NamingContext_ptr ctx)
  {
    DANCE_TRACE ("Name_Utilities::bind_context");
    
    if (CORBA::is_nil (ctx))
      {
        DANCE_ERROR ((LM_WARNING, DLINFO "Name_Utilities::bind_context - "
                      "Provided naming context is nil, the naming context will not be bound."));
      }

    CosNaming::Name newname (nm.length ());
    
    for (CORBA::ULong i = 0;
         i < (nm.length () - 1); ++i)
      {
        newname.length (i + 1);
        newname[i] = nm[i];
        
        try
          {
            ctx->bind_new_context (newname);
            DANCE_DEBUG ((LM_TRACE, DLINFO "Name_Utilities::bind_context - "
                          "Bound new context %C\n", newname[i].id.in ()));
          }
        catch (CosNaming::NamingContext::AlreadyBound &) 
          {
            DANCE_DEBUG ((LM_TRACE, DLINFO "Name_Utilities::bind_context - "
                          "Context %C already bound.\n", newname[i].id.in ()));
          }
      }
  }
  
  bool
  Name_Utilities::unbind_object (const char *name, 
                                 CosNaming::NamingContext_ptr ctx)
  {
    DANCE_TRACE ("Name_Utilities::unbind_object");

    if (CORBA::is_nil (ctx))
      {
        DANCE_ERROR ((LM_WARNING, DLINFO "Name_Utilities::unbind_object - "
                      "Provided naming context is nil, instance %s will not be unbound\n",
                      name));
      }
    
    CosNaming::Name nm;
    Name_Utilities::build_name (name, nm);
    
    try
      {
        ctx->unbind (nm);
      }
    catch (CORBA::Exception &e)
      {
        DANCE_ERROR ((LM_ERROR, DLINFO "Name_Utilities::unbind_object - "
                      "Caught CORBA exception whilst unbinding name %C: %C\n",
                      name, e._info ().c_str ()));
        return false;
      }
    return true;
  }
  
  void
  Name_Utilities::build_name (const char *name,
                              CosNaming::Name &nm)
  {
    DANCE_TRACE ("Name_Utilities::build_name");
    
    /*ACE_Auto_Basic_Array_Ptr<ACE_TCHAR>*/  ACE_TCHAR *safe_array (new char[ACE_OS::strlen (name) + 1]);
    
    ACE_Tokenizer parser (ACE_OS::strcpy (safe_array/*.get ()*/, name));
    parser.delimiter ('/');
    
    ACE_TCHAR *next (0);
    
    while ((next = parser.next ()) != 0)
      {
        CORBA::ULong i = nm.length ();
        nm.length (i + 1);
        
        DANCE_DEBUG ((LM_TRACE, DLINFO "Name_Utilities::build_name - "
                      "Found name component %C\n",
                      next));
        
        nm[i].id = CORBA::string_dup (next);
      }
  }
  
}
