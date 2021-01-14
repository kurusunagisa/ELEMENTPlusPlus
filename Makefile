# icon.oが存在するか
icon := $(shell ls | grep icon.o)
# icon.oが存在しなかった場合，アイコン画像をオブジェクトファイル化
ifeq ($(icon),)
$(shell windres icon.rc icon.o)
endif

# 最終目的のファイル
TARGET = element.exe

# ソースファイル(*.c)の一覧
SRCS = main.c display.c move.c

# オブジェクトファイル(*.o)の一覧
OBJS = ${SRCS:.c=.o} icon.o

# ヘッダファイルの一覧
HEADERS = main.h display.h move.h

# コンパイラリンカの指定
CC = gcc
CCFLAGS = -Wall -I/usr/include/opengl
LIBS =  -lglpng -lglut32 -lglu32 -lopengl32 -lm
LDFLAGS = -O3

# OBJSからTARGET
$(TARGET) : $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET) $(LIBS)

# *.cから*.oを作る
.c.o :
	$(CC) $(CCFLAGS) -c $<

# *.oはHEADERSとMakefileに依存
$(OBJS) : $(HEADERS) Makefile

# make clean
clean :
	rm -f $(TARGET) $(OBJS) core *~
