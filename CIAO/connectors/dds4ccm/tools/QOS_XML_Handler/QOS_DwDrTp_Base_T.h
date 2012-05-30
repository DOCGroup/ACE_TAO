/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 */
#ifndef QOS_DWDRTP_BASE_T_H_
#define QOS_DWDRTP_BASE_T_H_

template <typename XML_QOS_TYPE, typename DDS_QOS_TYPE>
class QOS_DwDrTp_Base_T
{
public:
  QOS_DwDrTp_Base_T (void);
  ~QOS_DwDrTp_Base_T (void);

  void read_qos (DDS_QOS_TYPE&, const XML_QOS_TYPE);
};

#include "dds4ccm/tools/QOS_XML_Handler/QOS_DwDrTp_Base_T.cpp"

#endif /* QOS_DWDRTP_BASE_T_H_ */
