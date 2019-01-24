CC      = g++
OBJ     = main.o handler.o  rmtool.o
VPATH   = src:..
server:$(OBJ)
	$(CC) -o $@ $(OBJ) -lglog

%.o:%.cpp
	$(CC) -c $<

#$<  是代表第一个依赖文件的名称
#$@ 是代表目标文件的名称
#VPATH 会按照这个顺序进行搜索。目录由“冒号”分隔
#vpath %.h include    //指定.h类型文件的搜索路径是include
#vpath %.cpp src      //指定.cpp类型文件的搜索路径是src
#.PHONY 指明目标为伪目标

.PHONY:clean
clean:
	-rm -f server $(OBJ)
