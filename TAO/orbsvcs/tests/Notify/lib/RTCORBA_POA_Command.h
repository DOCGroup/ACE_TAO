/* -*- C++ -*- */
/**
 *  @file RTCORBA_POA_Command.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_POA_COMMAND_H
#define TAO_NS_RTCORBA_POA_COMMAND_H
#include "ace/pre.h"

#include "rtcorba_notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/PortableServer/PortableServer.h"
#include "Command.h"

/**
 * @class TAO_NS_RTCORBA_POA_Command
 *
 * @brief Command for managing RTCORBA POAs.
 *
 */
class TAO_RTCORBA_NOTIFY_TEST_Export TAO_NS_RTCORBA_POA_Command : public TAO_NS_Command
{
public:
  /// Constuctor
  TAO_NS_RTCORBA_POA_Command (void);

  /// Destructor
  ~TAO_NS_RTCORBA_POA_Command ();

  /// Parse args and populate options.
  /// The arg_shifter options are read in the following manner: -Create <name> -PriorityModel <CLIENT|SERVER> <priority> -Lanes <count> (-Lane <priority> ,<static_threads> <dynamic_threads>)* -Bands <count> (-Band <low> <high>)*
  virtual void init (ACE_Arg_Shifter& arg_shifter);

  /// Execute the command.
  virtual void execute_i (ACE_ENV_SINGLE_ARG_DECL);

  /// Get the command name.
  virtual const char* get_name (void);
  static const char* name (void);

protected:
  /// Create the POA with the params parsed.
  void create (RTCORBA::RTORB_ptr rt_orb, PortableServer::POA_ptr parent_poa ACE_ENV_ARG_DECL);

  enum COMMAND
  {
    CREATE,
    DESTROY,
    DUMP_STATE
  };

  COMMAND command_;

  ACE_CString POA_name_;

  RTCORBA::PriorityModel priority_model_;
  RTCORBA::Priority server_priority_;
  RTCORBA::ThreadpoolLanes lanes_;
  RTCORBA::PriorityBands bands_;

  CORBA::ULong thread_pool_static_threads_;
  RTCORBA::Priority thread_pool_default_priority_;
};

#if defined (__ACE_INLINE__)
#include "RTCORBA_POA_Command.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_POA_COMMAND_H */
