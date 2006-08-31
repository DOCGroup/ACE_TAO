echo switch to category: Environment_Variables...\n
cd Environment_Variables

echo NameServicePort:\
show NameServicePort
echo \n

echo Set value to 12...\n
set NameServicePort 12
echo NameServicePort:\
show NameServicePort
echo \n

echo Value:\
write
echo \n

echo Clear the value...\n
clear NameServicePort
echo NameServicePort:\
show NameServicePort
echo \n

echo Value:\
write
echo \n

echo Define equality rule (NameServicePort = 3)...\n
define_equality_rule rule1 NameServicePort 3
echo Committing.\n
commit rule1
echo NameServicePort:\
show NameServicePort
echo \n

echo Value:\
write
echo \n

echo Define equality rule (ImplRepoServicePort=7  => TradingServicePort=13)...\n
define_equality_rule if_rule1 ImplRepoServicePort 7
define_equality_rule if_rule2 TradingServicePort 13
define_if_rule if_rule if_rule1 if_rule2
commit if_rule
apply

echo ImplRepoServicePort:\
show ImplRepoServicePort
echo \n
echo TradingServicePort:\
show TradingServicePort
echo \n

echo Setting ImplRepoServicePort=7...\n
set ImplRepoServicePort 7

echo Value:\
write
echo \n

echo ImplRepoServicePort:\
show ImplRepoServicePort
echo \n
echo TradingServicePort:\
show TradingServicePort
echo \n

echo Clearing ImplRepoServicePort...\n
clear ImplRepoServicePort

echo Value:\
write
echo \n

echo ImplRepoServicePort:\
show ImplRepoServicePort
echo \n
echo TradingServicePort:\
show TradingServicePort
echo \n

echo Setting ImplRepoServicePort=5...\n
set ImplRepoServicePort 5

echo ImplRepoServicePort:\
show ImplRepoServicePort
echo \n
echo TradingServicePort:\
show TradingServicePort
echo \n

echo Setting ImplRepoServicePort=7...\n
set ImplRepoServicePort 7

echo ImplRepoServicePort:\
show ImplRepoServicePort
echo \n
echo TradingServicePort:\
show TradingServicePort
echo \n

echo Value:\
write
echo \n

quit
