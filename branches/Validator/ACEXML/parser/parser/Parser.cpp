// $Id$

#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/Transcode.h"
#include "ACEXML/common/AttributesImpl.h"
#include "ace/ACE.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/Parser.i"
#endif /* __ACEXML_INLINE__ */

static const ACEXML_Char default_attribute_type[] = ACE_TEXT ("CDATA");
static const ACEXML_Char empty_string[] = { 0 };

const ACEXML_Char
ACEXML_Parser::simple_parsing_feature_[] = ACE_TEXT ("Simple");

const ACEXML_Char
ACEXML_Parser::namespaces_feature_[] = ACE_TEXT ("http://xml.org/sax/features/namespaces");

const ACEXML_Char
ACEXML_Parser::namespace_prefixes_feature_[] = ACE_TEXT ("http://xml.org/sax/features/namespace-prefixes");


static const ACEXML_Char* ACEXML_Parser_Msg[] = {
  ACE_TEXT ("Invalid input source"),
  ACE_TEXT ("Expecting '<'"),
  ACE_TEXT ("Expecting '>'"),
  ACE_TEXT ("Invalid comment"),
  ACE_TEXT ("Duplicate DOCTYPE definition"),
  ACE_TEXT ("Unexpected EOF"),
  ACE_TEXT ("Invalid XMLDecl ('<?xml' ?)"),
  ACE_TEXT ("Unrecognized XMLDecl ('version'?)"),
  ACE_TEXT ("ACEXML only supports XML Version 1.0 documents"),
  ACE_TEXT ("Encoding declaration doesn't match auto-detected encoding"),
  ACE_TEXT ("ACEXML Parser Internal error"),
  ACE_TEXT ("PITarget name cannot start with 'xml'"),
  ACE_TEXT ("Expecting keyword 'DOCTYPE'"),
  ACE_TEXT ("Expecting a DOCTYPE name"),
  ACE_TEXT ("Root element missing"),
  ACE_TEXT ("Error reading attribute"),
  ACE_TEXT ("Duplicate namespace prefix"),
  ACE_TEXT ("Duplicate attribute found"),
  ACE_TEXT ("Cannot have both namespaces and namespace_prefixes simultaneously"),
  ACE_TEXT ("Unexpected character"),
  ACE_TEXT ("Mismatched End-tag encountered"),
  ACE_TEXT ("Expecting '[CDATA[' section"),
  ACE_TEXT ("Invalid keyword in markupdecl"),
  ACE_TEXT ("Invalid character following '<!' in markupdecl"),
  ACE_TEXT ("Expecting markupdecl or DeclSep"),
  ACE_TEXT ("Expecting keyword `ELEMENT'"),
  ACE_TEXT ("Error reading element name"),
  ACE_TEXT ("Expecting keyword `EMPTY' in ELEMENT definition."),
  ACE_TEXT ("Expecting keyword `ANY' in ELEMENT definition."),
  ACE_TEXT ("Error reading ELEMENT definition."),
  ACE_TEXT ("Expecting keyword `ENTITY'"),
  ACE_TEXT ("Can't use a reference when defining entity name"),
  ACE_TEXT ("Error reading ENTITY name."),
  ACE_TEXT ("Error reading ENTITY value."),
  ACE_TEXT ("Duplicate ENTITY definition"),
  ACE_TEXT ("Invalid ExternalID definition (system ID missing)"),
  ACE_TEXT ("Unexpected keyword NDATA in PEDecl"),
  ACE_TEXT ("Expecting keyword NDATA"),
  ACE_TEXT ("Expecting keyword `ATTLIST'"),
  ACE_TEXT ("Error reading attribute name"),
  ACE_TEXT ("Expecting keyword `CDATA'"),
  ACE_TEXT ("Expecting keyword `ID', `IDREF', or `IDREFS'"),
  ACE_TEXT ("Expecting keyword `ENTITY', or `ENTITIES'"),
  ACE_TEXT ("Expecting keyword `NMTOKEN', `NMTOKENS', or `NOTATION'"),
  ACE_TEXT ("Expecting keyword `NMTOKEN' or `NMTOKENS'"),
  ACE_TEXT ("Expecting keyword `NOTATION'"),
  ACE_TEXT ("Expecting `(' following NOTATION"),
  ACE_TEXT ("Error reading NOTATION name"),
  ACE_TEXT ("Error reading enumerated NMTOKEN name"),
  ACE_TEXT ("Invalid Attribute Type"),
  ACE_TEXT ("Expecting keyword `#REQUIRED'"),
  ACE_TEXT ("Expecting keyword `#IMPLIED'"),
  ACE_TEXT ("Expecting keyword `#FIXED'"),
  ACE_TEXT ("Error parsing `#FIXED' attribute value"),
  ACE_TEXT ("Invalid notation name."),
  ACE_TEXT ("Expecting keyword 'SYSTEM'"),
  ACE_TEXT ("Expecting keyword 'PUBLIC'"),
  ACE_TEXT ("Error parsing system/public literal"),
  ACE_TEXT ("Expecting either keyword `SYSTEM' or `PUBLIC'."),
  ACE_TEXT ("Expecting keyword `#PCDATA'"),
  ACE_TEXT ("Expecting end of Mixed section"),
  ACE_TEXT ("Expecting closing `)*' or ')'"),
  ACE_TEXT ("Error reading sub-element name"),
  ACE_TEXT ("Expecting `,', `|', or `)' while defining an element."),
  ACE_TEXT ("Invalid character reference")
};

ACEXML_Parser::ACEXML_Parser (void)
  :   dtd_handler_ (0),
      entity_resolver_ (0),
      content_handler_ (0),
      error_handler_ (0),
      instream_ (0),
      doctype_ (0),
      dtd_system_ (0),
      dtd_public_ (0),
      locator_(),
      simple_parsing_ (0),
      namespaces_(1),
      namespace_prefixes_ (0)
{
}

ACEXML_Parser::~ACEXML_Parser (void)
{
}

int
ACEXML_Parser::getFeature (const ACEXML_Char *name ACEXML_ENV_ARG_DECL)
   ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
                    ACEXML_SAXNotSupportedException))
{
  if (ACE_OS::strcmp (name, ACEXML_Parser::simple_parsing_feature_) == 0)
    {
      return this->simple_parsing_;
    }
  else if (ACE_OS::strcmp (name, ACEXML_Parser::namespaces_feature_) == 0)
    {
      return this->namespaces_;
    }
  else if (ACE_OS::strcmp (name,
                                  ACEXML_Parser::namespace_prefixes_feature_) == 0)
    {
      return this->namespace_prefixes_;
    }

  ACEXML_THROW_RETURN (ACEXML_SAXNotRecognizedException (name), -1);
}



void
ACEXML_Parser::setFeature (const ACEXML_Char *name,
                           int boolean_value ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
                         ACEXML_SAXNotSupportedException))
{
  if (ACE_OS::strcmp (name, ACEXML_Parser::simple_parsing_feature_) == 0)
    {
      this->simple_parsing_ = (boolean_value == 0 ? 0 : 1);
      return;
    }
  else if (ACE_OS::strcmp (name, ACEXML_Parser::namespaces_feature_) == 0)
    {
      this->namespaces_ = (boolean_value == 0 ? 0 : 1);
      return;
    }
  else if (ACE_OS::strcmp (name,
                                  ACEXML_Parser::namespace_prefixes_feature_) == 0)
    {
      this->namespace_prefixes_ = (boolean_value == 0 ? 0 : 1);
      return;
    }

  ACEXML_THROW (ACEXML_SAXNotRecognizedException (name));
}

void *
ACEXML_Parser::getProperty (const ACEXML_Char *name ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
                   ACEXML_SAXNotSupportedException))
{
  ACEXML_THROW_RETURN (ACEXML_SAXNotSupportedException (name), 0);
}

void
ACEXML_Parser::setProperty (const ACEXML_Char *name,
                            void *value ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
                   ACEXML_SAXNotSupportedException))
{
  ACE_UNUSED_ARG (value);

  ACEXML_THROW (ACEXML_SAXNotSupportedException (name));
}

void
ACEXML_Parser::report_error (ACEXML_Error minor_code ACEXML_ENV_ARG_DECL)
{
  ACEXML_SAXParseException* exception = 0;
  ACE_NEW_NORETURN (exception,
                    ACEXML_SAXParseException (ACEXML_Parser_Msg[minor_code]));
  if (this->error_handler_)
    this->error_handler_->error (*exception ACEXML_ENV_ARG_PARAMETER);
  else
    ACEXML_ENV_RAISE (exception);
  return;
}

void
ACEXML_Parser::report_warning (ACEXML_Error minor_code ACEXML_ENV_ARG_DECL)
{
  ACEXML_SAXParseException* exception = 0;
  ACE_NEW_NORETURN (exception,
                    ACEXML_SAXParseException (ACEXML_Parser_Msg[minor_code]));
  if (this->error_handler_)
    this->error_handler_->warning (*exception ACEXML_ENV_ARG_PARAMETER);
  return;
}

void
ACEXML_Parser::report_fatal_error (ACEXML_Error minor_code ACEXML_ENV_ARG_DECL)
{
  ACEXML_SAXParseException* exception = 0;
  ACE_NEW_NORETURN (exception,
                    ACEXML_SAXParseException (ACEXML_Parser_Msg[minor_code]));
  if (this->error_handler_)
    this->error_handler_->fatalError (*exception ACEXML_ENV_ARG_PARAMETER);
  ACEXML_ENV_RAISE (exception);
  return;
}

void
ACEXML_Parser::parse (ACEXML_InputSource *input ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (input == 0 || (this->instream_ = input->getCharStream ())  == 0)
    {
      this->report_fatal_error(ACEXML_INVIP ACEXML_ENV_ARG_PARAMETER);
      return;
    }

  // Set up Locator. At this point, the systemId and publicId are null. We
  // can't do better, as we don't know anything about the InputSource
  // currently, and according to the SAX spec, the parser should set up the
  // locator before reporting any document events.
  if (this->content_handler_)
    this->content_handler_->setDocumentLocator (&this->locator_);

  if (this->simple_parsing_ == 0)
    {
      this->parse_xml_prolog (ACEXML_ENV_SINGLE_ARG_PARAMETER);
      ACEXML_CHECK;
    }

  this->content_handler_->startDocument (ACEXML_ENV_SINGLE_ARG_PARAMETER);
  ACEXML_CHECK;

  int doctype_defined = 0;

  for (int prolog_done = 0; prolog_done == 0; )
    {
      if (this->skip_whitespace (0) != '<')
        {
          this->report_fatal_error (ACEXML_LESS ACEXML_ENV_ARG_PARAMETER);
          return;
        }
      ACEXML_Char fwd = this->peek ();
      switch (fwd)
        {
        case '!':
          this->get ();         // consume the '!'
          fwd = this->peek ();
          if (fwd == 'D' && !doctype_defined)       // DOCTYPE
            {
              // This will also take care of the trailing MISC block if any.
              this->parse_doctypedecl (ACEXML_ENV_SINGLE_ARG_PARAMETER);
              ACEXML_CHECK;
              doctype_defined = 1;
              break;
            }
          else if (fwd == '-')  // COMMENT
            {
              if (this->grok_comment () < 0)
                {
                  this->report_fatal_error(ACEXML_INVCO
                                           ACEXML_ENV_ARG_PARAMETER);
                  return;
                }
            }
          else
            {
              this->report_fatal_error (ACEXML_DUPDOC
                                        ACEXML_ENV_ARG_PARAMETER);
              return;
            }
          break;
        case '?':
          this->parse_processing_instruction (ACEXML_ENV_SINGLE_ARG_PARAMETER);
          ACEXML_CHECK;
          break;
        case 0:
          this->report_fatal_error (ACEXML_EOF ACEXML_ENV_ARG_PARAMETER);
          return;
        default:                // Root element begins
          prolog_done = 1;
          break;
        }
    }

  // Now parse root element.
  this->parse_element (1 ACEXML_ENV_ARG_PARAMETER);
  ACEXML_CHECK;

  this->content_handler_->endDocument (ACEXML_ENV_SINGLE_ARG_PARAMETER);
  ACEXML_CHECK;

  // Reset the Locator held within the parser
  this->locator_.reset();
}

void
ACEXML_Parser::parse (const ACEXML_Char *systemId ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // @@ Not implemented.
  ACE_UNUSED_ARG (systemId);

  ACEXML_THROW (ACEXML_SAXNotSupportedException ());
}


void
ACEXML_Parser::parse_xml_prolog (ACEXML_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->parse_token (ACE_TEXT("<?xml")) < 0)
    {
      this->report_fatal_error(ACEXML_INVXMLDECL ACEXML_ENV_ARG_PARAMETER);
      return;
    }

  ACEXML_Char *astring;
  if (this->skip_whitespace (0) != 'v' // Discard whitespace
      || this->parse_token (ACE_TEXT("ersion")) < 0
      || this->skip_equal () != 0
      || this->get_quoted_string (astring) != 0)
    {
      this->report_fatal_error (ACEXML_INVVERSION ACEXML_ENV_ARG_PARAMETER);
      return;
    }
  if (ACE_OS::strcmp (astring, ACE_TEXT ("1.0")) != 0)
    {
      this->report_fatal_error (ACEXML_ENOTSUP ACEXML_ENV_ARG_PARAMETER);
      return;
    }

  ACEXML_Char fwd = this->skip_whitespace (0);
      if (fwd != '?')
        {
      if (fwd == 'e')
            {
              if ((this->parse_token (ACE_TEXT("ncoding")) == 0) &&
                  this->skip_equal () == 0 &&
                  this->get_quoted_string (astring) == 0)
                {
                      if (ACE_OS::strcmp (astring,
                                          this->instream_->getEncoding()) != 0)
                        {
                              ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("Detected Encoding is %s ")
                              ACE_TEXT (": Declared Encoding is %s\n"),
                                          this->instream_->getEncoding(), astring));
                  this->report_warning (ACEXML_ENCMISMATCH
                                        ACEXML_ENV_ARG_PARAMETER);
                    }
              fwd = this->skip_whitespace (0);
                }
            }
      if (fwd == 's')
            {
              if ((this->parse_token (ACE_TEXT("tandalone")) == 0) &&
                  this->skip_equal () == 0 &&
                  this->get_quoted_string (astring) == 0)
                {
                  if (ACE_OS::strcmp (astring, ACE_TEXT ("yes")) == 0)
                this->standalone_ = 1;
                  else if (ACE_OS::strcmp (astring, ACE_TEXT ("no")) == 0)
                this->standalone_ = 0;
              fwd = this->skip_whitespace (0);
                    }
                }
            }
  if (fwd == '?' && this->get() == '>')
      return;
  // All the rules fail. So return an error.
  this->report_fatal_error (ACEXML_INVXMLDECL ACEXML_ENV_ARG_PARAMETER);
  return;
}

int
ACEXML_Parser::grok_comment (void)
{
  /// Simply filter out all the comment
  int state = 0;

  if (this->get () != '-' ||    // Skip the opening "<!--"
      this->get () != '-' ||    // completely.
      this->get () == '-')      // and at least something not '-'.
    return -1;

  while (state < 3)
    // Waiting for the trailing three character '-->'. Notice that
    // according to the spec, '--->' is not a valid closing comment
    // sequence. But we'll let it pass anyway.
    {
      ACEXML_Char fwd = this->get ();
      if ((fwd == '-' && state < 2) ||
          (fwd == '>' && state == 2))
        state += 1;
      else
        state = 0;              // Reset parse state.
    }
  return 0;
}

int
ACEXML_Parser::parse_processing_instruction (ACEXML_ENV_SINGLE_ARG_DECL)
{
  if (this->get () != '?')
    {                           // How did we get here?
      this->report_fatal_error(ACEXML_EINT ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }
  const ACEXML_Char *pitarget = this->read_name ();
  ACEXML_Char *instruction = 0;

  if (ACE_OS::strcasecmp (ACE_TEXT ("xml"), pitarget) != 0)
    {
      // Invalid PITarget name.
      this->report_fatal_error(ACEXML_INVPI ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  int state = 0;
  ACEXML_Char ch = this->skip_whitespace (0);

  while (state < 2)
    {
      switch (ch)
        {
        case '?':
          if (state == 0)
            state = 1;
          break;
        case '>':
          if (state == 1)
            {
              instruction = this->obstack_.freeze ();
              this->content_handler_->processingInstruction (pitarget,
                                                             instruction
                                                             ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK_RETURN (-1);
              this->obstack_.unwind (ACE_const_cast (ACEXML_Char*, pitarget));
              return 0;
            }
          break;
        case 0x0D:                // End-of-Line handling
          ch = (this->peek () == 0x0A ? this->get () : 0x0A);
          // Fall thru...
        case 0x0A:
          // Fall thru...
        default:
          if (state == 1)
            this->obstack_.grow ('?');
          this->obstack_.grow (ch);
          state = 0;
        }
      ch = this->get ();
    }
  return -1;
}

int
ACEXML_Parser::parse_doctypedecl (ACEXML_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->parse_token (ACE_TEXT ("DOCTYPE")) < 0)
    {
      this->report_fatal_error(ACEXML_INVDOCKEYWORD ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char nextch = this->skip_whitespace (0);
  if (nextch == 0)
    {
      this->report_fatal_error(ACEXML_INVDOCNAME ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  this->doctype_ = this->read_name (nextch);

  this->skip_whitespace_count (&nextch);

  if (nextch == 'S' || nextch == 'P') // ExternalID defined
    {
      this->parse_external_id_and_ref (this->dtd_public_,
                                       this->dtd_system_
                                       ACEXML_ENV_ARG_PARAMETER);
      ACEXML_CHECK_RETURN (-1);
//       if (this->dtd_public_ == 0)
//         ACE_DEBUG ((LM_DEBUG,
//                     ACE_TEXT ("ACEXML Parser got external DTD id: SYSTEM %s\n"),
//                     this->dtd_system_));
//       else
//         ACE_DEBUG ((LM_DEBUG,
//                     ACE_TEXT ("ACEXML Parser got DTD external id: PUBLIC %s %s\n"),
//                     this->dtd_public_, this->dtd_system_));
    }

  nextch = this->skip_whitespace (0);
  switch (nextch)
    {
    case '[':                   // Internal DTD definition
      if (this->parse_internal_dtd (ACEXML_ENV_SINGLE_ARG_PARAMETER) < 0)
        return -1;              // Error in markupdecl
      break;
    case '>':                   // End of DTD definition
      // this is an XML document without a dectypedecl.
      return 0;
    case '0':
      this->report_fatal_error (ACEXML_EOF ACEXML_ENV_ARG_PARAMETER);
      return -1;
    default:
      break;
    }

  if (this->skip_whitespace (0) != '>')
    {
      this->report_fatal_error(ACEXML_EINT ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }
  return 0;
}

void
ACEXML_Parser::parse_element (int is_root ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // Parse STag.
  const ACEXML_Char *startname = this->read_name ();
  if (startname == 0)
    {
      this->report_fatal_error (ACEXML_EOF ACEXML_ENV_ARG_PARAMETER);
      return;
    }
  if (is_root && this->doctype_ != 0
      && ACE_OS::strcmp (startname, this->doctype_) != 0)
    {
      this->report_fatal_error (ACEXML_MISSINGROOT ACEXML_ENV_ARG_PARAMETER);
      return;
    }
  ACEXML_AttributesImpl attributes;
  ACEXML_Char ch;
  int new_namespace = 0;
  const ACEXML_Char *endname = 0;
  const ACEXML_Char *ns_uri, *ns_lname; // namespace URI and localName
  ACEXML_Char* prefix = 0;
  ACEXML_Char* name = 0;
  for (int start_element_done = 0; start_element_done == 0;)
    {
      ch = this->skip_whitespace (0);

      switch (ch)
        {
        case 0:
          this->report_fatal_error(ACEXML_EINT ACEXML_ENV_ARG_PARAMETER);
          return;
        case '/':
          if (this->get () != '>')
            {
              this->report_fatal_error(ACEXML_GREAT ACEXML_ENV_ARG_PARAMETER);
              return;
            }
          else
            {
              this->xml_namespace_.processName(startname, ns_uri, ns_lname, 0);
              prefix = ACE_const_cast (ACEXML_Char*,
                                       this->xml_namespace_.getPrefix(ns_uri));
              this->report_prefix_mapping (prefix, ns_uri, ns_lname, 1
                                           ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK;
              this->content_handler_->startElement (ns_uri, ns_lname,
                                                    startname, &attributes
                                                    ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK;
              this->content_handler_->endElement (ns_uri, ns_lname, startname
                                                  ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK;
              this->report_prefix_mapping (prefix, ns_uri, ns_lname, 0
                                           ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK;
            }
          if (new_namespace != 0)
            this->xml_namespace_.popContext ();
          return;

        case '>':
          {
            this->xml_namespace_.processName (startname, ns_uri, ns_lname, 0);
            prefix = ACE_const_cast (ACEXML_Char*,
                                     this->xml_namespace_.getPrefix (ns_uri));
            this->report_prefix_mapping (prefix, ns_uri, ns_lname, 1
                                         ACEXML_ENV_ARG_PARAMETER);
            ACEXML_CHECK;
            this->content_handler_->startElement (ns_uri, ns_lname, startname,
                                                  &attributes
                                                  ACEXML_ENV_ARG_PARAMETER);
            ACEXML_CHECK;
            start_element_done = 1;
            break;
          }
        default:
          ACEXML_Char *attvalue = 0;
          ACEXML_Char *attname = this->read_name (ch);

          if (attname == 0 ||
              this->skip_equal () != 0 ||
              this->get_quoted_string (attvalue) != 0)
            {
              this->report_fatal_error(ACEXML_RDATTR ACEXML_ENV_ARG_PARAMETER);
              return;
            }

          // Handling new namespace if any. Notice that the order of
          // namespace declaration does matter.
          if (ACE_OS::strncmp (attname, ACE_TEXT("xmlns"), 5) == 0)
            {
              if (this->namespaces_)
                {
                  if (new_namespace == 0)
                    {
                      this->xml_namespace_.pushContext ();
                      new_namespace = 1;
                    }
                  name = ACE_OS::strchr (attname, ':');
                  const ACEXML_Char* ns_name = (name == 0)?empty_string:name+1;
                  if (this->xml_namespace_.declarePrefix (ns_name,
                                                          attvalue) == -1)
                    {
                      this->report_fatal_error(ACEXML_DUPPREFIX
                                               ACEXML_ENV_ARG_PARAMETER);
                      return;
                    }
                }
              if (this->namespace_prefixes_)
                {
                  // Namespace_prefixes_feature_ is required. So add the
                  // xmlns:foo to the list of attributes.
                  if (attributes.addAttribute (0, 0, attname,
                                               default_attribute_type,
                                               attvalue) == -1)
                    {
                      this->report_fatal_error(ACEXML_DUPATTR
                                               ACEXML_ENV_ARG_PARAMETER);
                      return;
                    }
                }
              if (!this->namespaces_ && !this->namespace_prefixes_)
                {
                  this->report_fatal_error(ACEXML_NSERR
                                           ACEXML_ENV_ARG_PARAMETER);
                  return;
                }
            }
          else
            {
              const ACEXML_Char *uri, *lName;
              this->xml_namespace_.processName (attname, uri, lName, 1);
              if (attributes.addAttribute (uri, lName, attname,
                                           default_attribute_type,
                                           attvalue) == -1)
                {
                  this->report_fatal_error(ACEXML_DUPATTR
                                           ACEXML_ENV_ARG_PARAMETER);
                  return;
                }
            }
          break;
        }
    }
  ACEXML_Char *cdata;
  size_t cdata_length = 0;

  // Parse element contents.
  while (1)
    {
      ACEXML_Char ch = this->get ();

      switch (ch)
        {
        case 0:
          this->report_fatal_error(ACEXML_EINT ACEXML_ENV_ARG_PARAMETER);
          return;
        case '<':
          // Push out old 'characters' event.
          if (cdata_length != 0)
            {
              cdata = this->obstack_.freeze ();
              this->content_handler_->characters (cdata, 0, cdata_length
                                                  ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK;
              this->obstack_.unwind (cdata);
              cdata_length = 0;
            }

          switch (this->peek ())
            {
            case '!':             // a comment or a CDATA section.
              this->get ();       // consume '!'
              ch = this->peek ();
              if (ch == '-')      // a comment
                {
                  if (this->grok_comment () < 0)
                    {
                      this->report_fatal_error(ACEXML_INVCO
                                               ACEXML_ENV_ARG_PARAMETER);
                      return;
                    }
                }
              else if (ch == '[') // a CDATA section.
                {
                  this->parse_cdata (ACEXML_ENV_SINGLE_ARG_PARAMETER);
                  ACEXML_CHECK;
                }
              else
                {
                  this->report_fatal_error(ACEXML_ECHAR
                                           ACEXML_ENV_ARG_PARAMETER);
                  return;
                }
              break;
            case '?':             // a PI.
              this->parse_processing_instruction (ACEXML_ENV_SINGLE_ARG_PARAMETER);
              ACEXML_CHECK;
              break;
            case '/':             // an ETag.
              {
                this->get ();       // consume '/'
                endname = this->read_name ();
                if (endname == 0 ||
                    ACE_OS::strcmp (startname, endname) != 0)
                  {
                    this->report_fatal_error(ACEXML_ETAG
                                             ACEXML_ENV_ARG_PARAMETER);
                    return ;
                  }
                if (this->skip_whitespace (0) != '>')
                  {
                    this->report_fatal_error(ACEXML_GREAT
                                             ACEXML_ENV_ARG_PARAMETER);
                    return;
                  }
                this->content_handler_->endElement (ns_uri, ns_lname, endname
                                                    ACEXML_ENV_ARG_PARAMETER);
                ACEXML_CHECK;
                prefix = ACE_const_cast (ACEXML_Char*,
                                         this->xml_namespace_.getPrefix(ns_uri));
                this->report_prefix_mapping (prefix, ns_uri, ns_lname, 0
                                             ACEXML_ENV_ARG_PARAMETER);
                    ACEXML_CHECK;
                if (new_namespace != 0)
                  this->xml_namespace_.popContext ();
                return;
              }
            default:              // a new nested element?
              this->parse_element (0 ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK;
              break;
            }
          break;
        case '&':
          {
            const ACEXML_String *replace = 0;
            ACEXML_String charval;
            ACEXML_Char buffer[6];

            if (this->peek () == '#')
              {
                if (this->parse_char_reference (buffer, 6) != 0)
                  {
                    this->report_fatal_error (ACEXML_INVCHAR
                                              ACEXML_ENV_ARG_PARAMETER);

                    return;
                  }
                charval.set (buffer, 0);
                replace = &charval;
              }
            else
              replace = this->parse_reference ();

            if (replace == 0)
              {
                this->report_fatal_error(ACEXML_EINT ACEXML_ENV_ARG_PARAMETER);
                return;
              }
                cdata_length = replace->length ();
                for (size_t i = 0; i < replace->length (); ++i)
                  this->obstack_.grow ((*replace)[i]);
          }
          break;
        case 0x0D:                // End-of-Line handling
          ch = (this->peek () == 0x0A ? this->get () : 0x0A);
          // Fall thru...
        case 0x0A:
          // Fall thru...
        default:
          ++cdata_length;
          cdata = this->obstack_.grow (ch);
          if (cdata == 0)
            {
              cdata = this->obstack_.freeze ();
              this->content_handler_->characters (cdata, 0, cdata_length
                                                  ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK;
              this->obstack_.grow (ch);
              cdata_length = 1;   // the missing char.
            }
        }
    }
  ACE_NOTREACHED (return;)
}

int
ACEXML_Parser::parse_char_reference (ACEXML_Char *buf, size_t len)
{
  if (this->get () != '#')      // Internal error.
      return -1;

  int hex = 0;
  if (this->peek () == 'x')
    {
      hex = 1;
      this->get ();
    }

  int more_digit = 0;
  ACEXML_UCS4 sum = 0;

  while (1)
    {
      ACEXML_Char ch = this->get ();
      switch (ch)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          sum = sum * (hex ? 16 : 10) + (ch - '0');
          break;
        case 'a':
        case 'A':
          if (!hex)
            return -1;
          sum = sum * 16 + 10;
          break;
        case 'b':
        case 'B':
          if (!hex)
            return -1;
          sum = sum * 16 + 11;
          break;
        case 'c':
        case 'C':
          if (!hex)
            return -1;
          sum = sum * 16 + 12;
          break;
        case 'd':
        case 'D':
          if (!hex)
            return -1;
          sum = sum * 16 + 13;
          break;
        case 'e':
        case 'E':
          if (!hex)
            return -1;
          sum = sum * 16 + 14;
          break;
        case 'f':
        case 'F':
          if (!hex)
            return -1;
          sum = sum * 16 + 15;
          break;
        case ';':
          if (more_digit == 0)  // no digit exist???
            return -1;
          int clen;
          // [WFC: Legal Character]
          if (sum == 0x9 || sum == 0xA || sum == 0xD
              || sum >= 0x20 && sum <= 0xD7FF
              || sum >= 0xE000 && sum <= 0xFFFD
              || sum >= 0x10000 && sum <= 0x10FFFF)
            {

#if defined (ACE_USES_WCHAR)    // UTF-16
          if ((clen = ACEXML_Transcoder::ucs42utf16 (sum, buf, len)) < 0)
            return -1;

#else                          // or UTF-8
          if ((clen = ACEXML_Transcoder::ucs42utf8 (sum, buf, len)) < 0)
            return -1;
#endif
          buf [clen] = 0;
          return 0;
            }
          return -1;
        default:
          return -1;
        }
      more_digit = 1;
    }
  ACE_NOTREACHED (return -1);
}

const ACEXML_String *
ACEXML_Parser::parse_reference (void)
{
  // @@ We'll use a temporary buffer here as the Obstack is most likely in
  // use when we are here. This puts a limit on the max length of a
  // reference.
  ACEXML_Char ref[MAXPATHLEN];

  size_t loc = 0;

  while (loc < MAXPATHLEN -1)
    {
      ACEXML_Char ch = this->get ();
      if (ch == ';')
        {
          ref[loc] = 0;
          break;
        }
      else
        ref[loc++] = ch;
    }

  return this->entities_.resolve_entity (ref);
}

int
ACEXML_Parser::parse_cdata (ACEXML_ENV_SINGLE_ARG_DECL)
{
  if (this->parse_token (ACE_TEXT ("[CDATA[")) < 0)
    {
      this->report_fatal_error(ACEXML_ECDATASEC ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char ch;
  int datalen = 0;
  ACEXML_Char *cdata = 0;
  while (1)
    {
      ch = this->get ();
      // Anything goes except the sequence "]]>".
      if (ch == ']' && this->peek() == ']')
        {
          ACEXML_Char temp = ch;
          ch = this->get();
          if (ch == ']' && this->peek() == '>')
            {
              ch = this->get();
              cdata = this->obstack_.freeze ();
              this->content_handler_->characters (cdata, 0, datalen
                                                  ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK_RETURN (-1);
              this->obstack_.unwind(cdata);
              return 0;
            }
          this->obstack_.grow (temp);
          ++datalen;
        }
      else if (ch == 0x0D)
                ch = (this->peek () == 0x0A ? this->get () : 0x0A);
              this->obstack_.grow (ch);
          ++datalen;
    };
  ACE_NOTREACHED (return -1);
}

int
ACEXML_Parser::parse_internal_dtd (ACEXML_ENV_SINGLE_ARG_DECL)
{
  ACEXML_Char nextch = this->skip_whitespace (0);

  do {
    switch (nextch)
      {
      case '<':                 // Start of markup Decl.
        nextch = this->peek ();
        switch (nextch)
          {
          case '!':
            this->get ();       // Discard '!'
            nextch = this->peek ();
            switch (nextch)
              {
              case 'E':         // An ELEMENT or ENTITY decl
                this->get ();
                nextch = this->peek ();
                switch (nextch)
                  {
                  case 'L':
                    if (this->parse_element_decl (ACEXML_ENV_SINGLE_ARG_PARAMETER) < 0)
                      return -1;
                    break;

                  case 'N':
                    if (this->parse_entity_decl (ACEXML_ENV_SINGLE_ARG_PARAMETER) < 0)
                      return -1;
                    break;

                  default:
                    this->report_fatal_error(ACEXML_MKDECLKEYWORD
                                             ACEXML_ENV_ARG_PARAMETER);
                    return -1;
                  }
                break;

              case 'A':         // An ATTLIST decl
                if (this->parse_attlist_decl (ACEXML_ENV_SINGLE_ARG_PARAMETER) < 0)
                  return -1;
                break;

              case 'N':         // A NOTATION decl
                if (this->parse_notation_decl (ACEXML_ENV_SINGLE_ARG_PARAMETER) < 0)
                  return -1;
                break;

              case '-':         // a comment.
                if (this->grok_comment () < 0)
                  {
                    this->report_fatal_error(ACEXML_INVCO
                                             ACEXML_ENV_ARG_PARAMETER);
                    return -1;
                  }
                break;
              case 0:
                this->report_fatal_error (ACEXML_EOF ACEXML_ENV_ARG_PARAMETER);
                return -1;
              default:
                this->report_fatal_error (ACEXML_MKINVCH
                                          ACEXML_ENV_ARG_PARAMETER);
                return -1;
              }
            break;

          case '?':             // PI
            this->parse_processing_instruction (ACEXML_ENV_SINGLE_ARG_PARAMETER);
            ACEXML_CHECK_RETURN (-1);
            break;

          case 0:
            this->report_fatal_error (ACEXML_EOF ACEXML_ENV_ARG_PARAMETER);
            return -1;
          default:
            this->report_fatal_error (ACEXML_MKINVCH ACEXML_ENV_ARG_PARAMETER);
            return -1;
          }
        break;

      case '%':                 // DeclSep.  Define new PEreference...
        break;

      case ']':                 // End of internal definitions.
        return 0;               // Not applicable when parsing external DTD spec.

      case 0:                   // This may not be an error if we decide
                                // to generalize this function to handle both
                                // internal and external DTD definitions.
        this->report_fatal_error (ACEXML_EOF ACEXML_ENV_ARG_PARAMETER);
        return -1;

      default:
        this->report_fatal_error (ACEXML_MKDECL ACEXML_ENV_ARG_PARAMETER);
        return -1;
      };

    // To fully conform with the spec., whitespaces are only allowed
    // following a 'DeclSep' section.  However, I found it
    // hard/impossible to eliminate all the whitespaces between
    // markupdecls.

    nextch = this->skip_whitespace (0);

  } while (1);

  ACE_NOTREACHED (return -1);
}

int
ACEXML_Parser::parse_element_decl (ACEXML_ENV_SINGLE_ARG_DECL)
{
  if ((this->parse_token (ACE_TEXT ("LEMENT")) < 0) ||
      this->skip_whitespace_count () == 0)
    {
      this->report_fatal_error (ACEXML_EELEMENT ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char *element_name = this->read_name ();
  if (element_name == 0)
    {
      this->report_fatal_error (ACEXML_ERDELENAME ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char nextch ;
  this->skip_whitespace_count (&nextch);

  switch (nextch)
    {
    case 'E':                   // EMPTY
      if (this->parse_token (ACE_TEXT ("EMPTY")) < 0)
        {
          this->report_fatal_error (ACEXML_EEMPTY ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      break;
    case 'A':                   // ANY
      if (this->parse_token (ACE_TEXT ("ANY")) < 0)
        {
          this->report_fatal_error (ACEXML_EANY ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      break;
    case '(':                   // children
      this->parse_children_definition (ACEXML_ENV_SINGLE_ARG_PARAMETER);
      ACEXML_CHECK_RETURN (-1);
      break;
    default:                    // error
      this->report_fatal_error (ACEXML_ERDELE ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }
  if (this->skip_whitespace (0) != '>')
    {
      this->report_fatal_error (ACEXML_GREAT ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }
  return 0;
}

int
ACEXML_Parser::parse_entity_decl (ACEXML_ENV_SINGLE_ARG_DECL)
{
  ACEXML_Char nextch;

  if ((this->parse_token (ACE_TEXT ("NTITY")) < 0) ||
      this->skip_whitespace_count (&nextch) == 0)
    {
      this->report_fatal_error (ACEXML_EENTITY ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  int is_GEDecl = 1;
  if (nextch == '%')            // This is a PEDecl.
    {
      is_GEDecl = 0;
      this->get ();             // consume the '%'
      if (this->skip_whitespace_count (&nextch) == 0)
        {
          this->report_fatal_error (ACEXML_INVREF ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
    }

  ACEXML_Char *entity_name = this->read_name ();
  if (entity_name == 0)
    {
      this->report_fatal_error (ACEXML_ENTNAME ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  this->skip_whitespace_count (&nextch);

  if (nextch == '\'' || nextch == '"')
    {
      ACEXML_Char *entity_value = 0;

      if (this->get_quoted_string (entity_value) != 0)
        {
          this->report_fatal_error(ACEXML_ENTVALUE ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }

      if (is_GEDecl)
        {
          if (this->entities_.add_entity (entity_name, entity_value) != 0)
            {
              this->report_fatal_error(ACEXML_DUPENT ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }
        }
      else
        {
          // @@ need to implement PEdecl lookup mechanism
          ACEXML_THROW_RETURN (ACEXML_SAXNotSupportedException (), -1);
        }
    }
  else
    {
      ACEXML_Char *systemid, *publicid;

      this->parse_external_id_and_ref (publicid, systemid ACEXML_ENV_ARG_PARAMETER);
      ACEXML_CHECK_RETURN (-1);
      if (systemid == 0)
        {
          this->report_fatal_error(ACEXML_INVEXTID ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      this->skip_whitespace_count (&nextch);
      if (nextch == 'N')        // NDATA section followed
        {
          if (is_GEDecl == 0)
            {
              this->report_fatal_error(ACEXML_UNDATA ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }

          if ((this->parse_token (ACE_TEXT ("NDATA")) < 0) ||
              this->skip_whitespace_count (&nextch) == 0)
            {
              this->report_fatal_error(ACEXML_ENDATA ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }

          ACEXML_Char *ndata = this->read_name ();
          this->dtd_handler_->unparsedEntityDecl (entity_name, publicid,
                                                  systemid, ndata
                                                  ACEXML_ENV_ARG_PARAMETER);
          ACEXML_CHECK_RETURN (-1);
        }
      else
        {
          // @@ Need to support external CharStream sources
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("ENTITY: (%s) "),
                      entity_name));

          if (publicid == 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("SYSTEM %s\n"),
                        systemid));
          else
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("PUBLIC %s %s\n"),
                        publicid, systemid));
        }
    }

  // End of ENTITY definition
  if (this->skip_whitespace (0) != '>')
    {
      this->report_fatal_error(ACEXML_GREAT ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }
  return 0;
}

int
ACEXML_Parser::parse_attlist_decl (ACEXML_ENV_SINGLE_ARG_DECL)
{
  if ((this->parse_token (ACE_TEXT ("ATTLIST")) < 0) ||
      this->skip_whitespace_count () == 0)
    {
      this->report_fatal_error(ACEXML_EATTLIST ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char *element_name = this->read_name ();
  if (element_name == 0)
    {
      this->report_fatal_error(ACEXML_ERDELENAME ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char nextch = this->skip_whitespace (0);

  // Parse AttDef*
  while (nextch != '>')
    {
      // Parse attribute name
      ACEXML_Char *att_name = this->read_name (nextch);
      if (att_name == 0)
        {
          this->report_fatal_error(ACEXML_EATTNAME ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }

      /*
        Parse AttType:
        Possible keywords:
        CDATA                   // StringType
        ID                      // TokenizedType
        IDREF
        IDREFS
        ENTITY
        ENTITIES
        NMTOKEN
        NMTOKENS
        NOTATION                // EnumeratedType - NotationTYpe
        (                       // EnumeratedType - Enumeration
      */
      nextch = this->skip_whitespace (0);
      switch (nextch)
        {
        case 'C':               // CDATA
          if ((this->parse_token (ACE_TEXT ("DATA")) < 0) ||
              this->skip_whitespace_count () == 0)
            {
              this->report_fatal_error(ACEXML_ECDATA ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }
          // Else, we have successfully identified the type of the
          // attribute as CDATA
          // @@ Set up validator appropriately here.
          break;
        case 'I':               // ID, IDREF, or, IDREFS
          if (this->get () == 'D')
            {
              if (this->skip_whitespace_count (&nextch) > 0)
                {
                  // We have successfully identified the type of the
                  // attribute as ID @@ Set up validator as such.
                  break;
                }
              if (this->parse_token (ACE_TEXT ("REF")) == 0)
                {
                  if (this->skip_whitespace_count (&nextch) > 0)
                    {
                      // We have successfully identified the type of
                      // the attribute as IDREF
                      // @@ Set up validator as such.
                      break;
                    }
                  else if (nextch == 'S' &&
                           this->get () && // consume the 'S'
                           this->skip_whitespace_count () != 0)
                    {
                      // We have successfully identified the type of
                      // the attribute as IDREFS
                      // @@ Set up validator as such.
                      break;
                    }
                }
            }
          // Admittedly, this error message is not precise enough
          this->report_fatal_error(ACEXML_EID ACEXML_ENV_ARG_PARAMETER);
          return -1;
        case 'E':               // ENTITY or ENTITIES
          if (this->parse_token (ACE_TEXT ("NTIT")) == 0)
            {
              nextch = this->get ();
              if (nextch == 'Y')
                {
                  // We have successfully identified the type of
                  // the attribute as ENTITY
                  // @@ Set up validator as such.
                }
              else if (nextch == 'I'&& this->get () == 'E' &&
                       this->get () == 'S')
                {
                  // We have successfully identified the type of
                  // the attribute as ENTITIES
                  // @@ Set up validator as such.
                }
              if (this->skip_whitespace_count () > 0)
                {
                  // success
                  break;
                }
            }
          // Admittedly, this error message is not precise enough
          this->report_fatal_error(ACEXML_EENTITIES ACEXML_ENV_ARG_PARAMETER);
          return -1;
        case 'N':               // NMTOKEN, NMTOKENS, or, NOTATION
          nextch = this->get ();
          if (nextch != 'M' || nextch != 'O')
            {
              this->report_fatal_error(ACEXML_ENMTOKEN
                                       ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }
          if (nextch == 'M')
            {
              if (this->parse_token (ACE_TEXT ("TOKEN")) == 0)
                {
                  if (this->skip_whitespace_count (&nextch) > 0)
                    {
                      // We have successfully identified the type of
                      // the attribute as NMTOKEN
                      // @@ Set up validator as such.
                      break;
                    }
                  else if (nextch == 'S' && this->skip_whitespace_count () > 0)
                    {
                      // We have successfully identified the type of
                      // the attribute as NMTOKENS
                      // @@ Set up validator as such.
                      break;
                    }
                }
              this->report_fatal_error(ACEXML_ENMTOKENS
                                       ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }
          else                  // NOTATION
            {
              if ((this->parse_token (ACE_TEXT ("TATION")) < 0) ||
                  this->skip_whitespace_count () == 0)
                {
                  this->report_fatal_error(ACEXML_ENOTATION
                                           ACEXML_ENV_ARG_PARAMETER);
                  return -1;
                }

              if (this->get () != '(')
                {
                  this->report_fatal_error(ACEXML_LPAREN
                                           ACEXML_ENV_ARG_PARAMETER);
                  return -1;
                }

              this->skip_whitespace_count ();

              do {
                ACEXML_Char *notation_name = this->read_name ();
                if (notation_name == 0)
                  {
                    this->report_fatal_error(ACEXML_ENOTNAME
                                             ACEXML_ENV_ARG_PARAMETER);
                    return -1;
                  }
                // @@ get another notation name, set up validator as such
                this->skip_whitespace_count (&nextch);
              } while (nextch != ')');

              this->get ();     // consume the closing paren.
              this->skip_whitespace_count ();
            }
          break;
        case '(':               // EnumeratedType - Enumeration
          this->skip_whitespace_count ();

          do {
            ACEXML_Char *token_name = this->read_name (); // @@ need a special read_nmtoken?
            if (token_name == 0)
              {
                this->report_fatal_error(ACEXML_ENMNAME
                                         ACEXML_ENV_ARG_PARAMETER);
                return -1;
              }
            // @@ get another nmtoken, set up validator as such
            this->skip_whitespace_count (&nextch);
          } while (nextch != ')');

          this->get ();     // consume the closing paren.
          this->skip_whitespace_count ();
          break;
        default:
          {
            this->report_fatal_error(ACEXML_INVATTRTYPE
                                     ACEXML_ENV_ARG_PARAMETER);
            return -1;
          }
          ACE_NOTREACHED (break);
        }

      /*
        Parse DefaultDecl:
        #REQUIRED
        #IMPLIED
        #FIXED
        quoted string           // #FIXED
      */
      nextch = this->peek ();
      switch (nextch)
        {
        case '#':
          this->get ();         // consume the '#'
          switch (this->get ())
            {
            case 'R':
              if (this->parse_token (ACE_TEXT ("EQUIRED")) < 0)
                {
                  this->report_fatal_error(ACEXML_EREQUIRED
                                           ACEXML_ENV_ARG_PARAMETER);
                  return -1;
                }
              // We now know this attribute is required
              // @@ Set up the validator as such.
              break;
            case 'I':
              if (this->parse_token (ACE_TEXT ("MPLIED")) < 0)
                {
                  this->report_fatal_error(ACEXML_EIMPLIED
                                           ACEXML_ENV_ARG_PARAMETER);
                  return -1;
                }
              // We now know this attribute is impleid.
              // @@ Set up the validator as such.
              break;
            case 'F':
              if (this->parse_token (ACE_TEXT ("IXED")) < 0 ||
                  this->skip_whitespace_count () == 0)
                {
                  this->report_fatal_error(ACEXML_EFIXED
                                           ACEXML_ENV_ARG_PARAMETER);
                  return -1;
                }
              // We now know this attribute is fixed.

              ACEXML_Char *fixed_attr;
              if (this->get_quoted_string (fixed_attr) != 0)
                {
                  this->report_fatal_error(ACEXML_EINVFIXED
                                           ACEXML_ENV_ARG_PARAMETER);
                  return -1;
                }
              // @@ set up validator
              break;
            default:
              break;
            }
          break;
        case '\'':
        case '"':
          ACEXML_Char *fixed_attr;
          if (this->get_quoted_string (fixed_attr) != 0)
            {
              this->report_fatal_error(ACEXML_EINVFIXED
                                       ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }
          // @@ set up validator
          break;
        default:
          break;
        }
      this->skip_whitespace_count (&nextch);
    }

  this->get ();                 // consume closing '>'

  return 0;
}

int
ACEXML_Parser::parse_notation_decl (ACEXML_ENV_SINGLE_ARG_DECL)
{
  if (this->parse_token (ACE_TEXT ("NOTATION")) < 0 ||
      this->skip_whitespace_count () == 0)
    {
      this->report_fatal_error(ACEXML_ENOTATION ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char *notation = this->read_name ();
  if (notation == 0)
    {
      this->report_fatal_error(ACEXML_INVNOTNAME ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  this->skip_whitespace_count ();
  ACEXML_Char *systemid, *publicid;

  this->parse_external_id_and_ref (publicid, systemid ACEXML_ENV_ARG_PARAMETER);
  ACEXML_CHECK_RETURN (-1);

  if (this->get () != '>')
    {
      this->report_fatal_error(ACEXML_GREAT ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  this->dtd_handler_->notationDecl (notation,
                                    publicid,
                                    systemid ACEXML_ENV_ARG_PARAMETER);
  ACEXML_CHECK_RETURN (-1);

  return 0;
}

int
ACEXML_Parser::parse_external_id_and_ref (ACEXML_Char *&publicId,
                                          ACEXML_Char *&systemId ACEXML_ENV_ARG_DECL)
{
  publicId = systemId = 0;
  ACEXML_Char nextch = this->get ();

  switch (nextch)
    {
    case 'S':                   // External SYSTEM id.
      if (this->parse_token (ACE_TEXT ("YSTEM")) < 0 ||
          this->skip_whitespace_count () == 0)
        {
          this->report_fatal_error(ACEXML_ESYSTEM ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      if (this->get_quoted_string (systemId) != 0)
        {
          this->report_fatal_error(ACEXML_ELITERAL ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      this->locator_.setSystemId (systemId);
      break;
    case 'P':                   // External PUBLIC id or previously defined PUBLIC id.
      if (this->parse_token (ACE_TEXT ("UBLIC")) < 0 ||
          this->skip_whitespace_count () == 0)
        {
          this->report_fatal_error(ACEXML_EPUBLIC ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      if (this->get_quoted_string (publicId) != 0)
        {
          this->report_fatal_error(ACEXML_ELITERAL ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      this->locator_.setPublicId (publicId);

      this->skip_whitespace_count (&nextch);
      if (nextch == '\'' || nextch == '"') // not end of NOTATION yet.
        {
          if (this->get_quoted_string (systemId) != 0)
            {
              this->report_fatal_error(ACEXML_ELITERAL
                                       ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }
          this->locator_.setSystemId (systemId);
        }
      break;
    default:
      this->report_fatal_error(ACEXML_ESYSPUB ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }
  return 0;
}

int
ACEXML_Parser::parse_children_definition (ACEXML_ENV_SINGLE_ARG_DECL)
{
  this->get ();                 // consume the '('

  ACEXML_Char nextch;
  int subelement_number = 0;
  this->skip_whitespace_count (&nextch);

  switch (nextch)
    {
    case '#':                   // Mixed element,
      if (this->parse_token (ACE_TEXT ("#PCDATA")) < 0)
        {
          this->report_fatal_error(ACEXML_EPCDATA ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }

      this->skip_whitespace_count (&nextch);

      while (nextch != ')')
        {
          if (this->get () != '|')
            {
              this->report_fatal_error(ACEXML_EMIXED ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }
          this->skip_whitespace_count ();

          ACEXML_Char *name = this->read_name ();
          // @@ name will be used in the Validator later.
          ACE_UNUSED_ARG (name);
          ++subelement_number;
          // @@ Install Mixed element name into the validator.
          this->skip_whitespace_count (&nextch);
        }

      if (this->get () != ')' ||
          (subelement_number && this->get () != '*'))
        {
          this->report_fatal_error(ACEXML_ERPAREN ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      // @@ close the element definition in the validator.
      break;
    default:
      int status = this->parse_child (1 ACEXML_ENV_ARG_PARAMETER);
      ACEXML_CHECK_RETURN (-1);
      if (status != 0)
        return -1;
    }

  return 0;
}

int
ACEXML_Parser::parse_child (int skip_open_paren ACEXML_ENV_ARG_DECL)
{
  // Conditionally consume the open paren.
  if (skip_open_paren == 0 &&
      this->get () != '(')
    {
      this->report_fatal_error(ACEXML_LPAREN ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char node_type = 0;
  ACEXML_Char nextch;

  do {
    this->skip_whitespace_count (&nextch);
    switch (nextch)
      {
      case '(':
        this->parse_child (0 ACEXML_ENV_ARG_PARAMETER);
        ACEXML_CHECK_RETURN (-1);
        break;
      default:
        // must be an element name here.
        ACEXML_Char *subelement = this->read_name ();
        if (subelement == 0)
          {
            this->report_fatal_error(ACEXML_ESUBELE ACEXML_ENV_ARG_PARAMETER);
            return -1;
          }
        // @@ Inform validator of the new element here.
        break;
      }

    this->skip_whitespace_count (&nextch);
    switch (nextch)
      {
      case '|':
        switch (node_type)
          {
          case 0:
            node_type = '|';
            // @@ inform validator of this new type??
            break;
          case '|':
            break;
          default:
            this->report_fatal_error(ACEXML_ECHOICE ACEXML_ENV_ARG_PARAMETER);
            return -1;
          }
        break;
      case ',':
        switch (node_type)
          {
          case 0:
            node_type = ',';
            // @@ inform validator of this new type??
            break;
          case ',':
            break;
          default:
            this->report_fatal_error(ACEXML_ECHOICE ACEXML_ENV_ARG_PARAMETER);
            return -1;
          }
      case ')':
        break;
      default:
        this->report_fatal_error(ACEXML_ECHOICE ACEXML_ENV_ARG_PARAMETER);
        return -1;
      }
    this->get ();               // consume , | or )
  } while (nextch != ')');

  // Check for trailing '?', '*', '+'
  nextch = this->peek ();
  switch (nextch)
    {
    case '?':
      // @@ Consume the character and inform validator as such,
      this->get ();
      break;
    case '*':
      // @@ Consume the character and inform validator as such,
      this->get ();
      break;
    case '+':
      // @@ Consume the character and inform validator as such,
      this->get ();
      break;
    default:
      break;                    // not much to do.
    }

  return 0;
}

ACEXML_Char
ACEXML_Parser::skip_whitespace (ACEXML_Char **whitespace)
{
  ACEXML_Char ch = this->get ();

  if (this->is_whitespace (ch) == 0)
    {
      if (whitespace != 0)
        *whitespace = 0;
      return ch;
    }

  do
    {
      if (whitespace != 0)
        this->obstack_.grow (ch);
      ch = this->get ();
    }
  while (this->is_whitespace (ch));

  if (whitespace != 0)
    *whitespace = this->obstack_.freeze ();

  return ch;
}

int
ACEXML_Parser::skip_whitespace_count (ACEXML_Char *peeky)
{
  int wscount = 0;
  ACEXML_Char dummy;
  ACEXML_Char &forward = (peeky == 0 ? dummy : *peeky);

  for (;this->is_whitespace ((forward = this->peek ())); ++wscount)
    this->get ();

  return wscount;
}

int
ACEXML_Parser::parse_token (const ACEXML_Char* keyword)
{
  if (keyword == 0)
    return -1;
  const ACEXML_Char* ptr = keyword;
  ACEXML_Char ch;
  for (; *ptr != 0 && ((ch = this->get()) == *ptr); ++ptr)
    ;
  if (*ptr == 0)
    return 0;
  else
    return -1;
}

int
ACEXML_Parser::skip_equal (void)
{
  if (this->skip_whitespace (0) != '=')
    return -1;

  while (this->is_whitespace (this->peek ()))
    this->get ();
  return 0;
}

int
ACEXML_Parser::get_quoted_string (ACEXML_Char *&str)
{
  ACEXML_Char quote = this->get ();
  if (quote != '\'' && quote != '"')  // Not a quoted string.
    return -1;

  while (1)
    {
      ACEXML_Char ch = this->get ();

      // @@ Deoes not handle buffer overflow yet.
      if (ch == quote)
        {
          str = this->obstack_.freeze ();
          return 0;
        }

      const ACEXML_String *replace = 0;
      ACEXML_String charval;
      ACEXML_Char buffer[6];

      switch (ch)
        {
        case '&':
          if (this->peek () == '#')
            {
              if (this->parse_char_reference (buffer, 6) != 0)
                {
                  // [WFC: Legal Character]
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("Invalid character reference\n")));
                  return -1;
                }
              charval.set (buffer, 0);
              replace = &charval;
            }
          else
            replace = this->parse_reference ();

          if (replace == 0)
            {
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("Undefined reference\n")));
              return -1;
            }
          for (size_t i = 0; i < replace->length (); ++i)
            this->obstack_.grow ((*replace)[i]);
          // handle reference here.
          break;
        case 0x0D:                // End-of-Line handling
          ch = (this->peek () == 0x0A ? this->get () : 0x0A);
          // Fall thru...
        case 0x0A:
          // Fall thru...
        default:
          this->obstack_.grow (ch);
          break;
        }
    }
}

ACEXML_Char *
ACEXML_Parser::read_name (ACEXML_Char ch)
{
  if (ch == 0)
    {
      ch = this->get ();

      if (this->is_whitespace (ch))
        // No white space is allowed here.
        return 0;
    }
  else if (this->is_nonname (ch))
    return 0;

  while (1)
    {
      this->obstack_.grow (ch);
      ch = this->peek ();
      if (this->is_nonname (ch))
        break;
      ch = this->get ();
    };

  return this->obstack_.freeze ();
}

void
ACEXML_Parser::report_prefix_mapping (const ACEXML_Char* prefix,
                                      const ACEXML_Char* uri,
                                      const ACEXML_Char* name,
                                      int start ACEXML_ENV_ARG_DECL)
{
  if (this->namespaces_)
    {
      const ACEXML_Char* temp = (name == 0) ? empty_string : prefix;
      if (start) {
        this->content_handler_->startPrefixMapping (temp, uri ACEXML_ENV_ARG_PARAMETER);
        ACEXML_CHECK;
      }
      else
        {
          this->content_handler_->endPrefixMapping(temp ACEXML_ENV_ARG_PARAMETER);
          ACEXML_CHECK;
        }
    }
}
