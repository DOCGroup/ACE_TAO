// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Base.h
 *
 *  $Id$
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_SUPCALL_COMMANDS_H
#define TAO_SUPCALL_COMMANDS_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Upcall_Command.h"
#include "tao/PortableServer/Upcall_Wrapper.h"
#include "tao/PortableServer/SArg_Traits_T.h"
#include "tao/PortableServer/Basic_SArguments.h"
#include "tao/PortableServer/Special_Basic_SArguments.h"
#include "tao/PortableServer/Fixed_Size_SArgument_T.h"
#include "tao/PortableServer/Var_Size_SArgument_T.h"
#include "tao/PortableServer/Object_SArg_Traits.h"
#include "tao/PortableServer/UB_String_SArguments.h"
#include "tao/PortableServer/get_arg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class _is_a_Upcall_Command
 *
 * @brief Base class for skeletons and servants.
 *
 *
 */
class _is_a_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _is_a_Upcall_Command (
    TAO_ServantBase * servant,
    TAO_Operation_Details const * operation_details,
    TAO::Argument * const args[])
    : servant_ (servant)
    , operation_details_ (operation_details)
    , args_ (args)
  {}

  virtual void execute (void);

private:
  TAO_ServantBase * const servant_;
  TAO_Operation_Details const * const operation_details_;
  TAO::Argument * const * const args_;
};

/**
 * @class _non_existent_Upcall_Command
 *
 * @brief Base class for skeletons and servants.
 *
 *
 */
class _non_existent_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _non_existent_Upcall_Command (
    TAO_ServantBase * servant,
    TAO_Operation_Details const * operation_details,
    TAO::Argument * const args[])
    : servant_ (servant)
    , operation_details_ (operation_details)
    , args_ (args)
  {
  }

  virtual void execute (void);
private:
  TAO_ServantBase * const servant_;
  TAO_Operation_Details const * const operation_details_;
  TAO::Argument * const * const args_;
};

/**
 * @class _repository_id_Upcall_Command
 *
 * @brief Base class for skeletons and servants.
 *
 *
 */
class _repository_id_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _repository_id_Upcall_Command (
    TAO_ServantBase * servant,
    TAO_Operation_Details const * operation_details,
    TAO::Argument * const args[])
    : servant_ (servant)
    , operation_details_ (operation_details)
    , args_ (args)
  {
  }

  virtual void execute (void);

private:
  TAO_ServantBase * const servant_;
  TAO_Operation_Details const * const operation_details_;
  TAO::Argument * const * const args_;
};

/**
 * @class _get_component_Upcall_Command
 *
 * @brief Base class for skeletons and servants.
 *
 *
 */
class _get_component_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _get_component_Upcall_Command (
    TAO_ServantBase * servant,
    TAO_Operation_Details const * operation_details,
    TAO::Argument * const args[])
    : servant_ (servant)
    , operation_details_ (operation_details)
    , args_ (args)
  {
  }

  virtual void execute (void);

private:
  TAO_ServantBase * const servant_;
  TAO_Operation_Details const * const operation_details_;
  TAO::Argument * const * const args_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_SUPCALL_COMMANDS_H */
