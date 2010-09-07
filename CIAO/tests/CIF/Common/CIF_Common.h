// $Id$
#ifndef CIF_COMMON_H_
#define CIF_COMMON_H_

#include "CIF_Common_export.h"
#include "orbsvcs/CosNamingC.h"
#include "ccm/ComponentsC.h"

class CIF_COMMON_Export CIF_Common
{
public:
  CIF_Common (void);
  ~CIF_Common (void);

  int init (int argc,
            ACE_TCHAR *argv[]);

  void shutdown ();

  ::Components::Navigation_ptr get_navigation_interface ();
  ::Components::Receptacles_ptr get_receptacle_interface ();

private:
  ::CORBA::ORB_var orb_;
  ::CORBA::Object_var provider_cmp_;
  ::CORBA::Object_var user_cmp_;

  const char * naming_;

  int init_provider_component (::CosNaming::NamingContext_ptr naming_context);
  int init_user_component (::CosNaming::NamingContext_ptr naming_context);

  int parse_args (int argc, ACE_TCHAR *argv[]);
};

#endif
