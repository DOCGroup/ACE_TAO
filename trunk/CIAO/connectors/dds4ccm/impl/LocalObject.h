// -*- C++ -*-
// $Id$

#ifndef DDS4CCM_LOCALOBJECT_H_
#define DDS4CCM_LOCALOBJECT_H_

#include "tao/LocalObject.h"
#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    /**
     * Base class for all DDS4CCM local facets. It stores
     * an object reference to the component this local
     * facet belongs too.
     */
    class DDS4CCM_DDS_IMPL_Export LocalObject
      : public virtual ::CORBA::LocalObject
    {
    public:
      LocalObject (void);
      virtual ~LocalObject (void);

      void _set_component (::CORBA::Object_ptr component);
      virtual ::CORBA::Object_ptr _get_component (void);

    private:
      ::CORBA::Object_var component_;
    };
  }
}

#endif /* DDS4CCM_LOCALOBJECT_T_H_ */
