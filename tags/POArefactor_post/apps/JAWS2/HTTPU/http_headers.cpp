// $Id$

#include "ace/RB_Tree.h"
#include "HTTPU/http_headers.h"

HTTP_Hdr_Node

  HTTP_HCodes::HTTP ("HTTP", "HTTP%s %s"),
  HTTP_HCodes::ACCEPT ("Accept", "Accept: %s\r\n"),
  HTTP_HCodes::ACCEPTCHARSET ("Accept-Charset", "Accept-Charset: %s\r\n"),
  HTTP_HCodes::ACCEPTENCODING ("Accept-Encoding", "Accept-Encoding: %s\r\n"),
  HTTP_HCodes::ACCEPTLANGUAGE ("Accept-Language", "Accept-Language: %s\r\n"),
  HTTP_HCodes::ACCEPTRANGES ("Accept-Ranges", "Accept-Ranges: %s\r\n"),
  HTTP_HCodes::AGE ("Age", "Age: %s\r\n"),
  HTTP_HCodes::ALLOW ("Allow", "Allow: %s\r\n"),
  HTTP_HCodes::AUTHORIZATION ("Authorization", "Authorization: %s\r\n"),
  HTTP_HCodes::CACHECONTROL ("Cache-Control", "Cache-Control: %s\r\n"),
  HTTP_HCodes::CONNECTION ("Connection", "Connection: %s\r\n"),
  HTTP_HCodes::CONTENTENCODING ("Content-Encoding",
                                "Content-Encoding: %d\r\n"),
  HTTP_HCodes::CONTENTLENGTH ("Content-Length", "Content-Length: %d\r\n"),
  HTTP_HCodes::CONTENTLOCATION ("Content-Location", "Content-Location: %s\r\n"),
  HTTP_HCodes::CONTENTMD5 ("Content-MD5", "Content-MD5: %s\r\n"),
  HTTP_HCodes::CONTENTRANGE ("Content-Range", "Content-Range: %s\r\n"),
  HTTP_HCodes::CONTENTTYPE ("Content-Type", "Content-Type: %s\r\n"),
  HTTP_HCodes::DATE ("Date", "Date: %s\r\n"),
  HTTP_HCodes::ETAG ("ETag", "ETag: %s\r\n"),
  HTTP_HCodes::EXPECT ("Expect", "Expect: %s\r\n"),
  HTTP_HCodes::EXPIRES ("Expires", "Expires: %s\r\n"),
  HTTP_HCodes::FROM ("From", "From: %s\r\n"),
  HTTP_HCodes::HOST ("Host", "Host: %s\r\n"),
  HTTP_HCodes::IFMATCH ("If-Match", "If-Match: %s\r\n"),
  HTTP_HCodes::IFMODIFIEDSINCE ("If-Modified-Since",
                                "If-Modified-Since: %s\r\n"),
  HTTP_HCodes::IFNONEMATCH ("If-None-Match", "If-None-Match: %s\r\n"),
  HTTP_HCodes::IFRANGE ("If-Range", "If-Range: %s\r\n"),
  HTTP_HCodes::IFUNMODIFIEDSINCE ("If-Unmodified-Since",
                                  "If-Unmodified-Since: %s\r\n"),
  HTTP_HCodes::LASTMODIFIED ("Last-Modified", "Last-Modified: %s\r\n"),
  HTTP_HCodes::LOCATION ("Location", "Location: %s\r\n"),
  HTTP_HCodes::MAXFORWARDS ("Max-Forwards", "Max-Forwards: %s\r\n"),
  HTTP_HCodes::PRAGMA ("Pragma", "Pragma: %s\r\n"),
  HTTP_HCodes::PROXYAUTHENTICATE ("Proxy-Authenticate",
                                  "Proxy-Authenticate: %s\r\n"),
  HTTP_HCodes::PROXYAUTHORIZATION ("Proxy-Authorization",
                                   "Proxy-Authorization: %s\r\n"),
  HTTP_HCodes::RANGE ("Range", "Range: %s\r\n"),
  HTTP_HCodes::REFERER ("Referer", "Referer: %s\r\n"),
  HTTP_HCodes::SERVER ("Server", "Server: %s\r\n"),
  HTTP_HCodes::TE ("TE", "TE: %s\r\n"),
  HTTP_HCodes::TRAILER ("Trailer", "Trailer: %s\r\n"),
  HTTP_HCodes::TRANSFERENCODING ("Transfer-Encoding",
                                 "Transfer-Encoding: %s\r\n"),
  HTTP_HCodes::UPGRADE ("Ugrade", "Ugrade: %s\r\n"),
  HTTP_HCodes::USERAGENT ("User-Agent", "User-Agent: %s\r\n"),
  HTTP_HCodes::VARY ("Vary", "Vary: %s\r\n"),
  HTTP_HCodes::VIA ("Via", "Via: %s\r\n"),
  HTTP_HCodes::WARNING ("Warning", "Warning: %s\r\n"),
  HTTP_HCodes::WWWAUTHENTICATE ("WWW-Authenticate",
                                "WWW-Authenticate: %s\r\n"),
  HTTP_HCodes::GET ("GET", "GET %s HTTP/%s\r\n"),
  HTTP_HCodes::HEAD ("HEAD", "HEAD %s HTTP/%s\r\n"),
  HTTP_HCodes::POST ("POST", "POST %s HTTP/%s\r\n"),
  HTTP_HCodes::PUT ("PUT", "PUT %s HTTP/%s\r\n"),
  HTTP_HCodes::QUIT ("QUIT", "QUIT %s HTTP/%s\r\n"),
  HTTP_HCodes::DUNNO ("", ""),
  HTTP_HCodes::META ("<META", "<META %s>"),
  HTTP_HCodes::A ("<A", "<A %s>"),
  HTTP_HCodes::SCRIPT ("<SCRIPT", "<SCRIPT %s>"),
  HTTP_HCodes::APPLET ("<APPLET", "<APPLET %s>")

  //////////////
  ;


const int &HTTP_HCodes::NUM_HEADER_STRINGS
  = HTTP_Header_Nodes_Singleton::instance ()->num_header_strings_;

HTTP_Header_Nodes::HTTP_Header_Nodes (void)
  : num_header_strings_ (0)
{
}

HTTP_Hdr_Node::HTTP_Hdr_Node (const char *token, const char *format)
  : token_ (token),
    format_ (format)
{
  HTTP_Header_Nodes *header_nodes
    = HTTP_Header_Nodes_Singleton::instance ();

  this->index_ = header_nodes->num_header_strings_;
  header_nodes->insert (this->index_, this);
  header_nodes->num_header_strings_++;
}

HTTP_HCodes::HTTP_HCodes (void)
  : header_nodes_ (HTTP_Header_Nodes_Singleton::instance ())
{
}

HTTP_Headers::HTTP_Headers (void)
{
}

const char *
HTTP_Headers::header (int name) const
{
  return this->header_token (name);
}

const char *
HTTP_Headers::value (int index)
{
  this->value_reset ();
  return this->value_next (index);
}

const char *
HTTP_Headers::value_next (int index)
{
  const char *hs = 0;
  const char *hv = 0;
  JAWS_Header_Data *data;

  if (0 <= index && index < NUM_HEADER_STRINGS)
    {
      hs = this->header (index);
      data = this->table ()->find_next (hs);
      if (data != 0)
        hv = data->header_value ();
    }

  return hv;
}

void
HTTP_Headers::value_reset (void)
{
  this->table ()->iter ().first ();
}

#if !defined (ACE_HAS_INLINED_OSCALLS)
#   include "HTTPU/http_headers.i"
# endif /* ACE_HAS_INLINED_OSCALLS */


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_RB_Tree<int, const HTTP_Hdr_Node *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<int, const HTTP_Hdr_Node *>;
template class ACE_RB_Tree_Iterator_Base<int, const HTTP_Hdr_Node *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<int, const HTTP_Hdr_Node *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<int, const HTTP_Hdr_Node *, ACE_Less_Than<int>, ACE_Null_Mutex>;
template class ACE_Singleton<HTTP_Header_Nodes, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_RB_Tree<int, const HTTP_Hdr_Node *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<int, const HTTP_Hdr_Node *>
#pragma instantiate ACE_RB_Tree_Iterator_Base<int, const HTTP_Hdr_Node *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<int, const HTTP_Hdr_Node *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Reverse_Iterator<int, const HTTP_Hdr_Node *, ACE_Less_Than<int>, ACE_Null_Mutex>
#pragma instantiate ACE_Singleton<HTTP_Header_Nodes, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
