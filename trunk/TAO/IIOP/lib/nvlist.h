// This may look like C, but it's really -*- C++ -*-
//
// @(#) $Id$
// Copyright 1995 by Sun Microsystems, Inc.
//
// NamedValue ... these occur only in "NVList" (named value list)
// data structures.  The binary form of the data structure is frozen
// and visible to programs using it (e.g. from C).  The C++ class
// supports some programming discipline, e.g. to avoid memory leaks.
//
// They just represent parameters to calls.  The name is optional,
// and the value is packaged as an Any.  The flags indicate parameter
// mode, and some ownership rules for "top level" memory.
//

#if !defined(TAO_NVLIST_H)
#  define TAO_NVLIST_H

#  include "ace/OS.h"

class ACE_Svc_Export			CORBA_NamedValue;

void				CORBA_release (CORBA_NamedValue_ptr x);
CORBA_Boolean			CORBA_is_nil (CORBA_NamedValue_ptr x);

// No IID required
enum {
    CORBA_ARG_IN = 0x01,
    CORBA_ARG_OUT = 0x02,
    CORBA_ARG_INOUT = 0x04,
    CORBA_IN_COPY_VALUE = 0x08,
    CORBA_OUT_LIST_MEMORY = 0x10
};

class ACE_Svc_Export CORBA_NamedValue
{
public:
  const CORBA_String	_FAR name () { return (const CORBA_String) _name; }
  CORBA_Any_ptr	_FAR value () { return &_any; }
  CORBA_Flags		flags () const { return _flags; }

  ~CORBA_NamedValue ();

  //
  // Stuff required for COM IUnknown support
  //
  ULONG __stdcall     AddRef ();
  ULONG __stdcall     Release ();
  HRESULT __stdcall   QueryInterface (
				      REFIID      riid,
				      void        **ppv
				      );

private:
  unsigned            _refcount;
  ACE_Thread_Mutex lock_;

  CORBA_Any		_any;
  CORBA_Flags		_flags;
  const CORBA_Char	*_FAR _name;

  CORBA_NamedValue () : _flags (0), _name (0) { }

  friend class CORBA_NVList;
  friend class CORBA_Request;
};


//
// NVList ... this is used in the (client side) DII (Dynamic Invocation
// Interface) to hold parameters, except for the return parameter. It's
// used in the same role in the (server side) DSI (Dynamic Skeleton
// Interface).
//
// Each user (client, server) provides the typecode and memory for each
// parameter using an NVList, then talks to the ORB using a Request or
// ServerRequest pseudo-object.  The ORB copies data to/from the IPC
// messages (e.g. IIOP::Request, IIOP::Response) as appropriate.
//
class ACE_Svc_Export		CORBA_NVList;

void				CORBA_release (CORBA_NVList_ptr x);
CORBA_Boolean                   CORBA_is_nil (CORBA_NVList_ptr x);

extern "C" const IID 		IID_CORBA_NVList;

class ACE_Svc_Export CORBA_NVList
{
public:
  CORBA_ULong			count () const
  { return _len; }

  CORBA_NamedValue_ptr	add_value (
				   const CORBA_Char *_FAR ,
				   const CORBA_Any _FAR &,
				   CORBA_Flags,
				   CORBA_Environment _FAR &
				   );

  CORBA_NamedValue_ptr	item (CORBA_Long n) const
  { return &_values [(unsigned) n]; }

  ~CORBA_NVList ();

  //
  // Stuff required for COM IUnknown support
  //
  ULONG __stdcall             AddRef ();
  ULONG __stdcall             Release ();
  HRESULT __stdcall           QueryInterface (
					      REFIID      riid,
					      void        **ppv
					      );

private:
  CORBA_NamedValue		*_FAR _values;
  unsigned			_max;
  unsigned			_len;
  ACE_Thread_Mutex lock_;
  unsigned            	_refcount;

  CORBA_NVList ()
    : _values (0), _max (0),
      _len (0), _refcount (1)
  { }

  friend class	CORBA_ORB;
  friend class	CORBA_Request;
};

#endif
