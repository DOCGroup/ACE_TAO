sudo pkill mysqld 
sleep 4
sudo cp /proj/arms/etc/imp/my.cnf /etc/my.cnf
sudo /usr/local/bin/mysqld_safe >/dev/null 2>&1  &
