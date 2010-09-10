// $Id$

/**
 * @file DAnCE_Utility.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Utility methods used throughought DAnCE.

 */

#ifndef DAnCE_Utility_H
#define DAnCE_Utility_H

#include /**/ "ace/pre.h"

#include "DAnCE/DAnCE_stub_export.h"

#include "ace/config-all.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "tao/AnyTypeCode/Any.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  namespace Utility
  {
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    CORBA::Any,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> PROPERTY_MAP;

    typedef ACE_Refcounted_Auto_Ptr <PROPERTY_MAP,
                                     TAO_SYNCH_MUTEX> PMAP_PTR;

    DANCE_STUB_Export void build_property_map (PROPERTY_MAP &map,
                                               const ::Deployment::Properties &prop);

    DANCE_STUB_Export void build_property_sequence (::Deployment::Properties &prop,
                                                    const PROPERTY_MAP &map);

    template<class T>
    bool
    get_property_value (const char *name,
                        const PROPERTY_MAP &properties,
                        T &val);

    template<class T>
    bool
    get_property_value (const char *name,
                        const ::Deployment::Properties &properties,
                        T &val);

    template<class T>
    void
    update_property_value (const char *name,
                           ::Deployment::Properties &properties,
                           const T &val);

    template <typename EXCEPTION>
    void test_and_set_exception (bool &flag, EXCEPTION &exception,
                                 const char *name, const char *reason);


    DANCE_STUB_Export const char *
    get_instance_type (const ::Deployment::Properties& prop);

    DANCE_STUB_Export void append_properties (::Deployment::Properties &dest,
                            const ::Deployment::Properties &src);

    /// Write a string (usually a stringified IOR) to a file
    /// designated by the @a pathname.  The file named will always get
    /// overwritten.
    DANCE_STUB_Export int write_IOR (const ACE_TCHAR *pathname,
                                     const char *IOR);

    /// Attempt to extract the any into EXCEPTION type and
    /// throw.  Returns 'false' if extraction fails.
    template <typename EXCEPTION>
    bool extract_and_throw_exception (const CORBA::Any &excep);

    DANCE_STUB_Export bool
    throw_exception_from_any (const CORBA::Any &excep);

    template <typename EXCEPTION>
    CORBA::Any* create_any_from_exception (const EXCEPTION& ex);

    DANCE_STUB_Export CORBA::Any*
    create_any_from_user_exception (const CORBA::UserException& ex);

    template <typename EXCEPTION>
    bool stringify_exception (const CORBA::Any &excep,
                              std::string &result);

    DANCE_STUB_Export bool
    stringify_exception_from_any (const CORBA::Any &excep, std::string &result);
  }
}

// Template specializations.
namespace DAnCE
{
  namespace Utility
  {
    template<>
    DANCE_STUB_Export bool
    get_property_value (const char *name,
                        const PROPERTY_MAP &properties, bool &val);

    template<>
    DANCE_STUB_Export bool
    get_property_value (const char *name,
                        const PROPERTY_MAP &properties, const char *&val);

    template<>
    DANCE_STUB_Export bool
    get_property_value (const char *name,
                        const ::Deployment::Properties &properties,
                        const char * &val);

    template<>
    DANCE_STUB_Export bool
    get_property_value (const char *name,
                        const ::Deployment::Properties &properties,
                        bool &val);

    template<char*>
    DANCE_STUB_Export void
    update_property_value (const char *name,
                           ::Deployment::Properties &properties,
                           const char * &val);
  }
}


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "DAnCE/DAnCE_Utility.tpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("DAnCE/DAnCE_Utility.tpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"
#endif
