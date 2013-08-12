INCLUDE_PATH = \
	-I/home/users/cmoreau/Bureau/vbox/sfml/libcsfml-1.6/CSFML/include\
	-I/home/users/cmoreau/Bureau/vbox/opengl/mesa-7.7.1/include

LIBS_PATH = \
	-L/home/users/cmoreau/Bureau/vbox/sfml/libcsfml-1.6/CSFML/lib

BIN_PATH = ./bin/

BIN_NAME = VisualBinary

COMPILO = gcc

LIBS    = $(LIBS_PATH)

CFLAGS  = -Wall -std=c99 -fPIC -g -O0 -c $(INCLUDE_PATH)

OBJ_PATH = ./obj/

OBJ = \
	$(OBJ_PATH)main.o\
	$(OBJ_PATH)Analyzer/Analyzer.o\
	$(OBJ_PATH)AppState/Cube3D.o\
	$(OBJ_PATH)Ztring/Ztring.o\
	$(OBJ_PATH)BbQueue/BbQueue.o\
	$(OBJ_PATH)Frame/Frame.o\
	$(OBJ_PATH)Function/Function.o\
	$(OBJ_PATH)dbg/dbg.o\
	$(OBJ_PATH)GraphicObjects/GraphicObjects.o\
	$(OBJ_PATH)Randomizer/Randomizer.o
	
$(OBJ_PATH)%.o : %.c
	$(COMPILO) $(CFLAGS) -o $@ $<

$(BIN_NAME) : $(OBJ) 
	$(COMPILO) -o $(BIN_PATH)$(BIN_NAME) $(OBJ) $(LIBS)

clean : 
	rm -f $(OBJ_PATH)*.o;
	rm -f $(OBJ_PATH)*/*.o;
	rm -f $(BIN_PATH)$(BIN_NAME);
	
rebuild :
	make clean
	make

gdb :
	make rebuild
	echo 'r' | gdb $(BIN_PATH)$(BIN_NAME)
	
