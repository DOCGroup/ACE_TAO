/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_READ_T_H_
#define DDS_READ_T_H_

#include "dds4ccm/impl/DDS_Subscriber_Base_T.h"

template <typename CCM_TYPE, typename DDS_TYPE>
class DDS_Read_T
  : public DDS_Subscriber_Base_T<CCM_TYPE, DDS_TYPE>
{
public:
  DDS_Read_T (void);
  ~DDS_Read_T (void);
};

#include "dds4ccm/impl/DDS_Read_T.cpp"

#endif /* DDS_READ_T_H_ */
