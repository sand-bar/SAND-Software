.PHONY:clean

OBJS = main.o\
       BATTypeDef.o\
       BAT64128AVX2_32Way.o\
       BAT64128AVX2_64Way.o\
       BAT128128AVX2_16Way.o\
       BAT128128AVX2_32Way.o

main: $(OBJS)
%.o : %.c
	gcc -c -m64 -mavx2 -mbmi2 -O2 $^ -o $@

clean:
	rm -rf $(OBJS) main
