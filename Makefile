# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/03/11 17:23:00 by ffoissey          #+#    #+#              #
#    Updated: 2020/05/01 13:49:29 by ffoissey         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
################################                ################################
################################ MAIN VARIABLES ################################
################################                ################################
################################################################################

# Name client
CLIENT = taskmasterctl
DAEMON = taskmasterd

# Compiler
CC = clang

# Lib
PATH_LIBFT = libft/
LIBFT = $(PATH_LIBFT)libft.a

# Compiler Flags
CFLAGS += -Wall
CFLAGS += -Wextra

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
INCLUDES_TASKMASTER = includes/

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
PATH_CLIENT_SRCS += srcs/client/builtin/help_function
PATH_CLIENT_SRCS += srcs/client/history/
PATH_CLIENT_SRCS += srcs/client/completion/
PATH_CLIENT_SRCS += srcs/client/line_edition/

PATH_DAEMON_SRCS += srcs/daemon/

################################################################################
#################################               ################################
#################################    SOURCES    ################################
#################################               ################################
################################################################################

### CLIENT
CLIENT_SRCS += client_taskmaster.c
CLIENT_SRCS += opt.c
CLIENT_SRCS += history.c
CLIENT_SRCS += completion.c
CLIENT_SRCS += print_completion.c
CLIENT_SRCS += termmode.c
CLIENT_SRCS += readline.c
CLIENT_SRCS += keycodes.c
CLIENT_SRCS += actionkeys.c
CLIENT_SRCS += parser.c
CLIENT_SRCS += exit_routine.c
CLIENT_SRCS += client_connect.c
CLIENT_SRCS += client_signal.c
CLIENT_SRCS += transfert.c

CLIENT_SRCS += add.c
CLIENT_SRCS += avail.c
CLIENT_SRCS += blt.c
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
CLIENT_SRCS += help_function_a_f.c
CLIENT_SRCS += help_function_h_q.c
CLIENT_SRCS += help_function_r.c
CLIENT_SRCS += help_function_s.c
CLIENT_SRCS += help_function_t_v.c

### DAEMON
DAEMON_SRCS += daemon_taskmaster.c
DAEMON_SRCS += sockets.c
DAEMON_SRCS += config_parser.c
DAEMON_SRCS += log.c



################# ATTRIBUTION

vpath %.c $(PATH_CLIENT_SRCS)
vpath %.c $(PATH_DAEMON_SRCS)

################################################################################
#################################               ################################
#################################     OBJS      ################################
#################################               ################################
################################################################################

PATH_OBJS = objs/
CLIENT_OBJS = $(patsubst %.c, $(PATH_OBJS)%.o, $(CLIENT_SRCS))
DAEMON_OBJS = $(patsubst %.c, $(PATH_OBJS)%.o, $(DAEMON_SRCS))

################################################################################
#################################               ################################
#################################     RULES     ################################
#################################               ################################
################################################################################

#---------------------------------- STANDARD ----------------------------------#

all: $(CLIENT) $(DAEMON)

$(CLIENT): $(LIBFT) $(PATH_OBJS) $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(I_INCLUDES) $(CLIENT_OBJS) $(LIBFT) -o $@
	printf "$(GREEN)taskmasterctl is ready.\n$(NC)"

$(CLIENT_OBJS): $(PATH_OBJS)%.o: %.c $(HEADER) Makefile
	$(CC) $(CFLAGS) $(I_INCLUDES) -c $< -o $@
	printf "$(ONELINE)$(CYAN)Compiling $<"
	printf "                                                            \n$(NC)"

$(DAEMON): $(LIBFT) $(PATH_OBJS) $(DAEMON_OBJS)
	$(CC) $(CFLAGS) $(I_INCLUDES) $(DAEMON_OBJS) $(LIBFT) -o $@
	printf "$(GREEN)taskmasterd is ready.\n$(NC)"

$(DAEMON_OBJS): $(PATH_OBJS)%.o: %.c $(HEADER) Makefile
	$(CC) $(CFLAGS) $(I_INCLUDES) -c $< -o $@
	printf "$(ONELINE)$(CYAN)Compiling $<"
	printf "                                                            \n$(NC)"

$(PATH_OBJS):
	mkdir $@

$(LIBFT): FORCE 
	$(MAKE) -C $(PATH_LIBFT)

#--------------------------------- Basic Rules --------------------------------#

clean:
	$(RM) $(DAEMON_OBJS) $(CLIENT_OBJS)
	$(RM) -R $(PATH_OBJS)
	$(RM) -R $(DSYM)
	$(MAKE) -C $(PATH_LIBFT) clean
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
