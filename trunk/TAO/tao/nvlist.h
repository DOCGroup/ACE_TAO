// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    nvlist.h
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
// 
// ============================================================================

#if !defined (TAO_NVLIST_H)
#  define TAO_NVLIST_H

#  include "ace/OS.h"

#  include "tao/any.h"

// @@ IS it necessary to use ACE_Svc_Export on a forward decl?  I
// simply did a global replace of _EXPCLASS with ACE_Svc_Export.  I'm
// not familiar enough with crazy MSVC nuances to know if this is
// necessary, what it means, etc.  Perhaps Brian can shed some light
// on the subject? --cjc
class ACE_Svc_Export CORBA_NamedValue;

void CORBA_release (CORBA_NamedValue_ptr x);
CORBA_Boolean CORBA_is_nil (CORBA_NamedValue_ptr x);

// No IID required
enum 
{ 
  CORBA_ARG_IN = 0x01,
  CORBA_ARG_OUT = 0x02,
  CORBA_ARG_INOUT = 0x04,
  CORBA_IN_COPY_VALUE = 0x08,
  CORBA_OUT_LIST_MEMORY = 0x10
};

class ACE_Svc_Export CORBA_NamedValue
// = TITLE
// NamedValue ... these occur only in "NVList" (named value list) data
// structures.  The binary form of the data structure is frozen and
// visible to programs using it (e.g. from C).  The C++ class supports
// some programming discipline, e.g. to avoid memory leaks.
//
// They just represent parameters to calls.  The name is optional, and
// the value is packaged as an Any.  The flags indicate parameter
// mode, and some ownership rules for "top level" memory.
{
public:
  TAO_CONST CORBA_String _FAR name (void) 
    { return (const CORBA_String) _name; }

  CORBA_Any_ptr	_FAR value (void) { return &_any; }
  CORBA_Flags flags (void) const { return _flags; }

  ~CORBA_NamedValue (void);

  // = Methods required for COM IUnknown support.

  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID riid,
				    void **ppv);

private:
  u_int refcount_;
  ACE_SYNCH_MUTEX lock_;

  CORBA_Any _any;
  CORBA_Flags _flags;
  const CORBA_Char *_FAR _name;

  CORBA_NamedValue (void) : _flags (0), _name (0) { }

  friend class CORBA_NVList;
  friend class CORBA_Request;
};

class ACE_Svc_Export CORBA_NVList;

void CORBA_release (CORBA_NVList_ptr x);
CORBA_Boolean CORBA_is_nil (CORBA_NVList_ptr x);

// @@ Why is this extern "C"?
// See prior comments...
extern "C" const IID IID_CORBA_NVList;

class ACE_Svc_Export CORBA_NVList
  // = TITLE
  // NVList ... this is used in the (client side) DII (Dynamic
  // Invocation Interface) to hold parameters, except for the return
  // parameter. It's used in the same role in the (server side) DSI
  // (Dynamic Skeleton Interface).
  //
  // Each user (client, server) provides the typecode and memory for
  // each parameter using an NVList, then talks to the ORB using a
  // Request or ServerRequest pseudo-object.  The ORB copies data
  // to/from the IPC messages (e.g. IIOP::Request, IIOP::Response) as
  // appropriate.
{
public:
  CORBA_ULong count (void) const
  { return _len; }

  CORBA_NamedValue_ptr add_value (const CORBA_Char *_FAR ,
				  const CORBA_Any _FAR &,
				  CORBA_Flags,
				  CORBA_Environment _FAR &);

  CORBA_NamedValue_ptr item (CORBA_Long n) const
  { return &_values [(u_int) n]; }

  ~CORBA_NVList (void);

  // = Methods required for COM IUnknown support

  ULONG __stdcall AddRef (void);
  ULONG __stdcall Release (void);
  HRESULT __stdcall QueryInterface (REFIID riid,
				    void **ppv);

private:
  // @@ Do we really need to keep these _FAR macros?
  CORBA_NamedValue *_FAR _values;
  u_int _max;
  u_int _len;
  ACE_SYNCH_MUTEX lock_;
  u_int refcount_;

  CORBA_NVList (void)
    : _values (0), 
    _max (0),
    _len (0), 
    refcount_ (1)
  { }

  friend class CORBA_ORB;
  friend class CORBA_Request;
};

#endif /* TAO_NVLIST_H */
