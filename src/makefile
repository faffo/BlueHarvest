OBJ = main.o gtkfun.o handlers.o loadsave.o manipulation.o
DBG=-D DEBUG  #delete comment for DEBUG mode
CXXFLAGS=$(DBG)`pkg-config --cflags gtk+-3.0`
LDFLAGS=-export-dynamic `pkg-config --libs gtk+-3.0`

OUT_DIR=../out
OUT_OBJS=../out/objs

Blue_Harvest: $(OBJ)
	g++ -o ../out/Blue_Harvest $(OBJ) $(LDFLAGS)

-include dependencies

.PHONY: depend clean cleanall

depend:
	g++ -MM *.cc > dependencies

clean:
	rm -f *.o
cleanall:
	rm -f *.o *~
