// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    objtable.hh
//
// = DESCRIPTION
//    Header file for Win32 interface to CORBA's "Principal" type.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
// 
// ============================================================================

#if !defined (TAO_PRINCIPAL_H)
#  define TAO_PRINCIPAL_H

class ACE_Svc_Export CORBA_Principal : public IUnknown
  // = TITLE
  //    A "Principal" identifies an authenticated entity in the
  //    network administration framework.  Identities are used to
  //    control acccess (authorization) as well as in audit trails
  //    (accountability).
{
public:
  // To applications, the identifier is an opaque ID.

  //  CORBA::SEQUENCE <CORBA::Octet> id;
  CORBA::OctetSeq id;

  // XXX add "==", "<", ">" operators

  // Stuff required for COM IUnknown support

  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID riid,
				    void **ppv);

  CORBA_Principal (void);

private:
  ACE_SYNCH_MUTEX lock_;
  u_int refcount_;

  virtual ~CORBA_Principal (void);

  // = these are not provided
  CORBA_Principal &operator = (const CORBA::Principal_ptr &);
  CORBA_Principal (const CORBA::Principal_ptr &);

#if defined (__GNUG__)
  // @@ Fix this to use the ACE "NAMESPACE" macro?
  // G++ (even 2.6.3) stupidly thinks instances can't be
  // created.  This de-warns.
  friend class everyone_needs_a_friend;
#endif /* __GNUG__ */
};
#endif /* TAO_PRINCIPAL_H */
