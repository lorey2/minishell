# minishell

## GENERAL TODO LIST

----variable ... variable ... variable ... VARIABLE ... VVVVVVVV <br /><br />
----Simple/Double quote <br /><br />
----free everything <br /><br />
----environement variable dollar{..} dollar? 
----we can have tons of fun expanding variable :)))))))))))
-----------------------------------------------------------

BASIQUE
$ USER="lorey"
$ ${USER}
bash: ${USER}: command not found

LONGUEUR DE LA VARIABLE
$ STRING="hello"
$ ${#STRING}
5

DECOUPER UN STRING
$ STRING="lorey123"
$ ${STRING:5:3}
123

SETUP UNE VARIABLE AVEC UNE AURTE VARIABLE
$ HOME=$(pwd)
$ ${HOME}
/home/lorey

FAIRE DES OPERATION ARITHMETIQUES
$ ((result = 2 + 3))
$ echo $((result))
5

-----------------------------------------------------------

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
