#include "FileSaveDlg.hpp"

#include <cstdlib>

FileSaveDlg::FileSaveDlg(const char* title)
{
  // Initialize the OPENFILENAME structure
  ZeroMemory(&ofn_, sizeof(ofn_));
  ofn_.lStructSize = sizeof(ofn_);
  ofn_.hwndOwner = 0;
  ofn_.lpstrFile = new char[MAX_PATH];
  ofn_.lpstrTitle = strdup(title);
  ofn_.nMaxFile = MAX_PATH;
  ofn_.lpstrFilter = "All\0*.*\0"; // "All\0*.*\0Text\0*.txt\0";
  ofn_.nFilterIndex = 1;
  ofn_.lpstrFileTitle = NULL;
  ofn_.nMaxFileTitle = 0;
  ofn_.Flags = OFN_PATHMUSTEXIST;
}

FileSaveDlg::~FileSaveDlg()
{
  delete[] ofn_.lpstrFile;
  free((void*) ofn_.lpstrTitle);
}
  
bool
FileSaveDlg::show(const char* file_name)
{
  strcpy(ofn_.lpstrFile, file_name);

  // GetSaveFileName() system call shows up the save file dialog
  bool result;
  if (result = bool(GetSaveFileName(&ofn_)))
    file_name_ = ofn_.lpstrFile;
  return result;
}

const char*
FileSaveDlg::file_name() const
{
  return file_name_.c_str();
}
