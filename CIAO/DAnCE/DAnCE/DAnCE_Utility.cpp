// $Id$

#ifndef DAnCE_Utility_CPP
#define DAnCE_Utility_CPP

#include "DAnCE_Utility.h"
#include "ace/OS_NS_stdio.h"
#include "DAnCE_PropertiesC.h"
#include "Deployment/Deployment_PlanErrorC.h"
#include "Deployment/Deployment_StartErrorC.h"
#include "Deployment/Deployment_StopErrorC.h"
#include "tao/SystemException.h"
#include "tao/AnyTypeCode/Any_Impl.h"
#include "tao/AnyTypeCode/ExceptionA.h"

namespace DAnCE
{
  namespace Utility
  {
    void build_property_map (PROPERTY_MAP &map,
                             const ::Deployment::Properties &prop)
    {
      DANCE_TRACE("DAnCE::Utility::build_config_values_map");

      for (CORBA::ULong i = 0; i < prop.length (); ++i)
        {
          int const retval = map.rebind (prop[i].name.in (), prop[i].value);

          if (retval == 1)
            {
              DANCE_ERROR (1, (LM_WARNING, DLINFO
                               ACE_TEXT ("build_property_map: Duplicate value ")
                               ACE_TEXT ("for %C encountered, ")
                               ACE_TEXT ("old value overwritten.\n"),
                               prop[i].name.in ()));
            }
          else if (retval == -1)
            {
              DANCE_ERROR (1, (LM_WARNING, DLINFO
                               ACE_TEXT ("build_property_map: ")
                               ACE_TEXT ("Error binding value for %C, ignoring.\n"),
                               prop[i].name.in ()));
            }
          else
            {
              DANCE_DEBUG (9, (LM_TRACE, DLINFO
                               ACE_TEXT ("build_property_map: ")
                               ACE_TEXT ("Bound value for config value %C\n"),
                               prop[i].name.in ()));
            }
        }
    }

    void build_property_sequence (::Deployment::Properties &prop,
                                  const PROPERTY_MAP &pmap)
    {
      CORBA::ULong pos = prop.length ();

      prop.length (pos + pmap.current_size ());

      for (PROPERTY_MAP::const_iterator i = pmap.begin ();
           i != pmap.end ();
           ++i)
        {
          prop[pos].name = (*i).ext_id_.c_str ();
          prop[pos++].value = (*i).int_id_;
        }
    }

    int write_IOR (const ACE_TCHAR *pathname, const char *ior)
    {
      FILE* ior_output_file_ = ACE_OS::fopen (pathname, ACE_TEXT("w"));

      DANCE_DEBUG (7, (LM_DEBUG, DLINFO
                       ACE_TEXT ("DAnCE::Utility::write_IOR - ")
                       ACE_TEXT ("Writing ior <%C> to file <%s>\n"),
                       ior, pathname));

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

    void append_properties (::Deployment::Properties &dest,
                            const ::Deployment::Properties &src)
    {
      CORBA::ULong pos = dest.length ();
      dest.length (pos + src.length ());

      for (CORBA::ULong i = 0; i < src.length (); ++i)
        {
          dest[pos].name = CORBA::string_dup (src[i].name.in ());
          dest[pos].value = src[i].value;
          ++pos;
        }
    }

    template<>
    bool get_property_value (const char *name,
                             const PROPERTY_MAP &properties,
                             bool &val)
    {
      DANCE_TRACE ("DAnCE::Utility::get_property_value<bool>");

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT("DAnCE::Utility::get_property_value<bool> - ")
                       ACE_TEXT("Finding property value for name '%C'\n"),
                       name));

      CORBA::Any any;
      if (properties.find (name, any) == 0)
        {
          if (any >>= CORBA::Any::to_boolean(val))
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
                       ACE_TEXT("DAnCE::Utility::get_property_value<bool> - ")
                       ACE_TEXT("Property value for name '%C' has no value\n"),
                       name));

      return false;
    }

    template<>
    bool get_property_value (const char *name,
                             const PROPERTY_MAP &properties,
                             const char *&val)
    {
      DANCE_TRACE ("DAnCE::Utility::get_property_value<const char *>");
      CORBA::Any any;

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT("DAnCE::Utility::get_property_value<bool> - ")
                       ACE_TEXT("Finding property value for name '%C'\n"),
                       name));

      if (properties.find (name, any) == 0)
        {
          if (any >>= CORBA::Any::to_string(val, 0))
            {
              return true;
            }
          else
            {
              DANCE_ERROR (1, (LM_WARNING, DLINFO
                               ACE_TEXT("DAnCE::Utility::get_property_value<const char *> - ")
                               ACE_TEXT("Failed to extract property value for %C\n"), name));
              return false;
            }
        }

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT("DAnCE::Utility::get_property_value<bool> - ")
                       ACE_TEXT("Property value for name '%C' has no value\n"), name));

      return false;
    }

    template<>
    bool get_property_value (const char *name,
                             const ::Deployment::Properties &properties,
                             const char * &val)
    {
      DANCE_TRACE ("DAnCE::Utility::get_property_value<const char *>");

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                       ACE_TEXT("Finding property value for name '%C'\n"),
                       name));

      for (CORBA::ULong i = 0; i < properties.length (); ++i)
        {
          if (ACE_OS::strcmp (properties[i].name.in (), name) == 0)
            {
              DANCE_DEBUG (9, (LM_TRACE, DLINFO
                               ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                               ACE_TEXT("Found property '%C'\n"), name));
              if (properties[i].value >>= CORBA::Any::to_string (val, 0))
                {
                  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                                   ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                                   ACE_TEXT("Value is %C\n"), val));
                  return true;
                }
              else
                {
                  DANCE_ERROR (1, (LM_WARNING, DLINFO
                                   ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                                   ACE_TEXT("Failed to extract property value for %C\n"),
                                   name));
                  return false;
                }
            }
        }


      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                       ACE_TEXT("DAnCE::Utility::get_property_value<T> - ")
                       ACE_TEXT("Unable to find property named %C\n"), name));

      return false;
    }

    template<char*>
    void update_property_value (const char *name,
                                ::Deployment::Properties &properties,
                                const char* &val)
    {
      DANCE_TRACE ("DAnCE::Utility::update_property_value<char*>");

      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAnCE::Utility::update_property_value<char*> - ")
                       ACE_TEXT("Finding property value for name '%C'\n"),
                       name));

      for (CORBA::ULong i = 0; i < properties.length (); ++i)
        {
          if (ACE_OS::strcmp (properties[i].name.in (), name) == 0)
            {
              DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAnCE::Utility::update_property_value<char*> - ")
                               ACE_TEXT("Found property '%C'\n"), name));
              properties[i].value <<= val;
              return;
            }
        }


      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DAnCE::Utility::update_property_value<char*> - ")
                       ACE_TEXT("Property for name '%C' not found; adding property\n"), name));

      properties.length (properties.length () + 1);
      properties[properties.length () - 1].name = CORBA::string_dup (name);
      properties[properties.length () - 1].value <<= val;
    }

    const char *
    get_instance_type (const ::Deployment::Properties& prop)
    {
      DANCE_TRACE ("DAnCE::Utility::get_instance_type");

      for (CORBA::ULong i = 0; i < prop.length (); ++i)
        {
          DANCE_DEBUG (9, (LM_TRACE, DLINFO
                           ACE_TEXT("DAnCE::Utility::get_instance_type - ")
                           ACE_TEXT("Checking property %C\n"),
                           prop[i].name.in ()));

          if (ACE_OS::strcmp (prop[i].name.in (),
                              DAnCE::HOME_FACTORY) == 0)
            {
              DANCE_DEBUG (9, (LM_TRACE, DLINFO
                               ACE_TEXT("DAnCE::Utility::get_instance_type - ")
                               ACE_TEXT("Found Home type\n")));
              return DAnCE::CCM_HOME;
            }
          if (ACE_OS::strcmp (prop[i].name.in (),
                              DAnCE::COMPONENT_FACTORY) == 0)
            {
              DANCE_DEBUG (9, (LM_TRACE, DLINFO
                               ACE_TEXT("DAnCE::Utility::get_instance_type - ")
                               ACE_TEXT("Found unmanaged component type.\n")));
              return DAnCE::CCM_COMPONENT;
            }
          if (ACE_OS::strcmp (prop[i].name.in (),
                              DAnCE::EXPLICIT_HOME) == 0)
            {
              DANCE_DEBUG (9, (LM_TRACE, DLINFO
                               ACE_TEXT("DAnCE::Utility::get_instance_type - ")
                               ACE_TEXT("Found explicit home component type.\n")));
              return DAnCE::CCM_HOMEDCOMPONENT;;
            }

          if (ACE_OS::strcmp (prop[i].name.in (),
                              DAnCE::IMPL_TYPE) == 0)
            {
              const char *val = 0;
              if (get_property_value (DAnCE::IMPL_TYPE, prop, val))
                {
                  DANCE_DEBUG (9, (LM_TRACE, DLINFO
                                   ACE_TEXT("DAnCE::Utility::get_instance_type - ")
                                   ACE_TEXT("Found instance type %C\n"), val));
                  return val;
                }
            }
        }

      DANCE_ERROR (1, (LM_INFO, DLINFO
                       ACE_TEXT("DAnCE::Utility::get_instance_type - ")
                       ACE_TEXT("Unable to determine instance type, instance will be ignored.\n")));
      return 0;
    }

    bool
    throw_exception_from_any (const CORBA::Any &excep)
    {
      CORBA::TypeCode_ptr tc = excep._tao_get_typecode ();
      std::string ex_id = tc->id ();
      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                        ACE_TEXT("DAnCE::Utility::throw_exception_from_any - ")
                        ACE_TEXT("Found typecode %C\n"), ex_id.c_str ()));
      if (ex_id == Deployment::_tc_PlanError->id ())
        {
          extract_and_throw_exception<Deployment::PlanError> (excep);
        }
      else if (ex_id == Deployment::_tc_StartError->id ())
        {
          extract_and_throw_exception<Deployment::StartError> (excep);
        }
      else if (ex_id == Deployment::_tc_StopError->id ())
        {
          extract_and_throw_exception<Deployment::StopError> (excep);
        }
      else if (ex_id.find ("IDL:omg.org/CORBA/") == 0)
        {
          CORBA::SystemException* sysex = TAO::create_system_exception (ex_id.c_str ());
          if (sysex)
            {
              TAO_OutputCDR cdr_out;
              cdr_out << excep;
              TAO_InputCDR cdr_in(cdr_out);
              sysex->_tao_decode (cdr_in);
              sysex->_raise ();
            }
        }
      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                        ACE_TEXT("DAnCE::Utility::throw_exception_from_any - ")
                        ACE_TEXT("Exception with typecode %C unknown\n"), ex_id.c_str ()));
      throw CORBA::UNKNOWN ();
    }

    CORBA::Any*
    create_any_from_user_exception (const CORBA::UserException& ex)
    {
      CORBA::Any ex_any;
      ex_any <<= ex;
      TAO_OutputCDR cdr_out;
      cdr_out << ex_any;
      TAO_InputCDR cdr_in (cdr_out);

      CORBA::Any *tmp = 0;
      ACE_NEW_THROW_EX (tmp,
                        CORBA::Any,
                        CORBA::NO_MEMORY ());
      cdr_in >> *tmp;
      return tmp;
    }

  } /* namespace Utility */
}
#endif /*DAnCE_Utility_CPP*/
