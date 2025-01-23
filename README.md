# minishell

## GENERAL TODO LIST

----MAKE A FKING GIT IGNORE <br /><br />
----full path binary "/bin/ls" <br /><br />
----variable "salut=54" "salut=54 ls" gives only the var to ls <br /><br />
----Simple/Double quote <br /><br />
----pipex <br /><br />
----give the right token from parsing <br /><br />
----free everything <br /><br />
----environement variable dollar{..} dollar? 

## BUILTINS

### cd  DONE
  works with <br />
  ----"cd" "cd ~" "cd {absolute and relative path}" <br />
  ----"cd -" works perfectly<br />
  does not work with <br />
  ---- "cd -L" "cd -P" but the parsing in done for all of that <br />
  ---- there is a big chance we won't do that but it's here
  
### pwd DONE
  works with <br />
  ----no args "pwd" <br />
  does not work with <br />
  ----"pwd -L" "pwd -P" but the parsing is done for all or that <br />
  ----we normally won't do that

### env DONE
  works with <br />
  ----no args "env" <br />
  does not work with <br />
  ----there is a lot of possibilities with env but nothing mandatory --> we will see

### echo
  works with  <br />
  ----"echo {number of things to write}" "echo -n {...}" <br />
  does not work with <br />
  ----"echo" --> segfault "echo -n" --> segfault <br />
  ----"echo -e {...}" --> not done yet but it is useless before we hande '' and "" <-- not done yet

### export
  ----NOT DONE YET
  
### unset DONE
  works with <br />
  ----"unset" "unset -v" <br />
  does not work with <br />
  ----"unset -f" <br />
  ----I think we won't do that
  
### exit DONE
  works with <br />
  ----"exit" "exit 1-MAXINT" (modulo 256) <br />
  does not work with <br />
  ----? nothing for now :)
