/* -*- C++ -*- */
// $Id$

#include "URL_Visitor_Factory.h"

ACE_RCSID (Web_Crawler,
           URL_Visitor_Factory,
           "$Id$")


URL_Visitor_Factory::~URL_Visitor_Factory (void)
{
}

URL_Visitor *
URL_Validation_Visitor_Factory::make_visitor (void)
{
  URL_Visitor *v;

  ACE_NEW_RETURN (v,
                  URL_Validation_Visitor,
                  0);

  return v;
}

Command_Processor *
URL_Validation_Visitor_Factory::make_command_processor (void)
{
  Command_Processor *cp;

  ACE_NEW_RETURN (cp,
                  Command_Processor,
                  0);
  return cp;
}

URL_Visitor *
URL_Download_Visitor_Factory::make_visitor (void)
{
  URL_Visitor *v;

  ACE_NEW_RETURN (v,
                  URL_Download_Visitor,
                  0);
  return v;
}

Command_Processor *
URL_Download_Visitor_Factory::make_command_processor (void)
{
  return 0;
}
