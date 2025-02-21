#include "util.h"

URL::URL(char *input_buf) {
  char *buffer = new char[BUFSIZ];

  ACE_OS::strcpy(buffer,input_buf);
  if(buffer == 0)
    return;

  char *temp;
  char *lasts;

  if((temp = ACE_OS::strtok_r(buffer,": ",&lasts)))  {
    protocol_ = (char *) ACE_OS::malloc(ACE_OS::strlen(temp) + 1);
    ACE_OS::strcpy(protocol_, temp);
  }

  if((temp = ACE_OS::strtok_r(0,"/",&lasts)))  {
    hostname_ = (char *) ACE_OS::malloc(ACE_OS::strlen(temp) + 1);
    ACE_OS::strcpy(hostname_, temp);
  }
  if((temp = ACE_OS::strtok_r(0,"\0",&lasts))) {
    filename_ = (char *) ACE_OS::malloc(ACE_OS::strlen(temp) + 1);
    ACE_OS::strcpy(filename_, temp);
  }
  else {
    filename_ = (char *) ACE_OS::malloc(ACE_OS::strlen(INDEX_NAME) + 1);
    ACE_OS::strcpy(filename_,INDEX_NAME);
  }
}

char *URL::get_protocol() {
  return protocol_;
}

char *URL::get_hostname() {
  return hostname_;
}

char *URL::get_filename() {
  return filename_;
}




void cleanup() {
  ACE_OS::unlink(TEMPORARY_FILE_NAME);
  ACE_OS::unlink(INCOMING_FILE_NAME);
}

void sigint(int) {
  cleanup();
}
