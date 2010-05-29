// $Id$
#ifndef DAnCE_Utility_TPP
#define DAnCE_Utility_TPP

#include "DAnCE/DAnCE_Utility.h"

namespace DAnCE
{
  namespace Utility
  {
    template<class T>
    bool get_property_value (const char *name, const PROPERTY_MAP &properties, T &val)
    {
      DANCE_TRACE ("DAnCE::Utility::get_property_value<T>");
      CORBA::Any any;

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                       ACE_TEXT("Finding property value for name '%C'\n"),
                       name));

      if (properties.find (name, any) == 0)
        {
          if (any >>= val)
            {
              return true;
            }
          else
            {
              DANCE_ERROR (1, (LM_WARNING, DLINFO
                               ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                               ACE_TEXT("Failed to extract property value for %C\n"), name));
              return false;
            }
        }

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                       ACE_TEXT("Property value for name '%C' has no value\n"), name));

      return false;
    }

    template<class T>
    bool get_property_value (const char *name, const ::Deployment::Properties &properties, T &val)
    {
      DANCE_TRACE ("DAnCE::Utility::get_property_value<T>");

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                       ACE_TEXT("Finding property value for name '%C'\n"),
                       name));

      for (CORBA::ULong i = 0; i < properties.length (); ++i)
        {
          if (ACE_OS::strcmp (properties[i].name.in (), name) == 0)
            {
              DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                               ACE_TEXT("Found property '%C'\n"), name));
              if (properties[i].value >>= val)
                return true;
              else
                {
                  DANCE_ERROR (1, (LM_WARNING, DLINFO ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                                   ACE_TEXT("Failed to extract property value for %C\n"), name));
                  return false;
                }
            }
        }


      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                       ACE_TEXT("Property value for name '%C' has no value\n"), name));

      return false;
    }

    /// Tests flag, if false, sets it to true and replaces the name and
    /// reason flags of the exception.
    template <typename EXCEPTION>
    void test_and_set_exception (bool &flag, EXCEPTION &exception,
                                 const char *name, const char *reason)
    {
      if (!flag)
        {
          flag = true;
          exception.name = name;
          exception.reason = reason;
        }
    }        
  }
}

#endif
