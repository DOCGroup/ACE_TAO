// -*- C++ -*-

//=============================================================================
/**
 *  @file     ValueFactory.h
 *
 *  $Id$
 *
 *  @author  Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================

#ifndef TAO_VALUEFACTORY_H
#define TAO_VALUEFACTORY_H

#include /**/ "ace/pre.h"

#include "tao/Valuetype/valuetype_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Valuetype/Value_VarOut_T.h"

#include "tao/Basic_Types.h"
#include "tao/orbconf.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "ace/Null_Mutex.h"
#include "ace/Atomic_Op.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class AbstractBase;
  typedef AbstractBase *AbstractBase_ptr;

  class ValueBase;

  class ValueFactoryBase;

  extern TAO_Valuetype_Export void add_ref (ValueFactoryBase *);
  extern TAO_Valuetype_Export void remove_ref (ValueFactoryBase *);

  typedef TAO_Value_Var_T<ValueFactoryBase>
    ValueFactoryBase_var;

  typedef ValueFactoryBase_var ValueFactory_var;

  class TAO_Valuetype_Export ValueFactoryBase
  {
  public:
    ValueFactoryBase (void);
    virtual ~ValueFactoryBase (void);

    // non-virtual is non-standard
    void _add_ref (void);
    void _remove_ref (void);

    // private: %!
    /// In a derived class T use return type TAO_OBV_CREATE_RETURN_TYPE (T)
    /// (see at definition below)
    virtual CORBA::ValueBase * create_for_unmarshal (void) = 0;

    /// Not pure virtual because this will be overridden only by valuetypes
    /// that support an abstract interface.
    virtual CORBA::AbstractBase_ptr create_for_unmarshal_abstract (void);

  private:
    /// Reference counter.
    ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong> _tao_reference_count_;
  };

}  // End CORBA namespace

namespace TAO
{
  /**
   * @brief Specializations needed for using with Value_Var_T
   */
  template<>
  struct TAO_Valuetype_Export Value_Traits<CORBA::ValueFactoryBase>
  {
    static void add_ref (CORBA::ValueFactoryBase *);
    static void remove_ref (CORBA::ValueFactoryBase *);

    // For INOUT value type arguments, so they can use the same set
    // of arg classes as interfaces.
    static void release (CORBA::ValueFactoryBase *);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

// Use this macro for writing code that is independent from
// the compiler support of covariant return types of pointers to
// virtual inherited classes.
// (e.g. in egcs-2.90.29 980515 (egcs-1.0.3 release) its not yet implemented)
// (But it is TAO specific and at the moment the covariant case is not
// elaborated --- its just a suggestion.)

#ifdef TAO_HAS_OBV_COVARIANT_RETURN
#  define TAO_OBV_CREATE_RETURN_TYPE(TYPE) TYPE *
#else /*  TAO_HAS_OBV_COVARIANT_RETURN */
#  define TAO_OBV_CREATE_RETURN_TYPE(TYPE) CORBA::ValueBase *
#endif /* TAO_HAS_OBV_COVARIANT_RETURN */

// (The obtaining of the repository id is currently not yet like the OMG way. %!)
//
// Macro for on the fly registration of a factory (with type Factory).
// The repository id is taken from the static repository id of the
// valuetype via tao_repository_id () of the specialized factory.
// It forgets the pre-registered factory (if any) and the reference
// to the newly created one. (A new reference could be obtained with
// orb->lookup_value_factory (char * repo_id) .)

#define TAO_OBV_REGISTER_FACTORY(FACTORY, VALUETYPE) \
  { CORBA::ValueFactory factory = new FACTORY; \
    CORBA::ValueFactory prev_factory = \
    TAO_ORB_Core_instance ()->orb ()->register_value_factory (\
      VALUETYPE::_tao_obv_static_repository_id (),\
                                   factory); \
    if (prev_factory) prev_factory->_remove_ref (); \
    factory->_remove_ref (); }


#include /**/ "ace/post.h"

#endif /* TAO_VALUEFACTORY_H */
