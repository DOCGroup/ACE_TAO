// $Id$

#include "Options.h"
#include "Search_Struct.h"
#include "ace/Log_Msg.h"

Search_Struct::~Search_Struct (void)
{
  if (Options::get_opt (Options::DEBUG))
    ACE_DEBUG ((LM_DEBUG,
                "disposing Search_Struct\n"));
}

Search_Struct::Search_Struct (void)
  : count_ (0)
{}

int
Search_Struct::n_elems (void)
{
  return this->count_;
}

