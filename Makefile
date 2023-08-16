# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/14 15:20:40 by rrouille          #+#    #+#              #
#    Updated: 2023/08/16 14:58:37 by mariavillar      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name of the executable
NAME			= minishell
NAMEBNS			= philo_bonus

# Arguments passed to the executable
ARGS			= ${filter-out $@,${MAKECMDGOALS}}
PRINT_SCREEN	= YES
FAST_MODE		= NO

# Directories
SRCSDIR			= ./srcs
OBJSDIR			= objs
HDRDIR			= ./includes

# Colors for the terminal
GRAY			= \033[0;90m
RED				= \033[0;91m
GREEN			= \033[0;92m
YELLOW			= \033[0;93m
BLUE			= \033[0;94m
MAGENTA			= \033[0;95m
CYAN			= \033[0;96m
WHITE			= \033[0;97m
GRAY			= \033[0;90m
ENDCOLOR		= \033[0m

# Background colors
BG_G			= \033[42m
BG_R			= \033[41m
BG_Y			= \033[43m
BG_B			= \033[44m
BG_M			= \033[45m
BG_C			= \033[46m
BG_W			= \033[47m

# Text formatting
BOLD			= \033[1m
UNDERLINE		= \033[4m
ITALIC			= \033[3m
INVERTED		= \033[7m

# Clear screen
CLEAR			= \033c

# Clear line
CLEARLN			= \r\033[K

# Sources
SRCS			= ${shell find ${SRCSDIR} -type f -name '*.c'}
OBJS			= ${patsubst ${SRCSDIR}%,${OBJSDIR}%,${SRCS:%.c=%.o}}
CFLAGS			= -Werror -Wall -Wextra #-g3 -fsanitize=address
CC				= gcc
RM				= rm -rf
MAKE			= make
MKDIR			= mkdir -p

# Operating System
OS				:= ${shell uname}

# If using Linux echo command must be `echo -e`
ifeq ($(OS),Linux)
	ECHO = echo -e
	RLDIR = /usr/local/lib
else ifeq ($(OS),Darwin)
	ECHO = echo
	RLDIR = ~/.brew/opt/readline/lib/
endif

# Progress bar messages
START			= ${ECHO} "${YELLOW}\n🚀 Start of program compilation 🚀${ENDCOLOR}"
END_COMP		= ${ECHO} "${GREEN}\n\n✅ Compilation completed successfully! ✅${ENDCOLOR}"
S_OBJS			= ${ECHO} "${RED}🧹 Cleaning objects... 🧹${ENDCOLOR}"
S_NAME			= ${ECHO} "${RED}🧹 Cleaning program... 🧹${ENDCOLOR}"
CHARG_LINE		= ${ECHO} "${BG_G} ${ENDCOLOR}\c" && sleep 0.05
BS_N			= ${ECHO} "\n"

# Folders
OBJS_FOLDERS	= ${shell find ${SRCSDIR} -type d | sed "s|${SRCSDIR}|${OBJSDIR}|"}

# Folders
OBJS_FOLDERS	= ${shell find ${SRCSDIR} -type d | sed "s|${SRCSDIR}|${OBJSDIR}|"}

all: $(NAME)

os:
			@${ECHO} "${OS}"

# Build rule for object files
${OBJSDIR}/%.o : ${SRCSDIR}/%.c lib
			@${MKDIR} ${OBJS_FOLDERS}
<<<<<<< HEAD
			@${CC} ${CFLAGS} -I ${HDRDIR} -c $< -o $@ -I ~/.brew/opt/readline/include
=======
			@${CC} ${CFLAGS} -I ${HDRDIR} -c $< -o $@ -Ireadline/include/
>>>>>>> main

# Linking rule
${NAME}: ${OBJS}
			@${CHARG_LINE}
			@${CHARG_LINE} ${C_LAST};
			@${CC} ${CFLAGS} ${OBJS} mylib/objs/*/*.o -L ${RLDIR} -lreadline -o ${NAME}
			@${END_COMP}
			@sleep 0.5
 
# Run the program
run:	clear fast
			@${ECHO} "${GREEN}🔧 Operations completed: 🔧${ENDCOLOR}"
			@./${NAME} ${ARGS}
r:		clear fast
			@${ECHO} "${GREEN}🔧 Operations completed: 🔧${ENDCOLOR}"
			@./${NAME} ${ARGS}

###############################################################################
#                   ↓↓↓↓↓           CLEANING           ↓↓↓↓↓                  #
###############################################################################

# Clean object files and executable
clean:
			@${ECHO} "${CLEAR}\c"
			@${S_OBJS}
			@${RM} objs/ mylib/ .minishell_history
			@sleep 0.3
			@${ECHO} "${CLEAR}\c"
			@${ECHO} "${GREEN}✅ Simple clean completed! ✨\n"

# Clean everything
fclean: clean
			@${S_NAME}
			@${RM} ${NAME}
			@sleep 0.3
			@${ECHO} "${CLEAR}\c"
			@${ECHO} "${GREEN}✅ Deep clean completed! ✨"

###############################################################################
#                  ↓↓↓↓↓           UTILITIES           ↓↓↓↓↓                  #
###############################################################################

# Drawings
draw_begining:
			@${ECHO} "${CLEAR}${CYAN}\c"
			@cat ascii_art/prog_name | \
				while IFS= read -r line; do \
					printf '%s\n' "$$line"; \
					sleep 0.01; \
				done; \
				printf '%s' "$$line"
			@sleep 0.7
			@${ECHO} "${CLEAR}"

draw_bonus:
			@${ECHO} "${CLEAR}${BLUE}\c"
			@cat ascii_art/bonus | \
				while IFS= read -r line; do \
					printf '%s\n' "$$line"; \
					sleep 0.01; \
				done; \
				printf '%s' "$$line"
			@sleep 0.3
			@${ECHO} "${ENDCOLOR}"

draw_ready:
			@${ECHO} "${CLEAR}${GREEN}${BOLD}\c"
			@cat ascii_art/prog_ready | \
				while IFS= read -r line; do \
					printf '%s\n' "$$line"; \
					sleep 0.01; \
				done; \
				printf '%s' "$$line"
			@sleep 0.3
			@${ECHO} "${ENDCOLOR}"
			@make help PRINT_SCREEN=NO

draw_run:
			@for i in 1 2 3; do \
				${ECHO} "${CLEAR}${BLUE}${BOLD}\c"; \
				cat "ascii_art/prog_running_$$i"; \
				${ECHO} "${ENDCOLOR}"; \
				sleep 0.3; \
			done

draw_help:
			@${ECHO} "${GRAY}${BOLD}\c"
			@cat ascii_art/help_me | \
				while IFS= read -r line; do \
					printf '%s\n' "$$line"; \
				done; \
				printf '%s' "$$line"
			@${ECHO} "${ENDCOLOR}"

draw_norm_yes:
			@${ECHO} "${CLEAR}${GREEN}${BOLD}\c"
			@cat ascii_art/obama
			@${ECHO} "${ENDCOLOR}"

draw_norm_no:
			@${ECHO} "${CLEAR}${RED}${BOLD}\c"
			@cat ascii_art/obama_sad
			@${ECHO} "${ENDCOLOR}"

# Build mylib dependency
lib:	clear
			@if [ "${FAST_MODE}" = "NO" ]; then \
				if [ -d mylib ]; then \
					${ECHO} "${GREEN}🎉 Program already exists, updating it. 🔄\n${RESET}"; \
					git -C mylib pull; \
					${ECHO} ""; \
					make -C mylib; \
					${ECHO} "\c"; \
					sleep 0.3; \
				else \
					git clone https://github.com/rphlr/mylib --quiet; \
					make -C mylib; \
					${ECHO} "\c"; \
					sleep 0.3; \
					${START}; \
				fi; \
			else \
				if [ -d mylib ]; then \
					git -C mylib pull; \
					make fast -C mylib; \
				else \
					git clone https://github.com/rphlr/mylib --quiet; \
					make fast -C mylib; \
				fi; \
			fi

# Build rule for help function
help:
			@if [ "${PRINT_SCREEN}" = "YES" ]; then \
				${ECHO} "${CLEAR}\c"; \
				$(MAKE) draw_help; \
				for i in 3 2 1 0; do \
					printf '\r${BLUE}Help will be shown in: %d${ENDCOLOR}' "$$i"; \
					sleep 1; \
					printf '${CLEARLN}'; \
				done; \
			fi
			@${ECHO} "${GRAY}🏃 Run ${ITALIC}\`./${NAME}\`${ENDCOLOR}${GRAY} to see the program in action.${ENDCOLOR}${GRAY}"
			@${ECHO} "${BOLD}${UNDERLINE}💡 TIPS: 💡${ENDCOLOR}${GRAY}"
			@${ECHO} "\t- You can also use ${ITALIC}\`make run\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make r\`${ENDCOLOR}${GRAY} to try it out."
			@${ECHO} "\t- Check for memory leaks with ${ITALIC}\`make leaks\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make l\`${ENDCOLOR}${GRAY}."
			@${ECHO} "\t- Check the 42 norm with ${ITALIC}\`make norm\`${ENDCOLOR}${GRAY} or ${ITALIC}\`make n\`${ENDCOLOR}${GRAY}."
			@${ECHO} ""
			@${ECHO} "${YELLOW}🌟 Use ${ITALIC}\`make help\`${ENDCOLOR}${YELLOW} or ${ITALIC}\`make h\`${ENDCOLOR}${YELLOW} to display these helpful tips. 🚀${ENDCOLOR}"
h:		help

# Norminette
norm:
			@norminette ${SRCSDIR} >/dev/null 2>&1 && norminette ${HDRDIR} >/dev/null 2>&1 && $(MAKE) draw_norm_yes || $(MAKE) draw_norm_no && norminette ${SRCSDIR} && norminette ${HDRDIR}

n:		norm~

# fast
fast: FAST_MODE := YES

fast: lib ${OBJS}
			@$(CC) $(OBJS) mylib/objs/*/*.o -L ${RLDIR} -lreadline -o $(NAME)
			
f: fast

# Leaks
leaks:	clear fast
			@${ECHO} "${CLEAR}\c"
			@leaks -atExit -- ./${NAME} ${ARGS}
l:		clear fast
			@${ECHO} "${CLEAR}\c"
			@leaks -atExit -- ./${NAME} ${ARGS}

# Run the program with lldb
lldb:	clear fast
			@${ECHO} "${CLEAR}\c"
			@lldb ./${NAME} ${ARGS}
			@${ECHO} "${CLEAR}"

# Push the files to Git
git: fclean
			@${ECHO} "${CLEAR}\c"
			@git add .
			@${ECHO} "${CYAN}✅ Added files to git! 📁"
			@if [ -z "${ARGS}" ]; then \
				git commit -m "💻 Auto-commit"; \
			else \
				git commit -m "${ARGS}"; \
			fi
			@${ECHO} "${BLUE}✅ Changes committed! ✔️"
			@git push
			@${ECHO} "${GREEN}✅ All changes are now on GitHub! 🚀${ENDCOLOR}"

# Dummy target to prevent arguments with dashes from being interpreted as targets
%:
			@:

# Clear the screen
clear:
			@${ECHO} "${CLEAR}\c"

# Rebuild the program
re: fclean all

.PHONY: all clean fclean re run test bonus help norm leaks lldb git clear c