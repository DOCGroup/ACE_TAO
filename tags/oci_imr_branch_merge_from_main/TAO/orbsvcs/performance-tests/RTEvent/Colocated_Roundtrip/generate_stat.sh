#/bin/sh

for i in $*; do
  echo $i
  cat $i |
  awk 'BEGIN {
           s1 = 0;
           s2 = 0;
           s3 = 0;
           s4 = 0;
           n  = 0;
         }
         {
	   x = $1;
	   p = $2;
           v = p * x;
           s1 += v;
	   v *= x;
           s2 += v;
	   v *= x;
           s3 += v;
	   v *= x;
           s4 += v;
         }
         END {
	   up0 = 1;
	   up1 = s1;
	   up2 = s2;
	   up3 = s3;
	   up4 = s4;
	   u1 = 0;
	   u2 = up2 - up1 * up1;
	   u3 = 2 * up1 * up1 * up1 - 3 * up1 * up2 + up3;
	   u4 = -3 * up1 * up1 * up1 * up1 + 6 * up1 * up1 * up2
	        -4 * up1 * up3 + up4;

	   skew  = u3 / (u2 * sqrt(u2));
	   kurto = u4 / (u2 * u2) - 3;
           printf("%f %f %f %f\n",
                  up1, u2, skew, kurto);
         }'
done
