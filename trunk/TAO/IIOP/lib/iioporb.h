// This may look like C, but it's really -*- C++ -*-
//
// @(#) $Id$
// Copyright 1995 by Sun Microsystems, Inc.
// All Rights Reserved
//
// IIOP:	bridge implementation of IIOP_ORB
//
// This is one of the two main hooks into IIOP in this Win32 C/C++/COM
// based implementation; the other being IIOP_Object.
//

#ifndef	_iioporb_hh
#define	_iioporb_hh
typedef class IIOP_ORB		*IIOP_ORB_ptr;

extern const IID		IID_IIOP_ORB;

//
// ORB pseudo-objref
//
class _EXPCLASS IIOP_ORB : public CORBA_ORB
{
  public:
				IIOP_ORB (CORBA_Boolean flag)
				{ use_omg_ior_format = flag; }

				~IIOP_ORB () { }

    CORBA_Object_ptr		string_to_object (
				    CORBA_String	str,
				    CORBA_Environment	&env
				);
    CORBA_String		object_to_string (
				    CORBA_Object_ptr	obj,
				    CORBA_Environment	&env
				);

    HRESULT __stdcall           QueryInterface (
				    REFIID	riid,
				    void	**ppv
				);
  private:
    CORBA_Boolean		use_omg_ior_format;

    // these are not provided
				IIOP_ORB (const IIOP_ORB &);
    IIOP_ORB			&operator = (const IIOP_ORB &);
};

#endif	// _iioporb_hh
