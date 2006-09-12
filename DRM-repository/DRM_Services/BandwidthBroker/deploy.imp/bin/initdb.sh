sudo mysql -u imp -prondo < $ARMS_ROOT/bin/impNet.sql
sudo echo "impNet created"
sudo mysql -u imp -prondo < $ARMS_ROOT/bin/imp.sql
sudo echo "imp created"
sudo mysql -u imp -prondo < $ARMS_ROOT/bin/impNetOnly.sql
sudo echo "impNet populated"

