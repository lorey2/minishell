# minishell

## GENERAL TODO LIST

----MAKE A FKING GIT IGNORE <br /><br />
----Simple/Double quote <br /><br />
----pipex <br /><br />
----give the right token from parsing <br /><br />
----free everything<br /><br />
----environement variable dollar{..} dollar?

## BUILTINS

### cd  DONE
  works with <br />
  ----"cd" "cd ~" "cd {absolute and relative path}" <br />
  ----"cd -" works perfectly<br />
  does not work with <br />
  ---- "cd -L" "cd -P" but the parsing in done for all of that <br />
  ---- there is a big chance we wont do that but it's here
  
### pwd DONE
  works with <br />
  ----no args "pwd" <br />
  does not work with <br />
  ----"pwd -L" "pwd -P" but the parsing is done for all or that <br />

### env DONE
  works with <br />
  ----no args "env" <br />
  does not work with <br />
  ----there is a lot of possibilities with env but nothing mandatory --> we will see

### echo
  works with  <br />
  ----"echo {number of things to write}" "echo -n {...}" BUT SPACE ARE NOT MADE BETWEEN WORDS FOR NOW <br />
  does not work with <br />
  ----SPACE ARE NOT MADE BETWEEN WORDS FOR NOW, "echo" --> segfault "echo -n" --> segfault <br />
  ----"echo -e {...}" --> not done yet but it is useless before we hande '' and "" <-- not done yet

  ### export
  ----NOT DONE YET
  ### unset
  ----NOT DONE YET
  ### exit
  ----NOT DONE YET
