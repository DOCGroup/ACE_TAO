// $Id$

/**
 * @file DAnCE_Uility.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Utility methods used throughought DAnCE.

 */

#ifndef DAnCE_Utility_H
#define DAnCE_Utility_H

#include /**/ "ace/pre.h"

#include "DAnCE_stub_export.h"

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
                                     ACE_SYNCH_MUTEX> PMAP_PTR;

    DANCE_STUB_Export void build_property_map (PROPERTY_MAP &map,
                             const ::Deployment::Properties &prop);

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

    template <typename EXCEPTION>
    void test_and_set_exception (bool &flag, EXCEPTION &exception,
                                 const char *name, const char *reason);


    DANCE_STUB_Export const char *
    get_instance_type (const ::Deployment::Properties& prop);

    DANCE_STUB_Export void append_properties (::Deployment::Properties &dest,
                            const ::Deployment::Properties &src);

    /// Write a string (usually a stringified IOR) to a file
    /// designated by the @c pathname.  The file named will always get
    /// overwritten.
    DANCE_STUB_Export int write_IOR (const ACE_TCHAR *pathname,
                                     const char *IOR);
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
  }
}


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "DAnCE_Utility.tpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("DAnCE_Utility.tpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"
#endif
