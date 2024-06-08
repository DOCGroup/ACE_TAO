
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
  /// Constructor.
  Object_B_i ();

  /// Destructor.
  virtual ~Object_B_i ();

  virtual void foo(Object_A_ptr theObject_A_ptr);

  virtual void shutdown ();
};

#endif /* OBJECT_B_IMPL_H */
