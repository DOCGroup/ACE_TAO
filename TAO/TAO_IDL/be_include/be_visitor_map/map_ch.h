/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    map_ch.h
 *
 *  Concrete visitor for the Sequence class
 *  This one provides code generation for the Sequence node in the client
 *  header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_MAP_MAP_CH_H_
#define _BE_VISITOR_MAP_MAP_CH_H_

/**
 * @class be_visitor_map_ch
 *
 * @brief be_visitor_map_ch
 *
 * This is a concrete visitor to generate the client header for
 * maps
 */
class be_visitor_map_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_map_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_map_ch ();

  /// visit map node.
  virtual int visit_map (be_map *node);

  /// Generate the typedefs for our _var and _out template classes.
  void gen_varout_typedefs (be_map *node,
                            be_type *elem);
};

#endif /* _BE_VISITOR_MAP_MAP_CH_H_ */
