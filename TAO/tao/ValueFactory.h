// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ValueFactory.h
//
// = AUTHOR
//     Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================

#ifndef TAO_VALUEFACTORY_H
#define TAO_VALUEFACTORY_H

#include "tao/corbafwd.h"

#ifdef TAO_HAS_VALUETYPE


class TAO_Export CORBA_ValueFactoryBase
{
  /*  friend CORBA::Boolean _tao_unmarshal_pre (TAO_InputCDR &,
                                            CORBA_ValueFactory_ptr &,
                                            CORBA_ValueBase *&,
                                            const char * const);
  %! how can I make the friendship work ? (g++) */
public:
  CORBA_ValueFactoryBase ();
  virtual ~CORBA_ValueFactoryBase ();

  // non-standard
  virtual const char* tao_repository_id () = 0;

  // non-virtual is non-standard
  void _add_ref (void);
  void _remove_ref (void);

  // private: %!
  virtual CORBA_ValueBase* create_for_unmarshal (void) = 0;
  // In a derived class T use return type TAO_OBV_CREATE_RETURN_TYPE (T)
  // (see at definition below)

private:
  CORBA::ULong _tao_reference_count_;
  ACE_SYNCH_MUTEX _tao_reference_count_lock_;
}; // CORBA_ValueFactoryBase



// Use this macro for writing code that is independend from
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
// orb->lookup_value_factory (CORBA::String repo_id) .)

#define TAO_OBV_REGISTER_FACTORY(FACTORY) \
  { CORBA_ValueFactory_ptr factory = new FACTORY; \
    CORBA_ValueFactory_ptr prev_factory = \
    TAO_ORB_Core_instance ()->orb ()->register_value_factory (\
                                   factory->tao_repository_id (),\
                                   factory); \
    if (prev_factory) prev_factory->_remove_ref (); \
    factory->_remove_ref (); }


#if defined (__ACE_INLINE__)
# include "tao/ValueFactory.i"
#endif /* __ACE_INLINE__) */

#endif /* TAO_HAS_VALUETYPE */
#endif /* TAO_VALUEFACTORY_H */
