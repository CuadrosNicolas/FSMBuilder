CC = g++-7
CFLAGS = -Wall -std=c++1z
EXEC_NAME = bin/Test_fsm
LIB_NAME = bin/lib_fsm.a
OBJECTS = bin/objects/
INSTALL_DIR = /usr/bin
OBJ_FILES = $(OBJECTS)main.o $(OBJECTS)stringFSM.o
all : $(EXEC_NAME) $(LIB_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)


$(LIB_NAME) : $(OBJ_FILES)
	ar rvs $(LIB_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES)

$(OBJECTS)main.o: ./main.cpp
	$(CC) $(CFLAGS) -o $@ -c $<
$(OBJECTS)stringFSM.o: ./includes/stringFSM.cpp
	$(CC) $(CFLAGS) -o $@ -c $<
