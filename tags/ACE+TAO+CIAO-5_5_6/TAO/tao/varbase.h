// -*- C++ -*-

//=============================================================================
/**
 *  @file     varbase.h
 *
 *  $Id$
 *
 *   Contains a base class for the object _var classes.
 *
 *
 *  @author  Jeff Parsons.
 */
//=============================================================================

#ifndef TAO_VARBASE_H
#define TAO_VARBASE_H
#include /**/ "ace/pre.h"

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Base_var
 *
 * @brief TAO_Base_var
 *
 * Base class for object _var classes. Used to prevent the
 * _var class of a CORBA interface from accepting the _var
 * of a more derived interface in its copy constructor or
 * assignment operator.
 */
class TAO_Base_var
{
public:
  TAO_Base_var (void) {}

private:

  TAO_Base_var (const TAO_Base_var &);
  TAO_Base_var & operator= (const TAO_Base_var &);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_VARBASE_H */
