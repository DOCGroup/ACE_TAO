
//=============================================================================
/**
 *  @file    array_ch.h
 *
 *  Visitors for generation of code for Arrays in client header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ARRAY_CH_H_
#define _BE_VISITOR_ARRAY_CH_H_

/**
 * @class be_visitor_array_ch
 *
 * @brief be_visitor_array_ch
 *
 * Code to be generated for arrays in the client header file
 */
class be_visitor_array_ch : public be_visitor_array
{
public:
  /// constructor
  be_visitor_array_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_array_ch () override = default;

  /// visit the array node
  int visit_array (be_array *node) override;
};

#endif /* _BE_VISITOR_ARRAY_CH_H_ */
