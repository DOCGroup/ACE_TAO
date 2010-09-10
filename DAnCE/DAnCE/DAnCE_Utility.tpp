// $Id$
#ifndef DAnCE_Utility_TPP
#define DAnCE_Utility_TPP

#include "DAnCE/DAnCE_Utility.h"

#include <sstream>

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

    template<class T>
    void update_property_value (const char *name, ::Deployment::Properties &properties, const T &val)
    {
      DANCE_TRACE ("DAnCE::Utility::update_property_value<T>");

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAnCE::Utility::update_property_value<T> - ")
                       ACE_TEXT("Finding property value for name '%C'\n"),
                       name));

      for (CORBA::ULong i = 0; i < properties.length (); ++i)
        {
          if (ACE_OS::strcmp (properties[i].name.in (), name) == 0)
            {
              DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAnCE::Utility::update_property_value<T> - ")
                               ACE_TEXT("Found property '%C'\n"), name));
              properties[i].value <<= val;
              return;
            }
        }


      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAnCE::Utility::update_property_value<T> - ")
                       ACE_TEXT("Property for name '%C' not found; adding property\n"), name));

      properties.length (properties.length () + 1);
      properties[properties.length () - 1].name = CORBA::string_dup (name);
      properties[properties.length () - 1].value <<= val;
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

    template <typename EXCEPTION>
    bool extract_and_throw_exception (const CORBA::Any &excep)
    {
      const EXCEPTION *ex_holder = 0;

      if ((excep >>= ex_holder))
        ex_holder->_raise ();

      return false;
    }

    template <typename EXCEPTION>
    bool stringify_exception (const CORBA::Any &excep,
                              std::string &result)
    {
      const EXCEPTION *ex_holder = 0;

      if ((excep >>= ex_holder))
        {
          std::ostringstream excep_stream;
          
          excep_stream << " Name: " << ex_holder->name.in () 
                       << " Reason: " << ex_holder->reason.in ();
          
          result += excep_stream.str ();
          return true;
        }
      
      return false;
    }

    template <typename EXCEPTION>
    CORBA::Any* create_any_from_exception (const EXCEPTION& ex)
    {
      CORBA::Any *tmp = 0;
      ACE_NEW_THROW_EX (tmp,
                        CORBA::Any,
                        CORBA::NO_MEMORY ());
      *tmp <<= ex;
      return tmp;
    }
  }
}

#endif
