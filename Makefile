NAME = ft_traceroute
INCLUDES = ./includes
SRC_PATH = ./sources
OBJ_PATH = $(SRC_PATH)/debug
SRC_NAMES = main.c traceroute.c nslookup.c error.c parser.c debug.c utils.c \
			json.c location.c
OBJ_NAMES = $(SRC_NAMES:.c=.o)
SRCS = $(addprefix $(SRC_PATH)/,$(SRC_NAMES))
OBJS = $(addprefix $(OBJ_PATH)/,$(OBJ_NAMES))
LFT_PATH = ./libft
LFLAGS = -L$(LFT_PATH) -lft -lcurl
IFLAGS = -I$(INCLUDES)
CFLAGS = -Wall -Wextra -Werror 
DFLAGS = 
CC = clang

all: $(NAME)

welcome:
	@clear
	@figlet ft_traceroute | lolcat 2>/dev/null
	@echo "\n"

$(OBJ_PATH):
	@mkdir $@
	@echo "\e[4;35mBuilding ft_traceroute sources:\e[0m"

$(NAME): welcome $(OBJ_PATH) $(OBJS)
	@echo "✅ Source files: $(shell echo $(SRC_NAMES) | wc -w) / $(shell echo $(SRC_NAMES) | wc -w)\033[0m --> \033[1;32m[Done]\033[0m\n"
	@echo "\e[4;35mBuilding Libft:\e[0m"
	@make -C $(LFT_PATH) --no-print-directory
	@$(CC) $(DFLAGS) -o $@ $(OBJS) $(LFLAGS)
	@echo "\n"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS) -c $? -o $@
	@echo "🔧 Source files: $(shell ls -1 $(OBJ_PATH) | wc -l) / $(shell echo $(SRC_NAMES) | wc -w)\r\c"

clean:
	@rm -rf $(OBJ_PATH)
	@make -C $(LFT_PATH) fclean --no-print-directory

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
