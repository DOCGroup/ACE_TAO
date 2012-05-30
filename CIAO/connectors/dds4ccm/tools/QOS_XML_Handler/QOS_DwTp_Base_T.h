/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 */
#ifndef QOS_DWTP_BASE_T_H_
#define QOS_DWTP_BASE_T_H_

#include "QOS_DwDrTp_Base_T.h"

template <typename XML_QOS_TYPE, typename DDS_QOS_TYPE>
class QOS_DwTp_Base_T
  : public QOS_DwDrTp_Base_T<XML_QOS_TYPE, DDS_QOS_TYPE>
{
public:
  QOS_DwTp_Base_T (void);
  ~QOS_DwTp_Base_T (void);

  void read_qos (DDS_QOS_TYPE&, const XML_QOS_TYPE);

private:
  typedef QOS_DwDrTp_Base_T<XML_QOS_TYPE, DDS_QOS_TYPE> DwDrTpBase;
};

#include "dds4ccm/tools/QOS_XML_Handler/QOS_DwTp_Base_T.cpp"

#endif /* QOS_DWTP_BASE_T_H_ */
