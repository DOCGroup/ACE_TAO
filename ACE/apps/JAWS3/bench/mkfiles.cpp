#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <array>
#include <memory>

#include "ace/Get_Opt.h"

static float gammln (float xx);
static float poidev (float xm);

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt options (argc, argv, ACE_TEXT("m:s:x:n:"));
  // m -- median file size in kB
  // x -- maximum file size in kB
  // n -- number of files

  long median {8};
  long maximum {1024};
  long number {1000};

  int c;
  while ((c = options ()) != -1)
    {
      switch (c)
        {
        case 'm':
          median = std::atoi (options.optarg);
          break;
        case 'x':
          maximum = std::atoi (options.optarg);
          break;
        case 'n':
          number = std::atoi (options.optarg);
          break;
        default:
          break;
        }
    }

  std::array<char, 1024> filename {};
  char const * const seventyfive_bytes = "\
01010101010101010101010101010101010101010101010101010101010101010101010101\n\
";

  bool seen_max {};

  for (long i = 0; i < number; ++i)
    {
      float p = std::floor (std::pow (2, poidev (std::log (2 * median) / std::log (2)) - 1));
      if (p > maximum)
        p = maximum;
      p *= 1024;
      if (p < 1.0)
        p = 1.0;
      long size = static_cast<long> (p);
      if (i == number - 1)
        {
          if (!seen_max)
            size = maximum * 1024;
        }
      else
        seen_max = (size == (maximum * 1024));

      int const filename_length = std::snprintf (
        filename.data (), filename.size (), "file%011ld.html", i);
      if (filename_length < 0
          || filename_length >= static_cast<int> (filename.size ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Generated filename is too long\n"), 1);

      std::unique_ptr<FILE, int (*) (FILE *)> fp (
        std::fopen (filename.data (), "w+b"), std::fclose);
      if (!fp)
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output file %s\n", filename.data ()), 1);

      while (size > 75)
        {
          std::fprintf (fp.get (), "%s", seventyfive_bytes);
          size -= 75;
        }
      if (size > 15)
        {
          std::fprintf (fp.get (), "%0*.0f\n", static_cast<int> (size - 1), p);
        }
      else
        {
          std::fprintf (fp.get (), "%015.0f\n", p + 16 - size);
        }
    }

  return 0;
}

static float
gammln (float xx)
{
  double const x = xx;
  double y = x;
  double tmp = x + 5.5;
  double ser = 1.000000000190015;
  constexpr std::array<double, 6> cof {
    76.18009172947146,
    -86.50532032941677,
    24.01409824083091,
    -1.231739572450155,
    0.1208650973866179e-2,
    -0.5395239384953e-5
  };
  tmp -= (x + 0.5) * std::log (tmp);
  for (double const coefficient : cof)
    ser += coefficient / ++y;

  return -tmp + std::log (2.5066282746310005 * ser / x);
}

static float
poidev (float xm)
{
  constexpr double PI = 3.141592654;
  static float sq {};
  static float alxm {};
  static float g {};
  static float oldm {-1.0f};
  float em {};
  float t {};
  float y {};
  float fem {};

  if (xm < 2.0)
    {
      if (xm != oldm)
        {
          oldm = xm;
          g = std::exp (-xm);
        }
      em = -1.0;
      t = 1.0;
      do
        {
          ++em;
          t *= (1.0 + ACE_OS::rand ())/RAND_MAX;
        }
      while (t > g);
    }
  else
    {
      if (xm != oldm)
        {
          oldm = xm;
          sq = std::sqrt (2.0 + xm);
          alxm = std::log (xm);
          g = xm * alxm - gammln (xm + 1.0);
        }
      do
        {
          do
            {
              y = std::tan (PI * (1.0 + ::rand ())/RAND_MAX);
              em = sq * y + xm;
            }
          while (em < 0.0);
          fem = std::floor (em);
          t = 0.9 * (1.0 + y * y) * std::exp (fem * alxm - gammln (fem + 1.0) - g);
        }
      while ((1.0 + ACE_OS::rand ())/RAND_MAX > t);
    }

  return em;
}
