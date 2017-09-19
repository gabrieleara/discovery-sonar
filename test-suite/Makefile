CC = gcc

CFLAGS = -Wall --coverage
LFLAGS = -lcunit --coverage

DIR_SRC = src
DIR_OBJ	= obj
DIR_COV = res-coverage
DIR_UNIT = res-unit

DIR_COV_HTML = $(DIR_COV)/html

run: prepare $(DIR_OBJ)/main.o $(DIR_OBJ)/sensor.o 
	$(CC) -o $(DIR_OBJ)/test.exe $(DIR_OBJ)/main.o $(DIR_OBJ)/sensor.o $(LFLAGS)
	./$(DIR_OBJ)/test.exe
	lcov --capture --directory $(DIR_OBJ) --output-file $(DIR_COV)/coverage.info
	genhtml $(DIR_COV)/coverage.info --output-directory $(DIR_COV_HTML)

$(DIR_OBJ)/main.o: $(DIR_SRC)/main.c
	$(CC) -o $(DIR_OBJ)/main.o -c $(DIR_SRC)/main.c $(CFLAGS)

$(DIR_OBJ)/sensor.o: $(DIR_SRC)/sensor.c
	$(CC) -o $(DIR_OBJ)/sensor.o -c $(DIR_SRC)/sensor.c $(CFLAGS)

clean:
	rm -f -r $(DIR_OBJ) $(DIR_UNIT) $(DIR_COV)
	mkdir $(DIR_OBJ) $(DIR_UNIT) $(DIR_COV)

prepare:
	
	cp /usr/share/CUnit/*.xsl $(DIR_UNIT)