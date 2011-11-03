//=============================================================================
/**
 *  @file   amh_si.h
 *
 *  $Id$
 *
 *  Specialized interface visitor for AMH
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef AMH_SH_I_
#define AMH_SH_I_

class be_visitor_amh_interface_si : public be_visitor_interface_si
{
public:
  be_visitor_amh_interface_si (be_visitor_context *ctx);
  ~be_visitor_amh_interface_si (void);

  int visit_interface (be_interface *node);

  // Helper method passed to the template method to generate code for the
  // AMH skeletons in the inline files.
  static int gen_skel_helper (be_interface *,
                              be_interface *,
                              TAO_OutStream *);
};

#endif /* AMH_SH_I_ */
