// -*- C++ -*-  $Id$

#include "Svcconf_Handler.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

#if !defined (__ACEXML_INLINE__)
# include "Svcconf_Handler.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_Svcconf_Handler::ACEXML_Svcconf_Handler (void)
  : in_stream_def_ (0),
    in_module_ (0)
{
  // no-op
}

ACEXML_Svcconf_Handler::~ACEXML_Svcconf_Handler (void)
{
  // no-op
}

void
ACEXML_Svcconf_Handler::characters (const ACEXML_Char *,
                                    int,
                                    int ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::endDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::endElement (const ACEXML_Char *,
                                    const ACEXML_Char *,
                                    const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{


  if (ACE_OS_String::strcmp (qName, ACE_TEXT ("dynamic")) == 0)
    {
      if (this->in_stream_def_)
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Create dynamic %s for stream\n"),
                      this->stream_info_.name ()));
        }
      else
        {
          if (this->in_module_)
            {
              ACE_DEBUG ((LM_INFO, ACE_TEXT ("Push dynamic %s into stream %s\n"),
                          this->parsed_info_.name (),
                          this->stream_info_.name ()));
            }
          else
            {
              ACE_DEBUG ((LM_INFO, ACE_TEXT ("Apply dynamic %s\n"),
                          this->parsed_info_.name ()));
            }
          this->parsed_info_.reset ();
        }
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("static")) == 0)
    {
      if (this->in_stream_def_)
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Create sttaic %s for stream\n"),
                      this->stream_info_.name ()));
        }
      else
        {
          if (this->in_module_)
            {
              ACE_DEBUG ((LM_INFO, ACE_TEXT ("Push static %s into stream %s\n"),
                          this->parsed_info_.name (),
                          this->stream_info_.name ()));
            }
          else
            {
              ACE_DEBUG ((LM_INFO, ACE_TEXT ("Apply static %s\n"),
                          this->parsed_info_.name ()));
            }
          this->parsed_info_.reset ();
        }
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("module")) == 0)
    {
      this->in_module_ = 0;
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("streamdef")) == 0 ||
           ACE_OS_String::strcmp (qName, ACE_TEXT ("stream")) == 0)
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Apply stream %s of type %s\n"),
                  this->stream_info_.name (),
                  this->stream_info_.name ()));
      this->stream_info_.reset ();
    }
  else
    {
    }
}

void
ACEXML_Svcconf_Handler::endPrefixMapping (const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::ignorableWhitespace (const ACEXML_Char *,
                                             int,
                                             int ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::processingInstruction (const ACEXML_Char *,
                                               const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::setDocumentLocator (ACEXML_Locator* )
{
}

void
ACEXML_Svcconf_Handler::skippedEntity (const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::startDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
ACEXML_Svcconf_Handler::startElement (const ACEXML_Char *,
                                      const ACEXML_Char *,
                                      const ACEXML_Char *qName,
                                      ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (ACE_OS_String::strcmp (qName, ACE_TEXT ("dynamic")) == 0)
    {
      this->get_dynamic_attrs (alist ACEXML_ENV_ARG_PARAMETER);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("initializer")) == 0)
    {
      this->get_initializer_attrs (alist ACEXML_ENV_ARG_PARAMETER);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("static")) == 0)
    {
      this->get_static_attrs (alist ACEXML_ENV_ARG_PARAMETER);
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("stream")) == 0)
    {
      this->get_stream_id (alist ACEXML_ENV_ARG_PARAMETER);
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Retrieve stream %s from repository\n"),
                  this->stream_info_.name ()));
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("streamdef")) == 0)
    {
      this->in_stream_def_ = 1;
      // @@ Set up stream service object
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("module")) == 0)
    {
      this->in_stream_def_ = 0;
      this->in_module_ = 1;
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("resume")) == 0)
    {
      this->get_id (alist ACEXML_ENV_ARG_PARAMETER);
      if (this->in_module_)
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Resume %s in stream %s\n"),
                      this->parsed_info_.name (),
                      this->stream_info_.name ()));
        }
      else
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Resume %s\n"),
                      this->parsed_info_.name ()));
        }
      this->parsed_info_.reset ();
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("suspend")) == 0)
    {
      this->get_id (alist ACEXML_ENV_ARG_PARAMETER);
      if (this->in_module_)
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Suspend %s in stream %s\n"),
                      this->parsed_info_.name (),
                      this->stream_info_.name ()));
        }
      else
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Suspend %s\n"),
                      this->parsed_info_.name ()));
        }
      this->parsed_info_.reset ();
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("remove")) == 0)
    {
      this->get_id (alist ACEXML_ENV_ARG_PARAMETER);
      if (this->in_module_)
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Remove %s in stream %s\n"),
                      this->parsed_info_.name (),
                      this->stream_info_.name ()));
        }
      else
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Remove %s\n"),
                      this->parsed_info_.name ()));
        }
      this->parsed_info_.reset ();
    }
  else if (ACE_OS_String::strcmp (qName, ACE_TEXT ("ACE_Svc_Conf")) == 0)
    {
      // Main document tag. no-op.
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("ACE_Svc_Conf tag\n")));
    }
  else
    {
      // @@ Error.  Perhaps we should relay to user event handler here, if available.
    }

  return;

  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT (" %s = \"%s\""),
                    alist->getQName (i), alist->getValue (i)));
      }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (">")));
}

void
ACEXML_Svcconf_Handler::startPrefixMapping (const ACEXML_Char *,
                                            const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

// *** Methods inherit from ACEXML_DTDHandler.

void
ACEXML_Svcconf_Handler::notationDecl (const ACEXML_Char *,
                                      const ACEXML_Char *,
                                      const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_Svcconf_Handler::unparsedEntityDecl (const ACEXML_Char *,
                                            const ACEXML_Char *,
                                            const ACEXML_Char *,
                                            const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

// Methods inherit from ACEXML_EnitityResolver.

ACEXML_InputSource *
ACEXML_Svcconf_Handler::resolveEntity (const ACEXML_Char *,
                                       const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
  return 0;
}

// Methods inherit from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
void
ACEXML_Svcconf_Handler::error (ACEXML_SAXParseException & ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_Svcconf_Handler::fatalError (ACEXML_SAXParseException & ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_Svcconf_Handler::warning (ACEXML_SAXParseException & ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

int
ACEXML_Svcconf_Handler::get_stream_id (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
{


  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
          {
            this->stream_info_.name (alist->getValue (i));
          }
        else
          {
            // @@ Exception...
            return -1;
          }
      }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_id (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
{


  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
          {
            this->parsed_info_.name (alist->getValue (i));
          }
        else
          {
            // @@ Exception...
            return -1;
          }
      }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_dynamic_attrs (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
{


  if (alist != 0)
    {
      ACE_Parsed_Info *info = (this->in_stream_def_ == 0 ?
                               &this->parsed_info_ :
                               &this->stream_info_);
      for (size_t i = 0; i < alist->getLength (); ++i)
        {
          if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
            {
              info->name (alist->getValue (i));
            }
          else if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("status")) == 0)
            {
              if (ACE_OS_String::strcmp (alist->getValue (i), ACE_TEXT ("inactive")) == 0)
                {
                }
              else if (ACE_OS_String::strcmp (alist->getValue (i), ACE_TEXT ("active")) == 0)
                {
                }
              else
                {
                  // @@ error, invalid 'status' value.
                }
            }
          else if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("type")) == 0)
            {
              if (ACE_OS_String::strcmp (alist->getValue (i), ACE_TEXT ("service_object")) == 0)
                {
                  info->service_type (ACE_Parsed_Info::SERVICE_OBJECT_TYPE);
                }
              else if (ACE_OS_String::strcmp (alist->getValue (i), ACE_TEXT ("stream")) == 0)
                {
                  info->service_type (ACE_Parsed_Info::STREAM_TYPE);
                }
              else if (ACE_OS_String::strcmp (alist->getValue (i), ACE_TEXT ("module")) == 0)
                {
                  info->service_type (ACE_Parsed_Info::MODULE_TYPE);
                }
              else
                {
                  // @@ error, invalid 'type' value.
                }
            }
          else
            {
              // @@ Exception...
              return -1;
            }
        }
    }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_initializer_attrs (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
{


  if (alist != 0)
    {
      ACE_Parsed_Info *info = (this->in_stream_def_ == 0 ?
                               &this->parsed_info_ :
                               &this->stream_info_);
      for (size_t i = 0; i < alist->getLength (); ++i)
        {
          if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("init")) == 0)
            {
              info->init_func (alist->getValue (i));
            }
          else if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("path")) == 0)
            {
              info->path (alist->getValue (i));
            }
          else if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("params")) == 0)
            {
              info->init_params (alist->getValue (i));
            }
          else
            {
              // @@ Exception...
              return -1;
            }
        }
    }
  return 0;
}

int
ACEXML_Svcconf_Handler::get_static_attrs (ACEXML_Attributes *alist ACEXML_ENV_ARG_DECL)
{


  if (alist != 0)
    {
      ACE_Parsed_Info *info = (this->in_stream_def_ == 0 ?
                               &this->parsed_info_ :
                               &this->stream_info_);
      for (size_t i = 0; i < alist->getLength (); ++i)
        {
          if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
            {
              info->name (alist->getValue (i));
            }
          else if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("params")) == 0)
            {
              info->init_params (alist->getValue (i));
            }
          else
            {
              // @@ Exception...
              return -1;
            }
        }
    }
  return 0;
}
