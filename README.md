# minishell



## ?!?!crash list?!?! :
---- [🙄] builtin | [whatever]<br /><br />
---- [✅] echo -n {arg} / echo -n<br /><br />
---- [🙄] les guillemets deuxieme arg est split<br /><br />
---- [⏩] heredoc ctrlc... sigals<br /><br />
---- [✅] cat << EOF1 << EOF2<br /><br />
---- [✅] file redir do not create all files <br /><br />
---- [✅] export salutand all export... salut=123<br /><br />
---- [❌] ctrl c supprime la ligne pour l'instant les command sans \n marchent pas<br /><br />
---- [❌] !!!last exit!!!<br /><br />
---- [❌] builtins return<br /><br />
---- [✅] $?<br /><br />
---- [✅] ~ is expanded with getenv HOME<br /><br />
---- [✅] echo $ echo $ echo "$"<br /><br />
---- [✅] env -i and launch. TELLS THERE IS NO PATH AND EXIT<br /><br />
---- [❌] export salut="|"  ls $salut the pipe is a ls arg, not a pipe (we shurely wont do that----<br /><br />
---- [ ] ctrl cd\ after an infinite command   
## GENERAL TODO LIST :
---- variable ... variable ... variable ... VARIABLE ... VVVVVVVV <br /><br />
---- Simple/Double quote DONE (WE WILL SEE) <br /><br />
---- free everything <br /><br />
---- environement variable dollar{..} dollar?
---- between each command reparse bash

## MALLOC LIST :

list of every malloc done for now (usefull to make sure they are set to null at begining)

{animations}

animation.c
   (0) logo is malloced but normally freed in there
explo_anim.c
   (0) explo is malloced but normally freed in there

{gnl}

get_next_line_bonus.c
   (0) the static is not freed (classic)

{builtins}

builtins.c
    0
builtins_utils.c
    0
cd.c
    0
echo.c
    0
env.c
    0
exit.c
    0
export.c
    var_name with a lot of mallocs. To check.
    same for var_value
export_2.c
   (0) there is the copy of env (**dest) but it is freed in there
pwd.c
    0
unset.c
    0

{other}

error_free.c
    0 here we will free
exec.c
    TBD
free.c
    0 here we will free
handle_signal.c
    0
init.c
    here we malloc structure path and env
minishell.c
    input shell prompt others??
parsing.c
    TBD
quote.c
    (0) a lot of mallocs but freed in there
setup.c
    (0) the malloc in there are already handeled in other files
update_env.c
    (0) are these malloc mandatory?

## BUILTINS

### cd  DONE
  works with <br />
  "cd" "cd ~" "cd {absolute and relative path}" <br />
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
