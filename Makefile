SRCS = $(wildcard keys/*.cpp)
OBJS = ${SRCS:.cpp=.o}
all: ${OBJS} src/decryption.o src/encryption.o
	g++ src/decryption.o ${OBJS} -o decrypt -g -L/home/cml/boost_1_80_0/stage/lib -l:libboost_program_options.a
	g++ src/encryption.o ${OBJS} -o encrypt -g -L/home/cml/boost_1_80_0/stage/lib -l:libboost_program_options.a
	ar rcs libencryption.a ${OBJS}
	rm -f src/encryption.o src/decryption.o ${OBJS}
	sudo cp decrypt /usr/bin/decrypt
	sudo cp encrypt /usr/bin/encrypt
clean:
	rm -f src/encryption.o src/decryption.o ${OBJS}
%.o: %.cpp
	g++ -c $< -o $@ -Iinclude -g -std=c++17