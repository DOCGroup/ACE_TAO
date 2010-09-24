// -*- C++ -*-

//=============================================================================
/**
 *  @file    Parser.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_BASIC_PARSER_H_
#define _ACEXML_BASIC_PARSER_H_

#include /**/ "ace/pre.h"
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
#include "ace/Unbounded_Set.h"
#include "ace/Containers_T.h"
#include "ace/Auto_Ptr.h"
#include "ACEXML/parser/parser/Entity_Manager.h"
#include "ACEXML/parser/parser/ParserInternals.h"
#include "ACEXML/parser/parser/ParserContext.h"

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

  /**
   *  Initialize the parser state.
   *
   *  @retval 0 if parser was initialized correctly else -1.
   */
  int initialize (ACEXML_InputSource* input);

   /**
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
  virtual int getFeature (const ACEXML_Char *name);

  /**
   * Activating or deactivating a feature.
   */
  virtual void setFeature (const ACEXML_Char *name,
                           int boolean_value);

  /*
   * Look up the value of a property.
   */
  virtual void * getProperty (const ACEXML_Char *name);

  /*
   * Set the value of a property.
   */
  virtual void setProperty (const ACEXML_Char *name,
                            void *value);

  /*
   * Parse an XML document.
   */
  virtual void parse (ACEXML_InputSource *input);

  /*
   * Parse an XML document from a system identifier (URI).
   */
  virtual void parse (const ACEXML_Char *systemId)
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



protected:
  /**
   * Parse XML Prolog.
   */
  void parse_xml_prolog (void);

  /**
   * Parse VersionInfo declaration.
   *
   */
  void parse_version_info (void);

  /**
   *  Parse a EncodingDecl declaration.
   *
   */
  void parse_encoding_decl (void);

  /**
   *  Parse a XMLDecl declaration.
   *
   */
  void parse_xml_decl (void);

  /**
   *  Parse a TextDecl declaration.
   */
  int parse_text_decl (void);

  /**
   * Parse a PI statement.  The first character encountered
   * should always be '?' in the PI prefix "@<?".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_processing_instruction (void);

  /**
   * Parse the DOCTYPE declaration.  The first character encountered
   * should always be  'D' in doctype prefix: "@<@!DOCTYPE".
   */
  int parse_doctypedecl (void);

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
  void parse_element (int is_root);

  /**
   *  Parse a content declaration.
   *
   */
  int parse_content (const ACEXML_Char* startname, const ACEXML_Char*& ns_uri,
                     const ACEXML_Char*& ns_lname, int ns_flag);

  /**
   * Parse a character reference, i.e., "&#x20;" or "&#30;".   The first
   * character encountered should be the '#' char.
   *
   * @param buf points to a character buffer for the result.
   *
   * @param len In/out argument which initially specifies the size of the
   * buffer and is later set to the no. of characters in the reference.
   *
   * @retval 0 on success and -1 otherwise.
   */
  int parse_char_reference (ACEXML_Char *buf, size_t& len);

  /**
   * Parse a reference name, i.e., foo in "&foo;" or "%foo;". The first
   * character encountered should be the character following '&' or '%'.
   * Effectively the same as @sa parse_name but we don't use the parser's
   * obstack. Caller is responsible for deleting the memory.
   *
   * @retval  A pointer to name of reference, 0 otherwise.
   */
  ACEXML_Char* parse_reference_name (void);

  /**
   * Parse a CDATA section.  The first character should always be the first
   * '[' in CDATA definition.
   *
   * @retval 0 on success.
   * @retval -1 if fail.
   */
  int parse_cdata (void);

  /**
   * Parse a "markupdecl" section, this includes both "markupdecl" and
   * "DeclSep" sections in XML specification
   */
  int parse_internal_dtd (void);

  /**
   *  Skip over a comment. The first character encountered should always be
   *  the first '-' in the comment prefix "@<@!--".
   */
  int parse_comment (void);

  /**
   * Parse an "ELEMENT" decl.  The first character this method
   * expects is always the 'L' (the second char) in the word
   * "ELEMENT".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_element_decl (void);

  /**
   * Parse an "ENTITY" decl.  The first character this method expects
   * is always the 'N' (the second char) in the word "ENTITY".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_entity_decl (void);

  /**
   * Parse an "ATTLIST" decl.  Thse first character this method
   * expects is always the 'A' (the first char) in the word
   * "ATTLIST".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_attlist_decl (void);

  /**
   * Parse a AttType declaration.
   *
   */
  int parse_atttype (void);

  /**
   *Parse a "NOTATION" decl.  The first character this method
   * expects is always the 'N' (the first char) in the word
   * "NOTATION".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_notation_decl (void);

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
  int parse_external_id (ACEXML_Char *&publicId, ACEXML_Char *&systemId);

  /**
   *  Parse an external DTD.
   *
   */
  int parse_external_dtd (void);

  /**
   *  Parse an external subset. This does the actual parsing of an external
   *  subset and is called by @sa parse_external_dtd.
   *
   */
  int parse_external_subset (void);

  /**
   *  Parse a markupDecl section.
   *
   */
  int parse_markup_decl (void);

  /**
   *  Parse a conditionalSect declaration.
   *
   */
  int parse_conditional_section (void);

  /**
   *  Parse a includeSect declaration.
   *
   */
  int parse_includesect (void);

  /**
   *
   *  Parse a ignoreSect declaration.
   */
  int parse_ignoresect (void);

  /**
   * Parse a PEReference.
   *
   */
  int parse_PE_reference (void);

  /**
   *  Parse a Reference.
   *
   */
  int parse_entity_reference (void);

  /**
   *  Parse an entityValue.
   *
   */
  int parse_entity_value (ACEXML_Char *&str);

  /**
   *  Parse a DefaultDecl specification.
   *
   */
  int parse_defaultdecl (void);


  /**
   * Parse the "children" and "Mixed" non-terminals in contentspec.
   *
   * The first character this function sees must be the first
   * open paren '(' in children.
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_children_definition (void);

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
  int parse_child (int skip_open_paren);

  /**
   *  Parse a name from the input CharStream. If @a ch @!= 0, then we have
   *  already consumed the first name character from the input CharStream,
   *  otherwise, parse_name will use this->get() to acquire the initial
   *  character.
   *
   *  @return A pointer to the string in the obstack, 0 if it's not a
   *  valid name.
   */
  ACEXML_Char *parse_name (ACEXML_Char ch = 0);

  /**
   *  Parse a NMTOKEN from the input stream.
   *
   * @return A pointer to the string in the obstack, 0 if it's not a valid
   * NMTOKEN.
   */
  ACEXML_Char* parse_nmtoken (ACEXML_Char ch = 0);

  /**
   *  Parse the version string in an XML Prolog section.
   *
   *  @param str String containing the version number if successful.
   *  @return 0 if the string was read successfully, 0 otherwise.
   */
  int parse_version (ACEXML_Char*& str);

  /**
   *  Parse the version number in a VersionInfo declaration.
   */
  int parse_version_num (ACEXML_Char*& str);

  /**
   *  Parse the encoding name in an XML Prolog section.
   *
   *  @param str String containing the encoding name if successful.
   *  @return 0 if the string was read successfully, 0 otherwise.
   */
  int parse_encname (ACEXML_Char*& str);

  /**
   *  Parse a SDDecl string.
   *
   *  @param str String containing the encoding name if successful.
   *  @return 0 if the string was read successfully, -1 otherwise.
   */
  int parse_sddecl (ACEXML_Char*& str);

  /**
   *  Parse an attribute name.
   *
   *  @retval str String containing the value of the attribute name
   *             if successful.
   *  @retval 0 otherwise.
   */
  ACEXML_Char* parse_attname (void);

  /**
   *  Parse an attribute value.
   *
   *  @param str String containing the value of the attribute if successful.
   *  @return 0 if attribute value was read successfully, -1 otherwise.
   */
  int parse_attvalue (ACEXML_Char*& str);

  /**
   *  Parse a tokenized type attribute.
   *
   *  @return 0 if attribute type was read successfully, -1 otherwise.
   */
  int parse_tokenized_type (void);

  /**
   *  Parse a SystemLiteral.
   *
   *  @param str String containing the SystemLiteral if successful.
   *  @return 0 if the string was read successfully, 0 otherwise.
   */
  int parse_system_literal (ACEXML_Char*& str);

   /**
   *  Parse a PubidLiteral.
   *
   *  @param str String containing the PubidLiteral if successful.
   *  @return 0 if the string was read successfully, 0 otherwise.
   */
  int parse_pubid_literal (ACEXML_Char*& str);

  /**
   * Check if a character @a c is a whitespace.
   *
   * @retval 1 if @a c is a valid white space character. 0 otherwise.
   */
  int is_whitespace (const ACEXML_Char c) const;

  /**
   * Check if a character @a c is a valid Char.
   *
   * @retval 1 if @a c is a valid character. 0 otherwise.
   */
  int isChar (ACEXML_UCS4 c) const;

  /**
   * Check if a character @a c is a valid CharRef character.
   *
   * @retval 1 if @a c is a valid character reference character, 0 otherwise.
   */
  int isCharRef (const ACEXML_Char c) const;

  /**
   * Check if a character @a c is a BaseChar.
   *
   * @retval 1 if @a c is a valid BaseChar character, 0 otherwise.
   */
  int isBasechar (const ACEXML_Char c) const;

  /**
   * Check if a character @a c is a Ideographic.
   *
   * @retval 1 if @a c is a valid Ideographic character, 0 otherwise.
   */
  int isIdeographic (const ACEXML_Char c) const;

  /**
   * Check if a character @a c is a CombiningChar.
   *
   * @retval 1 if @a c is a valid CombiningChar character, 0 otherwise.
   */
  int isCombiningchar (const ACEXML_Char c) const;

  /**
   * Check if a character @a c is a Digit.
   *
   * @retval 1 if @a c is a valid Digit character, 0 otherwise.
   */
  int isDigit (const ACEXML_Char c) const;

  /**
   * Check if a character @a c is an Extender.
   *
   * @retval 1 if @a c is a valid Extender character, 0 otherwise.
   */
  int isExtender (const ACEXML_Char c) const;

  /**
   * Check if a character @a c is a Letter.
   *
   * @retval 1 if @a c is a valid Letter character, 0 otherwise.
   */
  int isLetter (const ACEXML_Char c) const;

  /**
   * Check if a character is an acceptable NameChar.
   *
   * @retval 1 if @a c is a valid NameChar character, 0 otherwise.
   */
  int isNameChar (const ACEXML_Char c) const;

  /**
   * Check if a character is a PubidChar.
   *
   * @retval 1 if @a c is a valid PubidChar character, 0 otherwise.
   */
  int isPubidChar (const ACEXML_Char c) const;

  /// Get a character.
  virtual ACEXML_Char get (void);

  /// Peek a character.
  virtual ACEXML_Char peek (void);

private:

  // *** Helper functions for parsing XML

  /**
   * Skip any whitespaces encountered until the first non-whitespace
   * character is encountered.
   *
   * @return The next non-whitespace character from the CharStream.
   *
   * @sa skip_whitespace_count
   */
  ACEXML_Char skip_whitespace (void);

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
   * Check if a character @a c is a Digit.
   *
   * @retval 1 if @a c is a valid Digit character, 0 otherwise.
   */
  int isNormalDigit (const ACEXML_Char c) const;

  /**
   * Dispatch errors to ErrorHandler.
   *
   */
  void error (const ACEXML_Char* msg);

  /**
   * Dispatch warnings to ErrorHandler.
   *
   */
  void warning (const ACEXML_Char* msg);

  /**
   * Dispatch fatal errors to ErrorHandler.
   *
   */
  void fatal_error (const ACEXML_Char* msg);

  /**
   * Dispatch prefix mapping calls to the ContentHandler.
   *
   * @param prefix Namespace prefix
   * @param uri Namespace URI
   * @param name Local name
   * @param start 1 => startPrefixMapping 0 => endPrefixMapping
   */
  void prefix_mapping (const ACEXML_Char* prefix,
                       const ACEXML_Char* uri,
                       int start);
  /**
   *  Parse a keyword.
   */
  int parse_token (const ACEXML_Char* keyword);

  /**
   *  Push the current context on to the stack.
   *
   */
  int push_context (ACEXML_Parser_Context* context);

  /**
   *  Pop the top element in the stack and replace current context with that.
   */
  size_t pop_context (int GE_ref);

  /**
   *  Create a new ACEXML_CharStream from @a systemId and @a publicId and
   *  replace the current input stream with the newly created stream.
   */
  virtual int switch_input (ACEXML_CharStream* cstream,
                            const ACEXML_Char* systemId,
                            const ACEXML_Char* publicId = 0);
  /**
   *  Create a new ACEXML_InputSource from @a systemId and @a publicId and
   *  replace the current input source with the newly created InputSource.
   */
  virtual int switch_input (ACEXML_InputSource* input,
                            const ACEXML_Char* systemId,
                            const ACEXML_Char* publicId = 0);

  /**
   * Check for a parameter entity reference. This is used to check for the
   * occurrence of a PE Reference withing markupDecl. Additionally this
   * function consumes any leading or trailing whitespace around the PE
   * Reference.
   *
   * @retval Number of whitespace characters skipped.
   */
  int check_for_PE_reference (void);

  /**
   *  Reset the parser state.
   *
   */
  void reset (void);

  /**
   * Very trivial, non-conformant normalization of a systemid.
   *
   */
  ACEXML_Char* normalize_systemid (const ACEXML_Char* systemId);

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

  /**
   *  @var validation_feature_
   *
   *  This constant string defines the SAX XML Validation feature. When
   *  this feature is enabled, the parser validates the document in
   *  addition to checking for well-formedness.
   */
  static const ACEXML_Char validation_feature_[];

  /* @} */

  /// Keeping track of the handlers. We do not manage the memory for
  /// handlers.
  ACEXML_DTDHandler *dtd_handler_;
  ACEXML_EntityResolver *entity_resolver_;
  ACEXML_ContentHandler *content_handler_;
  ACEXML_ErrorHandler *error_handler_;

  /// Document Type
  ACEXML_Char *doctype_;

  /// Current parser context
  ACEXML_Parser_Context* current_;

  /// Stack used to hold the Parser_Context
  ACE_Unbounded_Stack<ACEXML_Parser_Context*> ctx_stack_;

  /*
   * The following two are essentially chains of references and is used by
   * the parser to determine if there is any recursion. We keep two of
   * these one for general entities and one for parameter entities, as they
   * both fall under different namespaces.
   *
   */
  /// Set used to hold the general entity references that are active.
  ACE_Unbounded_Stack<ACEXML_Char*> GE_reference_;

  /// Set used to hold the parameter entity references that are active.
  ACE_Unbounded_Stack<ACEXML_Char*> PE_reference_;

  /// Obstack used by the parser to hold all the strings parsed
  ACE_Obstack_T<ACEXML_Char> obstack_;

  /// Alternative obstack used to hold any strings when the original is in use
  ACE_Obstack_T<ACEXML_Char> alt_stack_;

  /// Namespace stack used by the parser to implement support for Namespaces
  ACEXML_NamespaceSupport xml_namespace_;

  /// T => We are processing a nested namespace
  int nested_namespace_;

  /// Set of internal parsed general entities in the document
  ACEXML_Entity_Manager internal_GE_;

  /// Set of external parsed general entities in the document
  ACEXML_Entity_Manager external_GE_;

  /// Set of unparsed entities in the document
  ACEXML_Entity_Manager unparsed_entities_;

  /// Set of predefined entities used by the parser
  ACEXML_Entity_Manager predef_entities_;

  /// Set of internal parsed parameter entities in the document
  ACEXML_Entity_Manager internal_PE_;

  /// Set of external parsed parameter entities in the document
  ACEXML_Entity_Manager external_PE_;

  /// Set of notations declared in the document
  ACEXML_Entity_Manager notations_;

  /// State of the parser when it encounters a reference.
  ACEXML_ParserInt::ReferenceState ref_state_;

  /// T => We are parsing an external subset
  int external_subset_;

  /// T => We are parsing an external entity value
  int external_entity_;

  /// T => Internal DTD has parameter entity references
  int has_pe_refs_;

  /// If set, the document is a standalone XML document
  int standalone_;

  /// If set, the document has an external DTD subset
  int external_dtd_;

  /// If set, the document has an internal DTD
  int internal_dtd_;

  /// Feature flags
  /// If set, the parser should parse a document without a prolog
  int simple_parsing_;

  /// If set, the parser should also validate
  int validate_;

  /// If set, the parser should allow access by namespace qualified names.
  int namespaces_;

  /// If set, the parser should include namespace declarations in the list
  /// of attributes of an element.
  int namespace_prefixes_;

};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/Parser.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* _ACEXML_BASIC_PARSER_H_ */
