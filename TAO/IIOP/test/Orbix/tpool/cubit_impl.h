
#ifndef cubit_ih
#define cubit_ih

#include "cubit.h"


#ifdef Cubit_USE_BOA
class Cubit_Impl : public virtual CubitBOAImpl {
#else
class Cubit_Impl {
#endif /* Cubit_USE_BOA */

public:
        virtual CORBA::Octet cube_octet (CORBA::Octet o, CORBA::Environment &IT_env=CORBA::default_environment) throw (CORBA::SystemException);
        virtual CORBA::Short cube_short (CORBA::Short s, CORBA::Environment &IT_env=CORBA::default_environment) throw (CORBA::SystemException);
        virtual CORBA::Long cube_long (CORBA::Long l, CORBA::Environment &IT_env=CORBA::default_environment) throw (CORBA::SystemException);
        virtual Cubit::Many cube_struct (const Cubit::Many& values, CORBA::Environment &IT_env=CORBA::default_environment) throw (CORBA::SystemException);
        virtual Cubit::oneof cube_union (const Cubit::oneof& values, CORBA::Environment &IT_env=CORBA::default_environment) throw (CORBA::SystemException);
        virtual void please_exit (CORBA::Environment &IT_env=CORBA::default_environment) throw (CORBA::SystemException);
};

#ifndef Cubit_USE_BOA
DEF_TIE_Cubit (Cubit_Impl)
#endif /* Cubit_USE_BOA */

#endif
