// $Id$

#include "ACEXML/parser/parser/Parser.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/Parser.inl"
#endif /* __ACEXML_INLINE__ */

#include "ace/ACE.h"
#include "ACEXML/common/Transcode.h"
#include "ACEXML/common/AttributesImpl.h"
#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/common/StreamFactory.h"
#include "ACEXML/parser/parser/ParserInternals.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"

static const ACEXML_Char default_attribute_type[] = ACE_TEXT ("CDATA");
static const ACEXML_Char empty_string[] = { 0 };

const ACEXML_Char
ACEXML_Parser::simple_parsing_feature_[] = ACE_TEXT ("Simple");

const ACEXML_Char
ACEXML_Parser::namespaces_feature_[] = ACE_TEXT ("http://xml.org/sax/features/namespaces");

const ACEXML_Char
ACEXML_Parser::namespace_prefixes_feature_[] = ACE_TEXT ("http://xml.org/sax/features/namespace-prefixes");

const ACEXML_Char
ACEXML_Parser::validation_feature_[] = ACE_TEXT ("http://xml.org/sax/features/validation");

ACEXML_Parser::ACEXML_Parser (void)
  :   dtd_handler_ (0),
      entity_resolver_ (0),
      content_handler_ (0),
      error_handler_ (0),
      doctype_ (0),
      current_ (0),
      alt_stack_ (MAXPATHLEN),
      nested_namespace_ (0),
      ref_state_ (ACEXML_ParserInt::INVALID),
      external_subset_ (0),
      external_entity_ (0),
      has_pe_refs_ (0),
      standalone_ (0),
      external_dtd_ (0),
      internal_dtd_ (0),
      simple_parsing_ (0),
      validate_ (1),
      namespaces_(1),
      namespace_prefixes_ (0)
{
}

ACEXML_Parser::~ACEXML_Parser (void)
{

}

int
ACEXML_Parser::initialize(ACEXML_InputSource* input)
{
  // Initialize namespace support
  if (this->xml_namespace_.init() == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error initializing namespace support\n")));
      return -1;
    }
  for (int i = 0; i < 5; ++i)
    {
      if (this->predef_entities_.add_entity (ACEXML_ParserInt::predef_ent_[i],
                                             ACEXML_ParserInt::predef_val_[i])
          != 0)
        {
          ACE_ERROR ((LM_DEBUG,
                      ACE_TEXT ("Error adding entity %s to Manager\n"),
                      ACEXML_ParserInt::predef_ent_[i]));
          return -1;
        }
    }
  return this->switch_input (input, input->getSystemId());
}

void
ACEXML_Parser::parse (const ACEXML_Char *systemId)
{
  ACEXML_InputSource* input = 0;
  ACE_NEW (input, ACEXML_InputSource (systemId));
  this->parse (input);
}

void
ACEXML_Parser::parse (ACEXML_InputSource *input)
{
  if (input == 0)
    {
      this->fatal_error(ACE_TEXT ("Invalid input source"));
    }
  if (this->content_handler_ == 0)
    {
      this->fatal_error (ACE_TEXT ("No content handlers defined. Exiting.."));
    }

  if (this->validate_ && this->dtd_handler_ == 0)
    {
      this->fatal_error (ACE_TEXT ("No DTD handlers defined. Exiting.."));
    }

  if (this->initialize(input) == -1)
    {
      this->fatal_error (ACE_TEXT ("Failed to initialize parser state"));
    }
  // Set up Locator.
  this->content_handler_->setDocumentLocator (this->current_->getLocator());

  int xmldecl_defined = 0;
  ACEXML_Char fwd = this->get();  // Consume '<'
  if (fwd == '<' && this->peek() == '?')
    {
      this->get();      // Consume '?'
      fwd = this->peek();
      if (fwd == 'x' && !xmldecl_defined)
        {
          this->parse_xml_decl ();
          xmldecl_defined = 1;
        }
    }
  // We need a XMLDecl in a Valid XML document
  if (this->validate_ && !xmldecl_defined)
    {
      this->fatal_error (ACE_TEXT ("Expecting an XMLDecl at the beginning of")
                         ACE_TEXT (" a valid document"));
    }
  this->content_handler_->startDocument ();

  int doctype_defined = 0;
  for (int prolog_done = 0; prolog_done == 0; )
    {
      // Expect a '<' only if we have encountered a XMLDecl, or we are
      // looping through Misc blocks.
      if (xmldecl_defined)
        {
          if (this->skip_whitespace () != '<')
            {
              this->fatal_error (ACE_TEXT ("Expecting '<' at the beginning of ")
                                 ACE_TEXT ("Misc section"));
            }
          fwd = this->peek();
        }
      switch (fwd)
        {
          case '?':
            this->get();
            this->parse_processing_instruction ();
            xmldecl_defined = 1;
            break;
          case '!':
            this->get();
            fwd = this->peek ();
            if (fwd == 'D' && !doctype_defined)       // DOCTYPE
              {
                // This will also take care of the trailing MISC block if any.
                this->parse_doctypedecl ();
                doctype_defined = 1;
                // Now that we have a DOCTYPE Decl defined, we shouldn't
                // accept XML Decl any longer
                xmldecl_defined = 1;
              }
            else if (fwd == 'D')
              {
                this->fatal_error (ACE_TEXT ("Duplicate DOCTYPE declaration"));
              }
            else if (fwd == '-')  // COMMENT
              {
                if (this->parse_comment () < 0)
                  {
                    this->fatal_error(ACE_TEXT ("Invalid comment in document"));
                  }
                xmldecl_defined = 1;
              }
            break;
          case 0:
            this->fatal_error (ACE_TEXT ("Unexpected end-of-file"));
          default:                // Root element begins
            prolog_done = 1;
            break;
        }
    }

  if (this->validate_ && !doctype_defined)
    {
      this->warning (ACE_TEXT ("No doctypeDecl in valid document"));
    }

  // Now parse root element.
  this->parse_element (1);

  this->content_handler_->endDocument ();

  // Reset the parser state
  this->reset();

}

int
ACEXML_Parser::parse_doctypedecl (void)
{
  if (this->parse_token (ACE_TEXT ("DOCTYPE")) < 0)
    {
      this->fatal_error(ACE_TEXT ("Expecting keyword DOCTYPE in a doctypedecl"));
    }

  ACEXML_Char nextch = 0;
  if (this->skip_whitespace_count (&nextch) == 0)
    {
      this->fatal_error(ACE_TEXT ("Expecting a space between DOCTYPE keyword ")
                        ACE_TEXT ("and name"));
    }

  this->doctype_ = this->parse_name ();
  if (this->doctype_ == 0)
    {
      this->fatal_error(ACE_TEXT ("Invalid DOCTYPE name"));
    }
  int count = this->skip_whitespace_count (&nextch);

  if (nextch == 'S' || nextch == 'P') // ExternalID defined
    {
      if (count == 0)
        {
          this->fatal_error(ACE_TEXT ("Expecting a space between DOCTYPE")
                            ACE_TEXT ("keyword and name"));
        }
      this->external_dtd_ = 1;
      this->parse_external_dtd ();
    }

  nextch = this->skip_whitespace ();
  switch (nextch)
    {
      case '[':
        this->internal_dtd_ = 1;  // Internal DTD definition
        this->parse_internal_dtd ();
        break;
      case '>':                   // End of DTD definition
        // This is an XML document without a doctypedecl.
        if (this->validate_ && !this->external_dtd_)
          {
            this->fatal_error (ACE_TEXT ("No DTD defined"));
          }
        return 0;
      case '0':
        this->fatal_error (ACE_TEXT ("Unexpected end-of-file"));
      default:
        break;
    }

  if (this->skip_whitespace() != '>')
    {
      this->fatal_error(ACE_TEXT ("Expecting '>' at end of doctypedecl"));
    }
  return 0;
}

int
ACEXML_Parser::parse_internal_dtd (void)
{
  this->ref_state_ = ACEXML_ParserInt::IN_INT_DTD;
  ACEXML_Char nextch = this->skip_whitespace ();
  do {
    switch (nextch)
      {
        case '<':
          nextch = this->get();
          switch (nextch)
            {
              case '!':
                this->parse_markup_decl ();
                break;
              case '?':
                this->parse_processing_instruction ();
                break;
              default:
                this->fatal_error (ACE_TEXT ("Invalid internal subset"));
                break;
            }
          break;
        case '%':
          this->has_pe_refs_ = 1;
          this->parse_PE_reference ();
          break;
        case ']':                 // End of internal definitions.
          return 0;
        case '&':
          this->fatal_error (ACE_TEXT ("Invalid Reference in internal DTD"));
          break;
        case 0:
          this->pop_context (0);
          break;
        default:
          this->fatal_error (ACE_TEXT ("Invalid content in internal subset"));
      };
    nextch = this->skip_whitespace ();
  } while (1);

  ACE_NOTREACHED (return -1);
}

int
ACEXML_Parser::parse_external_dtd (void)
{
  this->ref_state_ = ACEXML_ParserInt::IN_EXT_DTD;
  ACEXML_Char* publicId = 0;
  ACEXML_Char* systemId = 0;
  if (this->parse_external_id (publicId, systemId) != 0)
    {
      this->fatal_error (ACE_TEXT ("Error in parsing ExternalID"));
    }
  if (this->validate_)
    {
      ACEXML_Char* uri = this->normalize_systemid (systemId);
      ACE_Auto_Basic_Array_Ptr<ACEXML_Char> cleanup_uri (uri);
      ACEXML_InputSource* ip = 0;
      if (this->entity_resolver_)
        {
          ip = this->entity_resolver_->resolveEntity (publicId,
                                                      (uri ? uri : systemId));
        }
      if (ip)
        {
          if (this->switch_input (ip, (uri ? uri : systemId), publicId) != 0)
            return -1;
        }
      else
        {
          ACEXML_StreamFactory factory;
          ACEXML_CharStream* cstream = factory.create_stream (uri ?
                                                              uri: systemId);
          if (!cstream) {
            this->fatal_error (ACE_TEXT ("Invalid input source"));
          }
          if (this->switch_input (cstream, systemId, publicId) != 0)
            return -1;
        }
      this->parse_external_subset ();
    }
  return 0;
}


int
ACEXML_Parser::parse_external_subset (void)
{
  this->ref_state_ = ACEXML_ParserInt::IN_EXT_DTD;
  this->external_subset_ = 1;
  size_t nrelems = 0;
  ACEXML_Char nextch = this->skip_whitespace();
  do {
    switch (nextch)
      {
        case '<':
          nextch = this->get();
          switch (nextch)
            {
              case '!':
                nextch = this->peek();
                if (nextch == '[')
                  this->parse_conditional_section ();
                else
                  this->parse_markup_decl ();
                break;
              case '?':
                nextch = this->peek();
                if (nextch == 'x')
                  this->parse_text_decl ();
                else
                  this->parse_processing_instruction ();
                break;
              default:
                this->fatal_error (ACE_TEXT ("Invalid content in external DTD"));
            }
          break;
        case '%':
          this->parse_PE_reference ();
          break;
        case 0:
          nrelems = this->pop_context (0);
          if (nrelems == 1)
            return 0;
          break;
        default:
          this->fatal_error (ACE_TEXT ("Invalid content in external DTD"));
      }
    nextch = this->skip_whitespace();
  } while (1);
}

int
ACEXML_Parser::parse_conditional_section (void)
{
  ACEXML_Char ch = this->get ();
  int include = 0;
  if (ch != '[')
    {
      this->fatal_error(ACE_TEXT ("Internal Parser Error"));
    }
  ch = this->skip_whitespace();
  if (ch == '%')
    {
      this->parse_PE_reference ();
      ch = this->skip_whitespace();
    }
  if (ch == 'I')
    {
      ch = this->get();
      switch (ch)
        {
          case 'N':
            if (this->parse_token (ACE_TEXT ("CLUDE")) < 0)
              {
                this->fatal_error (ACE_TEXT ("Expecting keyword INCLUDE in ")
                                   ACE_TEXT ("conditionalSect"));
              }
            include = 1;
            break;
          case 'G':
            if (this->parse_token (ACE_TEXT ("GNORE")) < 0)
              {
                this->fatal_error (ACE_TEXT ("Expecting keyword IGNORE in ")
                                   ACE_TEXT ("conditionalSect"));
              }
            include = 0;
            break;
          default:
            this->fatal_error (ACE_TEXT ("Invalid conditionalSect"));
        }
      ACEXML_Char fwd = '\xFF';
      this->skip_whitespace_count (&fwd);
      if (fwd == 0)
        {
          this->get(); // Consume the 0
          this->pop_context (0);
        }
    }
  else
    {
      this->fatal_error (ACE_TEXT ("Invalid conditionalSect"));
    }
  if (this->skip_whitespace() != '[')
    {
      this->fatal_error (ACE_TEXT ("Expecting '[' in conditionalSect"));
    }
  if (include)
    this->parse_includesect ();
  else
    this->parse_ignoresect ();
  return 0;
}

int
ACEXML_Parser::parse_ignoresect (void)
{
  ACEXML_Char nextch = this->skip_whitespace();
  int count = 0;
  int done = 0;
  do {
    switch (nextch)
      {
        case '<':
          if (this->peek() == '!')
            {
              this->get();
              if (this->peek() == '[')
                {
                  this->get();
                  ++count;
                }
            }
          break;
        case ']':
          if (this->peek() == ']')
            {
              this->get();
              if (this->peek() == '>')
                {
                  this->get();
                  if (count)
                    {
                      --count;
                      break;
                    }
                  done = 1;
                }
            }
          break;
        case 0: // [VC: Proper Conditional Section/PE Nesting]
          if (count != 0)
            {
              this->fatal_error (ACE_TEXT ("Invalid Conditional Section/PE ")
                                 ACE_TEXT ("Nesting "));
            }
        default:
          break;
      }
    if (done)
      break;
    nextch = this->get();
  } while (1);

  return 0;
}

int
ACEXML_Parser::parse_includesect (void)
{
  ACEXML_Char nextch = this->skip_whitespace();
  do {
    switch (nextch)
      {
        case '<':
          nextch = this->get();
          switch (nextch)
            {
              case '!':
                nextch = this->peek();
                if (nextch == '[')
                  this->parse_conditional_section ();
                else
                  this->parse_markup_decl ();
                break;
              case '?':
                nextch = this->peek();
                this->parse_processing_instruction ();
                break;
              default:
                this->fatal_error (ACE_TEXT ("Invalid includeSect"));
            }
          break;
        case '%':
          this->parse_PE_reference ();
          break;
        case 0: // [VC: Proper Conditional Section/PE Nesting]
          this->fatal_error (ACE_TEXT ("Invalid Conditional Section/PE ")
                             ACE_TEXT ("Nesting "));
        case ']':
          if (this->peek() == ']')
            {
              nextch = this->get();
              if (this->peek() == '>')
                {
                  nextch = this->get();
                  return 0;
                }
            }
        default:
          this->fatal_error (ACE_TEXT ("Invalid includeSect"));
      }
    nextch = this->skip_whitespace();
  } while (1);
}

int
ACEXML_Parser::parse_markup_decl (void)
{
  ACEXML_Char nextch = this->peek ();
  switch (nextch)
    {
      case 'E':         // An ELEMENT or ENTITY decl
        this->get ();
        nextch = this->peek ();
        switch (nextch)
          {
            case 'L':
              this->parse_element_decl ();
              break;
            case 'N':
              this->parse_entity_decl ();
              break;
            default:
              this->fatal_error(ACE_TEXT ("Expecting keyword ELEMENT/ENTITY"));
          }
        break;

      case 'A':         // An ATTLIST decl
        this->parse_attlist_decl ();
        break;

      case 'N':         // A NOTATION decl
        this->parse_notation_decl ();
        break;

      case '-':         // a comment.
        if (this->parse_comment () < 0)
          {
            this->fatal_error(ACE_TEXT ("Invalid comment"));
          }
        break;
      case 0: //  [VC: Proper Declaration/PE Nesting]
        this->fatal_error (ACE_TEXT ("Unexpected end-of-file"));
      default:
        this->fatal_error (ACE_TEXT ("Invalid markupDecl"));
    }
  return 0;
}

int
ACEXML_Parser::parse_external_id (ACEXML_Char *&publicId,
                                  ACEXML_Char *&systemId)
{
  publicId = systemId = 0;
  ACEXML_Char nextch = this->get ();
  ACEXML_Char fwd = 0;
  switch (nextch)
    {
      case 'S':                   // External SYSTEM id.
        if (this->parse_token (ACE_TEXT ("YSTEM")) < 0 ||
            this->skip_whitespace_count () < 1)
          {
            this->fatal_error(ACE_TEXT ("Expecting keyword SYSTEM"));
          }
        if (this->parse_system_literal (systemId) != 0)
          {
            this->fatal_error(ACE_TEXT ("Invalid systemLiteral"));
          }
        break;
      case 'P': // External PUBLIC id or previously defined PUBLIC id.
        if (this->parse_token (ACE_TEXT ("UBLIC")) < 0 ||
            this->skip_whitespace_count () < 1)
          {
            this->fatal_error(ACE_TEXT ("Expecting keyword PUBLIC"));
          }
        if (this->parse_pubid_literal (publicId) != 0)
          {
            this->fatal_error(ACE_TEXT ("Invalid PubidLiteral"));
          }
        this->skip_whitespace_count(&fwd);
        if (fwd == '\'' || fwd == '"')
          {
            if (this->parse_system_literal (systemId) != 0)
              {
                this->fatal_error(ACE_TEXT ("Invalid systemLiteral"));
              }
          }
        else if (this->ref_state_ != ACEXML_ParserInt::IN_NOTATION)
          {
            this->fatal_error(ACE_TEXT ("Expecting systemLiteral after a ")
                              ACE_TEXT ("PUBLIC keyword"));
          }
        break;
      default:
        this->fatal_error(ACE_TEXT ("Invalid system/public Literal"));
    }
  return 0;
}

ACEXML_Char*
ACEXML_Parser::normalize_systemid (const ACEXML_Char* systemId)
{
  if (ACE_OS::strstr (systemId, ACE_TEXT("ftp://")) != 0 ||
      ACE_OS::strstr (systemId, ACE_TEXT ("http://")) != 0 ||
      ACE_OS::strstr (systemId, ACE_TEXT ("file://")) != 0)
    return 0;
  else
    {
      ACEXML_Char* normalized_uri = 0;
      const ACEXML_Char* baseURI = this->current_->getLocator()->getSystemId();
      ACE_ASSERT (baseURI);
      const ACEXML_Char* temp = 0;
      if (ACE_OS::strstr (baseURI, ACE_TEXT ("http://")) != 0)
        {
          // baseURI is a HTTP URL and systemId is relative. Note that this
          // is not compliant with RFC2396. Caveat Emptor !
          temp = ACE_OS::strrchr (baseURI, '/');
        }
      else
        {
          // baseURI is a local file and systemId is relative
          // Unlike the HTTP one, this will work always.
          temp = ACE_OS::strrchr (baseURI, ACE_TEXT ('\\'));
          if (!temp)
            {
              temp = ACE_OS::strrchr (baseURI, ACE_TEXT ('/'));
            }
        }

      if (temp)
        {
          size_t pos = temp - baseURI + 1;
          size_t len = pos + ACE_OS::strlen (systemId) + 1;
          ACE_NEW_RETURN (normalized_uri, ACEXML_Char[len], 0);
          ACE_OS::strncpy (normalized_uri, baseURI, pos);
          ACE_OS::strcpy (normalized_uri + pos, systemId);
          return normalized_uri;
        }
      return 0;
    }
}

void
ACEXML_Parser::parse_element (int is_root)
{
  // Parse STag.
  const ACEXML_Char *startname = this->parse_name ();
  if (startname == 0)
    {
      this->fatal_error (ACE_TEXT ("Unexpected end-of-file"));
      return;
    }
  if (is_root && this->doctype_ != 0
      && ACE_OS::strcmp (startname, this->doctype_) != 0)
    {
      this->fatal_error (ACE_TEXT ("Root element different from DOCTYPE"));
      return ;
    }
  ACEXML_AttributesImpl attributes;
  ACEXML_Char ch;
  int ns_flag = 0;   // Push only one namespace context onto the stack
                     // if there are multiple namespaces declared.

  const ACEXML_Char* ns_uri = 0;
  const ACEXML_Char* ns_lname = 0; // namespace URI and localName
  for (int start_element_done = 0; start_element_done == 0;)
    {
      ch = this->skip_whitespace ();

      switch (ch)
        {
          case 0:
            this->fatal_error(ACE_TEXT ("Internal Parser error"));
            return;
          case '/':
            if (this->get () != '>')
              {
                this->fatal_error(ACE_TEXT ("Expecting '>' at end of element ")
                                  ACE_TEXT ("definition"));
                return;
              }
            this->xml_namespace_.processName(startname, ns_uri,
                                             ns_lname, 0);
            this->prefix_mapping (this->xml_namespace_.getPrefix(ns_uri),
                                  ns_uri, 1);
            this->content_handler_->startElement(ns_uri, ns_lname,
                                                 startname, &attributes);
            this->content_handler_->endElement (ns_uri, ns_lname, startname);
            this->prefix_mapping (this->xml_namespace_.getPrefix(ns_uri),
                                  ns_uri, 0);
            if (ns_flag)
              {
                this->xml_namespace_.popContext ();
                this->nested_namespace_--;
              }
            return;
          case '>':
            this->xml_namespace_.processName (startname, ns_uri,
                                              ns_lname, 0);
            this->prefix_mapping (this->xml_namespace_.getPrefix(ns_uri),
                                  ns_uri, 1);
            this->content_handler_->startElement(ns_uri, ns_lname, startname,
                                                 &attributes);
            start_element_done = 1;
            break;
          default:
            ACEXML_Char *attvalue = 0;
            ACEXML_Char *attname = this->parse_name (ch);

            if (attname == 0 ||
                this->skip_equal () != 0 ||
                this->parse_attvalue (attvalue) != 0)
              {
                this->fatal_error(ACE_TEXT ("Error reading attribute value"));
                return;
              }

            // Handling new namespace if any. Notice that the order of
            // namespace declaration does matter.
            if (ACE_OS::strncmp (attname, ACE_TEXT("xmlns"), 5) == 0)
              {
                if (this->namespaces_)
                  {
                    if (!ns_flag)
                      {
                        this->xml_namespace_.pushContext ();
                        this->nested_namespace_++;
                        ns_flag = 1;
                      }

                    ACEXML_Char* name = ACE_OS::strchr (attname, ':');
                    const ACEXML_Char* ns_name = (name == 0)?
                                                 empty_string:name+1;
                    if (this->xml_namespace_.declarePrefix (ns_name,
                                                            attvalue) == -1)
                      {
                        this->fatal_error(ACE_TEXT ("Duplicate definition of ")
                                          ACE_TEXT ("prefix"));
                        return;
                      }
                  }
                if (this->namespace_prefixes_)
                  {
                    // Namespace_prefixes_feature_ is required. So add the
                    // xmlns:foo to the list of attributes.
                    if (attributes.addAttribute (ACE_TEXT (""), ACE_TEXT (""),
                                                 attname,
                                                 default_attribute_type,
                                                 attvalue) == -1)
                      {
                        this->fatal_error(ACE_TEXT ("Duplicate attribute ")
                                          ACE_TEXT ("definition. Hint: Try ")
                                          ACE_TEXT ("setting namespace_prefix")
                                          ACE_TEXT ("es feature to 0"));
                        return;
                      }
                  }
                if (!this->namespaces_ && !this->namespace_prefixes_)
                  {
                    this->fatal_error(ACE_TEXT ("One of namespaces or ")
                                      ACE_TEXT ("namespace_prefixes should be")
                                      ACE_TEXT (" declared"));
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
                    this->fatal_error(ACE_TEXT ("Duplicate attribute ")
                                      ACE_TEXT ("definition"));
                    return;
                  }
              }
            break;
        }
    }
  if (this->parse_content (startname, ns_uri, ns_lname, ns_flag) != 0)
    return;
}

int
ACEXML_Parser::parse_content (const ACEXML_Char* startname,
                              const ACEXML_Char*& ns_uri,
                              const ACEXML_Char*& ns_lname, int ns_flag)
{
  ACEXML_Char *cdata;
  size_t cdata_length = 0;

  // Parse element contents.
  while (1)
    {
      ACEXML_Char ch = this->get ();
      switch (ch)
        {
          case 0:
            this->pop_context (1);
            break;
          case '<':
            // Push out old 'characters' event.
            if (cdata_length != 0)
              {
                cdata = this->obstack_.freeze ();
                this->content_handler_->characters (cdata, 0, cdata_length);
                this->obstack_.unwind (cdata);
                cdata_length = 0;
              }
            ch = this->peek();
            switch (ch)
              {
                case '!':             // a comment or a CDATA section.
                  this->get ();       // consume '!'
                  ch = this->peek ();
                  if (ch == '-')      // a comment
                    {
                      if (this->parse_comment () < 0)
                        {
                          this->fatal_error(ACE_TEXT ("Invalid comment in ")
                                            ACE_TEXT ("document"));
                        }
                    }
                  else if (ch == '[') // a CDATA section.
                    {
                      this->parse_cdata ();
                    }
                  else
                    {
                      this->fatal_error(ACE_TEXT ("Expecting a CDATA section ")
                                        ACE_TEXT ("or a comment section"));
                    }
                  break;
                case '?':             // a PI.
                  this->get();        // consume the '?'
                  this->parse_processing_instruction ();
                  break;
                case '/':             // an ETag.
                  {
                    this->get ();       // consume '/'
                    ACEXML_Char* endname = this->parse_name ();
                    if (endname == 0 ||
                        ACE_OS::strcmp (startname, endname) != 0)
                      {
                        this->fatal_error(ACE_TEXT ("Name in ETag doesn't ")
                                          ACE_TEXT ("match name in STag"));
                      }
                    if (this->skip_whitespace () != '>')
                      {
                        this->fatal_error(ACE_TEXT ("Expecting '>' at end ")
                                          ACE_TEXT ("of element"));
                        return -1;
                      }
                    this->content_handler_->endElement (ns_uri, ns_lname,
                                                        endname);
                    this->prefix_mapping (this->xml_namespace_.getPrefix(ns_uri),
                                          ns_uri, 0);
                    if (this->namespaces_ && ns_flag)
                      {
                        if (this->nested_namespace_ >= 1)
                          {
                            this->xml_namespace_.popContext ();
                            this->nested_namespace_--;
                          }
                      }
                    return 0;
                  }
                default:              // a new nested element?
                  this->parse_element (0);
                  break;
              }
            break;
          case '&':
            if (this->peek () == '#')
              {
                ACEXML_Char buf[7];
                size_t len = 0;
                do
                  {
                    len = sizeof (buf);
                    if (this->parse_char_reference (buf, len) != 0)
                      {
                        // [WFC: Legal Character]
                        this->fatal_error (ACE_TEXT ("Invalid CharRef"));
                      }
                  } while (buf[0] == '&' && this->peek() == '#');
                  for (size_t j = 0; j < len; ++j)
                    this->obstack_.grow (buf[j]);
                cdata_length += len;
              }
            else
              {
                this->ref_state_ = ACEXML_ParserInt::IN_CONTENT;
                int const length = this->parse_entity_reference();
                if (length == 1)
                  ++cdata_length;
              }
            break;
          case '\x20': case '\x0D': case '\x0A': case '\x09':
//             if (this->validate_)
//               {
//                 // Flush out any non-whitespace characters
//                 if (cdata_length != 0)
//                   {
//                     cdata = this->obstack_.freeze ();
//                     this->content_handler_->characters(cdata, 0, cdata_length
//                                                        ACEXML_ENV_ARG_PARAMETER);
//                     ACEXML_CHECK_RETURN (-1);
//                     this->obstack_.unwind (cdata);
//                     cdata_length = 0;
//                   }
//                 ++cdata_length;
//                 this->obstack_.grow (ch);
//                 while (1)
//                   {
//                     ch = this->peek();
//                     if (ch == '\x20' || ch == '\x0D' || ch == '\x0A' ||
//                         ch == '\x09')
//                       {
//                         ch = this->get();
//                         this->obstack_.grow (ch);
//                         continue;
//                       }
//                     break;
//                   }
//                 cdata = this->obstack_.freeze ();
//                 this->content_handler_->ignorableWhitespace (cdata, 0,
//                                                              cdata_length
//                                                              ACEXML_ENV_ARG_PARAMETER);
//                 ACEXML_CHECK_RETURN (-1);
//                 this->obstack_.unwind (cdata);
//                 cdata_length = 0;
//                 break;
//               }
            // Fall thru...
          default:
            ++cdata_length;
            this->obstack_.grow (ch);
        }
    }
  ACE_NOTREACHED (return 0;)
}


int
ACEXML_Parser::parse_cdata (void)
{
  if (this->parse_token (ACE_TEXT ("[CDATA[")) < 0)
    {
      this->fatal_error(ACE_TEXT ("Expecting '[CDATA[' at beginning of CDATA ")
                        ACE_TEXT ("section"));
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
              this->content_handler_->characters (cdata, 0, datalen);
              this->obstack_.unwind(cdata);
              return 0;
            }
          this->obstack_.grow (temp);
          ++datalen;
        }
      this->obstack_.grow (ch);
      ++datalen;
    };
  ACE_NOTREACHED (return -1);
}


int
ACEXML_Parser::parse_entity_decl (void)
{
  ACEXML_Char nextch = 0;

  if ((this->parse_token (ACE_TEXT ("NTITY")) < 0) ||
      this->skip_whitespace_count (&nextch) == 0)
    {
      this->fatal_error (ACE_TEXT ("Expecting keyword ENTITY followed by a ")
                         ACE_TEXT ("space"));
    }

  int is_GEDecl = 1;
  if (nextch == '%')            // This is a PEDecl.
    {
      is_GEDecl = 0;
      this->get ();             // consume the '%'
      if (this->skip_whitespace_count (&nextch) == 0)
        {
          this->fatal_error (ACE_TEXT ("Expecting space between % and ")
                             ACE_TEXT ("entity name"));
        }
    }

  ACEXML_Char *entity_name = this->parse_name ();
  if (entity_name == 0)
    {
      this->fatal_error (ACE_TEXT ("Invalid entity name"));
    }

  if (this->skip_whitespace_count (&nextch) == 0)
    {
      this->fatal_error (ACE_TEXT ("Expecting space between entity name and ")
                         ACE_TEXT ("entityDef"));
    }
  int retval = 0;
  if (nextch == '\'' || nextch == '"')
    {
      ACEXML_Char *entity_value = 0;
      if (this->parse_entity_value (entity_value) != 0)
        {
          this->fatal_error(ACE_TEXT ("Invalid EntityValue"));
        }
      if (is_GEDecl)
        retval = this->internal_GE_.add_entity (entity_name,
                                                entity_value);
      else
        retval = this->internal_PE_.add_entity (entity_name,
                                                entity_value);
      if (retval < 0)
        {
          this->fatal_error (ACE_TEXT ("Internal Parser Error in adding")
                             ACE_TEXT ("Entity to map"));
        }
      else if (retval == 1)
        {
          this->warning (ACE_TEXT ("Duplicate entity found"));
        }
    }
  else
    {
      ACEXML_Char *systemid = 0, *publicid = 0;

      this->parse_external_id (publicid, systemid);
      if (systemid == 0)
        {
          this->fatal_error(ACE_TEXT ("Invalid SystemLiteral"));
        }
      this->skip_whitespace_count (&nextch);
      if (nextch == 'N')        // NDATA section followed
        {
          if (is_GEDecl == 0)
            {
              this->fatal_error(ACE_TEXT ("Invalid NDataDecl in PEDef"));
            }

          if ((this->parse_token (ACE_TEXT ("NDATA")) < 0) ||
              this->skip_whitespace_count (&nextch) == 0)
            {
              this->fatal_error(ACE_TEXT ("Expecting keyword NDATA followed ")
                                ACE_TEXT ("by a space"));
            }

          ACEXML_Char *ndata = this->parse_name ();
          if (this->validate_) // [VC: Notation Declared]
            {
              if (!this->notations_.resolve_entity (ndata))
                {
                  this->fatal_error (ACE_TEXT ("Undeclared Notation name"));
                }
              this->dtd_handler_->unparsedEntityDecl(entity_name, publicid,
                                                     systemid, ndata);
            }
        }
      else
        {
          if (is_GEDecl)
            retval = this->external_GE_.add_entity (entity_name, systemid);
          else
            retval = this->external_PE_.add_entity (entity_name, systemid);
          if (retval < 0)
            {
              this->fatal_error(ACE_TEXT ("Internal Parser Error"));
            }
          else if (retval == 1)
            this->warning(ACE_TEXT ("Duplicate external entity"));
          if (is_GEDecl)
            retval = this->external_GE_.add_entity (entity_name, publicid);
          else
            retval = this->external_PE_.add_entity (entity_name, publicid);
          if (retval < 0)
            {
              this->fatal_error(ACE_TEXT ("Internal Parser Error"));
            }
          else if (retval == 1)
            this->warning (ACE_TEXT ("Duplicate entity definition"));
        }
    }

  // End of ENTITY definition
  if (this->skip_whitespace() != '>')
    {
      this->fatal_error(ACE_TEXT ("Expecting '>' at end of entityDef"));
    }
  return 0;
}

int
ACEXML_Parser::parse_attlist_decl (void)
{
  if (this->parse_token (ACE_TEXT ("ATTLIST")) < 0)
    {
      this->fatal_error(ACE_TEXT ("Expecting keyword 'ATTLIST'"));
    }
  int count = check_for_PE_reference ();
  if (!count)
    {
      this->fatal_error(ACE_TEXT ("Expecting space between ATTLIST and ")
                        ACE_TEXT ("element name"));
    }

  ACEXML_Char *element_name = this->parse_name ();
  if (element_name == 0)
    {
      this->fatal_error(ACE_TEXT ("Invalid element Name in attlistDecl"));
    }
  ACEXML_Char fwd = 0;
  count = this->skip_whitespace_count (&fwd);
  // Parse AttDef*
  while (fwd != '>')
    {
      if (!this->check_for_PE_reference ()
          && !count)
        this->fatal_error(ACE_TEXT ("Expecting space between element ")
                          ACE_TEXT ("name and AttDef"));
      this->skip_whitespace_count (&fwd);
      if (fwd == '>')
        break;

      count = this->check_for_PE_reference ();

      this->parse_attname ();

      count = this->check_for_PE_reference ();
      if (!count)
        {
          this->fatal_error(ACE_TEXT ("Expecting space between AttName and ")
                            ACE_TEXT ("AttType"));
        }
      this->parse_atttype ();

      count = this->check_for_PE_reference ();
      if (!count)
        {
          this->fatal_error(ACE_TEXT ("Expecting space between AttType and")
                            ACE_TEXT (" DefaultDecl"));
        }
      this->parse_defaultdecl ();

      count = this->check_for_PE_reference ();
      this->skip_whitespace_count(&fwd);
    }
  this->get ();                 // consume closing '>'
  return 0;
}

int
ACEXML_Parser::check_for_PE_reference (void)
{
  ACEXML_Char fwd = '\xFF';
  // Skip any leading whitespaces and store the number of such chars skipped
  int count = this->skip_whitespace_count (&fwd);
  if (fwd == 0)
    {
      this->get(); // Consume the 0
      this->pop_context (0);
      fwd = this->peek();
    }
  if (fwd == '%')
    {
      this->get();  // Consume the %
      if (this->external_subset_)
        {
          this->parse_PE_reference ();
        }
      else
        {
          this->fatal_error(ACE_TEXT ("Illegal PERef within markupDecl"));
        }
    }
  if (count)
    {
      // We have atleast one whitespace. So just skip any more whitespaces
      // and return the count
      this->skip_whitespace_count();
      return count;
    }
  return this->skip_whitespace_count();
}

ACEXML_Char*
ACEXML_Parser::parse_attname (void)
{
  // Parse attribute name
  ACEXML_Char *att_name = this->parse_name ();
  if (att_name == 0)
    {
      this->fatal_error(ACE_TEXT ("Invalid AttName"));
    }
  return att_name;
}

int
ACEXML_Parser::parse_defaultdecl (void)
{
  // DefaultDecl ::=  '#REQUIRED' | '#IMPLIED' | (('#FIXED' S)? AttValue)
  ACEXML_Char nextch = this->peek ();
  ACEXML_Char *fixed_attr = 0;
  switch (nextch)
    {
      case '#':
        this->get ();         // consume the '#'
        switch (this->get ())
          {
            case 'R':
              if (this->parse_token (ACE_TEXT ("EQUIRED")) < 0)
                {
                  this->fatal_error(ACE_TEXT ("Expecting keyword REQUIRED"));
                }
              // We now know this attribute is required
              // @@ Set up the validator as such.
              break;
            case 'I':
              if (this->parse_token (ACE_TEXT ("MPLIED")) < 0)
                {
                  this->fatal_error(ACE_TEXT ("Expecting keyword IMPLIED"));
                }
              // We now know this attribute is implied.
              // @@ Set up the validator as such.
              break;
            case 'F':
              if (this->parse_token (ACE_TEXT ("IXED")) < 0 ||
                  this->skip_whitespace_count () == 0)
                {
                  this->fatal_error(ACE_TEXT ("Expecting keyword FIXED"));
                }
              // We now know this attribute is fixed.
              if (this->parse_attvalue (fixed_attr) != 0)
                {
                  this->fatal_error(ACE_TEXT ("Invalid Default AttValue"));
                }
              // @@ set up validator
              break;
            default:
              this->fatal_error (ACE_TEXT ("Invalid DefaultDecl"));
          }
        break;
      case '\'':
      case '"':
        if (this->parse_attvalue (fixed_attr) != 0)
          {
            this->fatal_error(ACE_TEXT ("Invalid AttValue"));
          }
        // @@ set up validator
        break;
      default:
        this->fatal_error (ACE_TEXT ("Invalid DefaultDecl"));
        break;
    }
  return 0;
}

int
ACEXML_Parser::parse_tokenized_type (void)
{
  ACEXML_Char ch = this->get();
  switch (ch)
    {
      case 'I':
        if (this->get () == 'D')
          {
            if (this->peek() != 'R' && this->is_whitespace (this->peek()))
              {
                // We have successfully identified the type of the
                // attribute as ID
                // @@ Set up validator as such.
                break;
              }
            if (this->parse_token (ACE_TEXT ("REF")) == 0)
              {
                if (this->peek() != 'S' && this->is_whitespace (this->peek()))
                  {
                    // We have successfully identified the type of
                    // the attribute as IDREF
                    // @@ Set up validator as such.
                    break;
                  }
                else if (this->peek() == 'S'
                         && this->get() // consume the 'S'
                         && this->is_whitespace (this->peek()))
                  {
                    // We have successfully identified the type of
                    // the attribute as IDREFS
                    // @@ Set up validator as such.
                    break;
                  }
              }
          }
        // Admittedly, this error message is not precise enough
        this->fatal_error(ACE_TEXT ("Expecting keyword `ID', `IDREF', or")
                          ACE_TEXT ("`IDREFS'"));
      case 'E':               // ENTITY or ENTITIES
        if (this->parse_token (ACE_TEXT ("NTIT")) == 0)
          {
            ACEXML_Char nextch = this->get ();
            if (nextch == 'Y')
              {
                // We have successfully identified the type of
                // the attribute as ENTITY
                // @@ Set up validator as such.
              }
            else if (this->parse_token (ACE_TEXT ("IES")) == 0)
              {
                // We have successfully identified the type of
                // the attribute as ENTITIES
                // @@ Set up validator as such.
              }
            if (this->is_whitespace (this->peek()))
              {
                // success
                break;
              }
          }
        // Admittedly, this error message is not precise enough
        this->fatal_error(ACE_TEXT ("Expecting keyword `ENTITY', or")
                          ACE_TEXT ("`ENTITIES'"));
      case 'M':
        if (this->parse_token (ACE_TEXT ("TOKEN")) == 0)
          {
            if (this->is_whitespace (this->peek()))
              {
                // We have successfully identified the type of
                // the attribute as NMTOKEN
                // @@ Set up validator as such.
                break;
              }
            else if (this->peek() == 'S'
                     && this->get()
                     && this->is_whitespace (this->peek()))
              {
                // We have successfully identified the type of
                // the attribute as NMTOKENS
                // @@ Set up validator as such.
                break;
              }
          }
        this->fatal_error(ACE_TEXT ("Expecting keyword `NMTOKEN' or `NMTO")
                          ACE_TEXT ("KENS'"));
        break;
      default:
        this->fatal_error (ACE_TEXT ("Internal Parser Error"));
        break;
    }
  return 0;
}


/**
 * AttType    ::=    StringType | TokenizedType | EnumeratedType
 * StringType ::=    'CDATA'
 * TokenizedType    ::=    'ID'       [VC: ID]
 *                                    [VC: One ID per Element Type]
 *                                    [VC: ID Attribute Default]
 *                       | 'IDREF' [VC: IDREF]
 *                       | 'IDREFS' [VC: IDREF]
 *                       | 'ENTITY' [VC: Entity Name]
 *                       | 'ENTITIES' [VC: Entity Name]
 *                       | 'NMTOKEN' [VC: Name Token]
 *                       | 'NMTOKENS'
 *
 * EnumeratedType  ::=    NotationType | Enumeration
 * NotationType    ::=   'NOTATION' S '(' S? Name (S? '|' S? Name)* S? ')'
 *                                  [VC: Notation Attributes]
 *                                  [VC: One Notation Per Element Type]
 *                                  [VC: No Notation on Empty Element]
 * Enumeration ::= '(' S? Nmtoken (S? '|' S? Nmtoken)* S? ')'
 *                                  [VC: Enumeration]
 */
int
ACEXML_Parser::parse_atttype (void)
{
  ACEXML_Char nextch = this->peek();
  switch (nextch)
    {
      case 'C':               // CDATA
        if (this->parse_token (ACE_TEXT ("CDATA")) < 0)
          {
            this->fatal_error(ACE_TEXT ("Expecting keyword 'CDATA'"));
          }
        // Else, we have successfully identified the type of the
        // attribute as CDATA
        // @@ Set up validator appropriately here.
        break;
      case 'I': case 'E':      // ID, IDREF, IDREFS, ENTITY or ENTITIES
        this->parse_tokenized_type ();
        break;
      case 'N':             // NMTOKEN, NMTOKENS, or NOTATION
        this->get();
        nextch = this->peek();
        if (nextch != 'M' && nextch != 'O')
          {
            this->fatal_error (ACE_TEXT ("Expecting keyword 'NMTOKEN', ")
                               ACE_TEXT ("'NMTOKENS' or 'NOTATION'"));
          }
        if (nextch == 'M')
          {
            this->parse_tokenized_type ();
            break;
          }
        else                   // NOTATION
          {
            if (this->parse_token (ACE_TEXT ("OTATION")) < 0)
              {
                this->fatal_error(ACE_TEXT ("Expecting keyword `NOTATION'"));
              }
            int count = this->check_for_PE_reference ();
            if (!count)
              {
                this->fatal_error (ACE_TEXT ("Expecting space between keyword ")
                                   ACE_TEXT ("NOTATION and '('"));
              }
            if (this->get () != '(')
              {
                this->fatal_error(ACE_TEXT ("Expecting '(' in NotationType"));
              }
            this->check_for_PE_reference ();
            do {
              this->skip_whitespace_count();
              ACEXML_Char *notation_name = this->parse_name ();
              if (notation_name == 0)
                {
                  this->fatal_error(ACE_TEXT ("Invalid notation name"));
                }
              // @@ get another notation name, set up validator as such
              this->check_for_PE_reference ();
              nextch = this->get();
            } while (nextch == '|');
            if (nextch != ')')
              {
                this->fatal_error (ACE_TEXT ("Expecting a ')' after a ")
                                   ACE_TEXT ("NotationType declaration"));
              }
          }
        break;
      case '(':               // EnumeratedType - Enumeration
        this->get();
        this->check_for_PE_reference ();
        do {
          this->skip_whitespace_count();
          ACEXML_Char *token_name = this->parse_nmtoken ();
          if (token_name == 0)
            {
              this->fatal_error(ACE_TEXT ("Invalid enumeration name"));
            }
          // @@ get another nmtoken, set up validator as such
          this->check_for_PE_reference ();
          nextch = this->get();
        } while (nextch == '|');
        if (nextch != ')')
          {
            this->fatal_error (ACE_TEXT ("Expecting a ')' after a ")
                               ACE_TEXT ("Enumeration declaration"));
          }
        break;
      default:
        {
          this->fatal_error(ACE_TEXT ("Invalid AttType"));
        }
        ACE_NOTREACHED (break);
    }
  return 0;
}

int
ACEXML_Parser::parse_notation_decl (void)
{
  if (this->parse_token (ACE_TEXT ("NOTATION")) < 0)
    {
      this->fatal_error(ACE_TEXT ("Expecting Keyword 'NOTATION'"));
    }
  int count = this->check_for_PE_reference ();
  if (!count)
    {
      this->fatal_error (ACE_TEXT ("Expecting a space between keyword NOTATION")
                         ACE_TEXT (" and notation name"));
    }
  ACEXML_Char *notation = this->parse_name ();
  if (notation == 0)
    {
      this->fatal_error(ACE_TEXT ("Invalid Notation name"));
    }
  count = this->check_for_PE_reference ();
  if (!count)
    {
      this->fatal_error (ACE_TEXT ("Expecting a space between notation name ")
                         ACE_TEXT ("and ExternalID/PublicID"));
    }

  ACEXML_Char *systemid, *publicid;

  // Gross hack but otherwise we need to go around a lot of loops to parse,
  // When the ExternalID starts with 'PUBLIC' we cannot distinguish a
  // PublicId from a ExternalID by looking using a one character read-ahead
  ACEXML_ParserInt::ReferenceState temp = this->ref_state_;
  this->ref_state_ = ACEXML_ParserInt::IN_NOTATION;

  this->parse_external_id (publicid, systemid);
  // Restore the original value.
  this->ref_state_ = temp;

  // [VC: Unique Notation Name]
  if (systemid && this->notations_.add_entity (notation, systemid) != 0
      && this->validate_)
    {
      this->fatal_error(ACE_TEXT ("Internal Parser Error"));
    }
  if (publicid)
    {
      int retval = this->notations_.add_entity (notation, publicid);
      if (retval != 0 && !systemid && this->validate_)
        {
          this->fatal_error(ACE_TEXT ("Internal Parser Error"));
        }
    }

  if (this->skip_whitespace() != '>')
    {
      this->fatal_error(ACE_TEXT ("Expecting '>' at end of NotationDecl"));
    }

  if (this->validate_ && this->dtd_handler_)
    {
      this->dtd_handler_->notationDecl (notation, publicid, systemid);
    }
  return 0;
}

int
ACEXML_Parser::parse_element_decl (void)
{
  if (this->parse_token (ACE_TEXT ("LEMENT")) < 0)
    {
      this->fatal_error (ACE_TEXT ("Expecting keyword ELEMENT"));
    }
  int count = this->check_for_PE_reference ();
  if (!count)
    {
      this->fatal_error (ACE_TEXT ("Expecting a space between keyword ELEMENT")
                         ACE_TEXT (" and element name"));
    }
  ACEXML_Char *element_name = this->parse_name ();
  if (element_name == 0)
    {
      this->fatal_error (ACE_TEXT ("Invalid element name"));
    }
  count = this->check_for_PE_reference ();
  if (!count)
    {
      this->fatal_error (ACE_TEXT ("Expecting a space between element name ")
                         ACE_TEXT ("and element definition"));
    }
  ACEXML_Char nextch = this->peek();
  switch (nextch)
    {
      case 'E':                   // EMPTY
        if (this->parse_token (ACE_TEXT ("EMPTY")) < 0)
          {
            this->fatal_error (ACE_TEXT ("Expecting keyword EMPTY"));
          }
        break;
      case 'A':                   // ANY
        if (this->parse_token (ACE_TEXT ("ANY")) < 0)
          {
            this->fatal_error (ACE_TEXT ("Expecting keyword ANY"));
          }
        break;
      case '(':                   // children
        this->parse_children_definition ();
        break;
      default:                    // error
        this->fatal_error (ACE_TEXT ("Invalid element definition"));
    }
  count = this->check_for_PE_reference ();
  if (this->skip_whitespace () != '>')
    {
      this->fatal_error (ACE_TEXT ("Expecting '>' after element defintion"));
    }
  return 0;
}


int
ACEXML_Parser::parse_children_definition (void)
{
  this->get ();                 // consume the '('
  this->check_for_PE_reference ();
  int subelement_number = 0;
  ACEXML_Char nextch = this->peek();
  switch (nextch)
    {
      case '#':                   // Mixed element,
        if (this->parse_token (ACE_TEXT ("#PCDATA")) < 0)
          {
            this->fatal_error(ACE_TEXT ("Expecting keyword '#PCDATA'"));
          }
        this->check_for_PE_reference ();
        nextch = this->get();
        while (nextch == '|')
          {
            this->check_for_PE_reference ();
            ACEXML_Char *name = this->parse_name ();
            // @@ name will be used in the Validator later.
            ACE_UNUSED_ARG (name);
            ++subelement_number;
            // @@ Install Mixed element name into the validator.
            this->check_for_PE_reference ();
            nextch = this->skip_whitespace();
          }
        if (nextch != ')' ||
            (subelement_number && this->get () != '*'))
          {
            this->fatal_error(ACE_TEXT ("Expecing ')' or ')*' at end of Mixed")
                              ACE_TEXT (" element"));
          }
        // @@ close the element definition in the validator.
        break;
      default:
        int status = this->parse_child (1);
        if (status != 0)
          return -1;
    }

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

int
ACEXML_Parser::parse_child (int skip_open_paren)
{
  // Conditionally consume the open paren.
  if (skip_open_paren == 0 && this->get () != '(')
    {
      this->fatal_error(ACE_TEXT ("Expecting '(' at beginning of children"));
    }

  ACEXML_Char node_type = 0;
  ACEXML_Char nextch = 0;

  do {
    this->check_for_PE_reference ();
    this->skip_whitespace_count (&nextch);
    switch (nextch)
      {
        case '(':
          this->check_for_PE_reference ();
          this->parse_child (0);
          break;
        default:
          this->check_for_PE_reference ();
          // must be an element name here.
          ACEXML_Char *subelement = this->parse_name ();
          if (subelement == 0)
            {
              this->fatal_error(ACE_TEXT ("Invalid subelement name"));
            }
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

          // @@ Inform validator of the new element here.
          break;
      }
    this->check_for_PE_reference ();
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
                this->fatal_error (ACE_TEXT ("Expecting `,', `|', or `)' ")
                                   ACE_TEXT ("while defining an element"));
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
                this->fatal_error (ACE_TEXT ("Expecting `,', `|', or `)' ")
                                   ACE_TEXT ("while defining an element"));
            }
          break;
        case ')':
          break;
        default:
          this->fatal_error (ACE_TEXT ("Expecting `,', `|', or `)' ")
                             ACE_TEXT ("while defining an element"));
      }
    nextch = this->get();  // Consume the `,' or `|' or `)'
    if (nextch == ')')
      break;
    this->check_for_PE_reference ();
    this->skip_whitespace_count (&nextch);
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

int
ACEXML_Parser::parse_char_reference (ACEXML_Char *buf, size_t& len)
{
  if (len < 7)   // Max size of a CharRef plus terminating '\0'
    return -1;
  ACEXML_Char ch = this->get();
  if (ch != '#')      // Internal error.
    return -1;
  int hex = 0;
  ch = this->peek();
  if (ch == 'x')
    {
      hex = 1;
      this->get ();
    }
  size_t i = 0;
  int more_digit = 0;
  ch = this->get ();
  for ( ; i < len &&
          (this->isNormalDigit (ch) || (hex ? this->isCharRef(ch): 0)); ++i)
    {
      buf[i] = ch;
      ch = this->get();
      ++more_digit;
    }
  if (ch != ';' || !more_digit)
    return -1;
  buf[i] = 0;
  ACEXML_UCS4 sum = (ACEXML_UCS4) ACE_OS::strtol (buf, 0, (hex ? 16 : 10));
  // [WFC: Legal Character]
  if (!this->isChar (sum))
    return -1;
  int clen;
#if defined (ACE_USES_WCHAR)
#  if (ACE_SIZEOF_WCHAR == 2)    // UTF-16
  if ((clen = ACEXML_Transcoder::ucs42utf16 (sum, buf, len)) < 0)
    return -1;
#  elif (ACE_SIZEOF_WCHAR == 4)  // UCS 4
  buf [0] = sum;
  buf [1] = 0;
  clen = 2;
#  endif /* ACE_SIZEOF_WCHAR */

#else                          // or UTF-8
  if ((clen = ACEXML_Transcoder::ucs42utf8 (sum, buf, len)) < 0)
    return -1;
#endif
  buf [clen] = 0;
  len = clen;
  return 0;
}

ACEXML_Char*
ACEXML_Parser::parse_reference_name (void)
{
  ACEXML_Char ch = this->get ();
  if (!this->isLetter (ch) && (ch != '_' && ch != ':'))
    return 0;
  while (ch) {
    this->alt_stack_.grow (ch);
    ch = this->peek ();
    if (!this->isNameChar (ch))
      break;
    ch = this->get ();
  };
  if (ch != ';')
    return 0;
  ch = this->get();
  return this->alt_stack_.freeze ();
}

int
ACEXML_Parser::parse_attvalue (ACEXML_Char *&str)
{
  ACEXML_Char quote = this->get ();
  if (quote != '\'' && quote != '"')  // Not a quoted string.
    return -1;
  ACEXML_Char ch = this->get ();
  while (1)
    {
      if (ch == quote)
        {
          ACEXML_Char* temp = this->obstack_.freeze ();
          // If the attribute type is not CDATA, then the XML processor
          // must further process the normalized attribute value by
          // discarding any leading and trailing space (#x20) characters,
          // and by replacing sequences of space (#x20) characters by a
          // single space (#x20) character.

          //           if (atttype != CDATA) {
          //             ACEXML_Char* start = temp;
          //             ACEXML_Char* end = temp + ACE_OS::strlen (temp);
          //             while (*start == '\x20')
          //               start++;
          //             if (start == end) // String which is all spaces
          //               str = start;
          //             while (*start != 0)
          //               {
          //                 this->obstack_.grow (*start);
          //                 start++;
          //                 while (*start == '\x20')
          //                   start++;
          //               }
          //             str = this->obstack_.freeze();
          //           }
          str = temp;
          return 0;
        }
      switch (ch)
        {
          case '&':
            if (this->peek () == '#')
              {
                ACEXML_Char buf[7];
                size_t len = sizeof (buf);
                if (this->parse_char_reference (buf, len) != 0)
                  {
                    // [WFC: Legal Character]
                    this->fatal_error (ACE_TEXT ("Invalid CharacterRef"));
                  }
                for (size_t j = 0; j < len; ++j)
                  this->obstack_.grow (buf[j]);
              }
            else
              {
                this->ref_state_ = ACEXML_ParserInt::IN_ATT_VALUE;
                this->parse_entity_reference ();
              }
            break;
          case '\x20': case '\x0D': case '\x0A': case '\x09':
            this->obstack_.grow ('\x20');
            break;
          case '<': // [WFC: No < in Attribute Values]
            this->fatal_error (ACE_TEXT ("Illegal '<' in AttValue"));
            break;
          case 0:
            this->pop_context (1);
            break;
          default:
            this->obstack_.grow (ch);
            break;
        }
      ch = this->get();
    }
}

int
ACEXML_Parser::parse_entity_reference (void)
{
  ACEXML_Char* replace = this->parse_reference_name ();
  if (replace == 0)
    {
      this->fatal_error (ACE_TEXT ("Invalid Reference name"));
    }

  //  [WFC: Parsed Entity]
  if (this->unparsed_entities_.resolve_entity (replace)) {
    this->fatal_error (ACE_TEXT ("EntityRef refers to unparsed entity"));
  }
  // Look in the internal general entities set first.
  const ACEXML_Char* entity = this->internal_GE_.resolve_entity(replace);

  // Look in the predefined entities.
  if (!entity)
    {
      entity = this->predef_entities_.resolve_entity (replace);
      if (entity)
        {
          // Special case to return the length in case of predefined entities
          this->obstack_.grow (*entity);
          return 1;
        }
    }

  if (!this->validate_)
    {
      if (this->standalone_)
        {
          // [WFC: Entity Declared]
          this->fatal_error (ACE_TEXT ("Undeclared Entity reference"));
        }
      else
        {
          this->content_handler_->skippedEntity (replace);
          return 0;
        }
    }

  // No match in internal subset
  if (!entity
          // or No DTDs
      && (!(this->internal_dtd_ || this->external_dtd_)
          // or Only Internal DTD and no parameter entity references
          || (this->internal_dtd_ && !this->external_dtd_
              && !this->has_pe_refs_)
          // or Standalone = 'yes'
          || this->standalone_))
    {
      // [WFC: Entity Declared]
      this->fatal_error (ACE_TEXT ("Undeclared Entity reference"));
    }

  ACEXML_Char* systemId = 0;
  ACEXML_Char* publicId = 0;
  if (!entity)
    {
      if (this->external_GE_.resolve_entity (replace, systemId, publicId) < 0)
        {
          this->fatal_error (ACE_TEXT ("Undeclared Entity reference"));
        }
      if (this->ref_state_ == ACEXML_ParserInt::IN_ATT_VALUE)
        {
          this->fatal_error (ACE_TEXT ("External EntityRef in Attribute Value"));
        }
      this->external_entity_++;
    }


  // [WFC: No Recursion]
  ACEXML_Char* ref_name = replace;
  int present = this->GE_reference_.insert (ref_name);
  if (present == 1 || present == -1)
    {
      while (this->GE_reference_.pop(ref_name) != -1)
        ;
      this->fatal_error (ACE_TEXT ("Recursion in resolving entity"));
    }

  if (!this->external_entity_)
    {
      ACEXML_StrCharStream* str = 0;
      ACE_NEW_RETURN (str, ACEXML_StrCharStream, -1);
      if (str->open (entity, replace) < 0
          || this->switch_input (str, replace) != 0)
        {
          this->fatal_error (ACE_TEXT ("Unable to create internal input ")
                             ACE_TEXT ("stream"));
        }
      return 0;
    }
  else
    {
      ACEXML_Char* uri = this->normalize_systemid (systemId);
      ACE_Auto_Basic_Array_Ptr<ACEXML_Char> cleanup_uri (uri);
      ACEXML_InputSource* ip = 0;
      if (this->entity_resolver_)
        {
          ip = this->entity_resolver_->resolveEntity (publicId,
                                                      (uri ? uri : systemId));
          if (ip)
            {
              if (this->switch_input (ip, (uri ? uri : systemId),
                                      publicId) != 0)
                {
                  this->fatal_error (ACE_TEXT ("Internal Parser Error"));
                }
              return 0;
            }
        }
      ACEXML_StreamFactory factory;
      ACEXML_CharStream* cstream = factory.create_stream (uri ? uri: systemId);
      if (!cstream) {
        this->fatal_error (ACE_TEXT ("Invalid input source"));
      }
      if (this->switch_input (cstream, systemId, publicId) != 0)
        {
          this->fatal_error (ACE_TEXT ("Internal Parser Error"));
        }
    }
  return 0;
}

int
ACEXML_Parser::parse_PE_reference (void)
{
  ACEXML_Char* replace = this->parse_reference_name ();
  if (replace == 0)
    {
      this->fatal_error (ACE_TEXT ("Invalid PEReference name"));
    }

  // Look in the internal general entities set first.
  const ACEXML_Char* entity = this->internal_PE_.resolve_entity(replace);

  if (!entity &&                     // No match in internal
      (!this->external_dtd_ ||       // or No External DTDs
       this->standalone_))           // or Standalone
    {
      // [VC: Entity Declared]
      this->fatal_error (ACE_TEXT ("Undefined Internal PEReference"));
    }

  ACEXML_Char* systemId = 0;
  ACEXML_Char* publicId = 0;
  if (!entity && this->validate_)
    {
      if (this->external_PE_.resolve_entity (replace, systemId, publicId) < 0)
        {
          this->fatal_error (ACE_TEXT ("Undefined PEReference"));
        }
      this->external_entity_++;
    }

  // [WFC: No Recursion]
  ACEXML_Char* ref_name = replace;
  int present = this->PE_reference_.insert (ref_name);
  if (present == 1 || present == -1)
    {
      while (this->PE_reference_.pop(ref_name) != -1)
        ;
      this->fatal_error (ACE_TEXT ("Recursion in resolving entity"));
    }

  if (entity && !this->external_entity_)
    {
      ACEXML_StrCharStream* sstream = 0;
      ACEXML_String str (entity);
      if (this->ref_state_ != ACEXML_ParserInt::IN_ENTITY_VALUE)
        {
          const ACEXML_Char* ch = ACE_TEXT (" ");
          str = ch + str + ch;
        }
      //  ACE_DEBUG ((LM_DEBUG,
      //             ACE_TEXT ("Entity is %s\n Replacement Text is : %s\n"),
      //             replace, str.c_str()));
      ACE_NEW_RETURN (sstream, ACEXML_StrCharStream, -1);
      if (sstream->open (str.c_str(), replace) < 0
          || this->switch_input (sstream, replace) != 0)
        {
          this->fatal_error (ACE_TEXT ("Error in switching InputSource"));
        }
      return 0;
    }
  else if (this->external_entity_ && this->validate_)
    {
      ACEXML_Char* uri = this->normalize_systemid (systemId);
      ACE_Auto_Basic_Array_Ptr<ACEXML_Char> cleanup_uri (uri);
      ACEXML_InputSource* ip = 0;
      if (this->entity_resolver_)
        {
          ip = this->entity_resolver_->resolveEntity (publicId,
                                                      (uri ? uri : systemId));
        }
      if (ip)
        {
          if (this->switch_input (ip, (uri ? uri : systemId), publicId) != 0)
            {
              this->fatal_error (ACE_TEXT ("Error in switching InputSource"));
            }
          return 0;
        }
      else
        {
          ACEXML_StreamFactory factory;
          ACEXML_CharStream* cstream = factory.create_stream (uri ? uri: systemId);
          if (!cstream) {
            this->fatal_error (ACE_TEXT ("Invalid input source"));
          }
          if (this->switch_input (cstream, systemId, publicId) != 0)
            {
              this->fatal_error (ACE_TEXT ("Error in switching InputSource"));
            }
          if (this->ref_state_ == ACEXML_ParserInt::IN_ENTITY_VALUE)
            {
              ACEXML_Char less, mark;
              if (this->peek() == '<')
                {
                  less = this->get();
                  if (this->peek() == '?')
                    {
                      mark = this->get();
                      if (this->peek() == 'x')
                        {
                          this->parse_text_decl ();
                        }
                      else
                        {
                          this->obstack_.grow (less);
                          this->obstack_.grow (mark);
                        }
                    }
                  this->obstack_.grow (less);
                }
            }
          return 0;
        }
    }
  this->fatal_error (ACE_TEXT ("Undefined PEReference"));
  return -1;
}

int
ACEXML_Parser::parse_entity_value (ACEXML_Char *&str)
{
  ACEXML_ParserInt::ReferenceState temp = this->ref_state_;
  ACEXML_Char quote = this->get ();
  if (quote != '\'' && quote != '"')  // Not a quoted string.
    return -1;
  ACEXML_Char ch = this->get ();
  while (1)
    {
      if (ch == quote)
        {
          str = this->obstack_.freeze ();
          this->ref_state_ = temp;
          return 0;
        }
      switch (ch)
        {
          case '&':
            if (this->peek () == '#')
              {
                if (!this->external_entity_)
                  {
                    ACEXML_Char buf[7];
                    size_t len = sizeof (buf);
                    if (this->parse_char_reference (buf, len) != 0)
                      {
                        // [WFC: Legal Character]
                        this->fatal_error (ACE_TEXT ("Invalid character ")
                                           ACE_TEXT ("reference"));
                        return -1;
                      }
                    for (size_t j = 0; j < len; ++j)
                      this->obstack_.grow (buf[j]);
                    break;
                  }
              }
            this->obstack_.grow (ch);
            break;
          case '%':
            if (!this->external_entity_)
              {
                this->ref_state_ = ACEXML_ParserInt::IN_ENTITY_VALUE;
                this->parse_PE_reference();
                break;
              }
            this->obstack_.grow (ch);
            break;
          case 0:
            this->pop_context (0);
            break;
          default:
            this->obstack_.grow (ch);
            break;
        }
      ch = this->get();
    }
}

ACEXML_Char *
ACEXML_Parser::parse_name (ACEXML_Char ch)
{
  if (ch == 0)
    ch = this->get ();
  if (!this->isLetter (ch) && ch != '_' && ch != ':')
    return 0;
  while (ch) {
    this->obstack_.grow (ch);
    ch = this->peek ();
    if (!this->isNameChar (ch))
      break;
    ch = this->get ();
  };
  return this->obstack_.freeze ();
}

ACEXML_Char*
ACEXML_Parser::parse_nmtoken (ACEXML_Char ch)
{
  if (ch == 0)
    ch = this->get ();
  if (!this->isNameChar (ch))
    return 0;
  while (ch) {
    this->obstack_.grow (ch);
    ch = this->peek ();
    if (!this->isNameChar (ch))
      break;
    ch = this->get ();
  };
  return this->obstack_.freeze ();
}

int
ACEXML_Parser::parse_version_num (ACEXML_Char*& str)
{
  ACEXML_Char quote = this->get ();
  if (quote != '\'' && quote != '"')  // Not a quoted string.
    return -1;
  int numchars = 0;
  while (1)
    {
      ACEXML_Char ch = this->get ();
      if (ch == quote && !numchars)
        return -1;
      else if (ch == quote)
        {
          str = this->obstack_.freeze ();
          return 0;
        }
      // [26]    VersionNum ::= ([a-zA-Z0-9_.:] | '-')+
      if (ch == '-' || ((ch >= 'a' && ch <= 'z') ||
                        (ch >= 'A' && ch <= 'Z') ||
                        (ch >= '0' && ch <= '9') ||
                        (ch == '_' || ch == '.' || ch == ':')))
        {
          this->obstack_.grow (ch);
          numchars++;
        }
      else
        return -1;
    }
}

int
ACEXML_Parser::parse_system_literal (ACEXML_Char*& str)
{
  const ACEXML_Char quote = this->get();
  if (quote != '\'' && quote != '"')  // Not a quoted string.
    return -1;
  while (1)
    {
      ACEXML_Char ch = this->get ();
      if (ch == quote)
        {
          str = this->obstack_.freeze ();
          return 0;
        }
      switch (ch)
        {
          case '\x00': case '\x01': case '\x02': case '\x03': case '\x04':
          case '\x05': case '\x06': case '\x07': case '\x08': case '\x09':
          case '\x0A': case '\x0B': case '\x0C': case '\x0D': case '\x0E':
          case '\x0F': case '\x10': case '\x11': case '\x12': case '\x13':
          case '\x14': case '\x15': case '\x16': case '\x17': case '\x18':
          case '\x19': case '\x1A': case '\x1B': case '\x1C': case '\x1D':
          case '\x1E': case '\x1F': case '\x7F': case '\x20': case '<':
          case '>': case '#': case '%':
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Invalid char %c in SystemLiteral\n"), ch));
            return -1;
          default:
            this->obstack_.grow (ch);
        }
    }
}

int
ACEXML_Parser::parse_pubid_literal (ACEXML_Char*& str)
{
  const ACEXML_Char quote = this->get();
  if (quote != '\'' && quote != '"')  // Not a quoted string.
    return -1;
  while (1)
    {
      ACEXML_Char ch = this->get ();
      if (ch == quote)
        {
          str = this->obstack_.freeze ();
          return 0;
        }
      else if (this->isPubidChar (ch))
        this->obstack_.grow (ch);
      else
        return -1;
    }
}

int
ACEXML_Parser::parse_encname (ACEXML_Char*& str)
{
  const ACEXML_Char quote = this->get ();
  if (quote != '\'' && quote != '"')  // Not a quoted string.
    return -1;
  int numchars = 0;
  while (1)
    {
      ACEXML_Char ch = this->get ();
      if (ch == quote && !numchars)
        return -1;
      else if (ch == quote)
        {
          str = this->obstack_.freeze ();
          return 0;
        }
      // [81]    EncName ::= [A-Za-z] ([A-Za-z0-9._] | '-')*
      if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
          && !numchars)
        return -1;
      if (ch == '-' || ((ch >= 'a' && ch <= 'z') ||
                        (ch >= 'A' && ch <= 'Z') ||
                        (ch >= '0' && ch <= '9') ||
                        (ch == '_' || ch == '.')))
        {
          this->obstack_.grow (ch);
          numchars++;
        }
      else
        return -1;
    }
}

int
ACEXML_Parser::parse_sddecl (ACEXML_Char*& str)
{
  ACEXML_Char quote = this->get ();
  if (quote != '\'' && quote != '"')  // Not a quoted string.
    return -1;
  int numchars = 0;
  while (1)
    {
      ACEXML_Char ch = this->get ();
      if (ch == quote && numchars < 2)
        return -1;
      else if (ch == quote)
        {
          str = this->obstack_.freeze ();
          return 0;
        }
      // [32] SDDecl ::= S 'standalone' Eq (("'" ('yes' | 'no') "'")
      //                                   | ('"' ('yes' | 'no') '"'))
      switch (ch)
        {
          case 'y': case 'e': case 's': case 'n': case 'o':
            this->obstack_.grow (ch);
            numchars++;
            break;
          default:
            return -1;
        }
    }
}

void
ACEXML_Parser::prefix_mapping (const ACEXML_Char* prefix,
                               const ACEXML_Char* uri,
                               int start)
{
  if (this->namespaces_)
    {
      const ACEXML_Char* temp = (prefix == 0) ? empty_string : prefix;
      if (start) {
        this->content_handler_->startPrefixMapping (temp, uri);
      }
      else
        {
          this->content_handler_->endPrefixMapping(temp);
        }
    }
}

int
ACEXML_Parser::switch_input (ACEXML_CharStream* cstream,
                             const ACEXML_Char* systemId,
                             const ACEXML_Char* publicId)
{
  ACEXML_InputSource* input = 0;
  ACE_NEW_RETURN (input, ACEXML_InputSource (cstream), -1);
  return this->switch_input (input, systemId, publicId);
}

int
ACEXML_Parser::switch_input (ACEXML_InputSource* input,
                             const ACEXML_Char* systemId,
                             const ACEXML_Char* publicId)
{
  ACEXML_LocatorImpl* locator = 0;
  if (!systemId)
    systemId = input->getSystemId();
  ACE_NEW_RETURN (locator, ACEXML_LocatorImpl (systemId, publicId), -1);
  ACEXML_Parser_Context* new_context = 0;
  ACE_NEW_RETURN (new_context, ACEXML_Parser_Context(input, locator), -1);
  if (this->push_context (new_context) != 0)
    {
      ACE_ERROR ((LM_ERROR, "Unable to switch input streams"));
      delete new_context;
      return -1;
    }
  this->current_ = new_context;
  this->content_handler_->setDocumentLocator (this->current_->getLocator());
  return 0;
}

int
ACEXML_Parser::push_context (ACEXML_Parser_Context* context)
{
  if (this->ctx_stack_.push (context) < 0)
    {
      ACE_ERROR ((LM_ERROR, "Unable to push input source onto the stack"));
      return -1;
    }
  return 0;
}

size_t
ACEXML_Parser::pop_context (int GE_ref)
{
  size_t nrelems = this->ctx_stack_.size();
  if (nrelems <= 1)
    {
      this->fatal_error(ACE_TEXT ("Unexpected end-of-file"));
    }

  ACEXML_Parser_Context* temp = 0;
  int retval = this->ctx_stack_.pop (temp);
  if (retval != 0)
    {
      this->fatal_error (ACE_TEXT ("Unable to pop element of the input stack"));
    }
  delete temp;
  if (this->ctx_stack_.top (this->current_) != 0)
    {
      this->fatal_error (ACE_TEXT ("Unable to read top element of input stack"));
    }
  ACEXML_Char* reference = 0;
  if (GE_ref == 1 && this->GE_reference_.size() > 0)
    {
      if (this->GE_reference_.pop (reference) < 0)
        {
          this->fatal_error (ACE_TEXT ("Internal Parser Error"));
        }
    }
  else if (GE_ref == 0 && this->PE_reference_.size() > 0)
    {
      if (this->PE_reference_.pop (reference) < 0)
        {
          this->fatal_error (ACE_TEXT ("Internal Parser Error"));
        }
    }
  nrelems = this->ctx_stack_.size();

  if (this->external_entity_ && (GE_ref == 0 || GE_ref == 1))
    this->external_entity_--;

  this->content_handler_->setDocumentLocator (this->current_->getLocator());

  return nrelems;
}

int
ACEXML_Parser::getFeature (const ACEXML_Char *name)
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
  else if (ACE_OS::strcmp (name, ACEXML_Parser::validation_feature_) == 0)
    {
      return this->validate_;
    }
  throw ACEXML_SAXNotRecognizedException (name);
}



void
ACEXML_Parser::setFeature (const ACEXML_Char *name, int boolean_value)
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
  else if (ACE_OS::strcmp (name, ACEXML_Parser::validation_feature_) == 0)
    {
      this->validate_ = (boolean_value == 0 ? 0 : 1);
      return;
    }

  throw ACEXML_SAXNotRecognizedException (name);
}

void *
ACEXML_Parser::getProperty (const ACEXML_Char *name)
{
  throw ACEXML_SAXNotSupportedException (name);
}

void
ACEXML_Parser::setProperty (const ACEXML_Char *name,
                            void *)
{
  throw ACEXML_SAXNotSupportedException (name);
}

void
ACEXML_Parser::error (const ACEXML_Char* msg)
{
  ACEXML_SAXParseException* exception = 0;
  ACE_NEW_NORETURN (exception, ACEXML_SAXParseException (msg));
  if (this->error_handler_)
      this->error_handler_->error (*exception);
  else
    throw exception;
  return;
}

void
ACEXML_Parser::warning (const ACEXML_Char* msg)
{
  ACEXML_SAXParseException* exception = 0;
  ACE_NEW_NORETURN (exception, ACEXML_SAXParseException (msg));
  if (this->error_handler_)
    this->error_handler_->warning (*exception);
  delete exception;
  return;
}

void
ACEXML_Parser::fatal_error (const ACEXML_Char* msg)
{
  ACEXML_SAXParseException* exception = 0;
  ACE_NEW_NORETURN (exception, ACEXML_SAXParseException (msg));
  if (this->error_handler_)
    this->error_handler_->fatalError (*exception);
  this->reset();
  throw exception;
  return;
}

void
ACEXML_Parser::parse_version_info (void)
{
  ACEXML_Char* astring;
  if (this->parse_token (ACE_TEXT("ersion")) < 0
      || this->skip_equal () != 0
      || this->parse_version_num (astring) != 0)
    {
      this->fatal_error (ACE_TEXT ("Invalid VersionInfo specification"));
      return;
    }
  if (ACE_OS::strcmp (astring, ACE_TEXT ("1.0")) != 0)
    {
      this->fatal_error (ACE_TEXT ("ACEXML Parser supports XML version 1.0 ")
                         ACE_TEXT ("documents only"));
    }
}

void
ACEXML_Parser::parse_encoding_decl (void)
{
  ACEXML_Char* astring = 0;
  if ((this->parse_token (ACE_TEXT("ncoding")) < 0)
      || this->skip_equal () != 0
      || this->parse_encname (astring) != 0)
    {
      this->fatal_error (ACE_TEXT ("Invalid EncodingDecl specification"));
    }
  const ACEXML_Char* encoding = this->current_->getInputSource()->getEncoding();
  if (encoding != 0 && ACE_OS::strcasecmp (astring, encoding) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Detected Encoding is %s ")
                  ACE_TEXT (": Declared Encoding is %s\n"),
                  encoding, astring));
      this->warning (ACE_TEXT ("Declared encoding differs from detected ")
                     ACE_TEXT ("encoding"));
    }
}

int
ACEXML_Parser::parse_text_decl (void)
{
  // Read xml
  if (this->parse_token (ACE_TEXT("xml")) < 0)
    {
      this->fatal_error(ACE_TEXT ("Expecting keyword 'xml' in TextDecl"));
    }

  ACEXML_Char fwd = this->skip_whitespace();
  // Read version
  if (fwd == 'v')
    {
      this->parse_version_info ();
      fwd = this->skip_whitespace();
    }

  if (fwd == 'e')
    {
      this->parse_encoding_decl ();
      fwd = this->skip_whitespace();
    }
  else
    {
      this->fatal_error (ACE_TEXT ("Missing encodingDecl in TextDecl"));
    }

  if (fwd == '?' && this->get() == '>')
    return 0;
  // All the rules fail. So return an error.
  this->fatal_error (ACE_TEXT ("Invalid TextDecl"));
  return -1;
}

void
ACEXML_Parser::parse_xml_decl (void)
{
  // Read <?xml
  if (this->parse_token (ACE_TEXT("xml")) < 0)
    {
      this->fatal_error(ACE_TEXT ("Expecting keyword xml in XMLDecl"));
    }

  ACEXML_Char fwd = this->skip_whitespace();

  // Read version
  if (fwd != 'v')
    {
      this->fatal_error (ACE_TEXT ("Expecting VersionInfo declaration"));
    }

  this->parse_version_info ();

  fwd = this->skip_whitespace();
  if (fwd != '?')
    {
      if (fwd == 'e')
        {
          this->parse_encoding_decl ();
          fwd = this->skip_whitespace();
        }
      if (fwd == 's')
        {
          ACEXML_Char* astring;
          if ((this->parse_token (ACE_TEXT("tandalone")) == 0) &&
              this->skip_equal () == 0 &&
              this->parse_sddecl (astring) == 0)
            {
              if (ACE_OS::strcmp (astring, ACE_TEXT ("yes")) == 0)
                this->standalone_ = 1;
              fwd = this->skip_whitespace();
            }
        }
    }
  if (fwd == '?' && this->get() == '>')
    return;
  // All the rules fail. So return an error.
  this->fatal_error (ACE_TEXT ("Invalid XMLDecl declaration"));
}

int
ACEXML_Parser::parse_comment (void)
{
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
ACEXML_Parser::parse_processing_instruction (void)
{
  const ACEXML_Char *pitarget = this->parse_name ();
  ACEXML_Char *instruction = 0;

  if (!ACE_OS::strcasecmp (ACE_TEXT ("xml"), pitarget))
    {
      // Invalid PITarget name.
      this->fatal_error(ACE_TEXT ("PI can't have 'xml' in PITarget"));
    }

  int state = 0;

  ACEXML_Char ch = this->skip_whitespace();
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
                                                               instruction);
                this->obstack_.unwind (const_cast<ACEXML_Char*> (pitarget));
                return 0;
              }
            break;
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

void
ACEXML_Parser::reset (void)
{
  this->doctype_ = 0;
  if (this->ctx_stack_.pop (this->current_) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Mismatched push/pop of Context stack")));
  if (this->current_)
    {
      this->current_->getInputSource()->getCharStream()->rewind();

      this->current_->setInputSource (0);
      delete this->current_;
      this->current_ = 0;
    }

  ACEXML_Char* temp = 0;
  while (this->GE_reference_.pop (temp) != -1)
    ;
  while (this->PE_reference_.pop (temp) != -1)
    ;
  this->obstack_.release();
  this->alt_stack_.release();
  this->xml_namespace_.reset();
  this->nested_namespace_ = 0;
  this->internal_GE_.reset();
  this->external_GE_.reset();
  this->unparsed_entities_.reset();
  this->predef_entities_.reset();
  this->internal_PE_.reset();
  this->external_PE_.reset();
  this->notations_.reset();
  this->ref_state_ = ACEXML_ParserInt::INVALID;
  this->external_subset_ = 0;
  this->external_entity_ = 0;
  this->has_pe_refs_ = 0;
  this->standalone_ = 0;
  this->external_dtd_ = 0;
  this->internal_dtd_ = 0;
}

