class  CCM_DDS_string_Writer_i
  : public virtual POA_CCM_DDS::string_Writer
{
public:
  // Constructor 
  CCM_DDS_string_Writer_i (void);
  
  // Destructor 
  virtual ~CCM_DDS_string_Writer_i (void);
  
  virtual
  void write (
    const char * an_instance);
};
