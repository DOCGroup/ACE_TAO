#include "Options.h"
#include "Search_Struct.h"
#include "ace/Log_Msg.h"

Search_Struct::~Search_Struct ()
{
  if (Options::get_opt (Options::DEBUGGING))
    ACE_DEBUG ((LM_DEBUG,
                "disposing Search_Struct\n"));
}

Search_Struct::Search_Struct () = default;

int
Search_Struct::n_elems ()
{
  return this->count_;
}

