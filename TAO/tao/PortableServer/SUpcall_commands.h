// -*- C++ -*-

//=============================================================================
/**
 *  @file    SUpcall_commands.h
 *
 *  $Id$
 *
 *  @author  Marcel Smit <msmit@remedy.nl>
 *
 * @brief Upcall command exection classes for static skeleton methods
 *
 * This file contains the classes needed to perform an upcall command for
 * the following static methods, defined in Servant_Base.h
 * - _is_a_skel
 * - _non_existent_skel
 * - _component_skel
 * - _repository_id_skel
 * - _interface_skel
 *
 */
//=============================================================================

#ifndef TAO_SUPCALL_COMMANDS_H
#define TAO_SUPCALL_COMMANDS_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Upcall_Command.h"
#include "tao/PortableServer/Upcall_Wrapper.h"
#include "tao/operation_details.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class _is_a_thru_poa_Upcall_Command
 *
 * @brief Execution class for _is_a_skel.
 *
 *
 */
class _is_a_thru_poa_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _is_a_thru_poa_Upcall_Command (
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
 * @class _is_a_Upcall_Command
 *
 * @brief Execution class for _is_a_skel when through POA option is used.
 *
 *
 */
class _is_a_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _is_a_Upcall_Command (
    TAO_ServantBase * servant,
    TAO::Argument * const args[])
    : servant_ (servant)
    , args_ (args)
  {
  }

  virtual void execute (void);

private:
  TAO_ServantBase * const servant_;
  TAO::Argument * const * const args_;
};


/**
 * @class _non_existent_thru_poa_Upcall_Command
 *
 * @brief Execution class for _non_existent_skel.
 *
 *
 */
#if (TAO_HAS_MINIMUM_CORBA == 0)
class _non_existent_thru_poa_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _non_existent_thru_poa_Upcall_Command (
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
 * @class _non_existent_Upcall_Command
 *
 * @brief Execution class for _non_existent_skel when through POA option is used.
 *
 *
 */
class _non_existent_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _non_existent_Upcall_Command (
    TAO_ServantBase * servant,
    TAO::Argument * const args[])
    : servant_ (servant)
    , args_ (args)
  {
  }

  virtual void execute (void);

private:
  TAO_ServantBase * const servant_;
  TAO::Argument * const * const args_;
};


# if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
/**
 * @class _get_component_thru_poa_Upcall_Command
 *
 * @brief Execution class for _component_skel.
 *
 *
 */
class _get_component_thru_poa_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _get_component_thru_poa_Upcall_Command (
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
 * @brief Execution class for _get_component when through POA option is used.
 *
 *
 */
class _get_component_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _get_component_Upcall_Command (
    TAO_ServantBase * servant,
    TAO::Argument * const args[])
    : servant_ (servant)
      , args_ (args)
  {
  }

  virtual void execute (void);

private:
  TAO_ServantBase * const servant_;
  TAO::Argument * const * const args_;
};
# endif /* !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

/**
 * @class _repository_id_thru_poa_Upcall_Command
 *
 * @brief Execution class for _repository_id_skel
 *
 *
 */
class _repository_id_thru_poa_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _repository_id_thru_poa_Upcall_Command (
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
 * @brief Execution class for _repository_id when through POA option is used.
 *
 *
 */
class _repository_id_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _repository_id_Upcall_Command (
    TAO_ServantBase * servant,
    TAO::Argument * const args[])
    : servant_ (servant)
    , args_ (args)
  {
  }

  virtual void execute (void);

private:
  TAO_ServantBase * const servant_;
  TAO::Argument * const * const args_;
};

#endif /* TAO_HAS_MINIMUM_CORBA */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_SUPCALL_COMMANDS_H */
