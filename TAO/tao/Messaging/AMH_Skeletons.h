// -*- C++ -*-

//=============================================================================
/**
 *  @file    AMH_Skeletons.h
 *
 *  $Id$
 *
 *  @author  Marcel Smit <msmit@remedy.nl>
 *
 *  @brief Upcall command exection classes for static AMH skeleton methods
 *
 * This file contains the classes needed to perform an upcall command for
 * the following static AMH methods.
 * - _is_a_skel
 * - _non_existent_skel
 * - _component_skel
 * - _repository_id_skel
 * - _interface_skel
 * These are the same methods as in PortableServer but placed here since
 * these methods do contain an invocation to tao_send_reply.
 * Since we don't want to bother the regular core with AMH stuff,
 * we put these methods and classes here.
 *
 */
//=============================================================================

#ifndef AMH_SKELETONS_H
#define AMH_SKELETONS_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/Messaging/messaging_export.h"

#include "tao/PortableServer/Upcall_Command.h"
#include "tao/PortableServer/Upcall_Wrapper.h"
#include "tao/operation_details.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ServerRequest;
class TAO_ServantBase;

/**
 * @class _is_a_amh_Upcall_Command
 *
 * @brief Execution class for _is_a_skel.
 *
 *
 */
class _is_a_amh_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _is_a_amh_Upcall_Command (
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
 * @class _non_existent_amh_Upcall_Command
 *
 * @brief Execution class for _non_existent_skel.
 *
 *
 */
#if (TAO_HAS_MINIMUM_CORBA == 0)
class _non_existent_amh_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _non_existent_amh_Upcall_Command (
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

# if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
/**
 * @class _get_component_amh_Upcall_Command
 *
 * @brief Execution class for _component_skel.
 *
 *
 */
class _get_component_amh_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _get_component_amh_Upcall_Command (
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
# endif /* !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */

/**
 * @class _repository_id_amh_Upcall_Command
 *
 * @brief Execution class for _repository_id_skel
 *
 *
 */
class _repository_id_amh_Upcall_Command
  : public TAO::Upcall_Command
{
public:
  inline _repository_id_amh_Upcall_Command (
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
#endif /* TAO_HAS_MINIMUM_CORBA */


class TAO_Messaging_Export TAO_AMH_Skeletons
{
public:
  static void _is_a_amh_skel (TAO_ServerRequest & server_request,
                              TAO::Portable_Server::Servant_Upcall* servant_upcall,
                              void * servant);

#if (TAO_HAS_MINIMUM_CORBA == 0)
  static void _non_existent_amh_skel (TAO_ServerRequest & server_request,
                                      TAO::Portable_Server::Servant_Upcall* servant_upcall,
                                      void * servant);

# if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  static void _interface_amh_skel (TAO_ServerRequest & server_request,
                                  TAO::Portable_Server::Servant_Upcall* /* servant_upcall */,
                                  void * servant);

  static void _component_amh_skel (TAO_ServerRequest & server_request,
                                  TAO::Portable_Server::Servant_Upcall* servant_upcall,
                                  void * servant);
# endif /* !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */
  static void _repository_id_amh_skel (TAO_ServerRequest & server_request,
                                      TAO::Portable_Server::Servant_Upcall* servant_upcall,
                                      void * servant);
#endif /* TAO_HAS_MINIMUM_CORBA */
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* AMH_SKELETONS_H */
