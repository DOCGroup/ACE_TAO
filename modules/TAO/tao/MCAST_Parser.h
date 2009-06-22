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

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CORBA_String.h"
#include "tao/IOR_Parser.h"

#if (TAO_HAS_MCAST_PARSER == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_MCAST_Parser
 *
 * @brief Implements the @c mcast: IOR format
 *
 * This class implements the @c mcast: IOR format.
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
  virtual bool match_prefix (const char *ior_string) const;

  /// Parse the ior-string that is passed.
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb);

private:

  CORBA::Object_ptr multicast_to_service (const char *service_name,
                                          unsigned short port,
                                          const char *mcast_address,
                                          int mcast_ttl,
                                          const char *mcast_nic,
                                          CORBA::ORB_ptr orb,
                                          ACE_Time_Value *timeout);

  int multicast_query (char *&buf,
                       const char *service_name,
                       unsigned short port,
                       const char *mcast_address,
                       int mcast_ttl,
                       const char *mcast_nic,
                       ACE_Time_Value *timeout,
                       CORBA::ORB_ptr orb);

  /* Simple method to assign values to the global members:
     mcast_address_, mcast_port_, mcast_nic_, mcast_ttl_ */
  void assign_to_variables (char const * mcast_name_ptr);

private:

  CORBA::String_var mcast_address_;

  /// Default multicast port (currently Name Service mcast port).
  unsigned short    mcast_port_;

  /// Multicast network interface card.
  CORBA::String_var mcast_nic_;

  /// Default time-to-live (default is 1).
  int               mcast_ttl_;

  /// Multicast service name
  CORBA::String_var service_name_;

};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_MCAST_Parser)
ACE_FACTORY_DECLARE (TAO, TAO_MCAST_Parser)

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/MCAST_Parser.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_MCAST_PARSER == 1 */

#include /**/ "ace/post.h"
#endif /* TAO_MCAST_PARSER_H */
