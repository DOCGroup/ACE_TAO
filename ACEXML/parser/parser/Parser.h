// -*- C++ -*- $Id$

#ifndef _ACEXML_BASIC_PARSER_H_
#define _ACEXML_BASIC_PARSER_H_

#include "Common/XMLReader.h"
#include "Common/LocatorImpl.h"
#include "Common/NamespaceSupport.h"
#include "Common/CharStream.h"
#include "Parser_export.h"
#include "ace/Obstack.h"
#include "ace/Functor.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Containers_T.h"
#include "Entity_Manager.h"

typedef ACE_Hash_Map_Entry<ACEXML_String,
                           ACEXML_String> ACEXML_ENTITY_ENTRY;

typedef ACE_Hash_Map_Manager_Ex<ACEXML_String,
                                ACEXML_String,
                                ACE_Hash<ACEXML_String>,
                                ACE_Equal_To<ACEXML_String>,
                                ACE_Null_Mutex> ACEXML_ENTITIES_MANAGER;

typedef ACE_Hash_Map_Iterator_Ex<ACEXML_String,
                                 ACEXML_String,
                                 ACE_Hash<ACEXML_String>,
                                 ACE_Equal_To<ACEXML_String>,
                                 ACE_Null_Mutex> ACEXML_ENTITIES_MANAGER_ITER;

typedef ACE_Hash_Map_Reverse_Iterator_Ex<ACEXML_String,
                                         ACEXML_String,
                                         ACE_Hash<ACEXML_String>,
                                         ACE_Equal_To<ACEXML_String>,
                                         ACE_Null_Mutex> ACEXML_ENTITIES_MANAGER_REVERSE_ITER;

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

  /*
   * Look up the value of a feature.
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

  /*
   * Set the state of a feature.
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

  /// Skip any whitespace encounter and return the first
  /// none-white space characters (which will be consumed from
  /// the CharStream.)  If no whitespace is found,
  /// it will return 0.  @a whitespace return a pointer to
  /// the string of skipped whitespace after proper conversion.
  /// @a whitespace will be null if there's no whitespace found.
  ACEXML_Char skip_whitespace (ACEXML_Char **whitespace);

  /// Return 1 if @a c is a valid white space character.  Otherwise,
  /// return 0.
  int is_whitespace (ACEXML_Char c);
  int is_whitespace_or_equal (ACEXML_Char c);
  int is_nonname (ACEXML_Char c);

  /// Skip an equal sign.  Return 0 when succeeds, -1 if no equal sign
  /// is found.
  int skip_equal (void);

  /// Get a quoted string.  Return 0 on success, -1 otherwise.
  /// @a str return the un-quoted string.  Quoted strings are
  /// used to specify attribute values and this routine will
  /// replace character and entity references on-the-fly.  Parameter
  /// entitys is not allowed (or replaced) in this function.
  int get_quoted_string (ACEXML_Char *&str);

  /// Parse a PI statement.  The first character encountered
  /// should always be '?' in the PI prefix "<?".
  /// Return 0 on success, -1 otherwise.
  int parse_processing_instruction (ACEXML_Env &xmlenv);

  /// Filter out a comment. The first character encountered
  /// should always be the first '-' in the comment prefix
  /// "<!--"
  int grok_comment ();

  /// Read a name from the input CharStream (until white space).
  /// If @a ch != 0, then we have already consumed the first name
  /// character from the input CharStream, otherwise, read_name
  /// will use this->get() to acquire the initial character.
  /// Return a pointer to the string in the obstack, 0 if it's not
  /// a valid name.
  ACEXML_Char *read_name (ACEXML_Char ch = 0);

  /// Parse the DOCTYPE declaration.  The first character encountered
  /// should always be  'D' in doctype prefix: "<!DOCTYPE".
  int parse_doctypedecl (ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /// Parse an XML element.  The first character encountered should
  /// be the first character of the element "Name".
  void parse_element (ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /// Parse XML Prolog.
  void parse_xml_prolog (ACEXML_Env &xmlenv);

  /// Parse a character reference, i.e., "&#x20;" or "&#30;".  Return 0 on
  /// success and -1 otherwise.  @a buf points to a character buffer for
  /// the result and @a len specifies the capacities of the buffer.  The first
  /// character encountered should be the '#' char.
  int parse_char_reference (ACEXML_Char *buf, size_t len);

  /// Parse an entity reference, i.e., "&amp;".  Return a pointer to
  /// the resolved const ACEXML_String if success (previously
  /// defined), 0 otherwise.  The first character encountered should be
  /// the character following '&'.
  const ACEXML_String *parse_reference (void);

  /// Parse a CDATA section.  The first character should always be the first
  /// '[' in CDATA definition.  Return 0 on success, -1 otherwise.
  int parse_cdata (ACEXML_Env &xmlenv);

  /// Parse a "markupdecl" section, this includes both "markupdecl" and
  /// "DeclSep" sections in XML specification
  int parse_internal_dtd (ACEXML_Env &xmlenv);

  /// Parse an "ELEMENT" decl.  The first character this method
  /// expects is always the 'L' (the second char) in the word
  /// "ELEMENT".
  int parse_element_decl (ACEXML_Env &xmlenv);

  /// Parse an "ENTITY" decl.  The first character this method expects
  /// is always the 'N' (the second char) in the word "ENTITY".
  int parse_entity_decl (ACEXML_Env &xmlenv);

  /// Parse an "ATTLIST" decl.  Thse first character this method
  /// expects is always the 'A' (the first char) in the word
  /// "ATTLIST".
  int parse_attlist_decl (ACEXML_Env &xmlenv);

  /// Parse a "NOTATION" decl.  The first character this method
  /// expects is always the 'N' (the first char) in the word
  /// "NOTATION".
  int parse_notation_decl (ACEXML_Env &xmlenv);

  /// Get a character.
  ACEXML_Char get (void);

  /// Peek a character.
  ACEXML_Char peek (void);

  /// Check if more data can be added to a character buffer in obstack.
  /// If not, the existing data in the buffer will be cleared out by
  /// freezing the segment and pass it out thru a content_handler_->characters ()
  /// call.  @a counter records the length of the existing data in
  /// obstack.
  int try_grow_cdata (size_t size, size_t &len, ACEXML_Env &xmlenv);

private:
  /// Check and dispatch errors/warnings to ErrorHandler.
  /// Return 0 if the error/warning is handled successfully,
  /// -1, if the handler doesn't know how to handle the error/exceptions.
  int check_exception (ACEXML_Env &xmlenv);

  /// Keeping track of the handlers.  We do not manage
  /// the memory for handlers.
  ACEXML_DTDHandler *dtd_handler_;
  ACEXML_EntityResolver *entity_resolver_;
  ACEXML_ContentHandler *content_handler_;
  ACEXML_ErrorHandler *error_handler_;

  /// @@ Feature and properties management structure here.
  // Current input char stream.
  ACEXML_CharStream *instream_;

  ACE_Obstack_T<ACEXML_Char> obstack_;

  ACEXML_NamespaceSupport xml_namespace_;

  ACEXML_Entity_Manager entities_;

  // Locator
  ACEXML_LocatorImpl locator_;
};

#if defined (__ACEXML_INLINE__)
# include "Parser.i"
#endif /* __ACEXML_INLINE__ */
#endif /* _ACEXML_BASIC_PARSER_H_ */
