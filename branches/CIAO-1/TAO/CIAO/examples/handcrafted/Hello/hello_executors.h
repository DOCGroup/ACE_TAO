// $Id$

// This file contains executor implementations.

#include "helloEC.h"

#if !defined (HELLO_EXECUTORS_H)
#define (HELLO_EXECUTORS_H)

class HelloWorld_Impl : public virtual CCM_HelloWorld
{
public:
  /// Default ctor.
  HelloWorld ();

  /// Default dtor.
  ~HelloWorld ();

  virtual char *sayHello (const char *username
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
};

class HelloHome_Impl : public virtual CCM_HelloHome
{
public:
  /// Default ctor.
  HelloHome_impl ();

  /// Default dtor.
  ~HelloHome_impl ();

  virtual ::Components::EnterpriseComponent_ptr
  create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CreateFailure));
};
#endif /* HELLO_EXECUTORS_H */
