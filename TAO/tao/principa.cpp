// @ (#)principa.cpp	1.4 95/11/04
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// ORB:		Principal identifier pseudo-objref
//

#include <objbase.h>
#include <initguid.h>

#include "tao/orb.h"
#include "tao/principa.h"

void
CORBA_release (CORBA_Principal_ptr principal)
{
  if (principal)
    principal->Release ();
}

CORBA_Boolean
CORBA_is_nil (CORBA_Principal_ptr principal)
{
  return (CORBA_Boolean) (principal == 0);
}

CORBA_Principal::CORBA_Principal (void)
{
}

CORBA_Principal::~CORBA_Principal (void)
{
  assert (refcount_ == 0);

  if (id.buffer)
    delete [] id.buffer;
}

//
// For COM -- IUnKnown operations
//

// {A201E4C0-F258-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_Principal,
0xa201e4c0, 0xf258, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);


ULONG __stdcall
CORBA_Principal::AddRef (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, this->lock_, 0));

  return ++refcount_;
}

ULONG __stdcall
CORBA_Principal::Release (void)
{
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, mon, this->lock_, 0));

    if (--refcount_ != 0)
      return refcount_;
  }

  delete this;
  return 0;
}

HRESULT __stdcall
CORBA_Principal::QueryInterface (REFIID riid,
				 void **ppv)
{
  *ppv = 0;

  if (IID_CORBA_Principal == riid || IID_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (E_NOINTERFACE);

  (void) AddRef ();
  return NOERROR;
}
