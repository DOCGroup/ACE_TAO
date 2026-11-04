#include "FooS.h"
#include "tao/LocalObject.h"
#include "tao/Version.h"

#if TAO_MAJOR_VERSION < 2 && TAO_MINOR_VERSION < 6
# define RV refcount_.value
#else
# define RV _refcount_value
#endif

#define ASSERT_REFCOUNT(count, message)                                       \
  if (ximpl->RV () != count)                                                  \
    {                                                                         \
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%l %C: expected %d actual %d\n"), \
                        message, count, ximpl->RV ()), -1);                   \
    }

struct XImpl
#ifdef LOCAL
  : X
# if TAO_MAJOR_VERSION < 2 && TAO_MINOR_VERSION < 6
// FUZZ: disable check_for_TAO_Local_RefCounted_Object
  , TAO_Local_RefCounted_Object
# endif
#else
  : POA_X
#endif
{
  XImpl () = default;
  XImpl (XImpl const &) = delete;
  XImpl (XImpl &&) = delete;
  XImpl &operator= (XImpl const &) = delete;
  XImpl &operator= (XImpl &&) = delete;

#if defined LOCAL && (TAO_MAJOR_VERSION < 2 && TAO_MINOR_VERSION < 6)
  unsigned long _refcount_value () const { return refcount_.value (); }
#endif

  void op () override {}
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{
#ifdef LOCAL
  XImpl *const ximpl {new XImpl};
  // x owns the initial reference; ximpl is a non-owning alias for the checks.
  X_var const x (ximpl);
#else
  int zero_args {};
  CORBA::ORB_var const orb {CORBA::ORB_init (zero_args, nullptr)};
  CORBA::Object_var const obj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj);
  XImpl *const x_servant {new XImpl};
  // The servant var owns the servant; x_servant is a non-owning alias.
  PortableServer::ServantBase_var const sbv (x_servant);
  X_var const x {x_servant->_this ()};
  X *const ximpl = x;
#endif

  ASSERT_REFCOUNT (1, "baseline");

  {
    Y y;
    y.theX = x;
    ASSERT_REFCOUNT (2, "in struct");
    X_var x2 = y.theX;
    ASSERT_REFCOUNT (3, "from struct");
  }

  ASSERT_REFCOUNT (1, "baseline reestablished");

  {
    XSeq xseq (1);
    xseq.length (1);
    xseq[0] = x;
    ASSERT_REFCOUNT (2, "in sequence");
#ifdef _MSC_VER
    // For now, only attempt to compile this on MSVC -- it doesn't compile
    // on GCC (and possibly others) due to:
    // error: conversion from
    // 'TAO::details::object_reference_sequence_element<
    //   TAO::details::object_reference_traits<X, TAO_Objref_Var_T<X>, true> >'
    // to non-scalar type 'X_var' requested
    // When the bug is fixed, the #ifdef should be removed.  Until then, the
    // test will appear to pass on other compilers, but the MSVC failures should
    // be enough to notice that the bug needs fixing.
    X_var x2 = xseq[0];
    if (ximpl->RV () < 3) x2._retn (); // avoid a crash
    ASSERT_REFCOUNT (3, "from sequence");
#endif
  }

  return 0;
}
