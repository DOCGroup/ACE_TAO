// This may look like C, but it's really -*- C++ -*-
//
//
// Header file for Win32 interface to CORBA's "Principal" type.
//
// A "Principal" identifies an authenticated entity in the network
// administration framework.  Identities are used to control acccess
// (authorization) as well as in audit trails (accountability).
//
#if !defined(TAO_PRINCIPAL_H)
#  define TAO_PRINCIPAL_H

#  include "ace/OS.h"

#  include "ace/Synch.h"

void				CORBA_release (CORBA_Principal_ptr principal);
CORBA_Boolean			CORBA_is_nil (CORBA_Principal_ptr principal);

extern const IID		IID_CORBA_Principal;

class ACE_Svc_Export CORBA_Principal : public IUnknown
{
public:
  //
  // To applications, the identifier is an opaque ID.
  //
  CORBA_SEQUENCE <CORBA_Octet>	id;

  // XXX add "==", "<", ">" operators

  //
  // Stuff required for COM IUnknown support
  //
  ULONG __stdcall		AddRef ();
  ULONG __stdcall		Release ();
  HRESULT __stdcall          	QueryInterface (
						REFIID	riid,
						void	**ppv
						);

  CORBA_Principal ();
private:
  ACE_Thread_Mutex principal_lock_;
  unsigned			_refcount;

  virtual ~CORBA_Principal ();

  // these are not provided
  CORBA_Principal		&operator = (const CORBA_Principal_ptr &);
  CORBA_Principal (const CORBA_Principal_ptr &);

#if	defined (__GNUG__)
  //
  // G++ (even 2.6.3) stupidly thinks instances can't be
  // created.  This de-warns.
  //
  friend class everyone_needs_a_friend;
#endif
};
#endif
