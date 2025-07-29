>> MINISHELL:

minishell> <123 <456 hi | /bin/echo 42
minishell: unsupported node type

minishell> /bin/echo \\$USER
\ codespace

minishell> /bin/echo \\\\$USER
\\ codespace

minishell> /bin/echo \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\$USER \$PATH \\$PWD
\\\\\\\\\\\\\\\\$USER $PATH \ /workspaces/42_Minishell

minishell> /bin/echo -n test1
test1minish/bin/echo -n "test1 test2"est2"
test1 test2/bin/echo -n "test1 test2" test3est2" test3
test1 test2/bin/echo -n " test1 test2 " 'test3 ' test2 " 'test3 '
 test1 test
minishell> /bin/echo -n " test1 test2 " 'test3 '
 test1 test/bin/echo -n test1           test2est1           test2
test1 test2/bin/echo test1 -nho test1 -n
test1 -n
minishell> /bin/echo "test1 -n"
test1 -n
minishell> /bin/echo -n -n test1
test1minish/bin/echo -n -n -n -n -n test1test1
test1minish/bin/echo "  -nn "-nn "
  -nn 
minishell> /bin/echo "-n test1 -n test2"
-n test1 -n test2
minishell> /bin/echo "test1 -n test2"
test1 -n test2
minishell> /bin/echo -n -n -nasd
-nasdminish/bin/echo -n -n -n-nnnnnnnnnn
-n-nnnnnmin/bin/echo -n -nnnnnnn -n -nnn -nnnnn -n-nnnn -n-n
-n-nminishe/bin/echo -n -nnnnnnn -n -nnn -nnnnn -n feel my painpain
feel my pai/bin/echo -n -n -n-n
