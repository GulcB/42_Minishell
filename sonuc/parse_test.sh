>> MINISHELL:

@GulcB ➜ /workspaces/42_Minishell (main) $ ./minishell 
minishell> /bin/echo ""$?""
0
minishell> /bin/echo $?"42"
042
minishell> /bin/echo ''$?''"42"
042
minishell> /bin/echo '"$USER"'
"$USER"
minishell> /bin/echo "'$USER'"
'codespace'
minishell> /bin/echo ""'$USER'""
$USER
minishell> /bin/echo '"'$USER'"'
"codespace"
minishell> /bin/echo "''$USER''"
''codespace''
minishell> /bin/echo "'"'$USER'"'"
'$USER'
minishell> /bin/echo '"'"$USER"'"'
"codespace"
minishell> /bin/echo $"HOME"$USER
HOMEcodespace
minishell> /bin/echo $"HOM"E$USER
HOMEcodespace
minishell> /bin/echo "exit_code ->$? user ->$USER home -> $HOME"
exit_code ->0 user ->codespace home -> /home/codespace
minishell> /bin/echo $"HOME"
HOME
minishell> /bin/echo $"42$"
42$
minishell> /bin/echo "$ "
$ 
minishell> /bin/echo hi >./outfiles/outfile01 | /bin/echo bye
bye
minishell> <123 <456 hi | /bin/echo 42
minishell: unsupported node type
minishell> /bin/echo '$='
$=
minishell> /bin/echo '$ '
$ 
minishell> /bin/echo "$?"
0
minishell> /bin/echo '$?'
$?
minishell> /bin/echo "'$?'"
'0'
minishell> /bin/echo \$USER
$USER
minishell> /bin/echo \\$USER
\ codespace
minishell> /bin/echo \\\$USER
\$USER
minishell> /bin/echo \\\\$USER
\\ codespace
minishell> /bin/echo \\\\\$USER
\\$USER
minishell> /bin/echo \\\\\\\\\$USER
\\\\$USER
minishell> /bin/echo \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\$USER \$PATH \\$PWD
\\\\\\\\\\\\\\\\$USER $PATH \ /workspaces/42_Minishell
minishell> /bin/echo "cat lol.c | cat > lol.c"
cat lol.c | cat > lol.c
minishell> /bin/echo "cat lol.c '|' cat > lol.c"
cat lol.c '|' cat > lol.c
minishell> /bin/echo '$USER' "$USER" "text  ' text"
$USER codespace text  ' text
minishell> /bin/echo $USER =intergalaktikus miaf*szomez
codespace =intergalaktikus miaf*szomez
minishell> /bin/echo -n"-n" bonjour
-n-n bonjour
minishell> /bin/echo "'$USER'"
'codespace'
minishell> /bin/echo " '$USER' "
 'codespace' 
minishell> /bin/echo text"$USER"
textcodespace
minishell> /bin/echo text"'$USER'" ' $USER '
text'codespace'  $USER 
minishell> /bin/echo "text"   "$USER"    "$USER"
text codespace codespace
minishell> /bin/echo '              $USER          '
              $USER          
minishell> /bin/echo ''''''''''$USER''''''''''
codespace
minishell> /bin/echo """"""""$USER""""""""
codespace
minishell> /bin/echo '""""""""$USER""""""""'
""""""""$USER""""""""
minishell> /bin/echo ""'""""""$USER""""""'""
""""""$USER""""""
minishell> /bin/echo """"""""'$USER'""""""""
$USER
minishell> /bin/echo """""""'"$USER"'"""""""
'codespace'
minishell> /bin/echo """"""'""$USER""'""""""
""$USER""
minishell> /bin/echo ""'""'""""$USER""""'""'""
""codespace""
minishell> /bin/echo '""'""'""""$USER""""'""'""'
""""""$USER""""""
minishell> /bin/echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER ''
codespace$USERtext oui oui oui oui codespace oui codespace 
minishell> /bin/echo "text" "text$USER" ... "$USER"
text textcodespace ... codespace
minishell> /bin/echo

minishell> /bin/echo ''

minishell> /bin/echo ""

minishell> /bin/echo '' ""
 
minishell> /bin/echo /bin/echo
/bin/echo
minishell> /bin/echo test1
test1
minishell> /bin/echo 'test1'
test1
minishell> /bin/echo "/bin/echo hallo"
/bin/echo hallo
minishell> /bin/echo /bin/echo"1"
/bin/echo1
minishell> /bin/echo "test1 test2"
test1 test2
minishell> /bin/echo "test1  test2" test3
test1  test2 test3
minishell> /bin/echo " test1 test2 " 'test3 '
 test1 test2  test3 
minishell> /bin/echo test1              test2
test1 test2
minishell> /bin/echo $USER$TESTNOTFOUND$HOME$
codespace/home/codespace$
minishell> /bin/echo $USER$TESTNOTFOUND$HOME$WTF$PWD
codespace/home/codespace/workspaces/42_Minishell
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
minishell> /bin/echo ~42
~42
minishell> /bin/echo -n -n -nasd
-nasdminish/bin/echo -n -n -n-nnnnnnnnnn
-n-nnnnnmin/bin/echo -n -nnnnnnn -n -nnn -nnnnn -n-nnnn -n-n
-n-nminishe/bin/echo -n -nnnnnnn -n -nnn -nnnnn -n feel my painpain
feel my pai/bin/echo -n -n -n-n
-n-nminishe/bin/echo "'totally logical'"al'"
'totally logical'
minishell> /bin/echo 'totally logical'
totally logical
minishell> /bin/echo ''totally logical''
totally logical
minishell> /bin/echo ""'totally logical'""
totally logical
minishell>


>> BASH:

@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo ""$?""
0
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo $?"42"
042
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo ''$?''"42"
042
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo '"$USER"'
"$USER"
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "'$USER'"
'codespace'
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo ""'$USER'""
$USER
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo '"'$USER'"'
"codespace"
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "''$USER''"
''codespace''
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "'"'$USER'"'"
'$USER'
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo '"'"$USER"'"'
"codespace"
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo $"HOME"$USER
HOMEcodespace
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo $"HOM"E$USER
HOMEcodespace
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "exit_code ->$? user ->$USER home -> $HOME"
exit_code ->0 user ->codespace home -> /home/codespace
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo $"HOME"
HOME
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo $"42$"
42$
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "$ "
$ 
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo hi >./outfiles/outfile01 | /bin/echo bye
bye
@GulcB ➜ /workspaces/42_Minishell (main) $ <123 <456 hi | /bin/echo 42
bash: 123: No such file or directory
42
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo '$='
$=
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo '$ '
$ 
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "$?"
0
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo '$?'
$?
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "'$?'"
'0'
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo \$USER
$USER
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo \\$USER
\codespace
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo \\\$USER
\$USER
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo \\\\$USER
\\codespace
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo \\\\\$USER
\\$USER
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo \\\\\\\\\$USER
\\\\$USER
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\$USER \$PATH \\$PWD
\\\\\\\\\\\\\\\\$USER $PATH \/workspaces/42_Minishell
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "cat lol.c | cat > lol.c"
cat lol.c | cat > lol.c
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "cat lol.c '|' cat > lol.c"
cat lol.c '|' cat > lol.c
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo '$USER' "$USER" "text  ' text"
$USER codespace text  ' text
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo $USER =intergalaktikus miaf*szomez
codespace =intergalaktikus miaf*szomez
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n"-n" bonjour
-n-n bonjour
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "'$USER'"
'codespace'
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo " '$USER' "
 'codespace' 
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo text"$USER"
textcodespace
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo text"'$USER'" ' $USER '
text'codespace'  $USER 
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "text"   "$USER"    "$USER"
text codespace codespace
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo '              $USER          '
              $USER          
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo ''''''''''$USER''''''''''
codespace
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo """"""""$USER""""""""
codespace
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo '""""""""$USER""""""""'
""""""""$USER""""""""
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo ""'""""""$USER""""""'""
""""""$USER""""""
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo """"""""'$USER'""""""""
$USER
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo """""""'"$USER"'"""""""
'codespace'
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo """"""'""$USER""'""""""
""$USER""
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo ""'""'""""$USER""""'""'""
""codespace""
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo '""'""'""""$USER""""'""'""'
""""""$USER""""""
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER ''
codespace$USERtext oui oui oui oui codespace oui codespace 
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "text" "text$USER" ... "$USER"
text textcodespace ... codespace
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo

@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo ''

@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo ""

@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo '' ""
 
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo /bin/echo
/bin/echo
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo test1
test1
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo 'test1'
test1
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "/bin/echo hallo"
/bin/echo hallo
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo /bin/echo"1"
/bin/echo1
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "test1 test2"
test1 test2
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "test1  test2" test3
test1  test2 test3
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo " test1 test2 " 'test3 '
 test1 test2  test3 
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo test1              test2
test1 test2
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo $USER$TESTNOTFOUND$HOME$
codespace/home/codespace$
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo $USER$TESTNOTFOUND$HOME$WTF$PWD
codespace/home/codespace/workspaces/42_Minishell
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n test1
test1@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n "test1 test2"
test1 test2@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n "test1 test2" test3
test1 test2 test3@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n " test1 test2 " 'test3 '
 test1 test2  test3 @GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n test1           test2
test1 test2@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo test1 -n
test1 -n
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "test1 -n"
test1 -n
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n -n test1
test1@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n -n -n -n -n test1
test1@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "  -nn "
  -nn 
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "-n test1 -n test2"
-n test1 -n test2
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "test1 -n test2"
test1 -n test2
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo ~42
~42
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n -n -nasd
-nasd@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n -n -n-nnnnn
-n-nnnnn@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n -nnnnnnn -n -nnn -nnnnn -n-n
-n-n@GulcB ➜ /workspaces/42_Minishell (main/bin/echo -n -nnnnnnn -n -nnn -nnnnn -n feel my painpain
feel my pain@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo -n -n -n-n
-n-n@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo "'totally logical'"
'totally logical'
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo 'totally logical'
totally logical
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo ''totally logical''
totally logical
@GulcB ➜ /workspaces/42_Minishell (main) $ /bin/echo ""'totally logical'""
totally logical
@GulcB ➜ /workspaces/42_Minishell (main) $ 
