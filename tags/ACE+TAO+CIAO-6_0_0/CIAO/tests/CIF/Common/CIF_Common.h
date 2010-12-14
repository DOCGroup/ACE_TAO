// $Id$
#ifndef CIF_COMMON_H_
#define CIF_COMMON_H_

#include "CIF_Common_export.h"
#include "orbsvcs/CosNamingC.h"
#include "ccm/ComponentsC.h"
#include "Component/CIF_ComponentC.h"

class CIF_COMMON_Export CIF_Common
{
public:
    CIF_Common (void);
    ~CIF_Common (void);

    int init (int argc,
              ACE_TCHAR *argv[]);

    void shutdown ();

    ::Components::Navigation_ptr get_navigation_interface (void);
    ::Components::Receptacles_ptr get_receptacle_interface (void);
    ::CORBA::Object_ptr get_provider_cmp (void);
    ::CORBA::Object_ptr get_user_cmp (void);

    int test_provider_component (void);
    int test_user_component (void);

private:
    ::CORBA::ORB_var orb_;

    ::CIF::CIF_User_var     user_object_;
    ::CIF::CIF_Provider_var provider_object_;

    ACE_TCHAR * naming_;

    int init_provider_component (::CosNaming::NamingContext_ptr naming_context);
    int init_user_component (::CosNaming::NamingContext_ptr naming_context);

    int parse_args (int argc, ACE_TCHAR *argv[]);
};

#endif
