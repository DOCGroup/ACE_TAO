/**
 * @author Marcel Smit (msmit@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_READ_T_H_
#define DDS_READ_T_H_

#include "dds4ccm/impl/DDS_Subscriber_Base_T.h"

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
class DDS_Read_T
  : public DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>
{
public:
  DDS_Read_T (void);
  ~DDS_Read_T (void);
};

#include "dds4ccm/impl/DDS_Read_T.cpp"

#endif /* DDS_READ_T_H_ */
