OBJ = main.o gtkfun.o handlers.o loadsave.o manipulation.o
#DBG=-D DEBUG  #delete comment for DEBUG mode
CXXFLAGS=$(DBG)`pkg-config --cflags gtk+-3.0`
LDFLAGS=-export-dynamic `pkg-config --libs gtk+-3.0`

OUT_DIR=../out
OBJS_DIR=objs

Blue_Harvest: $(OBJ)
	g++ -o $(OUT_DIR)/Blue_Harvest $(OBJ) $(LDFLAGS)
	mv -f *.o $(OBJS_DIR)
	cp example $(OUT_DIR)/example
	echo > $(OUT_DIR)/default.conf

-include dependencies

.PHONY: depend clean cleanall dir

dir:
	mkdir $(OUT_DIR)
	mkdir $(OBJS_DIR)
depend:
	g++ -MM *.cc > dependencies

clean:
	rm -f objs/*.o
cleanall:
	rm -f objs/*.o *~
