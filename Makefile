MAKE	= make -C

#==============================================================================#
#                                RESOURCES URLS                                #
#==============================================================================#

LIBFT_URL 	= git@github.com:xSilverWasHere/Libft.git
MLX_URL			= https://cdn.intra.42.fr/document/document/36126/minilibx-linux.tgz

#==============================================================================#
#                                     NAMES                                    #
#==============================================================================#

NAME 			= so_long
NAME_BONUS 		= so_long_bonus
### Message Vars
_SUCCESS 		= [$(GRN)SUCCESS$(D)]
_INFO 			= [$(BLU)INFO$(D)]
_NORM 			= [$(MAG)Norminette$(D)]
_NORM_SUCCESS 	= $(GRN)=== OK:$(D)
_NORM_INFO 		= $(BLU)File no:$(D)
_NORM_ERR 		= $(RED)=== KO:$(D)
_SEP 			= =====================

#==============================================================================#
#                                    PATHS                                     #
#==============================================================================#

SRC_PATH		= src
INC_PATH		= src
LIBS_PATH		= lib
BUILD_PATH		= .build

BONUS_SRC_PATH		= src_bonus
BONUS_INC_PATH		= src_bonus
BONUS_BUILD_PATH		= .build_bonus

FILES	= main.c init.c render.c events.c map.c

SRC						= $(addprefix $(SRC_PATH)/, $(FILES))
OBJS					= $(SRC:$(SRC_PATH)/%.c=$(BUILD_PATH)/%.o)
HEADERS				= $(INC_PATH)/so_long.h

BONUS_FILES	= main_bonus.c init_bonus.c render_bonus.c events_bonus.c map_bonus.c

BONUS_SRC						= $(addprefix $(BONUS_SRC_PATH)/, $(BONUS_FILES))
BONUS_OBJS					= $(BONUS_SRC:$(BONUS_SRC_PATH)/%.c=$(BONUS_BUILD_PATH)/%.o)
BONUS_HEADERS				= $(BONUS_INC_PATH)/so_long_bonus.h



LIBFT_PATH		= $(LIBS_PATH)/libft
LIBFT_ARC			= $(LIBFT_PATH)/libft.a

MLX_PATH			= $(LIBS_PATH)/mlx
MLX_ARC				= $(MLX_PATH)/libmlx_Linux.a


#==============================================================================#
#                              COMPILER & FLAGS                                #
#==============================================================================#

CC						= cc

CFLAGS				= -Wall -Wextra -Werror -g

RFLAGS				= -L ./lib/mlx -lmlx -L/usr/lib/X11 -lXext -lX11 -lm -g3
INC						= -I $(INC_PATH)
BONUS_INC						= -I $(BONUS_INC_PATH)

RM		= rm -rf
AR		= ar rcs
MKDIR_P	= mkdir -p

#==============================================================================#
#                                  RULES                                       #
#==============================================================================#


all: deps $(BUILD_PATH) $(NAME)	## Compile

$(NAME): $(BUILD_PATH) $(LIBFT_ARC) $(MLX_ARC) $(OBJS)			## Compile
	@echo "$(YEL)Compiling $(MAG)$(NAME)$(YEL) mandatory version$(D)"
	$(CC) $(CFLAGS) $(OBJS) $(INC) $(LIBFT_ARC) $(MLX_ARC) $(RFLAGS) -o $(NAME)
	@echo "[$(_SUCCESS) compiling $(MAG)$(NAME)$(D) $(YEL)🖔$(D)]"

deps:		## Download/Update deps
	@if test -d "$(LIBFT_PATH)" && test -d "$(MLX_PATH)"; then \
		echo " $(RED)$(D) [$(GRN)Nothing to be done!$(D)]"; fi
	@if test ! -d "$(LIBFT_PATH)"; then make get_libft; \
		else echo "$(YEL)[libft]$(D) folder found 🖔"; fi
	@if test ! -d "$(MLX_PATH)"; then make get_mlx; \
		else echo "$(YEL)[mlx]$(D) folder found 🖔"; fi


$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@echo -n "$(MAG)█$(D)"
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(BUILD_PATH):
	$(MKDIR_P) $(BUILD_PATH)
	@echo "* $(YEL)Creating $(CYA)$(BUILD_PATH)$(YEL) folder:$(D) $(_SUCCESS)"

$(LIBFT_ARC):
	$(MAKE) $(LIBFT_PATH)

get_libft:
	@echo "* $(CYA)Getting Libft submodule$(D)"
	@$(MKDIR_P) $(LIBS_PATH)
	@if test ! -d "$(LIBFT_PATH)"; then \
		git clone $(LIBFT_URL) $(LIBFT_PATH); \
		echo "* $(GRN)Libft submodule download$(D): $(_SUCCESS)"; \
	else \
		echo "* $(GRN)Libft submodule already exists 🖔"; \
	fi

$(MLX_ARC):
	$(MAKE) $(MLX_PATH)

get_mlx:
	@echo "* $(CYA)Getting MLX submodule$(D)"
	@$(MKDIR_P) $(LIBS_PATH)
	@if test ! -d "$(MLX_PATH)"; then \
		curl -kL $(MLX_URL) | tar zxvf -; \
		mv minilibx-linux ./lib/mlx; \
		echo "* $(GRN)MLX submodule download$(D): $(_SUCCESS)"; \
	else \
		echo "* $(GRN)MLX submodule already exists 🖔"; \
	fi

#BONUS RULES

bonus: all $(BONUS_BUILD_PATH) $(NAME_BONUS)	## Compile

$(NAME_BONUS): $(BONUS_BUILD_PATH) $(LIBFT_ARC) $(MLX_ARC) $(BONUS_OBJS)
	@echo "$(YEL)Compiling $(MAG)$(NAME)$(YEL) mandatory version$(D)"
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(BONUS_INC) $(LIBFT_ARC) $(MLX_ARC) $(RFLAGS) -o $(NAME_BONUS)
	@echo "[$(_SUCCESS) compiling $(MAG)$(NAME_BONUS)$(D) $(YEL)🖔$(D)]"

$(BONUS_BUILD_PATH)/%.o: $(BONUS_SRC_PATH)/%.c $(BONUS_HEADERS)
	@mkdir -p $(dir $@)
	@echo -n "$(MAG)█$(D)"
	$(CC) $(CFLAGS) $(BONUS_INC) -c $< -o $@

$(BONUS_BUILD_PATH):
	$(MKDIR_P) $(BONUS_BUILD_PATH)
	@echo "* $(YEL)Creating $(CYA)$(BONUS_BUILD_PATH)$(YEL) folder:$(D) $(_SUCCESS)"


clean:				## Remove object files
	@echo "*** $(YEL)Cleaning object files$(D)"
	$(RM) $(BUILD_PATH) $(BONUS_BUILD_PATH); \
	echo "* $(YEL)Removing $(CYA)$(BUILD_PATH) $(BONUS_BUILD_PATH)$(D) folder & files$(D): $(_SUCCESS)"; \

fclean: clean			## Remove executable and .gdbinit
	@echo "*** $(YEL)Cleaning executables$(D)"
	$(RM) $(NAME) $(NAME_BONUS);
	echo "* $(YEL)Removing $(CYA)$(NAME) $(NAME_BONUS)$(D) file: $(_SUCCESS)"; \

libclean: fclean	## Remove libs
	@echo "*** $(YEL)Cleaning libraries$(D)"
	$(RM) $(LIBS_PATH)
	@echo "* $(YEL)Removing lib folder & files!$(D) : $(_SUCCESS)"

re: libclean all	## Purge & Recompile

.PHONY: bonus clean fclean re help

#==============================================================================#
#                                  UTILS                                       #
#==============================================================================#

# Colors
#
# Run the following command to get list of available colors
# bash -c 'for c in {0..255}; do tput setaf $c; tput setaf $c | cat -v; echo =$c; done'
#
B  		= $(shell tput bold)
BLA		= $(shell tput setaf 0)
RED		= $(shell tput setaf 1)
GRN		= $(shell tput setaf 2)
YEL		= $(shell tput setaf 3)
BLU		= $(shell tput setaf 4)
MAG		= $(shell tput setaf 5)
CYA		= $(shell tput setaf 6)
WHI		= $(shell tput setaf 7)
GRE		= $(shell tput setaf 8)
BRED 	= $(shell tput setaf 9)
BGRN	= $(shell tput setaf 10)
BYEL	= $(shell tput setaf 11)
BBLU	= $(shell tput setaf 12)
BMAG	= $(shell tput setaf 13)
BCYA	= $(shell tput setaf 14)
BWHI	= $(shell tput setaf 15)
D 		= $(shell tput sgr0)
BEL 	= $(shell tput bel)
CLR 	= $(shell tput el 1)
