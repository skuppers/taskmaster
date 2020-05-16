# Taskmaster

Taskmaster is a client/server project for 42 school.

It allows its users to control a number of processes on UNIX-like operating systems.

Heavily inspired by : [Supervisor](https://github.com/Supervisor/supervisor)

Parsing of configuration files is done with: [Iniparser](https://github.com/ndevilla/iniparser)


## Supported Platforms

Taskmaster has been tested on ubuntu 16.04, ubuntu 18.04 and ubuntu 19.10

## Building

``$> git clone https://github.com/skuppers/taskmaster.git ``

``$> cd taskmaster; git submodule init && git submodule update``

  
   ``$> make``
   
   or
   
   ``$> make d=3 -j4``
   
   

## Running Taskmasterd and Taskmasterctl
The taskmaster project contains two binaries: `taskmasterd` and `taskmasterctl`

`taskmasterd` is the daemon which runs in background as default, and handles all the job control specified in the configuration file.

`taskmasterctl` is the console ( or client ) which permits you to communicate with the daemon.

A minimalist configuration file which works:
```[taskmasterctl]
serverurl = /tmp/taskmaster.d/taskmasterd.sock
prompt = skumasterd

[taskmasterd]
logfile = /tmp/taskmaster.d/taskmaster.log
loglevel = info
nodaemon = true
umask = 022
childlogdir = /tmp/taskmaster.d/childlog/

[program.ping]
command=ping 8.8.8.8
numprocs=2
autostart=true
autorestart=false

[program.tail]
command=tail -f /tmp/taskmaster.d/taskmaster.log
numprocs=2
startsecs=1
autostart=true
autorestart=false
```


## Documentation

You can view the current Supervisor documentation online [in HTML format](http://supervisord.org/) .

This is where you should go for detailed installation and configuration documentation.


## Configuration file keywords

List of supported keywords in configuration file:
### [taskmasterctl]

|                |Description                          |Not specified / Default                         |
|----------------|-------------------------------|-----------------------------|
|serverurl    |`path to server socket`            |`/tmp/taskmaster.d/taskmasterd.sock`
|prompt          |`prompt display message`            |`taskmasterd>          `  |


### [taskmasterd]
|                |Description                          |Not specified / Default                         |
|----------------|-------------------------------|-----------------------------|
|logfile    |`path to daemon logfile`            |`/tmp/taskmaster.d/taskmaster.log`
|loglevel          |`log display filter`            |`info          `  |
|nodaemon    |`do not daemonize on startup`            |`false`
|umask          |`umask to apply`            |`022          `  |
|userid    |`userid to apply`            |`do not change user`
|directory          |`change to this directory on startup`            |`do not change directory          `  |
|childlogdir          |`child processes log directory`            |`/tmp/taskmaster.d/childlog/>          `  |
|environment          |`Additional environment`	| `empty` |


### [program]
|                |Description                          |Not specified / Default                         |
|----------------|-------------------------------|-----------------------------|
|command    |`command to execute`            |`Must be specified`
|numprocs          |`number of instances to run`            |`1          `  |
|directory    |`change to this directory on startup`            |`do not change directory`
|umask          |`umask to apply`            |`022          `  |
|userid          |`userid to apply`	| `do not change user` |
|autostart    |`autostart program`            |`false`
|autorestart          |`auto restart program`            |`UNEXPECTED          `  |
|startsec          |`delay for marking process as running`            |`1 (second)          `  |
|startretries          |`Number of restarts to try before FATAL state`	| `3` |
|exitcodes          |`Explicit exitcodes; Used with "autorestart=UNEXPECTED"`	| `0` |
|stopsignal          |`Signal to send to gracefully terminate instance`	| `SIGTERM` |
|stopwaitsec          |`delay before killing stung process`	| `3 (seconds)` |
|stderr_logfile          |`Explicit instance stderr logfile`	| `AUTO` |
|stdout_logfile          |`Explicit instance stdout logfile`	| `AUTO` |
|environment          |`Additional environment`	| `empty` |

