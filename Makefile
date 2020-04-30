# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/03/11 17:23:00 by ffoissey          #+#    #+#              #
#    Updated: 2020/04/30 13:43:04 by ffoissey         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
################################                ################################
################################ MAIN VARIABLES ################################
################################                ################################
################################################################################

# Name client
NAME = taskmasterctl

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

################################################################################
#################################               ################################
#################################  PATH SOURCES ################################
#################################               ################################
################################################################################

PATH_SRCS += srcs/
PATH_SRCS += srcs/client
PATH_SRCS += srcs/client/builtin

################################################################################
#################################               ################################
#################################    SOURCES    ################################
#################################               ################################
################################################################################

### CLIENT
SRCS += client_taskmaster.c
SRCS += history.c
SRCS += completion.c
SRCS += termmode.c
SRCS += readline.c
SRCS += keycodes.c
SRCS += actionkeys.c
SRCS += parser.c
SRCS += exit_routine.c
SRCS += client_connect.c
SRCS += signal.c

SRCS += add.c
SRCS += avail.c
SRCS += blt.c
SRCS += clear.c
SRCS += exit.c
SRCS += fg.c
SRCS += maintail.c
SRCS += open.c
SRCS += pid.c
SRCS += quit.c
SRCS += reload.c
SRCS += remove.c
SRCS += reread.c
SRCS += shutdown.c
SRCS += tail.c
SRCS += update.c
SRCS += version.c

################# ATTRIBUTION

vpath %.c $(PATH_SRCS)

################################################################################
#################################               ################################
#################################     OBJS      ################################
#################################               ################################
################################################################################

PATH_OBJS = objs/
OBJS = $(patsubst %.c, $(PATH_OBJS)%.o, $(SRCS))

################################################################################
#################################               ################################
#################################     RULES     ################################
#################################               ################################
################################################################################

#---------------------------------- STANDARD ----------------------------------#

all: $(NAME)

$(NAME): $(LIBFT) $(PATH_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(I_INCLUDES) $(OBJS) $(LIBFT) -o $@
	printf "$(GREEN)taskmasterctl is ready.\n$(NC)"

$(OBJS): $(PATH_OBJS)%.o: %.c $(HEADER) Makefile
	$(CC) $(CFLAGS) $(I_INCLUDES) -c $< -o $@
	printf "$(ONELINE)$(CYAN)Compiling $<"
	printf "                                                            \n$(NC)"

$(PATH_OBJS):
	mkdir $@

$(LIBFT): FORCE 
	$(MAKE) -C $(PATH_LIBFT)

#--------------------------------- Basic Rules --------------------------------#

clean:
	$(RM) $(OBJS)
	$(RM) -R $(PATH_OBJS)
	$(RM) -R $(DSYM)
	$(MAKE) -C $(PATH_LIBFT) clean
	printf "$(RED)Objs from taskmasterctl removed\n$(NC)"

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(PATH_LIBFT) fclean
	printf "$(RED)taskmasterctl removed\n$(NC)"

re: fclean
	$(MAKE)

FORCE:

#----------------------------------- Special ----------------------------------#

.PHONY: clean fclean re all FORCE
.SILENT:
