// $Id$


#ifndef cubit_ih
#define cubit_ih

#include "cubitS.h"


#ifdef Cubit_USE_BOA
class Cubit_Impl : public virtual _sk_Cubit {
#else
class Cubit_Impl {
#endif /* Cubit_USE_BOA */

public:

#ifdef Cubit_USE_BOA
  Cubit_Impl (const char *obj_name = NULL) :
             _sk_Cubit(obj_name)
    {}
#else
  Cubit_Impl (const char *obj_name = NULL)
    {}
#endif /* Cubit_USE_BOA */

   virtual CORBA::Octet cube_octet(CORBA::Octet o);
   virtual CORBA::Short cube_short(CORBA::Short s);
   virtual CORBA::Long cube_long(CORBA::Long l);
   virtual Cubit::Many cube_struct(const Cubit::Many& values);
   virtual Cubit::oneof cube_union(const Cubit::oneof& values);
   virtual void please_exit();
};

#endif
