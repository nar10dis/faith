# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nardis <nardis@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/20 11:11:45 by elaudrez          #+#    #+#              #
#    Updated: 2026/04/09 19:00:07 by nardis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= faith
OBJ_DIR		= obj

# style des cellules vivantes : 1 = neon, 2 = organique, 3 = crt
STYLE		?= 1

CXX			= g++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++17 \
			  -I. \
			  -I../includes/glad/include \
			  -I../includes/glfw/include \
			  -DCELL_STYLE=$(STYLE)

LDFLAGS		= -L../build/src -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl

SRCS		= main.cpp \
			  game/Environment.cpp \
			  game/GameOfLife.cpp \
			  renderer/Renderer.cpp \
			  renderer/Shaders.cpp \
			  ../includes/glad/src/glad.c

OBJ			= $(patsubst %.cpp, $(OBJ_DIR)/%.o, \
			  $(patsubst %.c,   $(OBJ_DIR)/%.o, $(SRCS)))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)
	@echo "✅ Compilation réussie de $(NAME)"

# Règle pour les .cpp
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle pour glad.c (C, pas C++)
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	gcc -I../includes/glad/include -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# recompile Shaders.o des que STYLE change
STYLE_STAMP	= $(OBJ_DIR)/.style

$(STYLE_STAMP): FORCE | $(OBJ_DIR)
	@echo $(STYLE) | cmp -s - $@ || echo $(STYLE) > $@

$(OBJ_DIR)/renderer/Shaders.o: $(STYLE_STAMP)

FORCE:

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re FORCE