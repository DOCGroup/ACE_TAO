/**
 * @author Marcel Smit (msmit@remedy.nl)
 *
 * $Id$
 *
 * Starting point for parsing the Topic QOS settings.
 */
#ifndef QOS_DATAWRITER_T_H_
#define QOS_DATAWRITER_T_H_

#include "QOS_DwTp_Base_T.h"

template <typename XML_QOS_TYPE, typename DDS_QOS_TYPE>
class QOS_DataWriter_T
  : public QOS_DwTp_Base_T<XML_QOS_TYPE, DDS_QOS_TYPE>
{
public:
  QOS_DataWriter_T (void);
  ~QOS_DataWriter_T (void);

  void read_qos (DDS_QOS_TYPE&, const XML_QOS_TYPE);

private:
  typedef QOS_DwTp_Base_T<XML_QOS_TYPE, DDS_QOS_TYPE> DwTpBase;
};

#include "dds4ccm/tools/QOS_XML_Handler/QOS_DataWriter_T.cpp"

#endif /* QOS_DATAWRITER_T_H_ */
