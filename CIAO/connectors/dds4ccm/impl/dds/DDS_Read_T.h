/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_READ_T_H_
#define DDS_READ_T_H_

#include "dds4ccm/impl/dds/DDS_Subscriber_Base_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class DDS_Read_T
  : public DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>
{
public:
  DDS_Read_T (void);
  ~DDS_Read_T (void);
};

#include "dds4ccm/impl/dds/DDS_Read_T.cpp"

#endif /* DDS_READ_T_H_ */
