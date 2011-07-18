// $Id$

#include "Server_init.h"

#include <ccm/CCM_ComponentC.h>
#include <ccm/CCM_StandardConfiguratorC.h>
#include "Valuetype_Factories/Cookies.h"
#include "CIAO_common.h"
#include "Client_init.h"

namespace CIAO
{
  int
  Server_init (CORBA::ORB_ptr o)
  {
    Client_init (o);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 CIAO::Cookie_Impl_init,
                                 Components::Cookie);
    return 0;
  }

  namespace Utility
  {
    int write_IOR (const char *pathname, const char *ior)
    {
      FILE* ior_output_file_ = ACE_OS::fopen (pathname, "w");

      if (ior_output_file_)
        {
          ACE_OS::fprintf (ior_output_file_,
                          "%s",
                          ior);
          ACE_OS::fclose (ior_output_file_);
          return 0;
        }

      return -1;
    }

    // --------------------------------------------------------------
    //             Implementation of NameUtility class
    // --------------------------------------------------------------
    bool NameUtility::bind_name (const char *namestr,
                                 CORBA::Object_ptr obj,
                                 CosNaming::NamingContextExt_var &root)
    {
      if (namestr == 0)
        {
          CIAO_ERROR ((LM_WARNING, CLINFO
                      "NameUtility::bind_name - "
                      "called with null name, aborting registration.\n"));
          return false;
        }

      CosNaming::Name name;
      NameUtility::create_name (namestr, name);

      if (name.length () > 1)
        {
          // This name has contexts, create them.
          name.length (name.length () - 1);
          Utility::NameUtility::create_context_path (root.in (), name);
          name.length (name.length () + 1);
        }

      return NameUtility::bind_object_path (root.in (), name, obj);
    }

    void
    NameUtility::create_name (const char *namestr, CosNaming::Name &name)
    {
      ACE_Auto_Basic_Array_Ptr<char> namebfr (ACE::strnew (namestr));
      ACE_Tokenizer tok (namebfr.get ());

      tok.delimiter ('/');

      for (char *p = tok.next (); p; p=tok.next ())
        {
          CORBA::ULong pos = name.length ();
          name.length (pos + 1);
          name[pos].id = CORBA::string_dup (p);
        }
    }

    void NameUtility::create_context_path (const CosNaming::NamingContextExt_ptr nc,
                                           const CosNaming::Name& name)
    {
      bool isNotFound = false;
      CORBA::ULong lengthMissing = 0;
      CORBA::ULong OriginalLength = name.length ();
      CosNaming::Name tmpName;
      CosNaming::NamingContext_var tmpCtxVar;

      try
        {
          tmpCtxVar = nc->bind_new_context (name);
          CIAO_DEBUG ((LM_TRACE, CLINFO
                      "NameUtility::CreateContextPath - Bound Context.\n\n"));
        }
      catch (const CosNaming::NamingContext::AlreadyBound&)
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO
                      "NameUtility::CreateContextPath - Context Already Bound.\n\n"));
        }
      catch (const CosNaming::NamingContext::NotFound& nf)
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO
                      "NameUtility::CreateContextPath - Context not found.\n\n"));
          isNotFound = true;
          lengthMissing = nf.rest_of_name.length ();
        }

      if (lengthMissing == name.length ())
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                      "NameUtility::CreateContextPath - Null name length.\n\n"));
        }

      if (isNotFound)
        {
          for (CORBA::ULong l = OriginalLength - lengthMissing;
                l < OriginalLength;
                ++l)
              {
                tmpName.length (l + 1);

                for (CORBA::ULong i = 0; i <= l; ++i)
                  {
                    tmpName[i] = name[i];

                    CORBA::String_var newSCName = nc->to_string (tmpName);
                    CIAO_DEBUG ((LM_TRACE, CLINFO
                                "NameUtility::CreateContextPath - What's left of the name:%C\n",
                                newSCName.in ()));
                  }

                tmpCtxVar = nc->bind_new_context (tmpName);
                CIAO_DEBUG ((LM_TRACE, CLINFO
                            "NameUtility::CreateContextPath - Bound New Context.\n"));
              }
          }
    }

    //---------------------------------------------------------------------------------------------
    bool NameUtility::bind_object_path (const CosNaming::NamingContextExt_ptr nc,
                                        const CosNaming::Name& name,
                                        const CORBA::Object_ptr obj)
    {
      CosNaming::Name tmpName;
      CORBA::String_var newSCName = nc->to_string (name);
      CIAO_DEBUG ((LM_TRACE, CLINFO
      "NameUtility::BindObjectPath - The name is: %C\n", newSCName.in ()));

      try
        {
          nc->rebind(name, obj);
        }

      catch (const CosNaming::NamingContext::NotFound&)
        {
          CIAO_DEBUG ((LM_TRACE, CLINFO
                      "NameUtility::BindObjectPath - Name not found, doing new bind.\n"));
          nc->bind (name, obj);
        }
      catch (...)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                      "NameUtility::BindObjectPath - Caught exception while binding name in nameing service.\n"));
          return false;
        }
      return true;
    }

    //---------------------------------------------------------------------------------------------
    CosNaming::BindingList *
    NameUtility::list_bindings (const CosNaming::NamingContext_ptr nc,
                                const CosNaming::Name& name,
                                CORBA::ULong max_list_size)
    {
      CosNaming::BindingList_var basicListV;
      CosNaming::BindingIterator_var bIterV;

      CORBA::Object_var objV;
      CosNaming::NamingContext_var tmpContextV;

      if (name.length () == 0)
        {
          tmpContextV = CosNaming::NamingContext::_duplicate (nc);
        }
      else
        {
          objV = nc->resolve (name);
          tmpContextV = CosNaming::NamingContext::_narrow (objV.in ());
        }

      if (CORBA::is_nil (tmpContextV.in ()))
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                      "NameUtility::listBindings: Nil context.\n"));
          return 0;
        }

      tmpContextV->list(max_list_size, basicListV.out (), bIterV.out ());

      CORBA::Long max_remaining = max_list_size - basicListV->length ();
      CORBA::Boolean moreBindings = !CORBA::is_nil (bIterV.in ());

      if (moreBindings)
        {
          while (moreBindings && (max_remaining > 0) )
            {
              CosNaming::BindingList_var tmpListV;

              moreBindings = bIterV->next_n (max_remaining, tmpListV.out ());

              // Append 'tmpListV' to 'basicListV'
              CORBA::ULong basicListLen = basicListV->length ();
              basicListV->length (basicListLen+tmpListV->length ());

              for (CORBA::ULong i = 0; i < tmpListV->length (); ++i)
                {
                  (*basicListV)[i+basicListLen] = (*tmpListV)[i];
                }

              // Re-calculate 'max_remaining'
              max_remaining = max_list_size - basicListV->length();
            }

          bIterV->destroy ();
        }

      return basicListV._retn ();
    }

    //---------------------------------------------------------------------------------------------
    void
    NameUtility::recursive_unbind (const CosNaming::NamingContext_ptr nc,
                                  const CosNaming::Name& name)
    {
      CORBA::Object_var objV;
      CosNaming::NamingContext_var tmpContextV;

      objV = nc->resolve(name);
      tmpContextV = CosNaming::NamingContext::_narrow (objV.in ());

      if (CORBA::is_nil (tmpContextV.in ()))
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                      "NameUtility::recursiveUnbind - Nil context reference.\n"));
          return;
        }

      CosNaming::BindingList_var blV;
      CosNaming::Name tmpName;
      tmpName.length (0);

      blV = NameUtility::list_bindings (tmpContextV.in (),
                                        tmpName,
                                        10000);  // 'max_list_size'

      for (CORBA::ULong i = 0; i < blV->length (); ++i)
        {
          tmpName = (*blV)[i].binding_name;

          if ((*blV)[i].binding_type == CosNaming::nobject)
            {
              tmpContextV->unbind(tmpName);
            }
          else if ((*blV)[i].binding_type==CosNaming::ncontext)
            {
              NameUtility::recursive_unbind (tmpContextV.in (), tmpName);
            }
        }

      nc->unbind (name);
      tmpContextV->destroy ();
    }
  } /* namespace Utility */
} /* namespace CIAO */
