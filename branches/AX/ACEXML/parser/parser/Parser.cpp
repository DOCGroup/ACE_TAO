// $Id$

#include "parser/parser/Parser.h"
#include "common/Transcode.h"
#include "common/AttributesImpl.h"

#if !defined (__ACEXML_INLINE__)
# include "parser/parser/Parser.i"
#endif /* __ACEXML_INLINE__ */

/***
TO-DO:

END-OF-LINE handling: x read quoted string
                        ignore whitespace
                        processing instruction
                      x element contents

Figure out how to handle namespace here:
and when to invoke start/endPrefixMapping?

Make sure we are freezing the obstack in all cases.

***/

static const ACEXML_Char default_attribute_type[] = {'C', 'D', 'A', 'T', 'A', 0};
static const ACEXML_Char empty_string[] = { 0 };

const ACEXML_Char
ACEXML_Parser::simple_parsing_name_[] = { 'S', 'i', 'm', 'p', 'l', 'e', 0 };

ACEXML_Parser::ACEXML_Parser (void)
  :   dtd_handler_ (0),
      entity_resolver_ (0),
      content_handler_ (0),
      error_handler_ (0),
      instream_ (0),
      doctype_ (0),
      dtd_system_ (0),
      dtd_public_ (0),
      simple_parsing_ (0)
{
}

ACEXML_Parser::~ACEXML_Parser (void)
{
}

int
ACEXML_Parser::getFeature (const ACEXML_Char *name,
                           ACEXML_Env &xmlenv)
  // ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
  //                       ACEXML_SAXNotSupportedException))
{
  if (ACE_OS_String::strcmp (name, ACEXML_Parser::simple_parsing_name_) == 0)
    return this->simple_parsing_;

  xmlenv.exception (new ACEXML_SAXNotRecognizedException ());
  return -1;
}

void *
ACEXML_Parser::getProperty (const ACEXML_Char *name,
                            ACEXML_Env &xmlenv)
  //      ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
  //                       ACEXML_SAXNotSupportedException))
{
  // @@ Not implemented.
  ACE_UNUSED_ARG (name);

  xmlenv.exception (new ACEXML_SAXNotSupportedException ());
  return 0;
}

void
ACEXML_Parser::setFeature (const ACEXML_Char *name,
                           int boolean_value,
                           ACEXML_Env &xmlenv)
  //      ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
  //                       ACEXML_SAXNotSupportedException))
{
  if (ACE_OS_String::strcmp (name, ACEXML_Parser::simple_parsing_name_) == 0)
    this->simple_parsing_ = (boolean_value == 0 ? 0 : 1);

  xmlenv.exception (new ACEXML_SAXNotRecognizedException ());
}

void
ACEXML_Parser::setProperty (const ACEXML_Char *name,
                            void *value,
                            ACEXML_Env &xmlenv)
  //      ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
  //                       ACEXML_SAXNotSupportedException))
{
  // @@ Not implemented.
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (value);

  xmlenv.exception (new ACEXML_SAXNotSupportedException ());
}


void
ACEXML_Parser::parse (ACEXML_InputSource *input,
                      ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (input == 0)
    {
      xmlenv.exception (new ACEXML_SAXException ("No valid input source available"));
      return;
    }

  // @@ Set up Locator.

  if ((this->instream_ = input->getCharStream ()) == 0)
    {
      xmlenv.exception (new ACEXML_SAXException ("No valid input source available"));
      return;
    }

  if (this->simple_parsing_ == 0)
    {
      this->parse_xml_prolog (xmlenv);
      ACEXML_CHECK;
    }
  // @@ Should startDocument come before or after parsing the DTD definition?
  this->content_handler_->startDocument (xmlenv);
  ACEXML_CHECK;

  int doctype_defined = 0;

  for (int prolog_done = 0; prolog_done == 0; )
    {
      if (this->skip_whitespace (0) != '<')
        {
          xmlenv.exception (new ACEXML_SAXParseException ("Expecting '<'"));
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
              this->parse_doctypedecl (xmlenv);
              ACEXML_CHECK;
              doctype_defined = 1;
              break;
            }
          else if (fwd == '-')  // COMMENT
            {
              if (this->grok_comment () < 0)
                {
                  xmlenv.exception (new ACEXML_SAXParseException ("Invalid comment"));
                  return;
                }
            }
          else
            {
              xmlenv.exception (new ACEXML_SAXParseException ("Duplicate DOCTYPE definitions"));
              return;
            }
          break;
        case '?':
          this->parse_processing_instruction (xmlenv);
          ACEXML_CHECK;
          break;
        case 0:
          xmlenv.exception (new ACEXML_SAXParseException ("Unexpected EOF"));
          break;
        default:                // Root element begins
          prolog_done = 1;
          break;
        }
    }

  // Now parse root element.
  this->parse_element (1, xmlenv);
  ACEXML_CHECK;

  this->content_handler_->endDocument (xmlenv);
  // ACEXML_CHECK;
}

void
ACEXML_Parser::parse (const ACEXML_Char *systemId,
                      ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // @@ Not implemented.
  ACE_UNUSED_ARG (systemId);

  xmlenv.exception (new ACEXML_SAXNotSupportedException ());
  return ;
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

void
ACEXML_Parser::parse_xml_prolog (ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->get () != '<' ||
      this->get () != '?' ||
      this->get () != 'x' ||
      this->get () != 'm' ||
      this->get () != 'l')
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Unrecognized XML Decl"));
      return;
    }

  ACEXML_Char *astring;

  if (this->skip_whitespace (0) != 'v' || // Discard whitespace
      this->get () != 'e' ||
      this->get () != 'r' ||
      this->get () != 's' ||
      this->get () != 'i' ||
      this->get () != 'o' ||
      this->get () != 'n' ||
      this->skip_equal () != 0 ||
      this->get_quoted_string (astring) != 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Unrecognized XML Decl"));
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
              // Do nothing.  Fall down to consume the '?'
              // and wrap up the XML Decl parsing.
            }
          else if (xmldecl_state == 0 && fwd == 'e')
            {
              if (this->get () == 'n' &&
                  this->get () == 'c' &&
                  this->get () == 'o' &&
                  this->get () == 'd' &&
                  this->get () == 'i' &&
                  this->get () == 'n' &&
                  this->get () == 'g' &&
                  this->skip_equal () == 0 &&
                  this->get_quoted_string (astring) == 0)
                {
                  seen_encoding = 1;
                  // @@ Handle encoding here.  We don't handle
                  // various encodings for this parser.

                  continue;
                }
              else
                {
                  xmlenv.exception (new ACEXML_SAXParseException ("Unrecognized XML Decl"));
                  return;
                }
            }
          else if (xmldecl_state < 2 && fwd == 's')
            {
              if (this->get () == 't' &&
                  this->get () == 'a' &&
                  this->get () == 'n' &&
                  this->get () == 'd' &&
                  this->get () == 'a' &&
                  this->get () == 'l' &&
                  this->get () == 'o' &&
                  this->get () == 'n' &&
                  this->get () == 'g' &&
                  this->skip_equal () == 0 &&
                  this->get_quoted_string (astring) == 0)
                {
                  xmldecl_state = 2;
                  if (ACE_OS::strcmp (astring, "yes") == 0)
                    {
                      // @@ This is a standalone XML file.
                      continue;
                    }
                  else if (ACE_OS::strcmp (astring, "no") == 0)
                    {
                      // @@ This is not a stand alone XML file.
                      continue;
                    }
                }
              xmlenv.exception (new ACEXML_SAXParseException ("Unrecognized XML Decl"));
              return;
            }
          else
            {
              xmlenv.exception (new ACEXML_SAXParseException ("Unrecognized XML Decl"));
              return;
            }
        }


      this->get ();  // consume '?'

      if (this->get() != '>')
        {
          xmlenv.exception (new ACEXML_SAXParseException ("Unrecognized XML Decl"));
          return;
        }
      return;
    }
  // End parsing XML Decl.
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

  while (state < 3)             // Waiting for the trailing three
                                // character '-->'.  Notice that
                                // according to the spec, '--->'
                                // is not a valid closing comment
                                // sequence.  But we'll let it pass
                                // anyway.
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

int
ACEXML_Parser::parse_processing_instruction (ACEXML_Env &xmlenv)
{
  if (this->get () != '?')
    {                           // How did we get here?
      xmlenv.exception (new ACEXML_SAXParseException ("Internal error"));
      return -1;
    }
  const ACEXML_Char *pitarget = this->read_name ();
  ACEXML_Char *instruction = 0;

  if (ACE_OS_String::strcasecmp ("xml", pitarget) != 0)
    {                           // Invalid PITarget name.
      xmlenv.exception
        (new ACEXML_SAXParseException ("PITarget name cannot start with 'xml'"));
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
                                                             instruction,
                                                             xmlenv);
              ACEXML_CHECK_RETURN (-1);
              return 0;
            }
          break;
        case 0x0D:                // End-of-Line handling
          ch = (this->peek () == 0x0A ? this->get () : 0x0A);
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
ACEXML_Parser::parse_doctypedecl (ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->get () != 'D' ||
      this->get () != 'O' ||
      this->get () != 'C' ||
      this->get () != 'T' ||
      this->get () != 'Y' ||
      this->get () != 'P' ||
      this->get () != 'E')
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Expecting keyword 'DOCTYPE'"));
      return -1;
    }

  ACEXML_Char nextch = this->skip_whitespace (0);
  if (nextch == 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Expecting a DOCTYPE name"));
      return -1;
    }

  this->doctype_ = this->read_name (nextch);

  this->skip_whitespace_count (&nextch);

  if (nextch == 'S' || nextch == 'P') // ExternalID defined
    {
      this->parse_external_id_and_ref (this->dtd_public_,
                                       this->dtd_system_,
                                       xmlenv);
      if (xmlenv.exception () != 0)
        return -1;
      else if (this->dtd_public_ == 0)
          cout << "ACEXML Parser got external DTD id: SYSTEM " << this->dtd_system_
               << endl;
      else
          cout << "==> ACEXML Parser got DTD external id: PUBLIC "
               << this->dtd_public_
               << " "
               << this->dtd_system_
               << endl;
    }

  nextch = this->skip_whitespace (0);
  switch (nextch)
    {
    case '[':                   // Internal DTD definitionl
      if (this->parse_internal_dtd (xmlenv) < 0)
        return -1;              // Error in markupdecl
      break;
    case '>':                   // End of DTD definition
      // this is an XML document without a dectypedecl.
      return 0;
    case '0':
      xmlenv.exception (new ACEXML_SAXParseException ("Unexpected EOF"));
      return -1;
    default:
      break;
    }

  if (this->skip_whitespace (0) != '>')
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Internal error"));
      return -1;
    }
  return 0;
}

void
ACEXML_Parser::parse_element (int is_root, ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // Parse STag.

  const ACEXML_Char *startname = this->read_name ();

  if (startname == 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Unexpected EOF"));
      return;
    }

  if (is_root &&
      this->doctype_ != 0 &&
      ACE_OS_String::strcmp (startname, this->doctype_) != 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Root element missing."));
      return;
    }

  const ACEXML_Char *endname = 0;
  ACEXML_AttributesImpl attributes;
  ACEXML_Char ch;
  int new_namespace = 0;
  const ACEXML_Char *ns_uri, *ns_lname; // namespace URI and localName

  for (int start_element_done = 0; start_element_done == 0;)
    {
      ch = this->skip_whitespace (0);

      switch (ch)
        {
        case 0:
          xmlenv.exception (new ACEXML_SAXParseException ("Internal error"));
          return;

        case '/':
          if (this->get () != '>')
            {
              xmlenv.exception (new ACEXML_SAXParseException ("Expectint '>'"));
              return;
            }
          else
            {
              this->xml_namespace_.processName (startname, ns_uri, ns_lname, 0);
              this->content_handler_->startElement (ns_uri,
                                                    ns_lname,
                                                    startname,
                                                    &attributes,
                                                    xmlenv);
              ACEXML_CHECK;
              this->content_handler_->endElement (ns_uri, ns_lname, startname, xmlenv);
              ACEXML_CHECK;
            }
          if (new_namespace != 0)
            this->xml_namespace_.popContext ();
          return;

        case '>':
          this->xml_namespace_.processName (startname, ns_uri, ns_lname, 0);
          this->content_handler_->startElement (ns_uri,
                                                ns_lname,
                                                startname,
                                                &attributes,
                                                xmlenv);
          ACEXML_CHECK;
          start_element_done = 1;
          break;
        default:
          {
            ACEXML_Char *attvalue = 0;
            ACEXML_Char *attname = this->read_name (ch);

            if (attname == 0 ||
                this->skip_equal () != 0 ||
                this->get_quoted_string (attvalue) != 0)
              {
                xmlenv.exception (new ACEXML_SAXParseException ("Error reading attribute"));
                return;
              }

            // Handling new namespace if any.  Notice that the order of namespace
            // declaration does matter.
            if (attname[0] == 'x' &&
                attname[1] == 'm' &&
                attname[2] == 'l' &&
                attname[3] == 'n' &&
                attname[4] == 's')
              {
                if (new_namespace == 0)
                  {
                    this->xml_namespace_.pushContext ();
                    new_namespace = 1;
                  }

                ACE_Tokenizer ns_att (attname);
                ns_att.delimiter_replace (':', 0);
                ACEXML_Char *xmlns_prefix, *ns_name;

                xmlns_prefix = ns_att.next ();
                ns_name = ns_att.next ();

                if (ns_name == 0)
                  {
                    // @@ Check return value?
                    this->xml_namespace_.declarePrefix (empty_string,
                                                        attvalue);
                  }
                else
                  {
                    // @@ Check return value?
                    this->xml_namespace_.declarePrefix (ns_name,
                                                        attvalue);
                  }
              }
            else
              {
                const ACEXML_Char *uri, *lName;
                this->xml_namespace_.processName (attname, uri, lName, 1);

                attributes.addAttribute (uri,
                                         lName,
                                         attname,
                                         default_attribute_type,
                                         attvalue);
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
    ch = this->get ();

    switch (ch)
      {
      case 0:
        xmlenv.exception (new ACEXML_SAXParseException ("Internal error"));
        return;

      case '<':
        // Push out old 'characters' event.
        if (cdata_length != 0)
          {
            cdata = this->obstack_.freeze ();
            this->content_handler_->characters (cdata,
                                                0,
                                                cdata_length,
                                                xmlenv);
            ACEXML_CHECK;
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
                    xmlenv.exception
                      (new ACEXML_SAXParseException ("Error parsing comment"));
                    return ;
                  }
              }
            else if (ch == '[') // a CDATA section.
              {
                this->parse_cdata (xmlenv);
                ACEXML_CHECK;
              }
            else
              {
                xmlenv.exception (new ACEXML_SAXParseException ("Unexpected character"));
                return;
              }
            break;
          case '?':             // a PI.
            this->parse_processing_instruction (xmlenv);
            ACEXML_CHECK;
            break;
          case '/':             // an ETag.
            this->get ();       // consume '/'
            endname = this->read_name ();
            if (endname == 0 ||
                ACE_OS_String::strcmp (startname, endname) != 0)
              {
                xmlenv.exception
                  (new ACEXML_SAXParseException ("Mismatched End-tag encountered"));
                return ;
              }
            if (this->skip_whitespace (0) != '>')
              {
                xmlenv.exception
                  (new ACEXML_SAXParseException ("Expecting '>' in an end-tag"));
                return;
              }
            this->content_handler_->endElement (ns_uri,
                                                ns_lname,
                                                endname,
                                                xmlenv);
            ACEXML_CHECK;

            if (new_namespace != 0)
              this->xml_namespace_.popContext ();
            return;

          default:              // a new nested element?
            this->parse_element (0, xmlenv);
            ACEXML_CHECK;
            break;
          }
        break;
      case '&':
        {
          const ACEXML_String *replace = 0;
          ACEXML_String charval;
          char buffer[6];

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
              xmlenv.exception (new ACEXML_SAXParseException ("Internal Error?"));
              return;
            }
          if (this->try_grow_cdata (replace->length (), cdata_length, xmlenv) == 0)
            {
              cdata_length = replace->length ();
              for (size_t i = 0; i < replace->length (); ++i)
                this->obstack_.grow ((*replace)[i]);
            }
          else
            return;
        }
        break;
      case 0x0D:                // End-of-Line handling
        ch = (this->peek () == 0x0A ? this->get () : 0x0A);
        // Fall thru...
      default:
        ++cdata_length;
        cdata = this->obstack_.grow (ch);
        if (cdata == 0)
          {
            cdata = this->obstack_.freeze ();
            this->content_handler_->characters (cdata,
                                                0,
                                                cdata_length,
                                                xmlenv);
            ACEXML_CHECK;
            this->obstack_.grow (ch);
            cdata_length = 1;   // the missing char.
          }
      }
  }

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
#if 1                           // depending on what we default char we are using here
                                // we assume UTF-8 here
          if ((clen = ACEXML_Transcoder::ucs42utf8 (sum, buf, len)) < 0)
            return -1;
#elif 1                         // or UTF-16, currently disabled.
          if ((clen = ACEXML_Transcoder::ucs42utf16 (sum, buf, len)) < 0)
            return -1;
#elif 0                         // UCS 4, not likely
          buf [0] = sum;
          buf [1] = 0;
#endif
          buf [clen] = 0;
          return 0;
        default:
          return -1;
        }
      more_digit = 1;
    }
  return -1;
}

const ACEXML_String *
ACEXML_Parser::parse_reference (void)
{
  // @@ We'll use a temporary buffer here as the Obstack are most likely
  // be in use when we come here.  This put a limit on the max length of
  // a reference.
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
ACEXML_Parser::parse_cdata (ACEXML_Env &xmlenv)
{
  if (this->get () != '[' ||
      this->get () != 'C' ||
      this->get () != 'D' ||
      this->get () != 'A' ||
      this->get () != 'T' ||
      this->get () != 'A' ||
      this->get () != '[')
    {
      xmlenv.exception (new ACEXML_SAXParseException ("'[CDATA[' expected"));
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
              this->content_handler_->characters (cdata,
                                                  0,
                                                  datalen,
                                                  xmlenv);
              // ACEXML_CHECK_RETURN (-1);
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
          if (this->try_grow_cdata (1, datalen, xmlenv) < 0)
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
ACEXML_Parser::try_grow_cdata (size_t size, size_t &len, ACEXML_Env &xmlenv)
{
  if (this->obstack_.request (size) != 0)
    {
      if (len != 0)
        {
          ACEXML_Char *cdata = this->obstack_.freeze ();
          if (cdata == 0)
            {
              xmlenv.exception (new ACEXML_SAXParseException ("Internal Error growing CDATA buffer"));
              return -1;
            }
          this->content_handler_->characters (cdata,
                                              0,
                                              len,
                                              xmlenv);
          ACEXML_CHECK_RETURN (-1);
          len = 0;              // reset counter
          if (this->obstack_.request (size) == 0)
            return 0;
        }

      xmlenv.exception (new ACEXML_SAXParseException ("Internal error, buffer overflowed"));
      return -1;
    }
  return 0;
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
  if (quote != '\'' && quote != '"')  // Not a quoted string found.
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
      char buffer[6];
      size_t i = 0;

      switch (ch)
        {
        case '&':

          if (this->peek () == '#')
            {
              if (this->parse_char_reference (buffer, 6) != 0)
                {
                  //                  xmlenv.exception (new ACEXML_SAXParseException ("CharRef does not resolves to a valid character"));
                  return -1;
                }
              charval.set (buffer, 0);
              replace = &charval;
            }
          else
              replace = this->parse_reference ();

          if (replace == 0)
            {
              //              xmlenv.exception (new ACEXML_SAXParseException ("Undefined reference"));
              return -1;
            }
          for (i = 0; i < replace->length (); ++i)
            this->obstack_.grow ((*replace)[i]);
          // handle reference here.
          break;
        case 0x0D:                // End-of-Line handling
          ch = (this->peek () == 0x0A ? this->get () : 0x0A);
          // Fall thru...
        default:
          this->obstack_.grow (ch);
          break;
        }
    }
}

int
ACEXML_Parser::parse_internal_dtd (ACEXML_Env &xmlenv)
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
                    if (this->parse_element_decl (xmlenv) < 0)
                      return -1;
                    break;

                  case 'N':
                    if (this->parse_entity_decl (xmlenv) < 0)
                      return -1;
                    break;

                  default:
                    xmlenv.exception (new ACEXML_SAXParseException ("Invalid keyword in decl spec"));
                    return -1;
                  }
                break;

              case 'A':         // An ATTLIST decl
                if (this->parse_attlist_decl (xmlenv) < 0)
                  return -1;
                break;

              case 'N':         // A NOTATION decl
                if (this->parse_notation_decl (xmlenv) < 0)
                  return -1;
                break;

              case '-':         // a comment.
                if (this->grok_comment () < 0)
                  {
                    xmlenv.exception (new ACEXML_SAXParseException ("Error parsing comment"));
                    return -1;
                  }
                break;
              case 0:
                xmlenv.exception (new ACEXML_SAXParseException ("Unexpected EOF"));
                return -1;
              default:
                xmlenv.exception (new ACEXML_SAXParseException ("Invalid char. follows '<!' in markupdecl"));
                return -1;
              }
            break;

          case '?':             // PI
            this->parse_processing_instruction (xmlenv);
            ACEXML_CHECK_RETURN (-1);
            break;

          case 0:
            xmlenv.exception (new ACEXML_SAXParseException ("Unexpected EOF"));
            return -1;

          default:
            xmlenv.exception (new ACEXML_SAXParseException ("Invalid char. follow '<' in markupdecl"));
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
        xmlenv.exception (new ACEXML_SAXParseException ("Unexpected EOF"));
        return -1;

      default:
        xmlenv.exception (new ACEXML_SAXParseException ("Expecting markupdecl or DecSep"));
        return -1;
      };

    // To fully conform with the spec., whitespaces are only allowed
    // following a 'DeclSep' section.  However, I found it
    // hard/impossible to eliminate all the whitespaces between
    // markupdecls.

    nextch = this->skip_whitespace (0);

  } while (1);

  ACE_NOTREACHED (return -1;)
}

int
ACEXML_Parser::parse_element_decl (ACEXML_Env &xmlenv)
{
  if (this->get () != 'L' ||
      this->get () != 'E' ||
      this->get () != 'M' ||
      this->get () != 'E' ||
      this->get () != 'N' ||
      this->get () != 'T' ||
      this->skip_whitespace_count () == 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Expecting keyword `ELEMENT'"));
      return -1;
    }

  ACEXML_Char *element_name = this->read_name ();
  if (element_name == 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException
                        ("Error reading element name while defining ELEMENT."));
      return -1;
    }

  ACEXML_Char nextch ;
  this->skip_whitespace_count (&nextch);

  switch (nextch)
    {
    case 'E':                   // EMPTY
      if (this->get () != 'E' ||
          this->get () != 'M' ||
          this->get () != 'P' ||
          this->get () != 'T' ||
          this->get () != 'Y')
        {
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Expecting keyword `EMPTY' in ELEMENT definition."));
          return -1;
        }
      break;
    case 'A':                   // ANY
      if (this->get () != 'A' ||
          this->get () != 'N' ||
          this->get () != 'Y')
        {
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Expecting keyword `ANY' in ELEMENT definition."));
          return -1;
        }
      break;
    case '(':                   // children
      this->parse_children_definition (xmlenv);
      if (xmlenv.exception () != 0)
        return -1;
      break;
    default:                    // error
      xmlenv.exception (new ACEXML_SAXParseException
                        ("Error reading ELEMENT definition."));
      return -1;
    }
  if (this->skip_whitespace (0) != '>')
    {
      xmlenv.exception (new ACEXML_SAXParseException
                        ("Expecting '>' in ELEMENT definition."));
      return -1;
    }
  return 0;
}

int
ACEXML_Parser::parse_entity_decl (ACEXML_Env &xmlenv)
{
  ACEXML_Char nextch;

  if (this->get () != 'N' ||
      this->get () != 'T' ||
      this->get () != 'I' ||
      this->get () != 'T' ||
      this->get () != 'Y' ||
      this->skip_whitespace_count (&nextch) == 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Expecting keyword `ENTITY'"));
      return -1;
    }

  int is_GEDecl = 1;
  if (nextch == '%')            // This is a PEDecl.
    {
      is_GEDecl = 0;
      this->get ();             // consume the '%'
      if (this->skip_whitespace_count (&nextch) == 0)
        {
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Can't use a reference when defining entity name"));
          return -1;
        }
    }

  ACEXML_Char *entity_name = this->read_name ();
  if (entity_name == 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Error reading ENTITY name."));
      return -1;
    }

  this->skip_whitespace_count (&nextch);

  if (nextch == '\'' || nextch == '"')
    {
      ACEXML_Char *entity_value = 0;

      if (this->get_quoted_string (entity_value) != 0)
        {
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Error reading ENTITY value."));
          return -1;
        }

      if (is_GEDecl)
        {
          if (this->entities_.add_entity (entity_name, entity_value) != 0)
            {
              xmlenv.exception (new ACEXML_SAXParseException
                                ("Error storing entity definition (duplicate definition?)"));
              return -1;
            }
        }
      else
        {
          // @@ need to implement PEdecl lookup mechanism
          xmlenv.exception (new ACEXML_SAXNotSupportedException ());
          return -1;
        }
    }
  else
    {
      ACEXML_Char *systemid, *publicid;

      this->parse_external_id_and_ref (publicid, systemid, xmlenv);
      if (xmlenv.exception () != 0)
        return -1;

      if (systemid == 0)
        {
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Invalid ExternalID definition (system ID missing.)"));
          return -1;
        }

      this->skip_whitespace_count (&nextch);
      if (nextch == 'N')        // NDATA section followed
        {
          if (is_GEDecl == 0)
            {
              xmlenv.exception (new ACEXML_SAXParseException
                                ("Unexpecting keyword NDATA in PEDecl."));
              return -1;
            }

          if (this->get () != 'N' ||
              this->get () != 'D' ||
              this->get () != 'A' ||
              this->get () != 'T' ||
              this->get () != 'A' ||
              this->skip_whitespace_count (&nextch) == 0)
            {
              xmlenv.exception (new ACEXML_SAXParseException
                                ("Expecting keyword NDATA."));
              return -1;
            }

          ACEXML_Char *ndata = this->read_name ();
          this->dtd_handler_->unparsedEntityDecl (entity_name,
                                                  publicid,
                                                  systemid,
                                                  ndata,
                                                  xmlenv);
          if (xmlenv.exception () != 0)
            return -1;
        }
      else
        {
          // @@ Need to support external CharStream sources
          cout << "ENTITY: (" << entity_name << ") ";

          if (publicid == 0)
            cout << "SYSTEM " << systemid << endl;
          else
            cout << "PUBLIC " << publicid << " " << systemid << endl;
        }
    }

  // End of ENTITY definition
  if (this->skip_whitespace (0) != '>')
    {
      xmlenv.exception (new ACEXML_SAXParseException
                        ("Expecting end of ENTITY definition."));
      return -1;
    }
  return 0;
}

int
ACEXML_Parser::parse_attlist_decl (ACEXML_Env &xmlenv)
{
  if (this->get () != 'A' ||
      this->get () != 'T' ||
      this->get () != 'T' ||
      this->get () != 'L' ||
      this->get () != 'I' ||
      this->get () != 'S' ||
      this->get () != 'T' ||
      this->skip_whitespace_count () == 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Expecting keyword `ATTLIST'"));
      return -1;
    }

  ACEXML_Char *element_name = this->read_name ();
  if (element_name == 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException
                        ("Error reading element name while defining ATTLIST."));
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
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Error reading attribute name while defining ATTLIST."));
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
          if (this->get () != 'D' ||
              this->get () != 'A' ||
              this->get () != 'T' ||
              this->get () != 'A' ||
              this->skip_whitespace_count () == 0)
            {
              xmlenv.exception (new ACEXML_SAXParseException
                                ("Expecting keyword `CDATA' while defining ATTLIST."));
              return -1;
            }
          // Else, we have successfully identified the type of the attribute as CDATA
          // @@ Set up validator appropriately here.
          break;
        case 'I':               // ID, IDREF, or, IDREFS
          if (this->get () == 'D')
            {
              if (this->skip_whitespace_count (&nextch) > 0)
                {
                  // We have successfully identified the type of the attribute as ID
                  // @@ Set up validator as such.
                  break;
                }
              if (this->get () == 'R' &&
                  this->get () == 'E' &&
                  this->get () == 'F')
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
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Expecting keyword `ID', `IDREF', or `IDREFS' while defining ATTLIST."));
          return -1;
        case 'E':               // ENTITY or ENTITIES
          if (this->get () == 'N' &&
              this->get () == 'T' &&
              this->get () == 'I' &&
              this->get () == 'T')
            {
              nextch = this->get ();
              if (nextch == 'Y')
                {
                  // We have successfully identified the type of
                  // the attribute as ENTITY
                  // @@ Set up validator as such.
                }
              else if (nextch == 'I'&&
                       this->get () == 'E' &&
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
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Expecting keyword `ENTITY', or `ENTITIES' while defining ATTLIST."));
          return -1;
        case 'N':               // NMTOKEN, NMTOKENS, or, NOTATION
          nextch = this->get ();
          if (nextch != 'M' || nextch != 'O')
            {
              xmlenv.exception (new ACEXML_SAXParseException
                                ("Expecting keyword `NMTOKEN', `NMTOKENS', or `NOTATION' while defining ATTLIST."));
              return -1;
            }
          if (nextch == 'M')
            {
              if (this->get () == 'T' &&
                  this->get () == 'O' &&
                  this->get () == 'K' &&
                  this->get () == 'E' &&
                  this->get () == 'N')
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
              xmlenv.exception (new ACEXML_SAXParseException
                                ("Expecting keyword `NMTOKEN' or `NMTOKENS' while defining ATTLIST."));
              return -1;
            }
          else                  // NOTATION
            {
              if (this->get () != 'T' ||
                  this->get () != 'A' ||
                  this->get () != 'T' ||
                  this->get () != 'I' ||
                  this->get () != 'O' ||
                  this->get () != 'N' ||
                  this->skip_whitespace_count () == 0)
                {
                  xmlenv.exception (new ACEXML_SAXParseException
                                    ("Expecting keyword `NOTATION' while defining ATTLIST."));
                  return -1;
                }

              if (this->get () != '(')
                {
                  xmlenv.exception (new ACEXML_SAXParseException
                                    ("Expecting `(' following NOTATION while defining ATTLIST."));
                  return -1;
                }

              this->skip_whitespace_count ();

              do {
                ACEXML_Char *notation_name = this->read_name ();
                if (notation_name == 0)
                  {
                    xmlenv.exception (new ACEXML_SAXParseException
                                      ("Error reading NOTATION name while defining ATTLIST."));
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
                xmlenv.exception (new ACEXML_SAXParseException
                                  ("Error reading enumerated nmtoken name while defining ATTLIST."));
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
            xmlenv.exception (new ACEXML_SAXParseException
                              ("Invalid Attribute Type while defining ATTLIST."));
            return -1;
          }
          break;
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
              if (this->get () != 'E' ||
                  this->get () != 'Q' ||
                  this->get () != 'U' ||
                  this->get () != 'I' ||
                  this->get () != 'R' ||
                  this->get () != 'E' ||
                  this->get () != 'D')
                {
                  xmlenv.exception (new ACEXML_SAXParseException
                                    ("Expecting keyword `#REQUIRED' while defining ATTLIST."));
                  return -1;
                }
              // We now know this attribute is required
              // @@ Set up the validator as such.
              break;
            case 'I':
              if (this->get () != 'M' ||
                  this->get () != 'P' ||
                  this->get () != 'L' ||
                  this->get () != 'I' ||
                  this->get () != 'E' ||
                  this->get () != 'D')
                {
                  xmlenv.exception (new ACEXML_SAXParseException
                                    ("Expecting keyword `#IMPLIED' while defining ATTLIST."));
                  return -1;
                }
              // We now know this attribute is impleid.
              // @@ Set up the validator as such.
              break;
            case 'F':
              if (this->get () != 'I' ||
                  this->get () != 'X' ||
                  this->get () != 'E' ||
                  this->get () != 'D' ||
                  this->skip_whitespace_count () == 0)
                {
                  xmlenv.exception (new ACEXML_SAXParseException
                                    ("Expecting keyword `#FIXED' while defining ATTLIST."));
                  return -1;
                }
              // We now know this attribute is fixed.

              ACEXML_Char *fixed_attr;
              if (this->get_quoted_string (fixed_attr) != 0)
                {
                  xmlenv.exception (new ACEXML_SAXParseException
                                    ("Error parsing `#FIXED' attribute value while defining ATTLIST."));
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
              xmlenv.exception (new ACEXML_SAXParseException
                                ("Error parsing `#FIXED' attribute value while defining ATTLIST."));
              return -1;
            }
          // @@ set up validator
          break;
        default:
          break;
        }
      this->skip_whitespace_count (&nextch);
    };

  this->get ();                 // consume closing '>'

  return 0;
}

int
ACEXML_Parser::parse_notation_decl (ACEXML_Env &xmlenv)
{
  if (this->get () != 'N' ||
      this->get () != 'O' ||
      this->get () != 'T' ||
      this->get () != 'A' ||
      this->get () != 'T' ||
      this->get () != 'I' ||
      this->get () != 'O' ||
      this->get () != 'N' ||
      this->skip_whitespace_count () == 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Expecting keyword `NOTATION'"));
      return -1;
    }

  ACEXML_Char *notation = this->read_name ();
  if (notation == 0)
    {
      xmlenv.exception (new ACEXML_SAXParseException ("Invalid notation name."));
      return -1;
    }

  this->skip_whitespace_count ();
  ACEXML_Char *systemid, *publicid;

  this->parse_external_id_and_ref (publicid, systemid, xmlenv);
  if (xmlenv.exception () != 0)
    return -1;

  if (this->get () != '>')
    {
      xmlenv.exception (new ACEXML_SAXParseException
                        ("Expecting NOTATION closing '>'."));
      return -1;
    }

  this->dtd_handler_->notationDecl (notation,
                                    publicid,
                                    systemid,
                                    xmlenv);
  if (xmlenv.exception () != 0)
    return -1;

  return 0;
}

int
ACEXML_Parser::parse_external_id_and_ref (ACEXML_Char *&publicId,
                                          ACEXML_Char *&systemId,
                                          ACEXML_Env &xmlenv)
{
  publicId = systemId = 0;
  ACEXML_Char nextch = this->get ();

  switch (nextch)
    {
    case 'S':                   // External SYSTEM id.
      if (this->get () != 'Y' ||
          this->get () != 'S' ||
          this->get () != 'T' ||
          this->get () != 'E' ||
          this->get () != 'M' ||
          this->skip_whitespace_count () == 0)
        {
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Expecting keyword 'SYSTEM'"));
          return -1;
        }
      if (this->get_quoted_string (systemId) != 0)
        {
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Error while parsing SYSTEM literal for SYSTEM id."));
          return -1;
        }
      break;
    case 'P':                   // External PUBLIC id or previously defined PUBLIC id.
      if (this->get () != 'U' ||
          this->get () != 'B' ||
          this->get () != 'L' ||
          this->get () != 'I' ||
          this->get () != 'C' ||
          this->skip_whitespace_count () == 0)
        {
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Expecting keyword 'PUBLIC'"));
          return -1;
        }
      if (this->get_quoted_string (publicId) != 0)
        {
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Error while parsing public literal for PUBLIC id."));
          return -1;
        }

      this->skip_whitespace_count (&nextch);
      if (nextch == '\'' || nextch == '"') // not end of NOTATION yet.
        {
          if (this->get_quoted_string (systemId) != 0)
            {
              xmlenv.exception (new ACEXML_SAXParseException
                                ("Error while parsing system literal for PUBLIC id."));
              return -1;
            }
        }
      break;
    default:
      xmlenv.exception (new ACEXML_SAXParseException
                        ("Expecting either keyword `SYSTEM' or `PUBLIC'."));
      return -1;
    }
  return 0;
}

int
ACEXML_Parser::parse_children_definition (ACEXML_Env &xmlenv)
{
  int level = 1;
  this->get ();                 // consume the '('

  ACEXML_Char nextch;
  int subelement_number = 0;
  this->skip_whitespace_count (&nextch);

  switch (nextch)
    {
    case '#':                   // Mixed element,
      if (this->get () != '#' ||
          this->get () != 'P' ||
          this->get () != 'C' ||
          this->get () != 'D' ||
          this->get () != 'A' ||
          this->get () != 'T' ||
          this->get () != 'A')
        {
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Expecting keyword `#PCDATA' while defining an element."));
          return -1;
        }

      this->skip_whitespace_count (&nextch);

      while (nextch != ')')
        {
          if (this->get () != '|')
            {
              xmlenv.exception (new ACEXML_SAXParseException
                                ("Expecting end of Mixed section while defining an element."));
              return -1;
            }
          this->skip_whitespace_count ();

          ACEXML_Char *name = this->read_name ();
          ++subelement_number;
          // @@ Install Mixed element name into the validator.
          this->skip_whitespace_count (&nextch);
        }

      if (this->get () != ')' ||
          (subelement_number && this->get () != '*'))
        {
          xmlenv.exception (new ACEXML_SAXParseException
                            ("Expecting closing `)*' or ')' while defining an element."));
          return -1;
        }
      // @@ close the element definition in the validator.
      break;
    default:
      if (this->parse_child (1, xmlenv) != 0 ||
          xmlenv.exception () != 0)
        return -1;
    }

  return 0;
}

int
ACEXML_Parser::parse_child (int skip_open_paren,
                            ACEXML_Env &xmlenv)
{
  // Conditionally consume the open paren.
  if (skip_open_paren == 0 &&
      this->get () != '(')
    {
      xmlenv.exception (new ACEXML_SAXParseException
                        ("Expecting opening `(' while defining an element."));
      return -1;
    }

  ACEXML_Char node_type = 0;
  ACEXML_Char nextch;

  do {
    this->skip_whitespace_count (&nextch);
    switch (nextch)
      {
      case '(':
        this->parse_child (0, xmlenv);
        if (xmlenv.exception != 0)
          return -1;
        break;
      default:
        // must be an element name here.
        ACEXML_Char *subelement = this->read_name ();
        if (subelement == 0)
          {
            xmlenv.exception (new ACEXML_SAXParseException
                              ("Error reading sub-element name while defining an element."));
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
            xmlenv.exception (new ACEXML_SAXParseException
                              ("Expecting `,', `|', or `)' while defining an element."));
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
            xmlenv.exception (new ACEXML_SAXParseException
                              ("Expecting `,', `|', or `)'while defining an element."));
            return -1;
          }
      case ')':
        break;
      default:
        xmlenv.exception (new ACEXML_SAXParseException
                          ("Expecting `,', `|', or `)' while defining an element."));
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
