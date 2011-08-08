
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
#include "be_type.h"
#include "be_identifier_helper.h"
#include "be_extern.h"
#include "be_generator.h"
#include "be_codegen.h"

#include "utl_identifier.h"

#include "ast_typedef.h"
#include "ast_structure.h"
#include "ast_structure_fwd.h"
#include "ast_string.h"

#include "ace/OS_NS_string.h"

void
be_util::gen_nested_namespace_begin (TAO_OutStream *os,
                                     be_module *node,
                                     bool skel)
{
  char *item_name = 0;
  bool first_level = true;

  for (UTL_IdListActiveIterator i (node->name ());
       !i.is_done ();
       i.next ())
    {
      item_name = i.item ()->get_string ();

      if (ACE_OS::strcmp (item_name, "") != 0)
        {
          // Leave the outermost root scope.
          *os << be_nl << "namespace ";

          if (first_level && skel)
            {
              // We are outermost module.
              *os << "POA_";
              first_level = false;
            }

          *os << item_name << be_nl
              << "{" << be_idt_nl;
        }
    }
}

void
be_util::gen_nested_namespace_end (TAO_OutStream *os,
                                   be_module *node)
{
  for (UTL_IdListActiveIterator i (node->name ());
       !i.is_done ();
       i.next ())
    {
      if (ACE_OS::strcmp (i.item ()->get_string (), "") != 0)
        {
          // Leave the outermost root scope.
          *os << be_uidt_nl << "}";
        }
    }
}

void
be_util::gen_nesting_open (TAO_OutStream &os, AST_Decl *node)
{
  AST_Decl::NodeType nt = node->node_type ();

  if (nt == AST_Decl::NT_root)
    {
      os << be_nl;
      return;
    }

  be_util::gen_nesting_open (
    os,
    ScopeAsDecl (node->defined_in ()));

  if (nt == AST_Decl::NT_module)
    {
      ACE_CString module_name (
        IdentifierHelper::try_escape (node->original_local_name ()));

      os << be_nl
         << "module " << module_name.c_str () << be_nl
         << "{" << be_idt;
   }
}

void
be_util::gen_nesting_close (TAO_OutStream &os, AST_Decl *node)
{
  AST_Decl *d = ScopeAsDecl (node->defined_in ());
  AST_Decl::NodeType nt = d->node_type ();

  while (nt != AST_Decl::NT_root)
    {
      os << be_uidt_nl
         << "};";

      d = ScopeAsDecl (d->defined_in ());
      nt = d->node_type ();
    }
}

// Prepare an argument for a BE
void
be_util::prep_be_arg (char *s)
{
  static const char arg_macro[]            = "export_macro=";
  static const char arg_include[]          = "export_include=";
  static const char skel_arg_macro[]       = "skel_export_macro=";
  static const char skel_arg_include[]     = "skel_export_include=";
  static const char stub_arg_macro[]       = "stub_export_macro=";
  static const char stub_arg_include[]     = "stub_export_include=";
  static const char anyop_arg_macro[]      = "anyop_export_macro=";
  static const char anyop_arg_include[]    = "anyop_export_include=";
  static const char exec_arg_macro[]       = "exec_export_macro=";
  static const char exec_arg_include[]     = "exec_export_include=";
  static const char svnt_arg_macro[]       = "svnt_export_macro=";
  static const char svnt_arg_include[]     = "svnt_export_include=";
  static const char conn_arg_macro[]       = "conn_export_macro=";
  static const char conn_arg_include[]     = "conn_export_include=";
  static const char arg_pch_include[]      = "pch_include=";
  static const char arg_pre_include[]      = "pre_include=";
  static const char arg_post_include[]     = "post_include=";
  static const char arg_versioning_begin[] = "versioning_begin=";
  static const char arg_versioning_end[]   = "versioning_end=";
  static const char obv_opt_accessor[]     = "obv_opt_accessor";
  static const char ciao_container_type[]  = "ciao_container_type=";
  static const char include_guard[]        = "include_guard=";
  static const char safe_include[]         = "safe_include=";
  static const char unique_include[]       = "unique_include=";
  static const char dds_impl[]             = "dds_impl=";
  static const char opendds_sequence_suffix[] = "opendds_sequence_suffix=";

  char* last = 0;

  for (char* arg = ACE_OS::strtok_r (s, ",", &last);
       arg != 0;
       arg = ACE_OS::strtok_r (0, ",", &last))
    {
      if (ACE_OS::strstr (arg, arg_macro) == arg)
        {
          char* val = arg + sizeof (arg_macro) - 1;
          be_global->skel_export_macro (val);
          be_global->stub_export_macro (val);
          be_global->anyop_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, arg_include) == arg)
        {
          char* val = arg + sizeof (arg_include) - 1;
          be_global->stub_export_include (val);
        }
      else if (ACE_OS::strstr (arg, skel_arg_macro) == arg)
        {
          char* val = arg + sizeof (skel_arg_macro) - 1;
          be_global->skel_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, skel_arg_include) == arg)
        {
          char* val = arg + sizeof (skel_arg_include) - 1;
          be_global->skel_export_include (val);
        }
      else if (ACE_OS::strstr (arg, stub_arg_macro) == arg)
        {
          char* val = arg + sizeof (stub_arg_macro) - 1;
          be_global->stub_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, stub_arg_include) == arg)
        {
          char* val = arg + sizeof (stub_arg_include) - 1;
          be_global->stub_export_include (val);
        }
      else if (ACE_OS::strstr (arg, anyop_arg_macro) == arg)
        {
          char* val = arg + sizeof (anyop_arg_macro) - 1;
          be_global->anyop_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, anyop_arg_include) == arg)
        {
          char* val = arg + sizeof (anyop_arg_include) - 1;
          be_global->anyop_export_include (val);
        }
      else if (ACE_OS::strstr (arg, exec_arg_macro) == arg)
        {
          char* val = arg + sizeof (exec_arg_macro) - 1;
          be_global->exec_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, exec_arg_include) == arg)
        {
          char* val = arg + sizeof (exec_arg_include) - 1;
          be_global->exec_export_include (val);
        }
      else if (ACE_OS::strstr (arg, svnt_arg_macro) == arg)
        {
          char* val = arg + sizeof (svnt_arg_macro) - 1;
          be_global->svnt_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, svnt_arg_include) == arg)
        {
          char* val = arg + sizeof (svnt_arg_include) - 1;
          be_global->svnt_export_include (val);
        }
      else if (ACE_OS::strstr (arg, conn_arg_macro) == arg)
        {
          char* val = arg + sizeof (conn_arg_macro) - 1;
          be_global->conn_export_macro (val);
        }
      else if (ACE_OS::strstr (arg, conn_arg_include) == arg)
        {
          char* val = arg + sizeof (conn_arg_include) - 1;
          be_global->conn_export_include (val);
        }
      else if (ACE_OS::strstr (arg, arg_pch_include) == arg)
        {
          char* val = arg + sizeof (arg_pch_include) - 1;
          be_global->pch_include (val);
        }
      else if (ACE_OS::strstr (arg, arg_pre_include) == arg)
        {
          char* val = arg + sizeof (arg_pre_include) - 1;
          be_global->pre_include (val);
        }
      else if (ACE_OS::strstr (arg, arg_post_include) == arg)
        {
          char* val = arg + sizeof (arg_post_include) - 1;
          be_global->post_include (val);
        }
      else if (ACE_OS::strstr (arg, include_guard) == arg)
        {
          char* val = arg + sizeof (include_guard) - 1;
          be_global->include_guard (val);
        }
      else if (ACE_OS::strstr (arg, safe_include) == arg)
        {
          char* val = arg + sizeof (safe_include) - 1;
          be_global->safe_include (val);
        }
      else if (ACE_OS::strstr (arg, unique_include) == arg)
        {
          char* val = arg + sizeof (unique_include) - 1;
          be_global->unique_include (val);
        }
      else if (ACE_OS::strstr (arg, obv_opt_accessor) == arg)
        {
          be_global->obv_opt_accessor (1);
        }
      else if (ACE_OS::strstr (arg, ciao_container_type) == arg)
        {
          char* val = arg + sizeof (ciao_container_type) - 1;
          be_global->ciao_container_type (val);
        }
      else if (ACE_OS::strstr (arg, arg_versioning_begin) == arg)
        {
          char const * const val =
            arg + sizeof (arg_versioning_begin) - 1;
          be_global->versioning_begin (val);
        }
      else if (ACE_OS::strstr (arg, arg_versioning_end) == arg)
        {
          char const * const val =
            arg + sizeof (arg_versioning_end) - 1;
          be_global->versioning_end (val);
        }
      else if (ACE_OS::strstr (arg, dds_impl) == arg)
        {
          char const * const val =
            arg + sizeof (dds_impl) - 1;
          be_global->dds_impl (val);
        }
      else if (ACE_OS::strstr (arg, opendds_sequence_suffix) == arg)
        {
          char const * const val =
            arg + sizeof (opendds_sequence_suffix) - 1;
          be_global->opendds_sequence_suffix (val);
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%C: invalid or unknown ")
                      ACE_TEXT ("argument <%C> to back end\n"),
                      idl_global->prog_name (),
                      arg));
        }
    }
}

void
be_util::arg_post_proc (void)
{
  // Let us try to use Perfect Hashing Operation Lookup Strategy. Let
  // us check whether things are fine with GPERF.
#if defined (ACE_HAS_GPERF) && !defined (ACE_USES_WCHAR)
  // If Perfect Hashing or Binary Search or Linear Search strategies
  // have been selected, let us make sure that it exists and will
  // work.
  if ((be_global->lookup_strategy () == BE_GlobalData::TAO_PERFECT_HASH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_BINARY_SEARCH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_LINEAR_SEARCH))
    {
      // Testing whether GPERF works or no.
      if (idl_global->check_gperf () == -1)
        {
          // If gperf_path is an absolute path, try to call this
          // again with
          ACE_DEBUG ((
              LM_DEBUG,
              ACE_TEXT ("TAO_IDL: warning, GPERF could not be executed\n")
              ACE_TEXT ("Perfect Hashing or Binary/Linear Search cannot be")
              ACE_TEXT (" done without GPERF\n")
              ACE_TEXT ("Now, using Dynamic Hashing..\n")
              ACE_TEXT ("To use Perfect Hashing or Binary/Linear")
              ACE_TEXT (" Search strategy\n")
              ACE_TEXT ("\t-Build gperf at $ACE_ROOT/apps/gperf/src\n")
              ACE_TEXT ("\t-Set the environment variable $ACE_ROOT")
              ACE_TEXT (" appropriately or add $ACE_ROOT/bin to the PATH\n")
              ACE_TEXT ("\t-Refer to Operation Lookup section in the TAO IDL")
              ACE_TEXT (" User Guide ($TAO_ROOT/docs/compiler.html)")
              ACE_TEXT (" for more details\n")
            ));

          // Switching over to Dynamic Hashing.
          be_global->lookup_strategy (BE_GlobalData::TAO_DYNAMIC_HASH);
        }
    }
#else /* Not ACE_HAS_GPERF */
  // If GPERF is not there, we cannot use PERFECT_HASH strategy. Let
  // us go for DYNAMIC_HASH.
  if ((be_global->lookup_strategy () == BE_GlobalData::TAO_PERFECT_HASH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_BINARY_SEARCH) ||
      (be_global->lookup_strategy () == BE_GlobalData::TAO_LINEAR_SEARCH))
    {
      be_global->lookup_strategy (BE_GlobalData::TAO_DYNAMIC_HASH);
    }
#endif /* ACE_HAS_GPERF */

  // Make sure that we are not suppressing TypeCode generation and asking for
  // optimized typecode support at the same time.
  if (!be_global->tc_support () && be_global->opt_tc ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Bad Combination -St and -Gt\n")));
    }
}

void
be_util::usage (void)
{
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,export_macro=<macro name>\t\t\tsets export macro ")
      ACE_TEXT ("for all files\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,export_include=<include path>\t\tsets export include ")
      ACE_TEXT ("file for all files\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,stub_export_macro=<macro name>\t\tsets export ")
      ACE_TEXT ("macro for client files only\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,stub_export_include=<include path>\t\tsets export ")
      ACE_TEXT ("include file for client only\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,skel_export_macro=<macro name>\t\tsets export ")
      ACE_TEXT ("macro for server files only\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,skel_export_include=<include path>\t\tsets export ")
      ACE_TEXT ("include file for server only\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,anyop_export_macro=<macro name>\t\tsets export macro ")
      ACE_TEXT ("for typecode/Any operator files only, when -GA option ")
      ACE_TEXT ("is used\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,anyop_export_include=<include path>\tsets export ")
      ACE_TEXT ("include file for typecode/Any operator files only, when -GA ")
      ACE_TEXT ("option is used\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,svnt_export_macro=<macro name>\t\tsets export macro ")
      ACE_TEXT ("for CIAO servant files only, when -Gsv option ")
      ACE_TEXT ("is used\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,svnt_export_include=<include path>\t\tsets export ")
      ACE_TEXT ("include file for CIAO servant files only, when -Gsv ")
      ACE_TEXT ("option is used\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,exec_export_macro=<macro name>\t\tsets export macro ")
      ACE_TEXT ("for CIAO executor impl files only, when -Gex option ")
      ACE_TEXT ("is used\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,exec_export_include=<include path>\t\tsets export ")
      ACE_TEXT ("include file for CIAO executor impl files only, when -Gex ")
      ACE_TEXT ("option is used\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,conn_export_macro=<macro name>\t\tsets export macro ")
      ACE_TEXT ("for CIAO connector impl files only, when -Gcn option ")
      ACE_TEXT ("is used\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,conn_export_include=<include path>\t\tsets export ")
      ACE_TEXT ("include file for CIAO connector impl files only, when -Gcn ")
      ACE_TEXT ("option is used\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,pch_include=<include path>\t\t\tsets include ")
      ACE_TEXT ("file for precompiled header mechanism\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,pre_include=<include path>\t\t\tsets include ")
      ACE_TEXT ("file generate before any other includes\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,post_include=<include path>\t\tsets include ")
      ACE_TEXT ("file generated at the end of the file\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,include_guard=<include path>\t\tguard to prevent ")
      ACE_TEXT ("the generated client header file\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,safe_include=<include path>\t\tinclude that should ")
      ACE_TEXT ("be used instead of the own generated client header file\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,unique_include=<include path>\t\tinclude that should ")
      ACE_TEXT ("be generated as only contents of the generated client ")
      ACE_TEXT ("header file.\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,container_type=<type>\t\t\ttype of container we generated\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,obv_opt_accessor\t\t\t\toptimizes access to base class ")
      ACE_TEXT ("data in valuetypes\n")
    ));
#if (defined (ACE_HAS_VERSIONED_NAMESPACE)      \
     && ACE_HAS_VERSIONED_NAMESPACE == 1)       \
  || (defined (TAO_HAS_VERSIONED_NAMESPACE)      \
     && TAO_HAS_VERSIONED_NAMESPACE == 1)
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,versioning_begin\t\t\tSet text that opens a ")
      ACE_TEXT ("a \"versioned\" namespace\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Wb,versioning_end\t\t\tSet text that closes a ")
      ACE_TEXT ("a \"versioned\" namespace\n")
    ));
#endif  /* ACE_HAS_VERSIONED_NAMESPACE || TAO_HAS_VERSIONED_NAMESPACE */
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -b\t\t\tUse a clonable argument type for oneway methods.\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -ci\t\t\tClient inline file name ending. Default is C.inl\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -cs\t\t\tClient stub's file name ending.")
      ACE_TEXT (" Default is C.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -g <gperf_path>\tPath for the GPERF program.")
      ACE_TEXT (" Default is $ACE_ROOT/bin/ace_gperf\n")));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GC \t\t\tGenerate the AMI classes\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GH \t\t\tGenerate the AMH classes\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GM \t\t\tGenerate the AMI4CCM classes\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gce \t\t\tGenerate code optimized for CORBA/e\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gmc \t\t\tGenerate code optimized for Minimum CORBA\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gcl \t\t\tGenerate code optimized for LwCCM\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gcm \t\t\tGenerate code optimized for noevent CCM\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gd \t\t\tGenerate the code for direct collocation. Default ")
      ACE_TEXT ("is thru-POA collocation\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gos \t\t\tGenerate std::ostream insertion operators.\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GI[h|s|b|e|c|a|d]\tGenerate Implementation Files\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\th - Implementation header file name ending.")
      ACE_TEXT (" Default is I.h\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\ts - Implementation skeleton file name ending.")
      ACE_TEXT (" Default is I.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\tb - Prefix to the implementation class names.")
      ACE_TEXT (" Default is 'no prefix'\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\te - Suffix to the implementation class names.")
      ACE_TEXT (" Default is _i\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\tc - Generate copy constructors in the servant")
      ACE_TEXT (" implementation template files (off by default)\n")
     ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\ta - Generate assignment operators in the servant")
      ACE_TEXT (" implementation template files (off by default)\n")
     ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("  \t\t\td - Generate debug (source file/line#) information.")
      ACE_TEXT (" (off by default)\n")
     ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gp \t\t\tGenerate the code for thru-POA collocation")
      ACE_TEXT (" (default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gsp\t\t\tGenerate the code for Smart Proxies\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gstl\t\t\tGenerate the alternate C++ mapping for")
      ACE_TEXT (" IDL strings and sequences\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gt\t\t\tenable optimized TypeCode support")
      ACE_TEXT (" (unopt by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GT\t\t\tgenerate tie class (and file)")
      ACE_TEXT (" generation (disabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT ("    \t\t\tNo effect if TypeCode generation is suppressed\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -GA\t\t\tgenerate Any operator and type codes in *A.{h,cpp}")
      ACE_TEXT (" (generated in *C.{h,cpp} by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Guc\t\t\tgenerate uninlined constant if declared ")
      ACE_TEXT ("in a module (inlined by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gse\t\t\tgenerate explicit export of sequence's ")
      ACE_TEXT ("template base class (not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gsv\t\t\tgenerate CIAO servant code ")
      ACE_TEXT ("(not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Glem\t\t\tgenerate CIAO executor IDL ")
      ACE_TEXT ("(not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Glfa\t\t\tgenerate executor IDL for all facets ")
      ACE_TEXT ("(only for facet interfaces in main file by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gex\t\t\tgenerate CIAO executor implementation ")
      ACE_TEXT ("code (not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gexr\t\t\tgenerate CIAO executor implementation ")
      ACE_TEXT ("code with an ACE_Reactor implementation (not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gcn\t\t\tgenerate CIAO connector implementation ")
      ACE_TEXT ("code (not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gts\t\t\tgenerate DDS type support IDL ")
      ACE_TEXT ("(not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gsc\t\t\tgenerate CIAO code for static ")
      ACE_TEXT ("configuration (not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gxhst\t\t\tgenerate export header file ")
      ACE_TEXT ("for stub (not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gxhsk\t\t\tgenerate export header file ")
      ACE_TEXT ("for skeleton (not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gxhsv\t\t\tgenerate export header file ")
      ACE_TEXT ("for CIAO servant (not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gxhex\t\t\tgenerate export header file ")
      ACE_TEXT ("for CIAO executor (not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Gxhcn\t\t\tgenerate export header file ")
      ACE_TEXT ("for CIAO connector (not generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -hc\t\t\tClient's header file name ending.")
      ACE_TEXT (" Default is C.h\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -hs\t\t\tServer's header file name ending.")
      ACE_TEXT (" Default is S.h\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -hT\t\t\tServer's template hdr file name ending.")
      ACE_TEXT (" Default is S_T.h\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H perfect_hash\tTo force perfect hashed operation")
      ACE_TEXT (" lookup strategy (default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H dynamic_hash\tTo force dynamic hashed operation")
      ACE_TEXT (" lookup strategy. Default is perfect hashing\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H linear_search\tTo force linear search operation")
      ACE_TEXT (" lookup strategy\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -H binary_search\tTo force binary search operation")
      ACE_TEXT (" lookup strategy\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -in \t\t\tTo generate <>s for standard #include'd")
      ACE_TEXT (" files (non-changing files)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -ic \t\t\tTo generate \"\"s for standard #include'd")
      ACE_TEXT (" files (changing files)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -iC <path>\t\tInclude path for the generated stub files ")
      ACE_TEXT ("in *A.h. Can be relative to $TAO_ROOT or $CIAO_ROOT. ")
      ACE_TEXT ("Default is $TAO_ROOT/tao or current directory\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -o <output_dir>\tOutput directory for the generated files.")
      ACE_TEXT (" Default is current directory\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -oS <output_dir>\tOutput directory for the generated ")
      ACE_TEXT ("skeleton files. Default is -o value or current directory\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -oA <output_dir>\tOutput directory for the generated anyop")
      ACE_TEXT ("files. Default is -o value or current directory\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -si\t\t\tServer's inline file name ending.")
      ACE_TEXT (" Default is S.inl\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -ss\t\t\tServer's skeleton file name ending.")
      ACE_TEXT (" Default is S.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -sT\t\t\tServer's template skeleton file name ending.")
      ACE_TEXT (" Default is S_T.cpp\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sa\t\t\tsuppress Any support")
      ACE_TEXT (" (support enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sal\t\t\tsuppress Any support")
      ACE_TEXT (" for local interfaces")
      ACE_TEXT (" (support enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Scdr\t\t\tsuppress CDR support")
      ACE_TEXT (" (support enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sat\t\t\tsuppress arg traits")
      ACE_TEXT (" generation")
      ACE_TEXT (" (arg traits generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -St\t\t\tsuppress TypeCode support")
      ACE_TEXT (" (support enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sp\t\t\tsuppress generating Thru POA collocated")
      ACE_TEXT (" stubs (enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sd\t\t\tsuppress generating Direct collocated")
      ACE_TEXT (" stubs (disable by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sm\t\t\tdisable IDL3 equivalent IDL preprocessing")
      ACE_TEXT (" (enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -SS\t\t\tsuppress generating skeleton implementation")
      ACE_TEXT (" and inline file (disabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sci\t\t\tsuppress generating client inline file")
      ACE_TEXT (" (disabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Ssi\t\t\tsuppress generating server inline file")
      ACE_TEXT (" (disabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sorb\t\t\tsuppress generating include of ORB.h")
      ACE_TEXT (" (disabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sfr\t\t\tsuppress generating valuetype factory")
      ACE_TEXT (" registration in CIAO (generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Se\t\t\tdisable custom header file name endings")
      ACE_TEXT (" for files\n\t\t\t")
      ACE_TEXT ("that are found in TAO specific include directories,\n\t\t\t")
      ACE_TEXT ("(i.e. $TAO_ROOT, $TAO_ROOT/tao, $TAO_ROOT/orbsvcs,\n\t\t\t")
      ACE_TEXT ("$TAO_ROOT/CIAO, $TAO_ROOT/CIAO/ciao, $TAO_ROOT/CIAO/ccm)\n\t\t\t")
      ACE_TEXT (" (enabled by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -Sg\t\t\tsuppress generating of unique header guards")
      ACE_TEXT (" (unique guards are generated by default)\n")
    ));
  ACE_DEBUG ((
      LM_DEBUG,
      ACE_TEXT (" -TS <value>\t\tset tab size for generated files")
      ACE_TEXT (" (default is 2 spaces)\n")
    ));
}

AST_Generator *
be_util::generator_init (void)
{
  tao_cg = TAO_CODEGEN::instance ();

  AST_Generator *gen = 0;
  ACE_NEW_RETURN (gen,
                  be_generator,
                  0);

  return gen;
}

const char *
be_util::get_output_path (bool for_anyop,
                          bool for_skel)
{
  if (for_anyop && 0 != be_global->anyop_output_dir ())
    {
      return be_global->anyop_output_dir ();
    }
  else if (for_skel && 0 != be_global->skel_output_dir ())
    {
      return be_global->skel_output_dir ();
    }
  else
    {
      return be_global->output_dir ();
    }
}

void
be_util::set_arg_seen_bit (be_type *bt)
{
  if (bt == 0)
    {
      return;
    }

  switch (bt->node_type ())
    {
      case AST_Decl::NT_typedef:
        {
          AST_Typedef *td = AST_Typedef::narrow_from_decl (bt);
          be_util::set_arg_seen_bit (
                    be_type::narrow_from_decl (td->primitive_base_type ())
                  );
          break;
        }
      case AST_Decl::NT_interface:
      case AST_Decl::NT_interface_fwd:
      case AST_Decl::NT_valuetype:
      case AST_Decl::NT_valuetype_fwd:
      case AST_Decl::NT_component:
      case AST_Decl::NT_component_fwd:
      case AST_Decl::NT_home:
      case AST_Decl::NT_eventtype:
      case AST_Decl::NT_eventtype_fwd:
        idl_global->object_arg_seen_ = true;
        break;
      case AST_Decl::NT_union:
      case AST_Decl::NT_struct:
        if (bt->size_type () == AST_Type::FIXED)
          {
            idl_global->fixed_size_decl_seen_ = true;
          }
        else
          {
            idl_global->var_size_decl_seen_ = true;
          }

        break;
      case AST_Decl::NT_struct_fwd:
      case AST_Decl::NT_union_fwd:
        {
          AST_StructureFwd *fwd = AST_StructureFwd::narrow_from_decl (bt);
          be_type *fd = be_type::narrow_from_decl (fwd->full_definition ());
          be_util::set_arg_seen_bit (fd);
          break;
        }
      case AST_Decl::NT_enum:
      case AST_Decl::NT_enum_val:
//        idl_global->basic_arg_seen_ = true;
        break;
      case AST_Decl::NT_string:
      case AST_Decl::NT_wstring:
        {
          AST_String *str = AST_String::narrow_from_decl (bt);

          if (str->max_size ()->ev ()->u.ulval == 0)
            {
              idl_global->ub_string_seen_ = true;
            }
          else
            {
              idl_global->bd_string_seen_ = true;
            }

          break;
        }
      case AST_Decl::NT_array:
        if (bt->size_type () == AST_Type::FIXED)
          {
            idl_global->fixed_array_decl_seen_ = true;
          }
        else
          {
            idl_global->var_array_decl_seen_ = true;
          }

        break;
      case AST_Decl::NT_sequence:
        idl_global->var_size_decl_seen_ = true;
        break;
      case AST_Decl::NT_pre_defined:
        {
          AST_PredefinedType *pdt = AST_PredefinedType::narrow_from_decl (bt);

          switch (pdt->pt ())
            {
              case AST_PredefinedType::PT_object:
              case AST_PredefinedType::PT_pseudo:
              case AST_PredefinedType::PT_value:
              case AST_PredefinedType::PT_abstract:
                idl_global->object_arg_seen_ = true;
                break;
              case AST_PredefinedType::PT_any:
                idl_global->var_size_decl_seen_ = true;
                idl_global->any_arg_seen_ = true;
                break;
              case AST_PredefinedType::PT_char:
              case AST_PredefinedType::PT_wchar:
              case AST_PredefinedType::PT_octet:
              case AST_PredefinedType::PT_boolean:
                idl_global->special_basic_decl_seen_ = true;
                break;
             default:
                break;
            }
        }
      default:
        break;
    }
}

