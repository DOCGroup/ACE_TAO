// $Id$

#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/Transcode.h"
#include "ACEXML/common/AttributesImpl.h"
#include "ace/ACE.h"

static const ACEXML_Char default_attribute_type[] = {'C', 'D', 'A', 'T', 'A', 0};
static const ACEXML_Char empty_string[] = { 0 };

const ACEXML_Char
ACEXML_Parser::simple_parsing_feature_[] = { 'S', 'i', 'm', 'p', 'l', 'e', 0 };

const ACEXML_Char
ACEXML_Parser::namespaces_feature_[] = {'h', 't', 't', 'p', ':', '/', '/', 'x', 'm', 'l', '.', 'o', 'r', 'g', '/', 's', 'a', 'x', '/', 'f', 'e', 'a', 't', 'u', 'r', 'e', 's', '/', 'n', 'a', 'm', 'e', 's', 'p', 'a', 'c', 'e', 's', 0 };

const ACEXML_Char
ACEXML_Parser::namespace_prefixes_feature_[] = {'h', 't', 't', 'p', ':', '/', '/', 'x', 'm', 'l', '.', 'o', 'r', 'g', '/', 's', 'a', 'x', '/', 'f', 'e', 'a', 't', 'u', 'r', 'e', 's', '/', 'n', 'a', 'm', 'e', 's', 'p', 'a', 'c', 'e', '-', 'p', 'r', 'e', 'f', 'i', 'x', 'e', 's', 0 };

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/Parser.i"
#endif /* __ACEXML_INLINE__ */

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
  if (ACE_OS::strcmp (name,
                             ACEXML_Parser::simple_parsing_feature_) == 0)
    {
      return this->simple_parsing_;
    }
  else if (ACE_OS::strcmp (name,
                                  ACEXML_Parser::namespaces_feature_) == 0)
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
  if (ACE_OS::strcmp (name,
                             ACEXML_Parser::simple_parsing_feature_) == 0)
    {
      this->simple_parsing_ = (boolean_value == 0 ? 0 : 1);
      return;
    }
  else if (ACE_OS::strcmp (name,
                                  ACEXML_Parser::namespaces_feature_) == 0)
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
ACEXML_Parser::report_error (const ACEXML_Char* message ACEXML_ENV_ARG_DECL)
{
  ACEXML_SAXParseException* exception = 0;
  ACE_NEW_NORETURN (exception,
                    ACEXML_SAXParseException (message));
  if (this->error_handler_)
    this->error_handler_->error (*exception ACEXML_ENV_ARG_PARAMETER);
  else
    ACEXML_ENV_RAISE (exception);
  return;
}

void
ACEXML_Parser::report_warning (const ACEXML_Char* message ACEXML_ENV_ARG_DECL)
{
  ACEXML_SAXParseException* exception = 0;
  ACE_NEW_NORETURN (exception,
                    ACEXML_SAXParseException (message));
  if (this->error_handler_)
    this->error_handler_->warning (*exception ACEXML_ENV_ARG_PARAMETER);
  return;
}

void
ACEXML_Parser::report_fatal_error (const ACEXML_Char* message ACEXML_ENV_ARG_DECL)
{
  ACEXML_SAXParseException* exception = 0;
  ACE_NEW_NORETURN (exception,
                    ACEXML_SAXParseException (message));
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
      this->report_fatal_error(ACE_TEXT ("Invalid input source") ACEXML_ENV_ARG_PARAMETER);
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
          this->report_fatal_error (ACE_TEXT ("Expecting '<'") ACEXML_ENV_ARG_PARAMETER);
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
                  this->report_fatal_error(ACE_TEXT ("Invalid comment") ACEXML_ENV_ARG_PARAMETER);
                  return;
                }
            }
          else
            {
              this->report_fatal_error (ACE_TEXT ("Duplicate DOCTYPE definitions") ACEXML_ENV_ARG_PARAMETER);
              return;
            }
          break;
        case '?':
          this->parse_processing_instruction (ACEXML_ENV_SINGLE_ARG_PARAMETER);
          ACEXML_CHECK;
          break;
        case 0:
          this->report_fatal_error (ACE_TEXT ("Unexpected EOF") ACEXML_ENV_ARG_PARAMETER);
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
      this->report_fatal_error(ACE_TEXT ("Invalid XMLDecl ('<?xml' ?)") ACEXML_ENV_ARG_PARAMETER);
      return;
    }

  ACEXML_Char *astring;

  if (this->skip_whitespace (0) != 'v' // Discard whitespace
      || (this->parse_token (ACE_TEXT("ersion")) < 0)
      || this->skip_equal () != 0
      || this->get_quoted_string (astring) != 0)
    {
      this->report_fatal_error (ACE_TEXT ("Unrecognized XMLDecl ('version'?)") ACEXML_ENV_ARG_PARAMETER);
      return;
    }
  // @@ Handle version number here.
  int xmldecl_state = 0;
  int seen_encoding = 0;

  while (1)
    {
      ACEXML_Char fwd = this->peek ();
      if (fwd != '?')
        {
          fwd = this->skip_whitespace (0); // Discard whitespace
          if (fwd == '?')
            {
              // Fall down to consume the '?' and wrap up the XML Decl parsing.
            }
          else if (xmldecl_state == 0 && fwd == 'e')
            {
              if ((this->parse_token (ACE_TEXT("ncoding")) == 0) &&
                  this->skip_equal () == 0 &&
                  this->get_quoted_string (astring) == 0)
                {
                  if (seen_encoding)
                    {
                      this->report_fatal_error (ACE_TEXT ("Duplicate encoding defined") ACEXML_ENV_ARG_PARAMETER);
                      return;
                    }
                  else
                    {
                    seen_encoding = 1;
                      if (ACE_OS::strcmp (astring,
                                          this->instream_->getEncoding()) != 0)
                        {
                          if (ACE_OS::strstr (astring,
                                              this->instream_->getEncoding()) != 0)
                            {
                              ACE_ERROR ((LM_ERROR,
                                          ACE_TEXT ("Detected Encoding is %s : Declared Encoding is %s"),
                                          this->instream_->getEncoding(), astring));
                              this->report_fatal_error (ACE_TEXT ("Encoding declaration doesn't match detected encoding") ACEXML_ENV_ARG_PARAMETER);
                              return;
                            }
                        }
                    }
                  continue;
                }
              else
                break;
            }
          else if (xmldecl_state < 2 && fwd == 's')
            {
              if ((this->parse_token (ACE_TEXT("tandalone")) == 0) &&
                  this->skip_equal () == 0 &&
                  this->get_quoted_string (astring) == 0)
                {
                  xmldecl_state = 2;
                  if (ACE_OS::strcmp (astring, ACE_TEXT ("yes")) == 0)
                    {
                      // @@ This is a standalone XML file.
                      continue;
                    }
                  else if (ACE_OS::strcmp (astring, ACE_TEXT ("no")) == 0)
                    {
                      // @@ This is not a stand alone XML file.
                      continue;
                    }
                }
              break;
            }
          else
            break;
        }
      if (this->parse_token (ACE_TEXT ("?>")) < 0)
        break;
      return;
    }   // End parsing XML Decl.
  this->report_fatal_error (ACE_TEXT ("Unrecognized XML Decl ('standalone'?)") ACEXML_ENV_ARG_PARAMETER);
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
      this->report_fatal_error(ACE_TEXT ("Internal error") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }
  const ACEXML_Char *pitarget = this->read_name ();
  ACEXML_Char *instruction = 0;

  if (ACE_OS::strcasecmp (ACE_TEXT ("xml"), pitarget) != 0)
    {
      // Invalid PITarget name.
      this->report_fatal_error(ACE_TEXT ("PITarget name cannot start with 'xml'") ACEXML_ENV_ARG_PARAMETER);
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
                                                             instruction ACEXML_ENV_ARG_PARAMETER);
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
      this->report_fatal_error(ACE_TEXT ("Expecting keyword 'DOCTYPE'") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char nextch = this->skip_whitespace (0);
  if (nextch == 0)
    {
      this->report_fatal_error(ACE_TEXT ("Expecting a DOCTYPE name") ACEXML_ENV_ARG_PARAMETER);
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
      this->report_fatal_error (ACE_TEXT ("Unexpected EOF") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    default:
      break;
    }

  if (this->skip_whitespace (0) != '>')
    {
      this->report_fatal_error(ACE_TEXT ("Internal error") ACEXML_ENV_ARG_PARAMETER);
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
      this->report_fatal_error (ACE_TEXT ("Unexpected EOF") ACEXML_ENV_ARG_PARAMETER);
      return;
    }
  if (is_root && this->doctype_ != 0
      && ACE_OS::strcmp (startname, this->doctype_) != 0)
    {
      this->report_fatal_error (ACE_TEXT ("Root element missing") ACEXML_ENV_ARG_PARAMETER);
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
          this->report_fatal_error(ACE_TEXT ("Internal error") ACEXML_ENV_ARG_PARAMETER);
          return;
        case '/':
          if (this->get () != '>')
            {
              this->report_fatal_error(ACE_TEXT ("Expecting '>'") ACEXML_ENV_ARG_PARAMETER);
              return;
            }
          else
            {
              this->xml_namespace_.processName(startname, ns_uri, ns_lname, 0);
              prefix = ACE_const_cast (ACEXML_Char*,
                                       this->xml_namespace_.getPrefix(ns_uri));
              this->report_prefix_mapping (prefix, ns_uri, ns_lname, 1 ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK;
              this->content_handler_->startElement (ns_uri, ns_lname,
                                                    startname, &attributes ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK;
              this->content_handler_->endElement (ns_uri, ns_lname, startname ACEXML_ENV_ARG_PARAMETER);
              ACEXML_CHECK;
              this->report_prefix_mapping (prefix, ns_uri, ns_lname, 0 ACEXML_ENV_ARG_PARAMETER);
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
            this->report_prefix_mapping (prefix, ns_uri, ns_lname, 1 ACEXML_ENV_ARG_PARAMETER);
            ACEXML_CHECK;
            this->content_handler_->startElement (ns_uri, ns_lname, startname,
                                                  &attributes ACEXML_ENV_ARG_PARAMETER);
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
              this->report_fatal_error(ACE_TEXT ("Error reading attribute") ACEXML_ENV_ARG_PARAMETER);
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
                      this->report_fatal_error(ACE_TEXT ("Duplicate namespace prefix") ACEXML_ENV_ARG_PARAMETER);
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
                      this->report_fatal_error(ACE_TEXT ("Duplicate attribute found") ACEXML_ENV_ARG_PARAMETER);
                      return;
                    }
                }
              if (!this->namespaces_ && !this->namespace_prefixes_)
                {
                  this->report_fatal_error(ACE_TEXT ("Both namespaces feature and namespace_prefixes feature are false. Illegal Mode") ACEXML_ENV_ARG_PARAMETER);
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
                  this->report_fatal_error(ACE_TEXT ("Duplicate attribute found") ACEXML_ENV_ARG_PARAMETER);
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
          this->report_fatal_error(ACE_TEXT ("Internal error") ACEXML_ENV_ARG_PARAMETER);
          return;
        case '<':
          // Push out old 'characters' event.
          if (cdata_length != 0)
            {
              cdata = this->obstack_.freeze ();
              this->content_handler_->characters (cdata, 0,
                                         ACE_static_cast (int, cdata_length)
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
                      this->report_fatal_error(ACE_TEXT ("Error parsing comment") ACEXML_ENV_ARG_PARAMETER);
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
                  this->report_fatal_error(ACE_TEXT ("Unexpected character") ACEXML_ENV_ARG_PARAMETER);
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
                    this->report_fatal_error(ACE_TEXT ("Mismatched End-tag encountered") ACEXML_ENV_ARG_PARAMETER);
                    return ;
                  }
                if (this->skip_whitespace (0) != '>')
                  {
                    this->report_fatal_error(ACE_TEXT ("Expecting '>' in an end-tag") ACEXML_ENV_ARG_PARAMETER);
                    return;
                  }
                this->content_handler_->endElement (ns_uri, ns_lname, endname ACEXML_ENV_ARG_PARAMETER);
                ACEXML_CHECK;
                prefix = ACE_const_cast (ACEXML_Char*,
                                         this->xml_namespace_.getPrefix(ns_uri));
                this->report_prefix_mapping (prefix, ns_uri, ns_lname, 0 ACEXML_ENV_ARG_PARAMETER);
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
                    // not referring to any character exception?
                    return;
                  }
                charval.set (buffer, 0);
                replace = &charval;
              }
            else
              replace = this->parse_reference ();

            if (replace == 0)
              {
                this->report_fatal_error(ACE_TEXT ("Internal error") ACEXML_ENV_ARG_PARAMETER);
                return;
              }
//             if (this->try_grow_cdata (replace->length (),
//                                       cdata_length, xmlenv) == 0)
//               {
                cdata_length += replace->length ();
                for (size_t i = 0; i < replace->length (); ++i)
                  this->obstack_.grow ((*replace)[i]);
//              }
//            else
//              return;
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
              this->content_handler_->characters (cdata, 0,
                                           ACE_static_cast (int, cdata_length)
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
  if (this->get () != '#')
    {
      // Internal error.
      return -1;
    }

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
#if defined (ACE_USES_WCHAR)    // UTF-16
          if ((clen = ACEXML_Transcoder::ucs42utf16 (sum, buf, len)) < 0)
            return -1;

#elif 1                         // or UTF-8
          if ((clen = ACEXML_Transcoder::ucs42utf8 (sum, buf, len)) < 0)
            return -1;
          //  #elif 0                         // UCS 4, not likely
          //            buf [0] = sum;
          //            buf [1] = 0;
#endif
          buf [clen] = 0;
          return 0;
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
      this->report_fatal_error(ACE_TEXT ("'[CDATA[' expected") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  int parse_state = 0;
  size_t datalen = 0;

  while (1)
    {
      ACEXML_Char ch;
      ACEXML_Char *cdata;

      ch = this->get ();
      // Anything goes except the sequence "]]>".
      switch (parse_state)
        {
        case 2:
          if (ch == ']')
            {
              parse_state = 3;
              continue;
            }
          break;
        case 3:
          if (ch == '>')        // Yay!
            {
              cdata = this->obstack_.freeze ();
              this->content_handler_->characters (cdata, 0,
                                                  ACE_static_cast (int, datalen)
                                                  ACEXML_ENV_ARG_PARAMETER);
              // ACEXML_CHECK_RETURN (-1);
              this->obstack_.unwind(cdata);
              return 0;
            }
          break;
        default:
          if (ch == ']')
            {
              parse_state = 2;
              continue;
            }
          else
            parse_state = 1;
        }
      while (parse_state > 0)
        {
          if (this->try_grow_cdata (1, datalen ACEXML_ENV_ARG_PARAMETER) < 0)
            return -1;

          if (parse_state != 1)
            this->obstack_.grow (']');
          else
            {
              if (ch == 0x0D)
                ch = (this->peek () == 0x0A ? this->get () : 0x0A);
              this->obstack_.grow (ch);
            }
          ++datalen;
          --parse_state;
        }
    };
  ACE_NOTREACHED (return -1);
}

int
ACEXML_Parser::try_grow_cdata (size_t size, size_t &len ACEXML_ENV_ARG_DECL)
{
  if (this->obstack_.request (size) != 0)
    {
      if (len != 0)
        {
          ACEXML_Char *cdata = this->obstack_.freeze ();
          if (cdata == 0)
            {
              this->report_fatal_error(ACE_TEXT ("Internal Error growing CDATA buffer") ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }
          this->content_handler_->characters (cdata,
                                              0,
                                              ACE_static_cast (int, len)
                                              ACEXML_ENV_ARG_PARAMETER);
          ACEXML_CHECK_RETURN (-1);
          len = 0;              // reset counter
          if (this->obstack_.request (size) == 0)
            return 0;
        }
      this->report_fatal_error(ACE_TEXT ("Internal Error, buffer overflowed") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }
  return 0;
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
                    this->report_fatal_error(ACE_TEXT ("Invalid keyword in decl spec") ACEXML_ENV_ARG_PARAMETER);
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
                    this->report_fatal_error(ACE_TEXT ("Error parsing comment") ACEXML_ENV_ARG_PARAMETER);
                    return -1;
                  }
                break;
              case 0:
                this->report_fatal_error (ACE_TEXT ("Unexpected EOF") ACEXML_ENV_ARG_PARAMETER);
                return -1;
              default:
                this->report_fatal_error (ACE_TEXT ("Invalid char. follows '<!' in markupdecl") ACEXML_ENV_ARG_PARAMETER);
                return -1;
              }
            break;

          case '?':             // PI
            this->parse_processing_instruction (ACEXML_ENV_SINGLE_ARG_PARAMETER);
            ACEXML_CHECK_RETURN (-1);
            break;

          case 0:
            this->report_fatal_error (ACE_TEXT ("Unexpected EOF") ACEXML_ENV_ARG_PARAMETER);
            return -1;
          default:
            this->report_fatal_error (ACE_TEXT ("Invalid char. follows '<!' in markupdecl") ACEXML_ENV_ARG_PARAMETER);
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
        this->report_fatal_error (ACE_TEXT ("Unexpected EOF") ACEXML_ENV_ARG_PARAMETER);
        return -1;

      default:
        this->report_fatal_error (ACE_TEXT ("Expecting markupdecl or DecSep") ACEXML_ENV_ARG_PARAMETER);
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
      this->report_fatal_error (ACE_TEXT ("Expecting keyword `ELEMENT'") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char *element_name = this->read_name ();
  if (element_name == 0)
    {
      this->report_fatal_error (ACE_TEXT ("Error reading element name while defining ELEMENT.") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char nextch ;
  this->skip_whitespace_count (&nextch);

  switch (nextch)
    {
    case 'E':                   // EMPTY
      if (this->parse_token (ACE_TEXT ("EMPTY")) < 0)
        {
          this->report_fatal_error (ACE_TEXT ("Expecting keyword `EMPTY' in ELEMENT definition.") ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      break;
    case 'A':                   // ANY
      if (this->parse_token (ACE_TEXT ("ANY")) < 0)
        {
          this->report_fatal_error (ACE_TEXT ("Expecting keyword `ANY' in ELEMENT definition.") ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      break;
    case '(':                   // children
      this->parse_children_definition (ACEXML_ENV_SINGLE_ARG_PARAMETER);
      ACEXML_CHECK_RETURN (-1);
      break;
    default:                    // error
      this->report_fatal_error (ACE_TEXT ("Error reading ELEMENT definition.") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }
  if (this->skip_whitespace (0) != '>')
    {
      this->report_fatal_error (ACE_TEXT ("Expecting '>' in ELEMENT definition.") ACEXML_ENV_ARG_PARAMETER);
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
      this->report_fatal_error (ACE_TEXT ("Expecting keyword `ENTITY'") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  int is_GEDecl = 1;
  if (nextch == '%')            // This is a PEDecl.
    {
      is_GEDecl = 0;
      this->get ();             // consume the '%'
      if (this->skip_whitespace_count (&nextch) == 0)
        {
          this->report_fatal_error (ACE_TEXT ("Can't use a reference when defining entity name") ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
    }

  ACEXML_Char *entity_name = this->read_name ();
  if (entity_name == 0)
    {
      this->report_fatal_error (ACE_TEXT ("Error reading ENTITY name.") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  this->skip_whitespace_count (&nextch);

  if (nextch == '\'' || nextch == '"')
    {
      ACEXML_Char *entity_value = 0;

      if (this->get_quoted_string (entity_value) != 0)
        {
          this->report_fatal_error(ACE_TEXT("Error reading ENTITY value.") ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }

      if (is_GEDecl)
        {
          if (this->entities_.add_entity (entity_name, entity_value) != 0)
            {
              this->report_fatal_error(ACE_TEXT("Error storing entity definition (duplicate definition?)") ACEXML_ENV_ARG_PARAMETER);
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
          this->report_fatal_error(ACE_TEXT("Invalid ExternalID definition (system ID missing.)") ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      this->skip_whitespace_count (&nextch);
      if (nextch == 'N')        // NDATA section followed
        {
          if (is_GEDecl == 0)
            {
              this->report_fatal_error(ACE_TEXT("Unexpected keyword NDATA in PEDecl.") ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }

          if ((this->parse_token (ACE_TEXT ("NDATA")) < 0) ||
              this->skip_whitespace_count (&nextch) == 0)
            {
              this->report_fatal_error(ACE_TEXT("Expecting keyword NDATA") ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }

          ACEXML_Char *ndata = this->read_name ();
          this->dtd_handler_->unparsedEntityDecl (entity_name,
                                                  publicid,
                                                  systemid,
                                                  ndata ACEXML_ENV_ARG_PARAMETER);
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
      this->report_fatal_error(ACE_TEXT("Expecting end of ENTITY definition.") ACEXML_ENV_ARG_PARAMETER);
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
      this->report_fatal_error(ACE_TEXT("Expecting keyword `ATTLIST'") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char *element_name = this->read_name ();
  if (element_name == 0)
    {
      this->report_fatal_error(ACE_TEXT("Error reading element name while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
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
          this->report_fatal_error(ACE_TEXT("Error reading attribute name while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
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
              this->report_fatal_error(ACE_TEXT("Expecting keyword `CDATA' while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
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
          this->report_fatal_error(ACE_TEXT("Expecting keyword `ID', `IDREF', or `IDREFS' while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
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
          this->report_fatal_error(ACE_TEXT("Expecting keyword `ENTITY', or `ENTITIES' while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
          return -1;
        case 'N':               // NMTOKEN, NMTOKENS, or, NOTATION
          nextch = this->get ();
          if (nextch != 'M' || nextch != 'O')
            {
              this->report_fatal_error(ACE_TEXT("Expecting keyword `NMTOKEN', `NMTOKENS', or `NOTATION' while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
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
              this->report_fatal_error(ACE_TEXT("Expecting keyword `NMTOKEN' or `NMTOKENS' while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }
          else                  // NOTATION
            {
              if ((this->parse_token (ACE_TEXT ("TATION")) < 0) ||
                  this->skip_whitespace_count () == 0)
                {
                  this->report_fatal_error(ACE_TEXT("Expecting keyword `NOTATION' while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
                  return -1;
                }

              if (this->get () != '(')
                {
                  this->report_fatal_error(ACE_TEXT("Expecting `(' following NOTATION while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
                  return -1;
                }

              this->skip_whitespace_count ();

              do {
                ACEXML_Char *notation_name = this->read_name ();
                if (notation_name == 0)
                  {
                    this->report_fatal_error(ACE_TEXT("Error reading NOTATION name while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
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
                this->report_fatal_error(ACE_TEXT("Error reading enumerated nmtoken name while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
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
            this->report_fatal_error(ACE_TEXT("Invalid Attribute Type while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
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
                  this->report_fatal_error(ACE_TEXT("Expecting keyword `#REQUIRED' while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
                  return -1;
                }
              // We now know this attribute is required
              // @@ Set up the validator as such.
              break;
            case 'I':
              if (this->parse_token (ACE_TEXT ("MPLIED")) < 0)
                {
                  this->report_fatal_error(ACE_TEXT("Expecting keyword `#IMPLIED' while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
                  return -1;
                }
              // We now know this attribute is impleid.
              // @@ Set up the validator as such.
              break;
            case 'F':
              if (this->parse_token (ACE_TEXT ("IXED")) < 0 ||
                  this->skip_whitespace_count () == 0)
                {
                  this->report_fatal_error(ACE_TEXT("Expecting keyword `#FIXED' while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
                  return -1;
                }
              // We now know this attribute is fixed.

              ACEXML_Char *fixed_attr;
              if (this->get_quoted_string (fixed_attr) != 0)
                {
                  this->report_fatal_error(ACE_TEXT("Error parsing `#FIXED' attribute value while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
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
              this->report_fatal_error(ACE_TEXT("Error parsing `#FIXED' attribute value while defining ATTLIST.") ACEXML_ENV_ARG_PARAMETER);
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
      this->report_fatal_error(ACE_TEXT("Expecting keyword `NOTATION'") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  ACEXML_Char *notation = this->read_name ();
  if (notation == 0)
    {
      this->report_fatal_error(ACE_TEXT("Invalid notation name.") ACEXML_ENV_ARG_PARAMETER);
      return -1;
    }

  this->skip_whitespace_count ();
  ACEXML_Char *systemid, *publicid;

  this->parse_external_id_and_ref (publicid, systemid ACEXML_ENV_ARG_PARAMETER);
  ACEXML_CHECK_RETURN (-1);

  if (this->get () != '>')
    {
      this->report_fatal_error(ACE_TEXT("Expecting NOTATION closing '>'.") ACEXML_ENV_ARG_PARAMETER);
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
          this->report_fatal_error(ACE_TEXT("Expecting keyword 'SYSTEM'") ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      if (this->get_quoted_string (systemId) != 0)
        {
          this->report_fatal_error(ACE_TEXT("Error while parsing SYSTEM literal for SYSTEM id.") ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      this->locator_.setSystemId (systemId);
      break;
    case 'P':                   // External PUBLIC id or previously defined PUBLIC id.
      if (this->parse_token (ACE_TEXT ("UBLIC")) < 0 ||
          this->skip_whitespace_count () == 0)
        {
          this->report_fatal_error(ACE_TEXT("Expecting keyword 'PUBLIC'") ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      if (this->get_quoted_string (publicId) != 0)
        {
          this->report_fatal_error(ACE_TEXT("Error while parsing public literal for PUBLIC id.") ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }
      this->locator_.setPublicId (publicId);

      this->skip_whitespace_count (&nextch);
      if (nextch == '\'' || nextch == '"') // not end of NOTATION yet.
        {
          if (this->get_quoted_string (systemId) != 0)
            {
              this->report_fatal_error(ACE_TEXT("Error while parsing system literal for PUBLIC id.") ACEXML_ENV_ARG_PARAMETER);
              return -1;
            }
          this->locator_.setSystemId (systemId);
        }
      break;
    default:
      this->report_fatal_error(ACE_TEXT("Expecting either keyword `SYSTEM' or `PUBLIC'.") ACEXML_ENV_ARG_PARAMETER);
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
          this->report_fatal_error(ACE_TEXT("Expecting keyword `#PCDATA' while defining an element.") ACEXML_ENV_ARG_PARAMETER);
          return -1;
        }

      this->skip_whitespace_count (&nextch);

      while (nextch != ')')
        {
          if (this->get () != '|')
            {
              this->report_fatal_error(ACE_TEXT("Expecting end of Mixed section while defining an element.") ACEXML_ENV_ARG_PARAMETER);
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
          this->report_fatal_error(ACE_TEXT("Expecting closing `)*' or ')' while defining an element.") ACEXML_ENV_ARG_PARAMETER);
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
      this->report_fatal_error(ACE_TEXT("Expecting opening `(' while defining an element.") ACEXML_ENV_ARG_PARAMETER);
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
            this->report_fatal_error(ACE_TEXT("Error reading sub-element name while defining an element.") ACEXML_ENV_ARG_PARAMETER);
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
            this->report_fatal_error(ACE_TEXT("Expecting `,', `|', or `)' while defining an element.") ACEXML_ENV_ARG_PARAMETER);
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
            this->report_fatal_error(ACE_TEXT("Expecting `,', `|', or `)'while defining an element.") ACEXML_ENV_ARG_PARAMETER);
            return -1;
          }
      case ')':
        break;
      default:
        this->report_fatal_error(ACE_TEXT("Expecting `,', `|', or `)' while defining an element.") ACEXML_ENV_ARG_PARAMETER);
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
    {
      // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ch = %c : ptr = %c"), ch, *ptr));
    }
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
      size_t i = 0;

      switch (ch)
        {
        case '&':
          if (this->peek () == '#')
            {
              if (this->parse_char_reference (buffer, 6) != 0)
                {
// xmlenv.exception (new ACEXML_SAXParseException
// (ACE_TEXT ("CharRef does not resolves to a valid character")));
                  return -1;
                }
              charval.set (buffer, 0);
              replace = &charval;
            }
          else
            replace = this->parse_reference ();

          if (replace == 0)
            {
              //              xmlenv.exception (new ACEXML_SAXParseException
              // (ACE_TEXT ("Undefined reference")));
              return -1;
            }
          for (i = 0; i < replace->length (); ++i)
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
