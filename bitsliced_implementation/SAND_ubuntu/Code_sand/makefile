.PHONY:clean

OBJS = main.o\
       SANDTypeDef.o\
       SAND64128AVX2_32Way.o\
       SAND64128AVX2_64Way.o\
       SAND128128AVX2_16Way.o\
       SAND128128AVX2_32Way.o

main: $(OBJS)
%.o : %.c
	gcc -c -m64 -mavx2 -mbmi2 -O2 $^ -o $@

clean:
	rm -rf $(OBJS) main
