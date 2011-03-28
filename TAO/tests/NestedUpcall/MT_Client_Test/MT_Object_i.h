
//=============================================================================
/**
 *  @file    MT_Object_A_i.h
 *
 *  $Id$
 *
 *  This class implements the Object A of the
 *  Nested Upcalls - MT Client test
 *
 *
 *  @author Michael Kircher
 */
//=============================================================================


#ifndef MT_OBJECT_IMPL_H
#  define MT_OBJECT_IMPL_H

#include "MT_Client_TestS.h"
#include "MT_Client_TestC.h"

/**
 * @class MT_Object_i
 *
 * @brief Implement the <MT_Object> IDL interface.
 */
class MT_Object_i : public POA_MT_Object
{
public:
  /// Constructor.
  MT_Object_i (void);

  /// Destructor.
  virtual ~MT_Object_i (void);

  virtual CORBA::Long yadda (CORBA::Long hop_count,
                             MT_Object_ptr partner);

};

#endif /* MT_OBJECT_IMPL_H */
