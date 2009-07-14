
class  CCM_DDS_string_RawListener_i
  : public virtual POA_CCM_DDS::string_RawListener
{
public:
  // Constructor 
  CCM_DDS_string_RawListener_i (void);
  
  // Destructor 
  virtual ~CCM_DDS_string_RawListener_i (void);
  
  virtual
  void on_data (
    const char * an_instance,
    const ::CCM_DDS::ReadInfo & info);
};
