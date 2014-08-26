CC = g++

DEBUG = true

CXXFLAGS = -std=c++11 -Iinclude
CFLAGS = -Iinclude
LDFLAGS = -rdynamic -lyaml-cpp -lfcgi++ -lfcgi -lpthread -ldl -L.

ifeq ($(DEBUG),true)
	OPTI = -O0 -g
else
	OPTI = -O2 -s
endif

CXXFLAGS += $(OPTI)
CFLAGS += $(OPTI)
LDFLAGS += $(OPTI)

SRCS = $(shell find src -name "*.cc")

SRCS_C = $(shell find src -name "*.c")

INCLUDES = $(shell find include -name "*.h") $(shell find include -name "*.hpp")

OBJS = $(patsubst src/%,obj/%,$(SRCS:.cc=.o)) $(patsubst src/%,obj/%,$(SRCS_C:.c=.o))

NAME = eclair

all: $(NAME)

controllers: projects/lib/user/v1/controllers/home.cc
	g++ -o home.o -c $< -std=c++11 -Iinclude $(OPTI) -fPIC
	g++ -shared -Wl,-soname,libhome.so -o projects/lib/user/v1/controllers/libhome.so home.o

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.cc
	mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CXXFLAGS)

obj/%.o: src/%.c
	mkdir -p $(dir $@)
	gcc -o $@ -c $< $(CFLAGS)

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all

ifeq ($(USER),becorp)
run: all
	./eclair test
else
endif

.PHONY: all clean fclean re
