// -*- C++ -*-
// $Id$

#ifndef DDS4CCM_LOCALOBJECT_T_H_
#define DDS4CCM_LOCALOBJECT_T_H_

#include "tao/LocalObject.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename CCM_TYPE>
    class LocalObject_T
      : public virtual ::CORBA::LocalObject
    {
    public:
      LocalObject_T (void);
      virtual ~LocalObject_T (void);

      void _set_component (typename CCM_TYPE::base_type::_ptr_type component);
      virtual ::CORBA::Object_ptr _get_component (void);

    private:
      typename CCM_TYPE::base_type::_var_type component_;
    };

    /**
     * Helper template that allocates a new LOCAL_OBJECT from the heap and that
     * makes sure we call ::CORBA::release() at destruction
     */
    template <typename LOCAL_OBJECT>
    class LocalObject_Auto_Ptr_T : private ACE_Copy_Disabled
    {
    public:
      LocalObject_Auto_Ptr_T (void);

      ~LocalObject_Auto_Ptr_T (void);

      explicit LocalObject_Auto_Ptr_T (LOCAL_OBJECT* lobject);

      LOCAL_OBJECT* operator-> (void) const;

      operator LOCAL_OBJECT *& (void);
    private:
      LOCAL_OBJECT* l;
    };
  }
}

#include "dds4ccm/impl/LocalObject_T.cpp"

#endif /* DDS4CCM_LOCALOBJECT_T_H_ */
