// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    DIOP_Connector.h
 *
 *  $Id$
 *
 *  DIOP specific connector processing
 *
 *
 *  @author Fred Kuhns <fredk@cs.wustl.edu>
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DIOP_CONNECTOR_H
#define TAO_DIOP_CONNECTOR_H
#include "ace/pre.h"

#include "ace/Connector.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Connector.h"
#include "tao/Pluggable.h"
#include "tao/Connector_Impl.h"
#include "DIOP_Connection_Handler.h"
#include "diop_export.h"

// ****************************************************************

/**
 * @class TAO_DIOP_Connector
 *
 * @brief DIOP-specific Connector bridge for pluggable protocols.
 *
 * Concrete instance of the TAO_Connector class.  Responsible
 * for establishing a connection with a server and is called from the
 * Connector_Registory.
 */
class TAO_DIOP_Export TAO_DIOP_Connector : public TAO_Connector
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_DIOP_Connector (CORBA::Boolean flag = 0);

  /// Destructor.
  ~TAO_DIOP_Connector (void);

  // = The TAO_Connector methods, please check the documentation on
  // Pluggable.h
  int open (TAO_ORB_Core *orb_core);
  int close (void);
  int connect (TAO_Transport_Descriptor_Interface *desc,
               TAO_Transport *&transport,
               ACE_Time_Value *max_wait_time,
               CORBA::Environment &ACE_TRY_ENV);
  int preconnect (const char *preconnections);
  TAO_Profile *create_profile (TAO_InputCDR& cdr);

  virtual int check_prefix (const char *endpoint);

  virtual char object_key_delimiter (void) const;

protected:

  /// = More TAO_Connector methods, please check the documentation on
  ///   Pluggable.h
  virtual void make_profile (const char *endpoint,
                             TAO_Profile *&,
                             CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());

  /// Obtain tcp properties that must be used by this connector, i.e.,
  /// initialize <tcp_properties_>.
  int init_tcp_properties (void);

public:

  typedef TAO_Connect_Concurrency_Strategy<TAO_DIOP_Connection_Handler>
          TAO_DIOP_CONNECT_CONCURRENCY_STRATEGY;

  typedef TAO_Connect_Creation_Strategy<TAO_DIOP_Connection_Handler>
          TAO_DIOP_CONNECT_CREATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_DIOP_Connection_Handler,
                               ACE_SOCK_CONNECTOR>
          TAO_DIOP_CONNECT_STRATEGY ;

  typedef ACE_Strategy_Connector<TAO_DIOP_Connection_Handler,
                                 ACE_SOCK_CONNECTOR>
          TAO_DIOP_BASE_CONNECTOR;

protected:

  /// TCP configuration properties to be used for all
  /// connections established by this connector.
  TAO_DIOP_Properties tcp_properties_;

  /// Do we need to use a GIOP_Lite for sending messages?
  CORBA::Boolean lite_flag_;

private:
  // @@ Michael: UDP Addition
  ACE_Hash_Map_Manager_Ex < ACE_TCHAR *,
                            TAO_DIOP_Connection_Handler *,
                            ACE_Hash < const char * >,
                            ACE_Equal_To < const char * >,
                            ACE_Null_Mutex > svc_handler_table_;

  typedef ACE_Hash_Map_Iterator_Ex < ACE_TCHAR *,
                                     TAO_DIOP_Connection_Handler *,
                                     ACE_Hash < const char * >,
                                     ACE_Equal_To < const char *>,
                                     ACE_Null_Mutex > SvcHandlerIterator;
};

#include "ace/post.h"
#endif  /* TAO_DIOP_CONNECTOR_H */
