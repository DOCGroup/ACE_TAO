// -*- C++ -*-

#include "SSL_Asynch_BIO.h"
#include "SSL_Asynch_Stream.h"

ACE_RCSID (ACE_SSL,
           SSL_Asynch_BIO,
           "$Id$")

#if OPENSSL_VERSION_NUMBER > 0x0090581fL && ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS)))

#define BIO_TYPE_ACE  ( 21 | BIO_TYPE_SOURCE_SINK )

static BIO_METHOD methods_ACE =
  {
    BIO_TYPE_ACE, // BIO_TYPE_PROXY_SERVER,
    "ACE_Asynch_BIO",
    ACE_Asynch_BIO_write,
    ACE_Asynch_BIO_read,
    ACE_Asynch_BIO_puts,
    NULL, /* ACE_Asynch_BIO_gets, */
    ACE_Asynch_BIO_ctrl,
    ACE_Asynch_BIO_new,
    ACE_Asynch_BIO_free,
    NULL
  };

BIO_METHOD *
BIO_s_ACE_Asynch (void)
{
  return (&methods_ACE);
}

BIO *
BIO_new_ACE_Asynch (void *ssl_asynch_stream)
{
  BIO * pBIO = BIO_new (BIO_s_ACE_Asynch ());

  if (pBIO)
    BIO_ctrl (pBIO,
              BIO_C_SET_FILE_PTR,
              0,
              ssl_asynch_stream);

  return pBIO;
}

int
ACE_Asynch_BIO_new (BIO *pBIO)
{
  pBIO->init  = 0;    // not initialized
  pBIO->num   = 0;    // still zero ( we can use it )
  pBIO->ptr   = 0;    // will be pointer to ACE_SSL_Asynch_Stream
  pBIO->flags = 0;    //

  return 1;
}

int
ACE_Asynch_BIO_free (BIO *pBIO)
{
  if (pBIO == 0)
    return 0;

  if (pBIO->shutdown)
    {
      pBIO->ptr   = 0;
      pBIO->init  = 0;
      pBIO->num   = 0;
      pBIO->flags = 0;
    }

  return 1;
}

int
ACE_Asynch_BIO_read (BIO * pBIO, char * buf, int len)
{
  BIO_clear_retry_flags (pBIO);

  ACE_SSL_Asynch_Stream * p_stream =
    ACE_static_cast (ACE_SSL_Asynch_Stream *, pBIO->ptr);

  if (pBIO->init == 0 || p_stream == 0)
    return -1;

  if (buf == 0)
    return -1;

  if (len <= 0 )
    return -1;

  BIO_clear_retry_flags (pBIO);

  int errval = 0;

  int retval =
    p_stream->ssl_bio_read (buf,
                            ACE_static_cast (size_t, len),
                            errval);

  if (retval >= 0)
    return retval;

  if (errval == EINPROGRESS)
    BIO_set_retry_read (pBIO);

  return -1;
}

int
ACE_Asynch_BIO_write (BIO * pBIO, const char * buf, int len)
{
  BIO_clear_retry_flags (pBIO);

  ACE_SSL_Asynch_Stream * p_stream =
    ACE_static_cast (ACE_SSL_Asynch_Stream *, pBIO->ptr);

  if (pBIO->init == 0 || p_stream == 0)
    return -1;

  if (buf == 0)
    return -1;

  if (len <= 0)
    return -1;

  BIO_clear_retry_flags (pBIO);

  int errval = 0;

  int retval =
    p_stream->ssl_bio_write (buf,
                             ACE_static_cast (size_t, len),
                             errval);

  if (retval >= 0)
    return retval;

  if (errval == EINPROGRESS)
    BIO_set_retry_write (pBIO);

  return -1;
}

long
ACE_Asynch_BIO_ctrl (BIO * pBIO, int cmd, long num, void *ptr)
{
  long ret = 1;

  switch (cmd)
    {
    case BIO_C_SET_FILE_PTR:
      pBIO->shutdown = ACE_static_cast (int, num);
      pBIO->ptr = ptr;
      pBIO->init = 1;
      break;

    case BIO_CTRL_INFO:
      ret = 0;
      break;

    case BIO_CTRL_GET_CLOSE:
      ret = pBIO->shutdown;
      break;

    case BIO_CTRL_SET_CLOSE:
      pBIO->shutdown = ACE_static_cast (int, num);
      break;

    case BIO_CTRL_PENDING:
    case BIO_CTRL_WPENDING:
      ret = 0;
      break;

    case BIO_CTRL_DUP:
    case BIO_CTRL_FLUSH:
      ret = 1;
      break;

    default:
      ret = 0;
      break;
  }

  return ret;
}


int
ACE_Asynch_BIO_puts (BIO *pBIO, const char *str)
{
  int n = ACE_OS::strlen (str);

  return ACE_Asynch_BIO_write (pBIO, str, n);
}

#endif  /* OPENSSL_VERSION_NUMBER > 0x0090581fL && (ACE_WIN32 ||
           ACE_HAS_AIO_CALLS) */
