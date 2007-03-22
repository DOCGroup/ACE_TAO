// -*- C++ -*-

//=============================================================================
/**
 *  @file    NVList.h
 *
 *  $Id$
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems Inc.
 *  @author  Aniruddha Gokhale <gokhale@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NVLIST_H
#define TAO_NVLIST_H

#include /**/ "ace/pre.h"

#include "ace/Unbounded_Queue.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/NVList_Adapter_Impl.h"

#include "ace/Unbounded_Queue.h"
#include "ace/Thread_Mutex.h"
#include "ace/Atomic_Op.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_InputCDR;

namespace CORBA
{
  enum
  {
    // = Flags for NVList add methods
    ARG_IN                  = 0x01,
    ARG_OUT                 = 0x02,
    ARG_INOUT               = 0x04,
    IN_COPY_VALUE           = 0x08,
    OUT_LIST_MEMORY         = 0x10,
    DEPENDENT_LIST          = 0x20,

    // = (Unused) flags for Context methods
    CTX_RESTRICT_SCOPE      = 0x40,
    CTX_DELETE_DESCENDENTS  = 0x80,

    // = Flags for deferred synchronous methods
    INV_NO_RESPONSE         = 0x100,
    INV_TERM_ON_ERR         = 0x200,
    RESP_NO_WAIT            = 0x400
  };

  typedef TAO_Pseudo_Var_T<NamedValue> NamedValue_var;
  typedef TAO_Pseudo_Out_T<NamedValue> NamedValue_out;

  typedef ULong Flags;

  /**
   * @class NamedValue
   *
   * @brief CORBA Name/value pair implementation.
   *
   * These occur only in "NVList" (named value list) data structures.
   * The binary form of the data structure is frozen and visible to
   * programs using it (e.g. from C).  The C++ class supports some
   * programming discipline, e.g. to avoid memory leaks.  They just
   * represent parameters to calls.  The name is optional, and the
   * value is packaged as an Any.  The flags indicate parameter mode,
   * and some ownership rules for "top level" memory.
   */
  class TAO_AnyTypeCode_Export NamedValue
  {
    friend class ::TAO_NVList_Adapter_Impl;
    friend class NVList;
    friend class Request;

  public:
    /// optional name
    const char * name (void) const;

    /// return the value
    Any_ptr value (void) const;

    /// return the parameter mode flag
    Flags flags (void) const;

    // The pseudo object static methods..
    static NamedValue * _duplicate (NamedValue *);
    static NamedValue * _nil (void);

    // = Reference counting.
    ULong _incr_refcnt (void);
    ULong _decr_refcnt (void);

    // Useful for template programming.
    typedef NamedValue_ptr _ptr_type;
    typedef NamedValue_var _var_type;
    typedef NamedValue_out _out_type;

  protected:

    /// Destructor
    /**
     * Protected destructor to enforce proper memory management
     * through the reference counting mechanism.
     */
    ~NamedValue (void);

  private:

    /// private constructor. Cannot be directly instantiated other than
    /// by its friends.
    NamedValue (void);

  private:

    /// Reference counter.
    ACE_Atomic_Op<TAO_SYNCH_MUTEX, unsigned long> refcount_;

    /// holds the value
    Any any_;

    /// parameter mode flags
    Flags flags_;

    /// optional IDL name of the parameter
    char * name_;
  };

  // ****************************************************************

  typedef TAO_Pseudo_Var_T<NVList> NVList_var;
  typedef TAO_Pseudo_Out_T<NVList> NVList_out;

  /**
   * @class NVList
   *
   * @brief CORBA::NVList implementation.

   * This is used in the (client side) DII (Dynamic Invocation
   * Interface) to hold parameters, except for the return
   * parameter. It's used in the same role in the (server side) DSI
   * (Dynamic Skeleton Interface).
   *
   * Each user (client, server) provides the typecode and memory for
   * each parameter using an NVList, then talks to the ORB using a
   * Request or ServerRequest pseudo-object.  The ORB copies data
   * to/from the IPC messages (e.g. IIOP::Request, IIOP::Response)
   * as appropriate.
   */
  class TAO_AnyTypeCode_Export NVList
  {
    friend class ::TAO_NVList_Adapter_Impl;
    friend class Request;

  public:

    /// return the current number of elements in the list
    ULong count (void) const;

    /// add an element and just initialize the flags
    NamedValue_ptr add (Flags);

    /// add an element and initialize its name and flags
    NamedValue_ptr add_item (const char *, Flags);

    /// initializes a value, name, and flags
    NamedValue_ptr add_value (const char *, const Any &, Flags);

    /// just like add_item. In addition, memory management of char *
    /// name is taken over by the NVList
    NamedValue_ptr add_item_consume (char *, Flags);

    /// just like add_value. In addition, the NVList controls the
    /// memory management of the char *name and Any *value parameter
    NamedValue_ptr add_value_consume (char *, Any_ptr, Flags);

    /// retrieve the item at the nth location. Raises Bounds
    NamedValue_ptr item (ULong n);

    //  CORBA::Status
    /// remove element at index n. Raises Bounds
    void remove (ULong n);

    // The pseudo object static methods..
    static NVList * _duplicate (NVList *);
    static NVList * _nil (void);

    // = Reference counting.
    ULong _incr_refcnt (void);
    ULong _decr_refcnt (void);

    // = TAO Extensions:

    /**
     * Set the incoming CDR stream, this is used by TAO to perform lazy
     * evaluation of the NVList in an incoming ServerRequest.
     * The <flag> is used to check which parameters (IN, OUT and/or
     * INOUT) are to be extracted
     */
    void _tao_incoming_cdr (TAO_InputCDR & cdr,
                            int flag,
                            bool &lazy_evaluation);

    /// Encode the NVList into the CDR stream. @a flag masks the type of
    /// arguments (IN, OUT or INOUT) that are to be marshaled.
    void _tao_encode (TAO_OutputCDR & cdr, int flag);

    /// Decode the NVList arguments from the @a cdr stream.
    void _tao_decode (TAO_InputCDR & cdr, int flag);

    /**
     * Return the required alignment to marshal the NVList without any
     * re-alignment.
     * It returns ACE_CDR::MAX_ALIGNMENT to indicate errors.
     */
    ptrdiff_t _tao_target_alignment (void);

    /**
     * If this list is used by a DII request, this will tell us if
     * our CDR stream contains any marshaled arguments (needed for
     * GIOP 1.2).
     */
    Boolean _lazy_has_arguments (void) const;

    // Useful for template programming.
    typedef NVList_ptr _ptr_type;
    typedef NVList_var _var_type;
    typedef NVList_out _out_type;

  protected:

    /// Destructor
    /**
     * Protected destructor to enforce proper memory management
     * through the reference counting mechanism.
     */
    ~NVList (void);

  private:
    /// Constructor - cannot be instantiated directly other than
    /// through the CORBA::ORB::create_list method
    NVList (void);

    /// Helper to increase the list size. This is used by all the add_
    /// methods of the NVList class
    NamedValue_ptr add_element (Flags);

    /// Lazy evaluation routine to fill up the Anys in the NVList from
    /// the CDR stream.
    void evaluate (void);

  private:
    /// Internal list of parameters stored as NamedValues
    ACE_Unbounded_Queue<NamedValue_ptr> values_;

    /// Maximum length of list
    ULong max_;

    /// Reference counter.
    ACE_Atomic_Op<TAO_SYNCH_MUTEX, unsigned long> refcount_;

    /// Protects the incoming pointer.
    TAO_SYNCH_MUTEX lock_;

    /**
     * When the NVList is used as part of a Server Request we can simply
     * store the CDR buffer and perform lazy evaluation to compute the
     * Anys.
     */
    TAO_InputCDR * incoming_;

    /// The flags used to check which parameters are actually extracted
    /// from the <incoming_> buffer
    int incoming_flag_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/NVList.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_NVLIST_H */
