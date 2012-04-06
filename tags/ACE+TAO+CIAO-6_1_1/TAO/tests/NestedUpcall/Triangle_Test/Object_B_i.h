
//=============================================================================
/**
 *  @file    Object_B_i.h
 *
 *  $Id$
 *
 *  This class implements the Object B of the
 *  Nested Upcalls - Triangle test.
 *
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
  Object_B_i (void);

  /// Destructor.
  virtual ~Object_B_i (void);

  virtual void foo(Object_A_ptr theObject_A_ptr);

  virtual void shutdown (void);
};

#endif /* OBJECT_B_IMPL_H */
