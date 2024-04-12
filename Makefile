NAME = ft_malcolm

SRCS_PATH = srcs
OBJ_PATH = objs

SOURCES =	main.c \

SRC = $(addprefix $(SRC_PATH)/,$(SOURCES))
OBJ = $(addprefix $(OBJ_PATH)/,$(SOURCES:.c=.o))

CC = cc

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): libft/libft.a $(OBJ)
		$(CC) $(CFLAGS)  $(OBJ) -L libft -lft -lm -o $(NAME)

$(OBJ_PATH)/%.o: $(SRCS_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

libft/libft.a:
	make -C libft

clean:
		rm -rf $(OBJ_PATH)
		make clean -C libft

fclean: clean
		rm -rf $(NAME)
		make fclean -C libft

re: fclean all

.PHONY: all clean fclean re