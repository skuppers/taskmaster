# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/03/11 17:23:00 by ffoissey          #+#    #+#              #
#    Updated: 2020/05/14 17:35:12 by ffoissey         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
################################                ################################
################################ MAIN VARIABLES ################################
################################                ################################
################################################################################

SHELL = /bin/sh
.SUFFIXES:

# Name client
CLIENT = taskmasterctl
DAEMON = taskmasterd

# Compiler
CC = clang

# Lib
PATH_LIBINI = libini/
LIBINI = $(PATH_LIBINI)libiniparser.a
PATH_LIBFT = libft/
LIBFT = $(PATH_LIBFT)libft.a

# Compiler Flags
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -D _GNU_SOURCE
#CFLAGS += -D _POSIX_C_SOURCE=200809L

# Compiler Debug Flags
ifeq ($(d), 0)
	CFLAGS += -g3
else ifeq ($(d), 1)
	CFLAGS += -g3
	CFLAGS += -fsanitize=address,undefined
else ifeq ($(d), 2)
	CFLAGS += -g3
	CFLAGS += -fsanitize=address,undefined
	CFLAGS += -Wpadded
else ifeq ($(d), 3)
	CFLAGS += -g3
	CFLAGS += -fsanitize=address,undefined
	CFLAGS += -Wpadded
	CFLAGS += -pedantic
else ifeq ($(d), 4)
	CFLAGS += -g3
	CFLAGS += -fsanitize=address,undefined
	CFLAGS += -Wpadded
	CFLAGS += -pedantic
	CFLAGS += -Weverything
endif
ifneq ($(err), no)
	CFLAGS += -Werror
endif

# Debug Dir
DSYM += $(NAME).dSYM
DSYM += $(DBNAME).dSYM

################################################################################
################################                ################################
################################   PRINT VAR    ################################
################################                ################################
################################################################################

# Reset
NC = \033[0m

# Regular Colors
BLACK = \033[0;30m
RED = \033[0;31m
GREEN = \033[32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m

# One Line Output
ONELINE =\e[1A\r

################################################################################
#################################               ################################
#################################    INCLUDES   ################################
#################################               ################################
################################################################################

INCLUDES_LIBFT = libft/includes/
INCLUDES_LIBINI = libini/src/
INCLUDES_TASKMASTER = includes/

I_LIBINIINC += -I $(INCLUDES_LIBINI)
I_INCLUDES += -I $(INCLUDES_LIBFT)
I_INCLUDES += -I $(INCLUDES_TASKMASTER)

################################################################################
#################################               ################################
#################################    HEADERS    ################################
#################################               ################################
################################################################################

vpath %.h $(INCLUDES_LIBFT)
vpath %.h $(INCLUDES_TASKMASTER)

# libft
HEADER += libft.h

# Common
HEADER += common.h
# client_taskmaster
HEADER += client_taskmaster.h
# deaemon taskmaster
HEADER += daemon_taskmaster.h

################################################################################
#################################               ################################
#################################  PATH SOURCES ################################
#################################               ################################
################################################################################

PATH_CLIENT_SRCS += srcs/client/
PATH_CLIENT_SRCS += srcs/client/builtin/
PATH_CLIENT_SRCS += srcs/client/builtin/help_function/
PATH_CLIENT_SRCS += srcs/client/completion/
PATH_CLIENT_SRCS += srcs/client/history/
PATH_CLIENT_SRCS += srcs/client/init/
PATH_CLIENT_SRCS += srcs/client/line_edition/
PATH_CLIENT_SRCS += srcs/client/routine/
PATH_CLIENT_SRCS += srcs/client/transfert/

PATH_DAEMON_SRCS += srcs/daemon/
PATH_DAEMON_SRCS += srcs/daemon/builtin/
PATH_DAEMON_SRCS += srcs/daemon/env_var/
PATH_DAEMON_SRCS += srcs/daemon/exit/
PATH_DAEMON_SRCS += srcs/daemon/init/
PATH_DAEMON_SRCS += srcs/daemon/init/iniparser/
PATH_DAEMON_SRCS += srcs/daemon/init/iniparser/getter/
PATH_DAEMON_SRCS += srcs/daemon/jobs/
PATH_DAEMON_SRCS += srcs/daemon/log/
PATH_DAEMON_SRCS += srcs/daemon/transfert/

PATH_COMMON_SRCS += srcs/common/

################################################################################
#################################               ################################
#################################    SOURCES    ################################
#################################               ################################
################################################################################

### CLIENT

CLIENT_SRCS += client_taskmaster.c
#Init
CLIENT_SRCS += init.c
CLIENT_SRCS += print_help.c
CLIENT_SRCS += opt.c
CLIENT_SRCS += load_config.c
CLIENT_SRCS += client_signal.c
CLIENT_SRCS += termmode.c
#History
CLIENT_SRCS += history.c
#Completion
CLIENT_SRCS += completion.c
CLIENT_SRCS += print_completion.c
CLIENT_SRCS += completion_tools.c
CLIENT_SRCS += completion_prog.c
CLIENT_SRCS += get_completion_type.c
#Line Edition
CLIENT_SRCS += readline.c
CLIENT_SRCS += keycodes.c
CLIENT_SRCS += actionkeys.c
CLIENT_SRCS += window.c
CLIENT_SRCS += dlrkeys.c
CLIENT_SRCS += specialkeys.c
CLIENT_SRCS += readline_tools.c
CLIENT_SRCS += readline_utils.c
CLIENT_SRCS += sigcatcher.c
CLIENT_SRCS += addchar.c
#Routine
CLIENT_SRCS += get_cmd.c
CLIENT_SRCS += exec_routine.c
CLIENT_SRCS += exec_tail.c
CLIENT_SRCS += exit_routine.c
#Transfert
CLIENT_SRCS += ctransfert.c
#Builtin
CLIENT_SRCS += add.c
CLIENT_SRCS += avail.c
CLIENT_SRCS += clear.c
CLIENT_SRCS += exit.c
CLIENT_SRCS += fg.c
CLIENT_SRCS += help.c
CLIENT_SRCS += maintail.c
CLIENT_SRCS += open.c
CLIENT_SRCS += pid.c
CLIENT_SRCS += quit.c
CLIENT_SRCS += reload.c
CLIENT_SRCS += remove.c
CLIENT_SRCS += reread.c
CLIENT_SRCS += restart.c
CLIENT_SRCS += signal.c
CLIENT_SRCS += shutdown.c
CLIENT_SRCS += start.c
CLIENT_SRCS += status.c
CLIENT_SRCS += stop.c
CLIENT_SRCS += tail.c
CLIENT_SRCS += update.c
CLIENT_SRCS += version.c
#Builtin/help
CLIENT_SRCS += help_function_a_f.c
CLIENT_SRCS += help_function_h_q.c
CLIENT_SRCS += help_function_r.c
CLIENT_SRCS += help_function_s.c
CLIENT_SRCS += help_function_t_v.c

### DAEMON

DAEMON_SRCS += daemon_taskmaster.c
#Builtin
DAEMON_SRCS += cmd_signal.c
DAEMON_SRCS += cmd_stop.c
DAEMON_SRCS += cmd_start.c
DAEMON_SRCS += cmd_restart.c
DAEMON_SRCS += cmd_status.c
DAEMON_SRCS += cmd_status_completion.c
DAEMON_SRCS += cmd_maintail.c
DAEMON_SRCS += cmd_tail.c
DAEMON_SRCS += cmd_add.c
DAEMON_SRCS += cmd_avail.c
DAEMON_SRCS += cmd_version.c
DAEMON_SRCS += cmd_clear.c
DAEMON_SRCS += cmd_fg.c
DAEMON_SRCS += cmd_pid.c
DAEMON_SRCS += cmd_reload.c
DAEMON_SRCS += cmd_remove.c
DAEMON_SRCS += cmd_reread.c
DAEMON_SRCS += cmd_reread_checker.c
DAEMON_SRCS += cmd_reread_tools.c
DAEMON_SRCS += cmd_reread_register.c
DAEMON_SRCS += cmd_shutdown.c
DAEMON_SRCS += cmd_update.c
DAEMON_SRCS += cmd_update_tools.c
DAEMON_SRCS += execute_cmd.c
DAEMON_SRCS += getter.c
DAEMON_SRCS += getter_tools.c
#Env_var
DAEMON_SRCS += env_var_manager.c
DAEMON_SRCS += env_var_free.c
DAEMON_SRCS += env_var_tools.c
#Exit
DAEMON_SRCS += exit_routine_d.c
DAEMON_SRCS += free.c
#Init
DAEMON_SRCS += help_d.c
DAEMON_SRCS += init_daemon.c
DAEMON_SRCS += daemonize.c
DAEMON_SRCS += opt_d.c
DAEMON_SRCS += daemon_signals.c
#Init/iniparser
DAEMON_SRCS += defaults.c
DAEMON_SRCS += config_parser.c
DAEMON_SRCS += iniparser_tools.c
DAEMON_SRCS += get1.c
DAEMON_SRCS += get2.c
DAEMON_SRCS += get3.c
#Jobs
DAEMON_SRCS += jobs.c
DAEMON_SRCS += child.c
DAEMON_SRCS += waiter.c
DAEMON_SRCS += instance_utils.c
DAEMON_SRCS += instance_tools.c
#Log
DAEMON_SRCS += print_starting_debug.c
DAEMON_SRCS += log.c
#Transfert
DAEMON_SRCS += sockets.c
DAEMON_SRCS += dtransfert.c
DAEMON_SRCS += listen_for_data.c

### COMMON

COMMON_SRCS += decode_cmd.c
COMMON_SRCS += generate_bytecode.c
COMMON_SRCS += cmd.c
COMMON_SRCS += transfert.c
COMMON_SRCS += path_tools.c

################# ATTRIBUTION

vpath %.c $(PATH_CLIENT_SRCS)
vpath %.c $(PATH_DAEMON_SRCS)
vpath %.c $(PATH_COMMON_SRCS)

################################################################################
#################################               ################################
#################################     OBJS      ################################
#################################               ################################
################################################################################

PATH_OBJS = objs/
CLIENT_OBJS = $(patsubst %.c, $(PATH_OBJS)%.o, $(CLIENT_SRCS))
DAEMON_OBJS = $(patsubst %.c, $(PATH_OBJS)%.o, $(DAEMON_SRCS))
COMMON_OBJS = $(patsubst %.c, $(PATH_OBJS)%.o, $(COMMON_SRCS))

################################################################################
#################################               ################################
#################################     RULES     ################################
#################################               ################################
################################################################################

#---------------------------------- STANDARD ----------------------------------#

all: $(CLIENT) $(DAEMON)

$(CLIENT): $(LIBINI) $(LIBFT) $(PATH_OBJS) $(CLIENT_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) $(I_INCLUDES) $(I_LIBINIINC) $(CLIENT_OBJS) $(COMMON_OBJS) \
		$(LIBFT) $(LIBINI) -o $@
	printf "$(GREEN)taskmasterctl is ready.\n$(NC)"

$(CLIENT_OBJS): $(PATH_OBJS)%.o: %.c $(HEADER) Makefile
	$(CC) $(CFLAGS) $(I_INCLUDES) $(I_LIBINIINC) -c $< -o $@
	printf "$(ONELINE)$(CYAN)Compiling $<"
	printf "                                                            \n$(NC)"

$(DAEMON): $(LIBINI) $(LIBFT) $(PATH_OBJS) $(DAEMON_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) $(I_INCLUDES) $(I_LIBINIINC) $(DAEMON_OBJS) $(COMMON_OBJS) \
		$(LIBFT) $(LIBINI) -o $@
	printf "$(GREEN)taskmasterd is ready.\n$(NC)"

$(DAEMON_OBJS): $(PATH_OBJS)%.o: %.c $(HEADER) Makefile
	$(CC) $(CFLAGS) $(I_INCLUDES) $(I_LIBINIINC) -c $< -o $@
	printf "$(ONELINE)$(CYAN)Compiling $<"
	printf "                                                            \n$(NC)"

$(COMMON_OBJS): $(PATH_OBJS)%.o: %.c $(HEADER) Makefile
	$(CC) $(CFLAGS) $(I_INCLUDES) $(I_LIBINIINC) -c $< -o $@
	printf "$(ONELINE)$(CYAN)Compiling $<"
	printf "                                                            \n$(NC)"

$(PATH_OBJS):
	mkdir $@

$(LIBFT): FORCE 
	$(MAKE) -C $(PATH_LIBFT)

$(LIBINI): FORCE
	$(MAKE) -C $(PATH_LIBINI)

#--------------------------------- Basic Rules --------------------------------#

clean:
	$(RM) -R $(PATH_OBJS)
	$(RM) -R $(DSYM)
	$(MAKE) -C $(PATH_LIBFT) clean
#	$(MAKE) -C $(PATH_LIBINI) clean
	printf "$(RED)Objs from taskmasterctl removed\n$(NC)"
	printf "$(RED)Objs from taskmasterd removed\n$(NC)"

fclean: clean
	$(RM) $(CLIENT) $(DAEMON)
	$(MAKE) -C $(PATH_LIBFT) fclean
	printf "$(RED)taskmasterctl removed\n$(NC)"
	printf "$(RED)taskmasterd removed\n$(NC)"

re: fclean
	$(MAKE)

FORCE:

#----------------------------------- Special ----------------------------------#

.PHONY: clean fclean re all FORCE
.SILENT:
