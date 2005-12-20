// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_util.cpp
//
// = DESCRIPTION
//    Static helper methods used by multiple visitors.
//
// = AUTHOR
//    Gary Maxey
//
// ============================================================================

#include "be_util.h"
#include "utl_identifier.h"
#include "ace/OS_NS_string.h"

ACE_RCSID (be, 
           be_util, 
           "$Id$")

void
be_util::gen_nested_namespace_begin (TAO_OutStream *os, be_module *node)
{
  char *item_name = 0;

  for (UTL_IdListActiveIterator i (node->name ()); !i.is_done (); i.next ())
    {
      item_name = i.item ()->get_string ();

      if (ACE_OS::strcmp (item_name, "") != 0)
        {
          // Leave the outermost root scope.
          *os << be_nl << "namespace " << item_name << be_nl
              << "{" << be_idt_nl;
        }
    }
}

void
be_util::gen_nested_namespace_end (TAO_OutStream *os, be_module *node)
{
  for (UTL_IdListActiveIterator i (node->name ()); !i.is_done (); i.next ())
    {
      if (ACE_OS::strcmp (i.item ()->get_string (), "") != 0)
        {
          // Leave the outermost root scope.
          *os << be_uidt_nl << "}";
        }
    }

  *os << be_nl << be_nl;
}
