// $Id$

#include "Name_Utilities.h"

#include "ace/Auto_Ptr.h"
#include "ace/SString.h"
#include "ace/Tokenizer_T.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace Name_Utilities {
    bool
    write_ior (const ACE_TCHAR *file, const char *ior)
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
    bind_object (const char *name,
                 CORBA::Object_ptr obj,
                 CosNaming::NamingContext_ptr ctx)
    {
      CIAO_TRACE ("Name_Utilities::bind_object");

      if (CORBA::is_nil (ctx))
        {
          CIAO_ERROR (1, (LM_WARNING, CLINFO "Name_Utilities::bind_object - "
                          "Provided naming context is nil, component %C will not be registered.",
                          name));
          return false;
        }

      try
        {
          CosNaming::Name nm;

          Name_Utilities::build_name (name, nm);

          if (nm.length () == 0)
            {
              CIAO_ERROR (1, (LM_WARNING, CLINFO
                              "Name_Utilities::bind_object - "
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
              CIAO_ERROR (1, (LM_WARNING, CLINFO "Name_Utilities::bind_object - "
                              "Name %C already bound, rebinding....\n",
                              name));
              ctx->rebind (nm, obj);
            }
        }
      catch (const CORBA::Exception &ex)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "Name_Utilities::bind_object - "
                          "Caught CORBA exception while attempting to bind name %C: %C\n",
                          name, ex._info ().c_str ()));
          return false;
        }
      catch (...)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "Name_Utilities::bind_object - "
                          "Caught unknown C++ exception while attemptint to bind name %C\n",
                          name));
          return false;
        }

      return true;
    }

    void
    bind_context (CosNaming::Name &nm,
                  CosNaming::NamingContext_ptr ctx)
    {
      CIAO_TRACE ("Name_Utilities::bind_context");

      if (CORBA::is_nil (ctx))
        {
          CIAO_ERROR (1, (LM_WARNING, CLINFO
                          "Name_Utilities::bind_context - "
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
              CIAO_DEBUG (9, (LM_TRACE, CLINFO
                              "Name_Utilities::bind_context - "
                              "Bound new context %C\n",
                              newname[i].id.in ()));
            }
          catch (CosNaming::NamingContext::AlreadyBound &)
            {
              CIAO_DEBUG (9, (LM_TRACE, CLINFO
                              "Name_Utilities::bind_context - "
                              "Context %C already bound.\n",
                              newname[i].id.in ()));
            }
        }
    }

    bool
    unbind_object (const char *name,
                   CosNaming::NamingContext_ptr ctx)
    {
      CIAO_TRACE ("Name_Utilities::unbind_object");

      if (CORBA::is_nil (ctx))
        {
          CIAO_ERROR (1, (LM_WARNING, CLINFO
                          "Name_Utilities::unbind_object - "
                          "Provided naming context is nil, instance %C will not be unbound\n",
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
          CIAO_ERROR (1, (LM_ERROR, CLINFO
                          "Name_Utilities::unbind_object - "
                          "Caught CORBA exception whilst unbinding name %C: %C\n",
                          name, e._info ().c_str ()));
          return false;
        }
      return true;
    }

    void
    build_name (const char *name, CosNaming::Name &nm)
    {
      CIAO_TRACE ("Name_Utilities::build_name");

      char* buf = ACE_OS::strdup (name);

      ACE_Tokenizer_T<char> parser (buf);
      parser.delimiter ('/');

      for (char *next = parser.next (); next; next = parser.next ())
        {
          CORBA::ULong const i = nm.length ();
          nm.length (i + 1);

          CIAO_DEBUG (9, (LM_TRACE, CLINFO
                          "Name_Utilities::build_name - "
                          "Found name component %C\n",
                          next));

          nm[i].id = CORBA::string_dup (next);
        }
      ACE_OS::free (buf);
    }
  }
}
