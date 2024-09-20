CC = g++

TARGET = cache

CFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations	\
         -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual			\
         -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op					\
         -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192			\
         -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE -mavx -mavx2				\

SRC_PATH = ./src/
OBJ_PATH = ./obj/

SRC = $(SRC_PATH)cache.cpp $(SRC_PATH)main.cpp
OBJ = $(OBJ_PATH)cache.cpp $(OBJ_PATH)main.cpp

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

$(OBJ_PATH)%.o : $(SRC_PATH)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm $(TARGET).exe $(PREF_OBJ)*.o