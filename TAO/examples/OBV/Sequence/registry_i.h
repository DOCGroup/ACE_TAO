// $Id$

#if !defined (REGISTRY_I_H)
#define REGISTRY_I_H
#include "seriesC.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class registry_factory;

class registry_impl : public virtual OBV_registry,
                      public virtual CORBA::DefaultValueRefCountBase
{
public:
  registery_impl ();
  registery_impl (CORBA::Long v);
};

class registry_factory : public registry_init
{
public:
  registry_factory ();

private:
  TAO_OBV_CREATE_RETURN_TYPE(registry) create_for_unmarshal ();
};

void print_registries (const registries &v);

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */
#endif /* REGISTRY_I_H */
