//
// $Id$
//

#include "idl.h"
#include "be.h"
#include "be_visitor_root.h"

be_visitor_root_ch::be_visitor_root_ch (void)
{
}

be_visitor_root_ch::~be_visitor_root_ch (void)
{
}

int be_visitor_root_ch::visit_sequence (be_sequence *node)
{
  return 0;
}



be_visitor_root_cs::be_visitor_root_cs (void)
{
}

be_visitor_root_cs::~be_visitor_root_cs (void)
{
}

int be_visitor_root_cs::visit_sequence (be_sequence *node)
{
  return 0;
}



be_visitor_root_ci::be_visitor_root_ci (void)
{
}

be_visitor_root_ci::~be_visitor_root_ci (void)
{
}

int be_visitor_root_ci::visit_sequence (be_sequence *node)
{
  return 0;
}

