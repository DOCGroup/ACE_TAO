// This may look like C, but it's really -*- C++ -*-

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

#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Unbounded_Queue.h"
#include "tao/Any.h"

class TAO_InputCDR;

namespace CORBA
{
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
  class TAO_Export NamedValue
  {
    friend class CORBA::ORB;
    friend class CORBA::NVList;
    friend class CORBA::Request;

  public:
    /// optional name
    const char *name (void) const;

    /// return the value
    CORBA::Any_ptr value (void) const;

    /// return the parameter mode flag
    CORBA::Flags flags (void) const;

    // The pseudo object static methods..
    static NamedValue* _duplicate (NamedValue*);
    static NamedValue* _nil (void);

    // = Reference counting.
    CORBA::ULong _incr_refcnt (void);
    CORBA::ULong _decr_refcnt (void);

    // Useful for template programming.
#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
    typedef NamedValue_ptr _ptr_type;
    typedef NamedValue_var _var_type;
#endif /* __GNUC__ */

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

    /// maintains how many references exist to this object
    CORBA::ULong refcount_;

    /// Protects the reference count.
    TAO_SYNCH_MUTEX refcount_lock_;

    /// holds the value
    CORBA::Any any_;

    /// parameter mode flags
    CORBA::Flags flags_;

    /// optional IDL name of the parameter
    char *name_;
  };

  /**
   * @class NamedValue_var
   *
   * @brief The T_var class for NamedValue
   *
   * As any other pseudo object NamedValue must have a T_var class,
   * the interface an semantics are specified in the CORBA spec.
   */
  class TAO_Export NamedValue_var
  {
  public:

    NamedValue_var (void); // default constructor
    NamedValue_var (NamedValue_ptr);
    NamedValue_var (const NamedValue_var &); // copy constructor
    ~NamedValue_var (void); // destructor

    NamedValue_var &operator= (NamedValue_ptr);
    NamedValue_var &operator= (const NamedValue_var &);
    NamedValue_ptr operator-> (void) const;

    /// in, inout, out, _retn
    operator const NamedValue_ptr &() const;
    operator NamedValue_ptr &();
    NamedValue_ptr in (void) const;
    NamedValue_ptr &inout (void);
    NamedValue_ptr &out (void);
    NamedValue_ptr _retn (void);
    NamedValue_ptr ptr (void) const;

  private:
    NamedValue_ptr ptr_;
  };

  /**
   * @class NamedValue_out
   *
   * @brief The T_out class for NamedValue
   *
   * As any other pseudo object NamedValue must have a T_out class,
   * the interface an semantics are specified in the CORBA spec.
   */
  class TAO_Export NamedValue_out
  {
  public:
    NamedValue_out (NamedValue_ptr &);
    NamedValue_out (NamedValue_var &);
    NamedValue_out (NamedValue_out &);
    NamedValue_out &operator= (NamedValue_out &);
    NamedValue_out &operator= (const NamedValue_var &);
    NamedValue_out &operator= (NamedValue_ptr);
    operator NamedValue_ptr &();
    NamedValue_ptr &ptr (void);
    NamedValue_ptr operator-> (void);

  private:
    NamedValue_ptr &ptr_;
  };

  // ****************************************************************

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
  class TAO_Export NVList
  {
    friend class CORBA::ORB;
    friend class CORBA::Request;

  public:

    /// return the current number of elements in the list
    CORBA::ULong count (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) const;

    /// add an element and just initialize the flags
    CORBA::NamedValue_ptr add (CORBA::Flags
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// add an element and initialize its name and flags
    CORBA::NamedValue_ptr add_item (const char *,
                                    CORBA::Flags
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// initializes a value, name, and flags
    CORBA::NamedValue_ptr add_value (const char *,
                                     const CORBA::Any &,
                                     CORBA::Flags
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// just like add_item. In addition, memory management of char *
    /// name is taken over by the NVList
    CORBA::NamedValue_ptr add_item_consume (char *,
                                            CORBA::Flags
                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// just like add_value. In addition, the NVList controls the
    /// memory management of the char *name and Any *value parameter
    CORBA::NamedValue_ptr add_value_consume (char *,
                                             CORBA::Any_ptr,
                                             CORBA::Flags
                                             ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// retrieve the item at the nth location. Raises Bounds
    CORBA::NamedValue_ptr item (CORBA::ULong n
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    //  CORBA::Status
    /// remove element at index n. Raises Bounds
    void remove (CORBA::ULong n
                 ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    // The pseudo object static methods..
    static NVList* _duplicate (NVList*);
    static NVList* _nil (void);

    // = Reference counting.
    CORBA::ULong _incr_refcnt (void);
    CORBA::ULong _decr_refcnt (void);

    // = TAO Extensions:

    /**
     * Set the incoming CDR stream, this is used by TAO to perform lazy
     * evaluation of the NVList in an incoming ServerRequest.
     * The <flag> is used to check which parameters (IN, OUT and/or
     * INOUT) are to be extracted
     */
    void _tao_incoming_cdr (TAO_InputCDR &cdr,
                            int flag,
                            int &lazy_evaluation
                            ACE_ENV_ARG_DECL);

    /// Encode the NVList into the CDR stream. <flag> masks the type of
    /// arguments (IN, OUT or INOUT) that are to be marshaled.
    void _tao_encode (TAO_OutputCDR &cdr,
                      TAO_ORB_Core *orb_core,
                      int flag
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// Decode the NVList arguments from the <cdr> stream.
    void _tao_decode (TAO_InputCDR &cdr,
                      int flag
                      ACE_ENV_ARG_DECL);

    /**
     * Return the required alignment to marshal the NVList without any
     * re-alignment.
     * It returns ACE_CDR::MAX_ALIGNMENT to indicate errors.
     */
    ptr_arith_t _tao_target_alignment (void);

    /**
     * If this list is used by a DII request, this will tell us if
     * our CDR stream contains any marshaled arguments (needed for
     * GIOP 1.2).
     */
    CORBA::Boolean _lazy_has_arguments (void) const;

    // Useful for template programming.
#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
    typedef NVList_ptr _ptr_type;
    typedef NVList_var _var_type;
#endif /* __GNUC__ */

  protected:

    /// Destructor
    /**
     * Protected destructor to enforce proper memory management
     * through the reference counting mechanism.
     */
    ~NVList (void);

  private:
    /// constructor - cannot be instantiated directly other than
    /// through the CORBA::ORB::create_list method
    NVList (void);

    /// helper to increase the list size. This is used by all the add_
    /// methods of the NVList class
    CORBA::NamedValue_ptr add_element (CORBA::Flags
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    /// Lazy evaluation routine to fill up the Anys in the NVList from
    /// the CDR stream.
    void evaluate (ACE_ENV_SINGLE_ARG_DECL);

  private:
    /// internal list of parameters stored as NamedValues
    ACE_Unbounded_Queue<CORBA::NamedValue_ptr> values_;

    /// maximum length of list
    CORBA::ULong max_;

    /// maintains how many references exist to this object
    CORBA::ULong refcount_;

    /// Protects the reference count.
    TAO_SYNCH_MUTEX refcount_lock_;

    /**
     * When the NVList is used as part of a Server Request we can simply
     * store the CDR buffer and perform lazy evaluation to compute the
     * Anys.
     */
    TAO_InputCDR *incoming_;

    /// The flags used to check which parameters are actually extracted
    /// from the <incoming_> buffer
    int incoming_flag_;
  };

  /**
   * @class NVList_var
   *
   * @brief The T_var class for NVList
   *
   * As any other pseudo object NVList must have a T_var class,
   * the interface an semantics are specified in the CORBA spec.
   */
  class TAO_Export NVList_var
  {
  public:
    NVList_var (void);
    NVList_var (NVList_ptr);
    NVList_var (const NVList_var &);
    ~NVList_var (void);

    NVList_var &operator= (NVList_ptr);
    NVList_var &operator= (const NVList_var &);
    NVList_ptr operator-> (void) const;

    /// in, inout, out, _retn
    operator const NVList_ptr &() const;
    operator NVList_ptr &();
    NVList_ptr in (void) const;
    NVList_ptr &inout (void);
    NVList_ptr &out (void);
    NVList_ptr _retn (void);
    NVList_ptr ptr (void) const;

  private:
    NVList_ptr ptr_;
   };

  /**
   * @class NVList_out
   *
   * @brief The T_out class for NVList
   *
   * As any other pseudo object NVList must have a T_out class,
   * the interface an semantics are specified in the CORBA spec.
   */
  class TAO_Export NVList_out
  {
  public:
    NVList_out (NVList_ptr &);
    NVList_out (NVList_var &);
    NVList_out (NVList_out &);
    NVList_out &operator= (NVList_out &);
    NVList_out &operator= (const NVList_var &);
    NVList_out &operator= (NVList_ptr);
    operator NVList_ptr &();
    NVList_ptr &ptr (void);
    NVList_ptr operator-> (void);

  private:
    NVList_ptr &ptr_;
   };
}

// ****************************************************************

#if defined (__ACE_INLINE__)
# include "tao/NVList.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NVLIST_H */
