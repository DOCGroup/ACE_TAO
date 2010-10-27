// $Id$

/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

*/

#include "be_visitor_root.h"
#include "be_visitor_ami_pre_proc.h"
#include "be_visitor_amh_pre_proc.h"
#include "be_visitor_ccm_pre_proc.h"
#include "be_visitor_context.h"
#include "be_root.h"
#include "be_extern.h"
#include "fe_extern.h"
#include "global_extern.h"

// Clean up before exit, whether successful or not.
TAO_IDL_BE_Export void
BE_cleanup (void)
{
  idl_global->destroy ();
}

// Abort this run of the BE.
TAO_IDL_BE_Export void
BE_abort (void)
{
  ACE_ERROR ((LM_ERROR,
              "Fatal Error - Aborting\n"));

  // BE_cleanup will be called after the exception is caught.
  throw Bailout ();
}

void
BE_visit_root (be_visitor_decl &root_visitor,
               const char *which_pass)
{
  static be_root *root =
    be_root::narrow_from_decl (idl_global->root ());

  if (-1 == root->accept (&root_visitor))
    {
      ACE_ERROR ((LM_ERROR,
                  "BE_visit_root - %C for Root failed\n",
                  which_pass));
      BE_abort ();
    }

  root_visitor.ctx ()->reset ();
}

// Do the work of this BE. This is the starting point for code generation.
TAO_IDL_BE_Export void
BE_produce (void)
{
  be_visitor_context ctx;

  if (!idl_global->ignore_idl3 ())
    {
      be_visitor_ccm_pre_proc ccm_preproc_visitor (&ctx);
      BE_visit_root (ccm_preproc_visitor, "CCM preprocessing");
    }

  if (be_global->ami_call_back ())
    {
      be_visitor_ami_pre_proc ami_preproc_visitor (&ctx);
      BE_visit_root (ami_preproc_visitor, "AMI preprocessing");
    }

  if (be_global->gen_amh_classes ())
    {
      be_visitor_amh_pre_proc amh_pre_proc_visitor (&ctx);
      BE_visit_root (amh_pre_proc_visitor, "AMH preprocessing");
    }

  const char *fname = be_global->be_get_anyop_header_fname ();

  // No-op if the -GA wasn't on the command line.
  if (-1 == tao_cg->start_anyop_header (fname))
    {
      BE_abort ();
    }

  ctx.state (TAO_CodeGen::TAO_ROOT_CH);
  be_visitor_root_ch root_ch_visitor (&ctx);
  BE_visit_root (root_ch_visitor, "client header");

  // Initialize the anyop source stream, if the option is set.
  // It has to be done after the stub header file generation,
  // where checks for recursive types are done,
  // and before stub source file generation, since
  // generation of Any-related #includes may be redirected.
  fname = be_global->be_get_anyop_source_fname ();

  if (-1 == tao_cg->start_anyop_source (fname))
    {
      BE_abort ();
    }

  if (be_global->gen_client_inline ())
    {
      ctx.state (TAO_CodeGen::TAO_ROOT_CI);
      be_visitor_root_ci root_ci_visitor (&ctx);
      BE_visit_root (root_ci_visitor, "client inline");
    }

  if (be_global->gen_client_stub ())
    {
      ctx.state (TAO_CodeGen::TAO_ROOT_CS);
      be_visitor_root_cs root_cs_visitor (&ctx);
      BE_visit_root (root_cs_visitor, "client stub");
    }

  ctx.state (TAO_CodeGen::TAO_ROOT_SH);
  be_visitor_root_sh root_sh_visitor (&ctx);
  BE_visit_root (root_sh_visitor, "server header");

  if (be_global->gen_server_inline ())
    {
      ctx.state (TAO_CodeGen::TAO_ROOT_SI);
      be_visitor_root_si root_si_visitor (&ctx);
      BE_visit_root (root_si_visitor, "server inline");
    }

  if (be_global->gen_server_skeleton ())
    {
      ctx.state (TAO_CodeGen::TAO_ROOT_SS);
      be_visitor_root_ss root_ss_visitor (&ctx);
      BE_visit_root (root_ss_visitor, "server skeleton");
    }

  // Inline and source files for tie classes are generated
  // by the corresponding skeleton visitors.
  if (be_global->gen_tie_classes ())
    {
      ctx.state (TAO_CodeGen::TAO_ROOT_TIE_SH);
      be_visitor_root_sth sth_visitor (&ctx);
      BE_visit_root (sth_visitor, "server template header");
    }

  if (be_global->gen_impl_files ())
    {
      ctx.state (TAO_CodeGen::TAO_ROOT_IH);
      be_visitor_root_ih root_ih_visitor (&ctx);
      BE_visit_root (root_ih_visitor, "implementation header");

      ctx.state (TAO_CodeGen::TAO_ROOT_IS);
      be_visitor_root_is root_is_visitor (&ctx);
      BE_visit_root (root_is_visitor, "implementation skeleton");
    }

  if (be_global->gen_ciao_svnt ())
    {
      ctx.state (TAO_CodeGen::TAO_ROOT_SVH);
      be_visitor_root_svh root_svh_visitor (&ctx);
      BE_visit_root (root_svh_visitor, "CIAO servant header");

      ctx.state (TAO_CodeGen::TAO_ROOT_SVS);
      be_visitor_root_svs root_svs_visitor (&ctx);
      BE_visit_root (root_svs_visitor, "CIAO servant source");
    }

  if (be_global->gen_ciao_exec_idl ())
    {
      ctx.state (TAO_CodeGen::TAO_ROOT_EX_IDL);
      be_visitor_root_ex_idl root_svs_visitor (&ctx);
      BE_visit_root (root_svs_visitor, "CIAO executor IDL");
    }

  if (be_global->gen_ciao_exec_impl ())
    {
      ctx.state (TAO_CodeGen::TAO_ROOT_EXH);
      be_visitor_root_exh root_exh_visitor (&ctx);
      BE_visit_root (root_exh_visitor, "CIAO exec impl header");

      ctx.state (TAO_CodeGen::TAO_ROOT_EXS);
      be_visitor_root_exs root_exs_visitor (&ctx);
      BE_visit_root (root_exs_visitor, "CIAO exec impl source");
    }

  if (be_global->gen_ciao_conn_impl ())
    {
      ctx.state (TAO_CodeGen::TAO_ROOT_CNH);
      be_visitor_root_cnh root_cnh_visitor (&ctx);
      BE_visit_root (root_cnh_visitor, "CIAO conn impl header");

      ctx.state (TAO_CodeGen::TAO_ROOT_CNS);
      be_visitor_root_cns root_cns_visitor (&ctx);
      BE_visit_root (root_cns_visitor, "CIAO conn impl source");
    }

  tao_cg->gen_export_files ();

  // Done with this IDL file.
  BE_cleanup ();
}

