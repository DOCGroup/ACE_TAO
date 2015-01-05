// -*- C++ -*-

//=============================================================================
/**
 *  @file    NamespaceSupport.h
 *
 *  $Id$
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACEXML_NAMESPACESUPPORT_H
#define ACEXML_NAMESPACESUPPORT_H

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"
#include "ace/Functor.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Containers_T.h"
#include "ace/Null_Mutex.h"


typedef ACE_Hash_Map_Entry<ACEXML_String,
                           ACEXML_String> ACEXML_NS_CONTEXT_ENTRY;

typedef ACE_Hash_Map_Manager_Ex<ACEXML_String,
                                ACEXML_String,
                                ACE_Hash<ACEXML_String>,
                                ACE_Equal_To<ACEXML_String>,
                                ACE_Null_Mutex> ACEXML_NS_CONTEXT;

typedef ACE_Hash_Map_Iterator_Ex<ACEXML_String,
                                 ACEXML_String,
                                 ACE_Hash<ACEXML_String>,
                                 ACE_Equal_To<ACEXML_String>,
                                 ACE_Null_Mutex> ACEXML_NS_CONTEXT_ITER;

typedef ACE_Hash_Map_Reverse_Iterator_Ex<ACEXML_String,
                                         ACEXML_String,
                                         ACE_Hash<ACEXML_String>,
                                         ACE_Equal_To<ACEXML_String>,
                                         ACE_Null_Mutex> ACEXML_NS_CONTEXT_REVERSE_ITER;

typedef ACE_Unbounded_Queue<const ACEXML_Char *> ACEXML_STR_LIST;

/**
 * @class ACEXML_Namespace_Context_Stack
 *
 * @brief ACEXML_Namespace_Context_Stack implements a simple stack
 * that ACEXML_NamespaceSupport uses to keep track of namespace scopes.
 *
 * @sa ACEXML_NamespaceSupport
 */
class ACEXML_Export ACEXML_Namespace_Context_Stack
{
public:
  /// Default constructor.
  ACEXML_Namespace_Context_Stack (void);

  /// Destructor.
  ~ACEXML_Namespace_Context_Stack (void);

  /// Push the old namespace before entering into a new namespace scope.
  int push (ACEXML_NS_CONTEXT * old);

  /// Pop the old namespace when exiting a namespace scope.
  ACEXML_NS_CONTEXT *pop (void);

private:
  /// Internal stack structure to hold namespace context.
  ACE_Unbounded_Stack<ACEXML_NS_CONTEXT*> stack_;
};

/**
 * @class ACEXML_NamespaceSupport NamespaceSupport.h "ACEXML/common/NamespaceSupport.h"
 *
 * @brief ACEXML_NamespaceSupport provides namespace management
 * operation for an XML parser.
 *
 * This class encapsulates the logic of Namespace processing: it
 * tracks the declarations currently in force for each context and
 * automatically processes qualified XML 1.0 names into their
 * Namespace parts; it can also be used in reverse for generating XML
 * 1.0 from Namespaces.
 *
 * Namespace support objects are reusable, but the reset method must
 * be invoked between each session.
 *
 * Here is a simple session (in Java :-p):
 * @code
 *  String parts[] = new String[3];
 *  NamespaceSupport support = new NamespaceSupport();
 *
 *  support.pushContext();
 *  support.declarePrefix("", "http://www.w3.org/1999/xhtml");
 *  support.declarePrefix("dc", "http://www.purl.org/dc#");
 *
 *  String parts[] = support.processName("p", parts, false);
 *  System.out.println("Namespace URI: " + parts[0]);
 *  System.out.println("Local name: " + parts[1]);
 *  System.out.println("Raw name: " + parts[2]);
 *
 *  String parts[] = support.processName("dc:title", parts, false);
 *  System.out.println("Namespace URI: " + parts[0]);
 *  System.out.println("Local name: " + parts[1]);
 *  System.out.println("Raw name: " + parts[2]);
 *
 *  support.popContext();
 * @endcode
 *
 * Note that this class is optimized for the use case where most
 * elements do not contain Namespace declarations: if the same
 * prefix/URI mapping is repeated for each context (for example), this
 * class will be somewhat less efficient.
 *
 * @sa ACEXML_Exception
 */
class ACEXML_Export ACEXML_NamespaceSupport
{
public:
  /**
   * Default constructor.
   */
  ACEXML_NamespaceSupport (void);

  /**
   * Default destructor.
   */
  ~ACEXML_NamespaceSupport (void);

  /**
   *  Initialize the namespace support object
   */
  int init(void);

  /**
   * XMLNS default prefix and URI strings.
   */
  static const ACEXML_Char *XMLNS_PREFIX;
  static const ACEXML_Char *XMLNS;

  /**
   * Declare a Namespace prefix.  Return -1 if the prefix was illegal
   * or an internal error occured.  Return 0 if the prefix gets declared
   * successfully, 1 if the prefix replaces an existing prefix definition.
   */
  int declarePrefix (const ACEXML_Char *prefix,
                     const ACEXML_Char *uri);

  /**
   * Return all prefixes declared in current context in
   * the user-supplied list @a prefixes.  It is user's reponsibility
   * to ensure the list was empty originally.
   */
  int getDeclaredPrefixes (ACEXML_STR_LIST &prefixes) const;

  /**
   * Return one of the prefixes mapped to a Namespace URI.
   */
  const ACEXML_Char *getPrefix (const ACEXML_Char *uri) const;

  /**
   * Return all prefixes currently declared in the user-supplied list.
   * @@ Known bug: This function should only return user-defined prefixes.
   */
  int getPrefixes (ACEXML_STR_LIST &prefixes) const;

  /**
   * Return all prefixes currently declared for a URI in the
   * user-supplied list.
   */
  int getPrefixes (const ACEXML_Char *uri,
                   ACEXML_STR_LIST &prefixes) const;

  /**
   * Look up a prefix and get the currently-mapped Namespace URI.
   */
  const ACEXML_Char *getURI (const ACEXML_Char *prefix) const;

  /**
   * Revert to the previous namespace context.
   */
  int popContext (void);

  /**
   * Process a raw XML 1.0 name.
   * @a qName is the raw XML name we want to parse,
   * @a uri contains the URI string of the raw name.  It points to a null
   *    string if the namespace is not valid or there's no namespace defined.
   * @a name contains the original name without the prefix.
   * @a is_attribute specifies whether the name is an attribute or not.
   *    Attributes have different scoping rules from elements.
   */
  int processName (const ACEXML_Char *qName,
                   const ACEXML_Char *&uri,
                   const ACEXML_Char *&name,
                   int is_attribute) const;

  /**
   * Start a new Namespace context.  Prefixes defined in previous
   * context are copied over to the new context.
   */
  int pushContext (void);

  /**
   * Reset this Namespace support object for reuse.
   *
   */
  int reset (void);

private:
  /**
   * Namespace Context stack.  When we entering a new namespace
   * context, the old context is duplicated and pushed into
   * this stack.
   */
  ACEXML_Namespace_Context_Stack ns_stack_;

  /**
   * The effective namespace context.
   */
  ACEXML_NS_CONTEXT *effective_context_;
};

#include /**/ "ace/post.h"

#endif /* ACEXML_NAMESPACESUPPORT_H */
