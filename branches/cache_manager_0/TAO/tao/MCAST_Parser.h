// -*- C++ -*-

//=============================================================================
/**
 *  @file   MCAST_Parser.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla (gontla_p@ociweb.com)
 */
//=============================================================================


#ifndef TAO_MCAST_PARSER_H
#define TAO_MCAST_PARSER_H
#include "ace/pre.h"

#include "tao/IOR_Parser.h"
#include "tao/ORB_Core.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

/**
 * @class TAO_MCAST_Parser
 *
 * @brief Implements the <mcast:> IOR format
 *
 * This class implements the <mcast:> IOR format.
 * It is dynamically loaded by the ORB and used to parse the
 * string to separate the individual <obj_addr> from the list of object
 * addresses <obj_addr_list>.
 */
class TAO_Export TAO_MCAST_Parser : public TAO_IOR_Parser
{
public:
  /// Constructor
  TAO_MCAST_Parser (void);

  /// The destructor
  virtual ~TAO_MCAST_Parser (void);

  /// = The IOR_Parser methods, please read the documentation in
  ///   IOR_Parser.h
  virtual int match_prefix (const char *ior_string) const;

  /// Parse the ior-string that is passed.
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb
                                          TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  CORBA_Object_ptr multicast_to_service (const char *service_name,
                                         CORBA::UShort port,
                                         const char *mcast_address,
                                         const char *mcast_ttl,
                                         const char *mcast_nic,
                                         CORBA::ORB_ptr orb,
                                         ACE_Time_Value *timeout
                                         TAO_ENV_ARG_DECL_NOT_USED);

  int multicast_query (char *&buf,
                       const char *service_name,
                       u_short port,
                       const char *mcast_address,
                       const char *mcast_ttl,
                       const char *mcast_nic,
                       ACE_Time_Value *timeout,
                       CORBA::ORB_ptr orb);

  /* Simple method to assign values to the global members:
     mcast_address_, mcast_port_, mcast_nic_, mcast_ttl_ */
  void assign_to_variables (const char * &mcast_name_ptr);

  // Global private variables
  CORBA::String_var mcast_address_;
  CORBA::String_var mcast_port_;
  CORBA::String_var mcast_nic_;
  CORBA::String_var mcast_ttl_;
  CORBA::String_var service_name_;
};

#if defined (__ACE_INLINE__)
# include "MCAST_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_MCAST_Parser)
ACE_FACTORY_DECLARE (TAO, TAO_MCAST_Parser)

#include "ace/post.h"
#endif /* TAO_MCAST_PARSER_H */
