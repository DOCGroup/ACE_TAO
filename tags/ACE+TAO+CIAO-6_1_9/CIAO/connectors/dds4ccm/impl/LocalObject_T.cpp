// $Id$

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename LOCAL_OBJECT>
    LocalObject_Auto_Ptr_T<LOCAL_OBJECT>::LocalObject_Auto_Ptr_T (void) : l (new LOCAL_OBJECT)
    {
    }

    template <typename LOCAL_OBJECT>
    LocalObject_Auto_Ptr_T<LOCAL_OBJECT>::~LocalObject_Auto_Ptr_T (void)
    {
      ::CORBA::release (l);
    }

    template <typename LOCAL_OBJECT>
    LocalObject_Auto_Ptr_T<LOCAL_OBJECT>::LocalObject_Auto_Ptr_T (LOCAL_OBJECT* lobject)
      : l (lobject)
    {
    }

    template <typename LOCAL_OBJECT>
    LOCAL_OBJECT*
    LocalObject_Auto_Ptr_T<LOCAL_OBJECT>::operator-> (void) const
    {
      return this->l;
    }

    template <typename LOCAL_OBJECT>
    LocalObject_Auto_Ptr_T<LOCAL_OBJECT>::operator LOCAL_OBJECT *& (void)
    {
      return this->l;
    }
  }
}
