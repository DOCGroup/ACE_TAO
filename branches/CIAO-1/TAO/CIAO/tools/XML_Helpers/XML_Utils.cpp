// -*- C++ -*-  $Id$

#include "XML_Utils.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/parser/parser/Parser.h"
#include "Assembly_Handlers.h"

#if !defined (__ACE_INLINE__)
# include "XML_Utils.inl"
#endif /* __ACE_INLINE__ */

int
CIAO::XML_Utils::parse_softpkg (CIAO::Softpkg_Handler::Softpkg_Info *info)
{
  if (info == 0)                // no way this is going to work. :)
    return -1;

  ACEXML_DefaultHandler *handler = 0;
  auto_ptr<ACEXML_DefaultHandler> cleanup_handler (handler);

  ACEXML_FileCharStream *fstm = 0;
  ACE_NEW_RETURN (fstm,
                  ACEXML_FileCharStream (),
                  1);

  if (fstm->open (info->csd_path_.c_str ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Fail to open XML file: %s\n"),
                       info->csd_path_.c_str ()),
                      -1);
  ACEXML_TRY_NEW_ENV
    {
      ACEXML_Parser parser;

      ACE_NEW_RETURN (handler,
                      CIAO::Softpkg_Handler (&parser,
                                             info
                                             ACEXML_ENV_ARG_PARAMETER),
                      -1);

      ACEXML_InputSource input(fstm);

      parser.setContentHandler (handler);
      parser.setDTDHandler (handler);
      parser.setErrorHandler (handler);
      parser.setEntityResolver (handler);

      parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
      ACEXML_TRY_CHECK;

      //      delete fstm;
      ACE_NEW_RETURN (fstm,
                      ACEXML_FileCharStream (),
                      1);

      if (fstm->open (info->ssd_path_.c_str ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Fail to open XML file: %s\n"),
                           info->ssd_path_.c_str ()),
                          -1);

      input.setCharStream (fstm);

      parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
      ACEXML_TRY_CHECK;
    }
  ACEXML_CATCH (ACEXML_SAXException, ex)
    {
      ex.print ();
      return -1;
    }
  ACEXML_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Caught unknown exception.\n"),
                        -1);
    }
  ACEXML_ENDTRY;
  return 0;
}

int
CIAO::XML_Utils::parse_componentassembly (const char *filename,
                                          CIAO::Assembly_Spec *spec)
{
  if (spec == 0)                // no way this is going to work. :)
    return -1;

  ACEXML_DefaultHandler *handler = 0;
  auto_ptr<ACEXML_DefaultHandler> cleanup_handler (handler);

  ACEXML_FileCharStream *fstm = 0;
  ACE_NEW_RETURN (fstm,
                  ACEXML_FileCharStream (),
                  1);

  if (fstm->open (filename) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Fail to open XML file: %s\n"),
                       filename),
                      -1);
  ACEXML_TRY_NEW_ENV
    {
      ACEXML_Parser parser;

      ACE_NEW_RETURN (handler,
                      CIAO::Assembly_Handler (&parser,
                                              spec
                                              ACEXML_ENV_ARG_PARAMETER),
                      -1);

      ACEXML_InputSource input(fstm);

      parser.setContentHandler (handler);
      parser.setDTDHandler (handler);
      parser.setErrorHandler (handler);
      parser.setEntityResolver (handler);

      parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
      ACEXML_TRY_CHECK;
    }
  ACEXML_CATCH (ACEXML_SAXException, ex)
    {
      ex.print ();
      return -1;
    }
  ACEXML_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Caught unknown exception.\n"),
                        -1);
    }
  ACEXML_ENDTRY;
  return 0;
}

long
CIAO::XML_Utils::get_id_and_cardinality (const char *&id,
                                         ACEXML_Attributes *atts
                                         ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  long retv = 1;

  for (size_t i = 0; i < atts->getLength (); ++i)
    {
      if (ACE_OS_String::strcmp (atts->getQName (i), ACE_TEXT ("id")) == 0)
        {
          id = atts->getValue (i);
        }
      else if (ACE_OS_String::strcmp (atts->getQName (i),
                                      ACE_TEXT ("cardinality")) == 0)
        {
          retv = ACE_OS::atoi (atts->getValue (i));
        }
      else
        ACEXML_THROW_RETURN
          (ACEXML_SAXException
           ("Invalid attribute found"),
           -1);
    }
  return retv;
}

long
CIAO::XML_Utils::get_single_attribute (const char *attname,
                                       const char *&id,
                                       ACEXML_Attributes *atts
                                       ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  for (size_t i = 0; i < atts->getLength (); ++i)
    {
      if (ACE_OS_String::strcmp (atts->getQName (i), attname) == 0)
        {
          id = atts->getValue (i);
        }
    }
  return 0;
}
