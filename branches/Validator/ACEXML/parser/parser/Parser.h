// -*- C++ -*-

//=============================================================================
/**
 *  @file    Parser.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_BASIC_PARSER_H_
#define _ACEXML_BASIC_PARSER_H_

#include "ace/pre.h"
#include "ACEXML/parser/parser/Parser_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XMLReader.h"
#include "ACEXML/common/LocatorImpl.h"
#include "ACEXML/common/NamespaceSupport.h"
#include "ACEXML/common/CharStream.h"
#include "ace/Obstack.h"
#include "ace/Functor.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Containers_T.h"
#include "ace/Auto_Ptr.h"
#include "ACEXML/parser/parser/Entity_Manager.h"
#include "ACEXML/parser/parser/ParserErrors.h"

/**
 * @class ACEXML_Parser Parser.h "ACEXML/parser/parser/Parser.h"
 *
 * @brief A SAX based parser.
 *
 */
class ACEXML_PARSER_Export ACEXML_Parser : public ACEXML_XMLReader
{
public:
  /// Default constructor.
  ACEXML_Parser (void);

  /// Destructor.
  virtual ~ACEXML_Parser (void);

  /*
   * Return the current content handler.
   */
  virtual ACEXML_ContentHandler *getContentHandler (void) const;

  /*
   * Return the current DTD handler.
   */
  virtual ACEXML_DTDHandler *getDTDHandler (void) const;

  /*
   * Return the current entity resolver.
   */
  virtual ACEXML_EntityResolver *getEntityResolver (void) const;

  /*
   * Return the current error handler.
   */
  virtual ACEXML_ErrorHandler *getErrorHandler (void) const;

  /**
   * Look up the value of a feature.  This method allows
   * programmers to check whether a specific feature has been
   * activated in the parser.
   */
  virtual int getFeature (const ACEXML_Char *name ACEXML_ENV_ARG_DECL)
          ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
                           ACEXML_SAXNotSupportedException)) ;

  /**
   * Activating or deactivating a feature.
   */
  virtual void setFeature (const ACEXML_Char *name,
                           int boolean_value ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
                     ACEXML_SAXNotSupportedException)) ;

  /*
   * Look up the value of a property.
   */
  virtual void * getProperty (const ACEXML_Char *name ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
                     ACEXML_SAXNotSupportedException)) ;

  /*
   * Set the value of a property.
   */
  virtual void setProperty (const ACEXML_Char *name,
                            void *value ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
                     ACEXML_SAXNotSupportedException)) ;

  /*
   * Parse an XML document.
   */
  virtual void parse (ACEXML_InputSource *input ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException)) ;

  /*
   * Parse an XML document from a system identifier (URI).
   */
  virtual void parse (const ACEXML_Char *systemId ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Allow an application to register a content event handler.
   */
  virtual void setContentHandler (ACEXML_ContentHandler *handler);

  /*
   * Allow an application to register a DTD event handler.
   */
  virtual void setDTDHandler (ACEXML_DTDHandler *handler);

  /*
   * Allow an application to register an entity resolver.
   */
  virtual void setEntityResolver (ACEXML_EntityResolver *resolver);

  /*
   * Allow an application to register an error event handler.
   */
  virtual void setErrorHandler (ACEXML_ErrorHandler *handler);

  // *** Helper functions for parsing XML

  /**
   * Skip any whitespaces encountered until the first non-whitespace
   * character is encountered and consumed from the current input
   * CharStream.
   *
   * @param whitespace Return a pointer to the string of skipped
   * whitespace after proper conversion.  Null if there's no
   * whitespace found.
   *
   * @return The first none-white space characters (which will be
   * consumed from the CharStream.)  If no whitespace is found, it
   * returns 0.
   *
   * @sa skip_whitespace_count
   */
  ACEXML_Char skip_whitespace (ACEXML_Char **whitespace);

  /**
   * Skip any whitespaces encountered until the first non-whitespace
   * character.  The first non-whitespace character is not consumed.
   * This method does peek into the input CharStream and therefore
   * is more expensive than @ref skip_whitespace.
   *
   * @param peek If non-null, @a peek points to a ACEXML_Char where
   *        skip_whitespace_count stores the first non-whitespace
   *        character it sees (character is not removed from the stream.)
   *
   * @return The number of whitespace characters consumed.
   *
   * @sa skip_whitespace
   */
  int skip_whitespace_count (ACEXML_Char *peek = 0);

  /**
   * Check if a character @a c is a whitespace.
   *
   * @retval 1 if @a c is a valid white space character. 0 otherwise.
   */
  int is_whitespace (ACEXML_Char c);

  /**
   * Check if a character @a c is a whitespace or '='.
   *
   * @retval 1 if true, 0 otherwise.
   */
  int is_whitespace_or_equal (ACEXML_Char c);

  /**
   * Check if a character @a c is a valid character for nonterminal NAME.
   *
   * @retval 1 if true, 0 otherwise.
   */
  int is_nonname (ACEXML_Char c);

  /**
   * Skip an equal sign.
   *
   * @retval 0 when succeeds, -1 if no equal sign is found.
   */
  int skip_equal (void);

  /**
   * Get a quoted string.  Quoted strings are used to specify
   * attribute values and this routine will replace character and
   * entity references on-the-fly.  Parameter entities are not allowed
   * (or replaced) in this function.  (But regular entities are.)
   *
   * @param str returns the un-quoted string.
   *
   * @retval 0 on success, -1 otherwise.
   */
  int get_quoted_string (ACEXML_Char *&str);

  /**
   * Parse a PI statement.  The first character encountered
   * should always be '?' in the PI prefix "@<?".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_processing_instruction (ACEXML_ENV_SINGLE_ARG_DECL);

  /**
   * Skip over a comment. The first character encountered
   * should always be the first '-' in the comment prefix
   * "@<@!--".
   */
  int grok_comment ();

  /**
   * Read a name from the input CharStream (until white space).
   * If @a ch @!= 0, then we have already consumed the first name
   * character from the input CharStream, otherwise, read_name
   * will use this->get() to acquire the initial character.
   *
   * @return A pointer to the string in the obstack, 0 if it's not
   * a valid name.
   */
  ACEXML_Char *read_name (ACEXML_Char ch = 0);

  /**
   * Parse the DOCTYPE declaration.  The first character encountered
   * should always be  'D' in doctype prefix: "@<@!DOCTYPE".
   */
  int parse_doctypedecl (ACEXML_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Parse an XML element.  The first character encountered should
   * be the first character of the element "Name".
   *
   * @param is_root If not 0, then we are expecting to see the "root"
   * element now, and the next element's name need to match the name
   * defined in DOCTYPE definition, i.e., @a this->doctype_.
   *
   * @todo Instead of simply checking for the root element based on the
   * argument @a is_root, we should instead either pass in some sort
   * of validator or allow the function to return the element name so it
   * can be used in a validator.
   */
  void parse_element (int is_root ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Parse XML Prolog.
   */
  void parse_xml_prolog (ACEXML_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));


  /**
   * Parse a character reference, i.e., "&#x20;" or "&#30;".   The first
   * character encountered should be the '#' char.
   *
   * @param buf points to a character buffer for the result.
   * @param len specifies the capacities of the buffer.
   *
   * @retval 0 on success and -1 otherwise.
   */
  int parse_char_reference (ACEXML_Char *buf, size_t len);

  /**
   * Parse an entity reference, i.e., "&amp;".  The first character
   * encountered should be the character following '&'.
   *
   * @return A pointer to the resolved const ACEXML_String if success
   * (previously defined), 0 otherwise.
   */
  const ACEXML_String *parse_reference (void);

  /**
   * Parse a CDATA section.  The first character should always be the first
   * '[' in CDATA definition.
   *
   * @retval 0 on success.
   * @retval -1 if fail.
   */
  int parse_cdata (ACEXML_ENV_SINGLE_ARG_DECL);

  /**
   * Parse a "markupdecl" section, this includes both "markupdecl" and
   * "DeclSep" sections in XML specification
   */
  int parse_internal_dtd (ACEXML_ENV_SINGLE_ARG_DECL);

  /**
   * Parse an "ELEMENT" decl.  The first character this method
   * expects is always the 'L' (the second char) in the word
   * "ELEMENT".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_element_decl (ACEXML_ENV_SINGLE_ARG_DECL);

  /**
   * Parse an "ENTITY" decl.  The first character this method expects
   * is always the 'N' (the second char) in the word "ENTITY".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_entity_decl (ACEXML_ENV_SINGLE_ARG_DECL);

  /**
   * Parse an "ATTLIST" decl.  Thse first character this method
   * expects is always the 'A' (the first char) in the word
   * "ATTLIST".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_attlist_decl (ACEXML_ENV_SINGLE_ARG_DECL);

  /**
   *Parse a "NOTATION" decl.  The first character this method
   * expects is always the 'N' (the first char) in the word
   * "NOTATION".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_notation_decl (ACEXML_ENV_SINGLE_ARG_DECL);

  /**
   * Parse an ExternalID or a reference to PUBLIC ExternalID.
   * Possible cases are in the forms of: <code>
   *
   * SYSTEM 'quoted string representing system resource'
   * PUBLIC 'quoted name of public ID' 'quoted resource'
   * PUBLIC 'quoted name we are referring to'
   * </code>
   *
   * The first character this function sees must be either 'S' or 'P'.
   * When the function finishes parsing, the input stream points
   * at the first non-whitespace character.
   *
   * @param publicId returns the unquoted publicId read.  If none
   *        is available, it will be reset to 0.
   * @param systemId returns the unquoted systemId read.  If none
   *        is available, it will be reset to 0.
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_external_id_and_ref (ACEXML_Char *&publicId,
                                 ACEXML_Char *&systemId ACEXML_ENV_ARG_DECL);

  /**
   * Parse the "children" and "Mixed" non-terminals in contentspec.
   *
   * The first character this function sees must be the first
   * open paren '(' in children.
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_children_definition (ACEXML_ENV_SINGLE_ARG_DECL);

  /**
   * Parse a @c cp non-terminal.  @c cp can either be a @c seq or a @c choice.
   * This function calls itself recursively.
   *
   * @param skip_open_paren when non-zero, it indicates that the open paren of
   *        the @c seq or @c choice has already been removed from the input
   *        stream.
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_child (int skip_open_paren ACEXML_ENV_ARG_DECL);

protected:
  /// Get a character.
  ACEXML_Char get (void);

  /// Peek a character.
  ACEXML_Char peek (void);

  // Feature names:

  /**
   * \addtogroup acexml_parser_features
   * @{
   */

  /**
   * @var simple_parsing_feature_
   *
   * This constant string defines the name of "simple XML parsing"
   * feature.  When this feature is enabled, ACEXML parser is allowed
   * to parse a simple XML stream without mandated XML prolog
   * and no DTD defintion.
   */
  static const ACEXML_Char simple_parsing_feature_[];

  /**
   * @var namespaces_feature_
   *
   * This constant string defines the SAX XML Namespace feature. When this
   * feature is enabled, ACEXML parser allows access by namespace qualified
   * names.
   */
  static const ACEXML_Char namespaces_feature_[];

  /**
   *  @var namespace_prefixes_feature_
   *
   *  This constant string defines the SAX XML Namespace prefixes feature.
   *  Normally the list of attributes returned by the parser will not
   *  contain attributes used as namespace declarations (xmlns*). When this
   *  feature is enabled, the list of attributes contains the namespace
   *  declarations also.
   */
  static const ACEXML_Char namespace_prefixes_feature_[];

  /* @} */

private:
  /**
   * Dispatch errors to ErrorHandler.
   *
   */
  void report_error (ACEXML_Error minor_code ACEXML_ENV_ARG_DECL);

  /**
   * Dispatch warnings to ErrorHandler.
   *
   */
  void report_warning (ACEXML_Error minor_code ACEXML_ENV_ARG_DECL);

  /**
   * Dispatch fatal errors to ErrorHandler.
   *
   */
  void report_fatal_error (ACEXML_Error minor_code ACEXML_ENV_ARG_DECL);

  /**
   * Dispatch prefix mapping calls to the ContentHandler.
   *
   * @param prefix Namespace prefix
   * @param uri Namespace URI
   * @param name Local name
   * @param start 1 => startPrefixMapping 0 => endPrefixMapping
   */
  void report_prefix_mapping (const ACEXML_Char* prefix,
                              const ACEXML_Char* uri,
                              const ACEXML_Char* name,
                              int start ACEXML_ENV_ARG_DECL);
  /**
   *  Parse a keyword.
   */
  int parse_token (const ACEXML_Char* keyword);

  /// Keeping track of the handlers. We do not manage the memory for
  /// handlers.
  ACEXML_DTDHandler *dtd_handler_;
  ACEXML_EntityResolver *entity_resolver_;
  ACEXML_ContentHandler *content_handler_;
  ACEXML_ErrorHandler *error_handler_;

  /// @@ Feature and properties management structure here.
  /// Current input char stream.
  ACEXML_CharStream *instream_;

  /// My doctype, if any.
  ACEXML_Char *doctype_;

  /// External DTD System Literal, if any.
  ACEXML_Char *dtd_system_;

  /// External DTD Public Literal, if any.
  ACEXML_Char *dtd_public_;

  ACE_Obstack_T<ACEXML_Char> obstack_;

  ACEXML_NamespaceSupport xml_namespace_;

  ACEXML_Entity_Manager entities_;

  // Locator
  ACEXML_LocatorImpl locator_;

  // Flag set if the document is a standalone XML document
  int standalone_;

  // Feature flags &
  int simple_parsing_;
  int namespaces_;
  int namespace_prefixes_;

};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/Parser.i"
#endif /* __ACEXML_INLINE__ */

#include "ace/post.h"

#endif /* _ACEXML_BASIC_PARSER_H_ */
