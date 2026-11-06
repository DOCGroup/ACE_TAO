/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    map_ch.h
 *
 *  Concrete visitor for the Map class
 *  This one provides code generation for the Map node in the client
 *  header.
 *
 *  @author Tyler Mayoff
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
  explicit be_visitor_map_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_map_ch () override = default;

  be_visitor_map_ch (const be_visitor_map_ch &) = delete;
  be_visitor_map_ch (be_visitor_map_ch &&) = delete;
  be_visitor_map_ch &operator= (const be_visitor_map_ch &) = delete;
  be_visitor_map_ch &operator= (be_visitor_map_ch &&) = delete;

  /// visit map node.
  int visit_map (be_map *node) override;
};

#endif /* _BE_VISITOR_MAP_MAP_CH_H_ */
