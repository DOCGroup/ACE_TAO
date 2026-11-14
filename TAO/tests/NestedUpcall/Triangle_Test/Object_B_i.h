//=============================================================================
/**
 *  @file    Object_B_i.h
 *
 *  This class implements the Object B of the
 *  Nested Upcalls - Triangle test.
 *
 *  @author Michael Kircher
 */
//=============================================================================
#ifndef OBJECT_B_IMPL_H
#  define OBJECT_B_IMPL_H

#include "Triangle_TestS.h"

/**
 * @class Object_B_i
 *
 * @brief Implement the <Object_B> IDL interface.
 */
class Object_B_i : public POA_Object_B
{
public:
  Object_B_i () = default;

  ~Object_B_i () override = default;

  Object_B_i (Object_B_i const &) = delete;
  Object_B_i (Object_B_i &&) = delete;
  Object_B_i &operator= (Object_B_i const &) = delete;
  Object_B_i &operator= (Object_B_i &&) = delete;

  void foo (Object_A_ptr theObject_A_ptr) override;

  void shutdown () override;
};

#endif /* OBJECT_B_IMPL_H */
