.PHONY:clean

OBJS = main.o\
       SimonTypeDef.o\
       Simon64128AVX2_32Way.o\
       Simon64128AVX2_64Way.o\
       Simon128128AVX2_16Way.o\
       Simon128128AVX2_32Way.o

main: $(OBJS)
%.o : %.c
	gcc -c -m64 -mavx2 -mbmi2 -O2 $^ -o $@

clean:
	rm -rf $(OBJS) main
