// $Id$

#include "Server_init.h"

#include "CIAO_common.h"
#include "CCM_ComponentC.h"
#include "CCM_StandardConfiguratorC.h"
#include "Cookies.h"

namespace CIAO
{
  int
  Server_init (CORBA::ORB_ptr o)
  {
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 CIAO::Cookie_Impl_init,
                                 Components::Cookie);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 Components::PortDescription_init,
                                 Components::PortDescription);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 Components::FacetDescription_init,
                                 Components::FacetDescription);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 Components::ConnectionDescription_init,
                                 Components::ConnectionDescription);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 Components::ReceptacleDescription_init,
                                 Components::ReceptacleDescription);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 Components::ConsumerDescription_init,
                                 Components::ConsumerDescription);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 Components::EmitterDescription_init,
                                 Components::EmitterDescription);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 Components::SubscriberDescription_init,
                                 Components::SubscriberDescription);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 Components::PublisherDescription_init,
                                 Components::PublisherDescription);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 Components::ConfigValue_init,
                                 Components::ConfigValue);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 Components::ComponentPortDescription_init,
                                 Components::ComponentPortDescription);
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

    void NameUtility::CreateContextPath (const CosNaming::NamingContextExt_ptr nc,
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
          ACE_DEBUG ((LM_DEBUG, "Bound Context.\n\n"));
        }
      catch (const CosNaming::NamingContext::AlreadyBound&)
        {
          ACE_DEBUG ((LM_DEBUG, "Context Already Bound.\n\n"));
        }
      catch (const CosNaming::NamingContext::NotFound& nf)
        {
          ACE_DEBUG ((LM_DEBUG, "Context not found.\n\n"));
          isNotFound = true;
          lengthMissing = nf.rest_of_name.length ();
        }

      if (lengthMissing == name.length ())
        {
          ACE_ERROR ((LM_ERROR, "Null name length.\n\n"));
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
                    ACE_DEBUG ((LM_DEBUG,
                                "What's left of the name:%s\n",
                                newSCName.in ()));
                  }

                tmpCtxVar = nc->bind_new_context (tmpName);
                ACE_DEBUG ((LM_DEBUG, "Bound New Context.\n"));
              }
          }
    }

    //---------------------------------------------------------------------------------------------
    void NameUtility::BindObjectPath (const CosNaming::NamingContextExt_ptr nc,
                                      const CosNaming::Name& name,
                                      const CORBA::Object_ptr obj)
    {
      CosNaming::Name tmpName;
      CORBA::String_var newSCName = nc->to_string (name);
      ACE_DEBUG ((LM_DEBUG, "The name is: %s\n", newSCName.in ()));

      try
        {
          nc->rebind(name, obj);
        }

      catch (const CosNaming::NamingContext::NotFound&)
        {
          ACE_DEBUG ((LM_DEBUG, "Name not found, doing new bind.\n"));
          nc->bind (name, obj);
        }
    }

    //---------------------------------------------------------------------------------------------
    CosNaming::BindingList *
    NameUtility::listBindings (const CosNaming::NamingContext_ptr nc,
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
          ACE_ERROR ((LM_ERROR, "listBindings: Nil context.\n"));
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
    NameUtility::recursiveUnbind (const CosNaming::NamingContext_ptr nc,
                                  const CosNaming::Name& name)
    {
      CORBA::Object_var objV;
      CosNaming::NamingContext_var tmpContextV;

      objV = nc->resolve(name);
      tmpContextV = CosNaming::NamingContext::_narrow (objV.in ());
      
      if (CORBA::is_nil (tmpContextV.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "recursiveUnbind: Nil context reference.\n"));
          return;
        }

      CosNaming::BindingList_var blV;
      CosNaming::Name tmpName;
      tmpName.length (0);

      blV = NameUtility::listBindings(tmpContextV.in (),
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
              NameUtility::recursiveUnbind(tmpContextV.in (), tmpName);
            }
        }
        
      nc->unbind (name);
      tmpContextV->destroy ();
    }
  } /* namespace Utility */
} /* namespace CIAO */
