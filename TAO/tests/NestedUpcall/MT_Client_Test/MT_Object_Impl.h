// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/MT_Client test
//
// = FILENAME
//    MT_Object_A_Impl.h
//
// = DESCRIPTION
//    This class implements the Object A of the 
//    Nested Upcalls - MT Client test
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#if !defined (MT_OBJECT_IMPL_H)
#  define MT_OBJECT_IMPL_H

#include "MT_Client_TestS.h"
#include "MT_Client_TestC.h"

class MT_Object_Impl : public POA_MT_Object
{
  // = TITLE
  //     Implement the <MT_Object> IDL interface.
public:
  MT_Object_Impl (void);
  // Constructor.

  virtual ~MT_Object_Impl (void);
  // Destructor.

  virtual CORBA::Long yadda (CORBA::Long hop_count,
                             MT_Object_ptr partner,
                             CORBA::Environment &_tao_environment);

};

#endif /* MT_OBJECT_IMPL_H */
