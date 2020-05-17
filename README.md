## Team
>[ffoissey](https://github.com/ffoissey)

>[skuppers](https://github.com/skuppers)

# Taskmaster

Taskmaster is a job control manager project for 42 school.

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
   
   ``$> make -j4``
   
   

## Running Taskmasterd and Taskmasterctl
The taskmaster project contains two binaries: `taskmasterd` and `taskmasterctl`

`taskmasterd` is the daemon which runs in background as default, and handles all the job control specified in the configuration file.

`taskmasterctl` is the console ( or client ) which permits you to communicate with the daemon.

### Daemon
![image](https://user-images.githubusercontent.com/29956389/82141534-ba54d880-9836-11ea-963b-1677c1bfa2e3.png)
### Client
![image](https://user-images.githubusercontent.com/29956389/82141525-aa3cf900-9836-11ea-8cc1-84f002c06569.png)

You can enumerate all commands in the taskmasterctl program:

![image](https://user-images.githubusercontent.com/29956389/82123222-78755500-9798-11ea-992e-509a72d1d8a9.png)


# Configuration

A minimalist configuration file:
```
[taskmasterctl]
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
```

Supported keywords in configuration file:
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

### Diagram
#### Job control State machine
![image](https://user-images.githubusercontent.com/29956389/82141186-719c2000-9834-11ea-95fb-4c17e7c9036b.png)

A process is in the STOPPED state if it has been stopped adminstratively or if it has never been started.

When an autorestarting process is in the BACKOFF state, it will be automatically restarted by taskmasterd. It will switch between STARTING and BACKOFF states until it becomes evident that it cannot be started because the number of startretries has exceeded the maximum, at which point it will transition to the FATAL state.

When a process is in the EXITED state, it will automatically restart:

- never if its autorestart parameter is set to false.
- unconditionally if its autorestart parameter is set to true.
- conditionally if its autorestart parameter is set to unexpected. If it exited with an exit code that doesn’t match one of the exit codes defined in the exitcodes configuration parameter for the process, it will be restarted.

A process automatically transitions from EXITED to RUNNING as a result of being configured to autorestart conditionally or unconditionally. The number of transitions between RUNNING and EXITED is not limited in any way: it is possible to create a configuration that endlessly restarts an exited process. This is a feature, not a bug.

An autorestarted process will never be automatically restarted if it ends up in the FATAL state (it must be manually restarted from this state).

A process transitions into the STOPPING state via an administrative stop request, and will then end up in the STOPPED state.

A process that cannot be stopped successfully will stay in the STOPPING state forever. This situation should never be reached during normal operations as it implies that the process did not respond to a final SIGKILL signal sent to it by supervisor, which is “impossible” under UNIX.


#### Unix socket communication

Communication between the daemon and the controller is done using unix sockets.
No protocol was used, so an application layer encapsulation was made:

![image](https://user-images.githubusercontent.com/29956389/82141715-fdfc1200-9837-11ea-8200-6bd9b971ae51.png)

![image](https://user-images.githubusercontent.com/29956389/82141789-782c9680-9838-11ea-9dcc-8d5a1d6f8eb1.png)

![image](https://user-images.githubusercontent.com/29956389/82123059-8bd3f080-9797-11ea-8a20-ad15b1051390.png)
