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

class TAO_Export CORBA_NamedValue
{
  // = TITLE
  // NamedValue ... these occur only in "NVList" (named value list) data
  // structures.  The binary form of the data structure is frozen and
  // visible to programs using it (e.g. from C).  The C++ class supports
  // some programming discipline, e.g. to avoid memory leaks.
  //
  // They just represent parameters to calls.  The name is optional, and
  // the value is packaged as an Any.  The flags indicate parameter
  // mode, and some ownership rules for "top level" memory.
public:
  TAO_CONST CORBA::String _FAR name (void) 
    { return (CORBA::String) _name; }

  CORBA::Any_ptr	_FAR value (void) { return &_any; }
  CORBA::Flags flags (void) const { return _flags; }

  ~CORBA_NamedValue (void);

  // = Methods required for COM IUnknown support.

  ULONG AddRef (void);
  ULONG Release (void);
  TAO_HRESULT QueryInterface (REFIID riid,
                              void **ppv);

private:
  u_int refcount_;
  ACE_SYNCH_MUTEX lock_;

  CORBA::Any _any;
  CORBA::Flags _flags;
  const CORBA::Char *_FAR _name;

  CORBA_NamedValue (void) : refcount_(0), _flags (0), _name (0) { }

  friend class CORBA_NVList;
  friend class CORBA_Request;
};

class TAO_Export CORBA_NVList
  // = TITLE
  //   NVList ... this is used in the (client side) DII (Dynamic
  //   Invocation Interface) to hold parameters, except for the return
  //   parameter. It's used in the same role in the (server side) DSI
  //   (Dynamic Skeleton Interface).
  //
  // = DESCRIPTION
  //   Each user (client, server) provides the typecode and memory for
  //   each parameter using an NVList, then talks to the ORB using a
  //   Request or ServerRequest pseudo-object.  The ORB copies data
  //   to/from the IPC messages (e.g. IIOP::Request, IIOP::Response)
  //   as appropriate.
{
public:
  CORBA::ULong count (void) const
  { return len_; }

  CORBA::NamedValue_ptr add_value (const CORBA::Char *_FAR ,
				   const CORBA::Any _FAR &,
				   CORBA::Flags,
				   CORBA::Environment _FAR &);

  CORBA::NamedValue_ptr item (CORBA::Long n) const
  { return &values_ [(u_int) n]; }

  ~CORBA_NVList (void);

  // = Methods required for COM IUnknown support

  ULONG AddRef (void);
  ULONG Release (void);
  TAO_HRESULT QueryInterface (REFIID riid,
                              void **ppv);

private:
  // @@ Do we really need to keep these _FAR macros?
  CORBA::NamedValue *_FAR values_;
  u_int max_;
  u_int len_;
  ACE_SYNCH_MUTEX lock_;
  u_int refcount_;

  CORBA_NVList (void)
    : values_ (0), 
    max_ (0),
    len_ (0), 
    refcount_ (1)
  { }

  friend class CORBA_ORB;
  friend class CORBA_Request;
};

#endif /* TAO_NVLIST_H */
