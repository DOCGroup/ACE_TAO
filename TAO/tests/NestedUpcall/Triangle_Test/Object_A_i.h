
//=============================================================================
/**
 *  @file    Object_A_i.h
 *
 *  This class implements the Object A of the
 *  Nested Upcalls - Triangle test.
 *
 *  @author Michael Kircher
 */
//=============================================================================


#ifndef OBJECT_B_IMPL_H
#  define OBJECT_B_IMPL_H

#include "Triangle_TestS.h"

/**
 * @class Object_A_i
 *
 * @brief Implement the <Object_A> IDL interface.
 */
class Object_A_i : public POA_Object_A
{
public:
  /// Constructor.
  Object_A_i ();

  /// Destructor.
  virtual ~Object_A_i ();

  virtual void foo (Initiator_ptr theInitiator_ptr);

  virtual void finish ();

  virtual void shutdown ();

  unsigned long finish_two_way_call_;
};

#endif /* OBJECT_B_IMPL_H */
