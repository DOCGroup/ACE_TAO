/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Svcconf.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACEXML_SVCCONF_H
#define ACEXML_SVCCONF_H

#include "ace/pre.h"
#include "ace/XML_Svc_Conf.h"
#include "ACEXML/parser/parser/Parser.h"
#include "Svcconf_Handler.h"

#if (ACE_USES_CLASSIC_SVC_CONF == 0)

class ACEXML_Svcconf_Parser : public ACE_XML_Svc_Conf
{
public:
  ACEXML_Svcconf_Parser ();

  virtual ~ACEXML_Svcconf_Parser ();

  virtual int parse_file (const ACE_TCHAR file[]);

  virtual int parse_string (const ACE_TCHAR str[]);

  void * operator new (size_t bytes);

  void operator delete (void *ptr);

protected:
  ACEXML_Parser parser_;
  ACEXML_Svcconf_Handler svcconf_handler_;
  ACEXML_InputSource input_stream_;
};

#endif /* ACE_USES_CLASSIC_SVC_CONF == 0 */

#include "ace/post.h"
#endif /* ACEXML_SVCCONF_H */
