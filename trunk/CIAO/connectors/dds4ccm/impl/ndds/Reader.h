// $Id$

class  CCM_DDS_string_Reader_i
  : public virtual POA_CCM_DDS::string_Reader
{
public:
  // Constructor
  CCM_DDS_string_Reader_i (void);

  // Destructor
  virtual ~CCM_DDS_string_Reader_i (void);

  virtual
  void read_all (
    ::CCM_DDS::string_Reader::stringSeq_out instances,
    ::CCM_DDS::ReadInfoSeq_out infos);

  virtual
  void read_all_history (
    ::CCM_DDS::string_Reader::stringSeq_out instances,
    ::CCM_DDS::ReadInfoSeq_out infos);

  virtual
  void read_one (
    char *& an_instance,
    ::CCM_DDS::ReadInfo_out info);

  virtual
  void read_one_history (
    const char * an_instance,
    ::CCM_DDS::string_Reader::stringSeq_out instances,
    ::CCM_DDS::ReadInfoSeq_out infos);

  virtual
  ::CCM_DDS::QueryFilter * filter (void);

  virtual
  void filter (
    const ::CCM_DDS::QueryFilter & filter);
};
