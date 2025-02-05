/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  Concrete visitor for the Map class
 *  This one provides code generation for the CDR operators for the map
 *  in the client header.
 *
 *  @author Tyler Mayoff
 */
//=============================================================================


#ifndef _BE_VISITOR_MAP_CDR_OP_CH_H_
#define _BE_VISITOR_MAP_CDR_OP_CH_H_

/**
 * @class be_visitor_map_cdr_op_ch
 *
 * @brief be_visitor_map_cdr_op_ch
 *
 * This is a concrete visitor for map that generates the CDR operator
 * declarations
 */
class be_visitor_map_cdr_op_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_map_cdr_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_map_cdr_op_ch () = default;

  /// visit map
  int visit_map (be_map *node) override;
};

#endif /* _BE_VISITOR_MAP_CDR_OP_CH_H_ */
