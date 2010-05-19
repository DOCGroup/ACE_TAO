
//=============================================================================
/**
 *  @file    be_util.cpp
 *
 *  $Id$
 *
 *  Static helper methods used by multiple visitors.
 *
 *
 *  @author Gary Maxey
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_util.h"
#include "be_helper.h"
#include "be_module.h"
#include "be_identifier_helper.h"

#include "utl_identifier.h"

#include "ace/OS_NS_string.h"

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

void
be_util::gen_nesting_open (TAO_OutStream &os, AST_Decl *node)
{
  os << be_nl;

  for (UTL_IdListActiveIterator i (node->name ()); ! i.is_done () ;)
    {
      Identifier *id = i.item ();
      ACE_CString test (id->get_string (), 0, false);
      
      if (test == "" || test == "::")
        {
          i.next ();
          continue;
        }
        
      UTL_ScopedName tmp (id, 0);
      AST_Decl *scope =
        node->defined_in ()->lookup_by_name (&tmp, true);

      ACE_CString module_name =
        IdentifierHelper::try_escape (scope->original_local_name ());

      if (module_name == "")
        {
          i.next ();
          continue;
        }

      i.next ();

      if (i.is_done ())
        {
          break;
        }

      os << be_nl
         << "module " << module_name.c_str () << be_nl
         << "{" << be_idt;
    }
}

void
be_util::gen_nesting_close (TAO_OutStream &os, AST_Decl *node)
{
  for (UTL_IdListActiveIterator i (node->name ()); ! i.is_done () ;)
    {
      ACE_CString module_name (i.item ()->get_string ());

      if (module_name == "")
        {
          i.next ();
          continue;
        }

      i.next ();

      if (i.is_done ())
        {
          break;
        }

      os << be_uidt_nl
         << "};";
    }
}

