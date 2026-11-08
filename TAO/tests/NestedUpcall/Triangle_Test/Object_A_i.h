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
  Object_A_i () = default;

  /// Destructor.
  ~Object_A_i () override = default;

  Object_A_i (Object_A_i const &) = delete;
  Object_A_i (Object_A_i &&) = delete;
  Object_A_i &operator= (Object_A_i const &) = delete;
  Object_A_i &operator= (Object_A_i &&) = delete;

  void foo (Initiator_ptr theInitiator_ptr) override;

  void finish () override;

  void shutdown () override;

  bool finish_two_way_call_ {};
};

#endif /* OBJECT_B_IMPL_H */
