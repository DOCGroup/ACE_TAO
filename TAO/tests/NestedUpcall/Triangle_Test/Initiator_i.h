//=============================================================================
/**
 *  @file    Initiator_i.h
 *
 *  This class implements the Initiator of the
 *  Nested Upcalls - Triangle test.
 *
 *  @author Michael Kircher
 */
//=============================================================================
#ifndef INITIATOR_IMPL_H
#  define INITIATOR_IMPL_H

#include "Triangle_TestS.h"

/**
 * @class Initiator_i
 *
 * @brief Implement the <Initiator> IDL interface.
 */
class Initiator_i : public POA_Initiator
{
public:
  Initiator_i (Object_A_ptr object_A_ptr,
                  Object_B_ptr object_B_ptr);

  ~Initiator_i () override = default;

  Initiator_i (Initiator_i const &) = delete;
  Initiator_i (Initiator_i &&) = delete;
  Initiator_i &operator= (Initiator_i const &) = delete;
  Initiator_i &operator= (Initiator_i &&) = delete;

  void foo_object_B () override;

private:
  /// reference to object A
  Object_A_var object_A_var_;

  /// reference to object B
  Object_B_var object_B_var_;
};

#endif /* INITIATOR_IMPL_H */
