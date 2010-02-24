// -*- C++ -*-

//=============================================================================
/**
 *  @file   DLL_Parser.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================

#ifndef TAO_DLL_PARSER_H
#define TAO_DLL_PARSER_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IOR_Parser.h"

#if (TAO_HAS_DDL_PARSER == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DLL_Parser
 *
 * @brief Implment the parser for the DLL-style IORs.
 *
 * The DLL-style IORs allows applications to transparently load object
 * implementations. The ORB can parse an IOR in the format:
 *
 * DLL:Service_Name
 *
 * the string_to_object() function will use ACE's Service Configurator
 * framework to dynamically load the service named 'Service_Name'
 * using all the normal platform-independent naming conventions and
 * environment variable lookup rules that are normally used by @c
 * ACE_DLL::open().  The ORB assumes that this service implements the
 * TAO_Object_Loader interface, and uses that interface to create a
 * new object implementation locally. The object reference for this
 * local object is returned to the application.
 *
 * This can be used in applications that sometimes require local
 * object while other times they may use a remote implementation.
 * For example, the application could be configured to use a remote
 * Event Service or to dynamically load an Event Service
 * implementation and use the local copy instead.  The local Event
 * Service would federate to its remote peers to work as-if a single
 * Event Service was in place.
 *
 * Such an application could be written as follows:
 *
 * int main (int argc, char* argv)
 * {
 *   CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
 *
 *   CORBA::Object_var object =
 *     orb->resolve_initial_references("EventService");
 *   // Use <object> here...
 * }
 *
 * if the application is invoked using:
 *
 * $ program -ORBInitRef EventService=IOR:....
 *
 * then a remote event service is used, but the application could also
 * be invoked using:
 *
 * $ program -ORBInitRef EventService=DLL:Event_Service_Loader
 *
 * In this case the Event_Service implementation will be dynamically
 * loaded without any modifications to the application code.
 *
 */
class TAO_DLL_Parser : public TAO_IOR_Parser
{
public:

  /// The destructor
  virtual ~TAO_DLL_Parser (void);

  // = The IOR_Parser methods, please read the documentation in
  //   IOR_Parser.h
  virtual bool match_prefix (const char *ior_string) const;
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb);
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_DLL_Parser)
ACE_FACTORY_DECLARE (TAO, TAO_DLL_Parser)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DDL_PARSER == 1 */

#include /**/ "ace/post.h"
#endif /* TAO_DLL_PARSER_H */
