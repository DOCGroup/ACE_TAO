
#ifndef cubit_ih
#define cubit_ih

#include "cubit.h"
#include "ace/Task.h"

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

#ifdef Cubit_USE_BOA
class Cubit_Factory_Impl : public virtual Cubit_FactoryBOAImpl {
#else
class Cubit_Factory_Impl {
#endif

public:
        virtual Cubit_ptr create_cubit (const char * name, CORBA::Environment &IT_env=CORBA::default_environment) throw (CORBA::SystemException);
};

class Cubit_Impl_Task : public virtual Cubit_Impl, public virtual ACE_Task<ACE_MT_SYNCH> {
public:
  // For ACE_Task
  int svc (void);
  // for narrowing
  virtual void *_deref ();
};

#ifndef Cubit_USE_BOA
DEF_TIE_Cubit (Cubit_Impl)
DEF_TIE_Cubit_Factory (Cubit_Factory_Impl)
#endif /* Cubit_USE_BOA */

#endif
