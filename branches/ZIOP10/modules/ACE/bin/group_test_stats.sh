unlink tmp
unlink tmp2
cat t.txt | grep '+[a-z|A-Z]' > tmp
sort tmp | uniq -c > tmp2
sort tmp2 > uniq.txt
unlink tmp
unlink tmp2
cat uniq.txt
