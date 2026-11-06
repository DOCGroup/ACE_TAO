/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    map_cs.h
 *
 *  Concrete visitor for the Map class
 *  This one provides code generation for the Map node in the client
 *  stubs.
 *
 *  @author Tyler Mayoff
 */
//=============================================================================


#ifndef _BE_VISITOR_MAP_MAP_CS_H_
#define _BE_VISITOR_MAP_MAP_CS_H_

/**
 * @class be_visitor_map_cs
 *
 * @brief be_visitor_map_cs
 *
 * This is a concrete visitor to generate the client stubs for
 * maps
 */
class TAO_OutStream;
class be_visitor_map_cs : public be_visitor_decl
{
public:
  /// constructor
  explicit be_visitor_map_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_map_cs () override = default;

  be_visitor_map_cs (const be_visitor_map_cs &) = delete;
  be_visitor_map_cs (be_visitor_map_cs &&) = delete;
  be_visitor_map_cs &operator= (const be_visitor_map_cs &) = delete;
  be_visitor_map_cs &operator= (be_visitor_map_cs &&) = delete;

  /// visit map node
  int visit_map (be_map *node) override;
};

#endif /* _BE_VISITOR_MAP_MAP_CS_H_ */
