// -*- C++ -*-  $Id$

#include "Softpkg_Handlers.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
# include "Softpkg_Handlers.inl"
#endif /* __ACE_INLINE__ */

void
CIAO::Softpkg_Handler::Softpkg_Info::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nCORBA Component Descriptor -> %s\n")
              ACE_TEXT ("CSD file pathname          -> %s\n")
              ACE_TEXT ("Executor UUID              -> %s\n")
              ACE_TEXT ("Executor entry point       -> %s\n")
              ACE_TEXT ("SSD file pathname          -> %s\n")
              ACE_TEXT ("Servant UUID               -> %s\n")
              ACE_TEXT ("Servant entry point        -> %s\n"),
              descriptor_.c_str (),
              csd_path_.c_str (),
              executor_UUID_.c_str (),
              executor_entrypt_.c_str (),
              ssd_path_.c_str (),
              servant_UUID_.c_str (),
              servant_entrypt_.c_str ()));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
CIAO::Softpkg_Handler::characters (const ACEXML_Char *cdata,
                                   int start,
                                   int len
                                   ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->rec_cdata_)
    {
      this->characters_.set (cdata + start,
                             len,
                             1);
      this->rec_cdata_ = false;
    }
}

void
CIAO::Softpkg_Handler::startElement (const ACEXML_Char *namespaceURI,
                                     const ACEXML_Char *localName,
                                     const ACEXML_Char *qName,
                                     ACEXML_Attributes *alist
                                     ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG (namespaceURI);
  ACE_UNUSED_ARG (localName);

  switch (this->state_)
    {
    case ROOT:

      if (ACE_OS::strcmp (qName, ACE_TEXT ("pkgtype")) == 0)
        {
          this->rec_cdata_ = true;
        }
      else if (ACE_OS::strcmp (qName, ACE_TEXT ("descriptor")) == 0)
        {
          // @@ We should really check the "type" attribute and make
          // sure that we are dealing with a CORBA Component
          // descriptor here before we switch state.

          this->state_ = DESCRIPTOR;
        }
      else if (ACE_OS::strcmp (qName, ACE_TEXT ("implementation")) == 0)
        {
          if (alist != 0)
            for (size_t i = 0; i < alist->getLength (); ++i)
              {
                if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("id")) == 0)
                  {
                    this->impluuid_.set (alist->getValue (i));
                  }
              }
          this->state_ = IMPLEMENTATION;
        }
      break;

    case DESCRIPTOR:
      if (ACE_OS::strcmp (qName, ACE_TEXT ("fileinarchive")) == 0)
        {
          if (alist != 0)
            for (size_t i = 0; i < alist->getLength (); ++i)
              {
                if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("name")) == 0)
                  {
                    this->softpkg_info_->descriptor_.set (alist->getValue (i));
                  }
              }
        }
      break;

    case IMPLEMENTATION:
      // @@ Note: WE currently ignore most everything without even
      // checking the validity of OS/compiler and such.
      if (ACE_OS::strcmp (qName, ACE_TEXT ("dependency")) == 0)
        {
          if (alist != 0)
            for (size_t i = 0; i < alist->getLength (); ++i)
              {
                if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("type")) == 0 &&
                    ACE_OS_String::strcmp (alist->getValue (i), ACE_TEXT ("CIAODLL")) == 0)
                  {
                    this->state_ = CIAODLL;
                  }
              }
        }
      else if (ACE_OS::strcmp (qName, ACE_TEXT ("code")) == 0)
        {
          // @@ We should check for "type" attribute here.
          this->state_ = CODE;
        }
      break;

    case CIAODLL:
      // CIAO extension.
      if (ACE_OS::strcmp (qName, ACE_TEXT ("fileinarchive")) == 0)
        {
          if (alist != 0)
            for (size_t i = 0; i < alist->getLength (); ++i)
              {
                if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("name")) == 0)
                  {
                    this->ssd_.set (alist->getValue (i));
                  }
              }
        }
      else if (ACE_OS::strcmp (qName, ACE_TEXT ("implref")) == 0)
        {
          if (alist != 0)
            for (size_t i = 0; i < alist->getLength (); ++i)
              {
                if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("idref")) == 0)
                  {
                    this->ssd_id_.set (alist->getValue (i));
                  }
              }
        }
      break;


    case CODE:
      if (ACE_OS::strcmp (qName, ACE_TEXT ("fileinarchive")) == 0)
        {
          if (alist != 0)
            for (size_t i = 0; i < alist->getLength (); ++i)
              {
                if (ACE_OS_String::strcmp (alist->getQName (i), ACE_TEXT ("name")) == 0)
                  {
                    this->path_.set (alist->getValue (i));
                  }
              }
        }
      else if (ACE_OS::strcmp (qName, ACE_TEXT ("entrypoint")) == 0)
        {
          this->rec_cdata_ = true;
        }
      break;

    default:
      ACEXML_THROW (ACEXML_SAXException
                    (ACE_TEXT ("Softpkg handler internal error")));
      break;
    }
}

void
CIAO::Softpkg_Handler::endElement (const ACEXML_Char *,
                                   const ACEXML_Char *,
                                   const ACEXML_Char *qName
                                   ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  switch (this->state_)
    {
    case ROOT:

      if (ACE_OS::strcmp (qName, ACE_TEXT ("pkgtype")) == 0)
        {
          if (ACE_OS::strcmp (this->characters_.c_str (),
                              ACE_TEXT ("CORBA Component")) == 0)
            {
              this->type_ = CSD;
            }
          else if (ACE_OS::strcmp (this->characters_.c_str (),
                                   ACE_TEXT ("CIAO Servant")) == 0)
            {
              this->type_ = SSD;
            }
          else
            ACEXML_THROW (ACEXML_SAXNotRecognizedException
                          (ACE_TEXT ("Unknown pkgtype")));
        }
      break;

    case DESCRIPTOR:
      if (ACE_OS::strcmp (qName, ACE_TEXT ("descriptor")) == 0)
        {
          this->state_ = ROOT;
        }
      break;

    case IMPLEMENTATION:
      if (ACE_OS::strcmp (qName, ACE_TEXT ("implementation")) == 0)
        {
          // @@ Check the validity somehow here before we move the data.
          if (this->type_ == CSD)
            {
              this->softpkg_info_->executor_UUID_ = this->impluuid_;
              this->softpkg_info_->executor_entrypt_ = this->entry_pt_;
              this->softpkg_info_->ssd_path_ = this->ssd_;
              this->softpkg_info_->servant_UUID_ = this->ssd_id_;
                }
          else if (this->type_ == SSD &&
                   this->softpkg_info_->servant_UUID_ == this->impluuid_)
            {

              this->softpkg_info_->servant_entrypt_ = this->entry_pt_;
            }

          this->state_ = ROOT;
        }
      break;

    case CIAODLL:
      if (ACE_OS::strcmp (qName, ACE_TEXT ("dependency")) == 0)
        {
          this->state_ = IMPLEMENTATION;
        }

      break;

    case CODE:
      if (ACE_OS::strcmp (qName, ACE_TEXT ("entrypoint")) == 0)
        {
          this->entry_pt_ = this->characters_;
        }
      else if (ACE_OS::strcmp (qName, ACE_TEXT ("code")) == 0)
        {
          this->state_ = IMPLEMENTATION;
        }
      break;

    default:
      ACEXML_THROW (ACEXML_SAXException
                    (ACE_TEXT ("Softpkg handler internal error")));
      break;
    }
}

void
CIAO::Softpkg_Handler::startDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}

void
CIAO::Softpkg_Handler::endDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // no-op
}
