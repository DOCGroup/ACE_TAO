// -*- C++ -*-  $Id$

#ifndef ACEXML_NAMESPACESUPPORT_H
#define ACEXML_NAMESPACESUPPORT_H

#include "Common/XML_Types.h"
#include "ace/Functor.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Containers_T.h"

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

class ACEXML_Export ACEXML_Namespace_Context_Stack
{
public:
  ACEXML_Namespace_Context_Stack (void);
  ~ACEXML_Namespace_Context_Stack (void);

  int push (ACEXML_NS_CONTEXT * old);
  ACEXML_NS_CONTEXT *pop (void);

private:
  struct NS_Node_T {
    ACEXML_NS_CONTEXT *item_;
    struct NS_Node_T *next_;
  };

  NS_Node_T *head_;
};

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
   * @todo Not implemented.
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


#if defined (__ACEXML_INLINE__)
# include "Common/NamespaceSupport.i"
#endif /* __ACEXML_INLINE__ */
#endif /* ACEXML_NAMESPACESUPPORT_H */
