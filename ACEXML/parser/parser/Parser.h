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

#include "common/XMLReader.h"
#include "common/LocatorImpl.h"
#include "common/NamespaceSupport.h"
#include "common/CharStream.h"
#include "parser/parser/Parser_export.h"
#include "ace/Obstack.h"
#include "ace/Functor.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Containers_T.h"
#include "parser/parser/Entity_Manager.h"

/**
 * @class ACEXML_Parser Parser.h "parser/parser/Parser.h"
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
  virtual int getFeature (const ACEXML_Char *name,
                  ACEXML_Env &xmlenv)
    //      ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
    //                       ACEXML_SAXNotSupportedException))
    ;

  /*
   * Look up the value of a property.
   */
  virtual void * getProperty (const ACEXML_Char *name,
                              ACEXML_Env &xmlenv)
    //      ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
    //                       ACEXML_SAXNotSupportedException))
    ;

  /*
   * Parse an XML document.
   */
  virtual void parse (ACEXML_InputSource *input,
                      ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Parse an XML document from a system identifier (URI).
   */
  virtual void parse (const ACEXML_Char *systemId,
                      ACEXML_Env &xmlenv)
    // @@ throw IOException???
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
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

  /**
   * Activating or deactivating a feature.
   */
  virtual void setFeature (const ACEXML_Char *name,
                           int boolean_value,
                           ACEXML_Env &xmlenv)
    //      ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
    //                       ACEXML_SAXNotSupportedException))
    ;

  /*
   * Set the value of a property.
   */
  virtual void setProperty (const ACEXML_Char *name,
                            void *value,
                            ACEXML_Env &xmlenv)
    //      ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
    //                       ACEXML_SAXNotSupportedException))
    ;

  // *** Helper functions for parsing XML

  /**
   * Skip any whitespaces encounter until the first non-whitespace
   * character is encountered and consumed from the current input
   * CharStream.
   *
   * @param whitespace Return a pointer to the string of skipped
   * whitespace after proper conversion.  Null if there's no
   * whitespace found.
   *
   * @retval The first none-white space characters (which will be
   * consumed from the CharStream.)  If no whitespace is found, it
   * will return 0.
   *
   * @sa skip_whitespace_count
   */
  ACEXML_Char skip_whitespace (ACEXML_Char **whitespace);

  /**
   * Skip any whitespaces encounter until the first non-whitespace
   * character.  The first non-whitespace character is not consumed.
   * This method does peek into the input CharStream and therefore
   * is more expensive than @ref skip_whitespace.
   *
   * @param peek If non-null, @a peek points to a ACEXML_Char where
   *        skip_whitespace_count store the first non-whitespace
   *        character it sees (character is not removed from the stream.)
   *
   * @retval The number of whitespace characters consumed.
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
  int parse_processing_instruction (ACEXML_Env &xmlenv);

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
   * @retval A pointer to the string in the obstack, 0 if it's not
   * a valid name.
   */
  ACEXML_Char *read_name (ACEXML_Char ch = 0);

  /**
   * Parse the DOCTYPE declaration.  The first character encountered
   * should always be  'D' in doctype prefix: "@<@!DOCTYPE".
   */
  int parse_doctypedecl (ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
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
  void parse_element (int is_root, ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Parse XML Prolog.
   */
  void parse_xml_prolog (ACEXML_Env &xmlenv);

  /**
   * Parse a character reference, i.e., "&#x20;" or "&#30;".   The first
   * character encountered should be the '#' char.
   *
   * @param buf points to a character buffer for
   * the result.
   * @param len specifies the capacities of the buffer.
   *
   * @retval 0 on success and -1 otherwise.
   */
  int parse_char_reference (ACEXML_Char *buf, size_t len);

  /**
   * Parse an entity reference, i.e., "&amp;".  The first character
   * encountered should be the character following '&'.
   *
   * @retval A pointer to the resolved const ACEXML_String if success
   * (previously defined), 0 otherwise.
   */
  const ACEXML_String *parse_reference (void);

  /**
   * Parse a CDATA section.  The first character should always be the first
   * '[' in CDATA definition.
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_cdata (ACEXML_Env &xmlenv);

  /**
   * Parse a "markupdecl" section, this includes both "markupdecl" and
   * "DeclSep" sections in XML specification
   */
  int parse_internal_dtd (ACEXML_Env &xmlenv);

  /**
   * Parse an "ELEMENT" decl.  The first character this method
   * expects is always the 'L' (the second char) in the word
   * "ELEMENT".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_element_decl (ACEXML_Env &xmlenv);

  /**
   * Parse an "ENTITY" decl.  The first character this method expects
   * is always the 'N' (the second char) in the word "ENTITY".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_entity_decl (ACEXML_Env &xmlenv);

  /**
   * Parse an "ATTLIST" decl.  Thse first character this method
   * expects is always the 'A' (the first char) in the word
   * "ATTLIST".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_attlist_decl (ACEXML_Env &xmlenv);

  /**
   *Parse a "NOTATION" decl.  The first character this method
   * expects is always the 'N' (the first char) in the word
   * "NOTATION".
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_notation_decl (ACEXML_Env &xmlenv);

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
   * @param publicID returns the unquoted publicID read.  If none
   *        is available, it will be reset to 0.
   * @param systemID returns the unquoted systemID read.  If none
   *        is available, it will be reset to 0.
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_external_id_and_ref (ACEXML_Char *&publicId,
                                 ACEXML_Char *&systemId,
                                 ACEXML_Env &xmlenv);

  /**
   * Parse the "children" and "Mixed" non-terminals in contentspec.
   *
   * The first character this function sees must be the first
   * open paren '(' in children.
   *
   * @retval 0 on success, -1 otherwise.
   */
  int parse_children_definition (ACEXML_Env &xmlenv);

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
  int parse_child (int skip_open_paren,
                   ACEXML_Env &xmlenv);

protected:
  /// Get a character.
  ACEXML_Char get (void);

  /// Peek a character.
  ACEXML_Char peek (void);

  /**
   * Check if more data can be added to a character buffer in obstack.
   * If not, the existing data in the buffer will be cleared out by
   * freezing the segment and pass it out thru a content_handler_->characters ()
   * call.  @a counter records the length of the existing data in
   * obstack.
   */
  int try_grow_cdata (size_t size, size_t &len, ACEXML_Env &xmlenv);

  // Feature names:

  /**
   * \addtogroup acexml_parser_features
   * @{
   */

  /**
   *
   */
  static const ACEXML_Char simple_parsing_name_[];

  /* @} */

private:
  /**
   * Check and dispatch errors/warnings to ErrorHandler.
   *
   * @retval 0 if the error/warning is handled successfully, -1, if
   * the handler doesn't know how to handle the error/exceptions.
   */
  int check_exception (ACEXML_Env &xmlenv);

  /// Keeping track of the handlers.  We do not manage
  /// the memory for handlers.
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

  // Feature flags &
  int simple_parsing_;

};

#if defined (__ACEXML_INLINE__)
# include "parser/parser/Parser.i"
#endif /* __ACEXML_INLINE__ */
#endif /* _ACEXML_BASIC_PARSER_H_ */
