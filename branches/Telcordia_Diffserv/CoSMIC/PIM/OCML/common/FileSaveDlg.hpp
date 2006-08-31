#ifndef FILE_SAVE_DLG_HPP
#define FILE_SAVE_DLG_HPP

#include <windows.h>
#include <commdlg.h>
#include <string>

class FileSaveDlg
{
public:
  FileSaveDlg(const char* title = "Save file");

  ~FileSaveDlg();
  
  bool show(const char* file_name = "");

  const char* file_name() const;

private:
  OPENFILENAME ofn_;
  std::string file_name_;
};

#endif // FILE_SAVE_DLG_HPP
