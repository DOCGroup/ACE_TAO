// -*- C++ -*-

class Main_Dialog : public CDialog
{
public:
  Main_Dialog (CWnd * parent = 0);
  virtual ~Main_Dialog (void);

  enum Menu_Option
  {
    OPT_GENERATE_MODELS = 0,
    OPT_GENERATE_SOURCE = 1
  };

  void DoDataExchange (CDataExchange * pDX);

  int option (void) const;

private:
  int opt_;
};
