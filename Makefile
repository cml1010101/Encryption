SRCS = $(wildcard keys/*.cpp)
OBJS = ${SRCS:.cpp=.o}
all: encryption_1.0-1_amd64.deb
	rm -rf encryption_1.0-1_amd64 ${OBJS} src/decryption.o src/encryption.o
encryption_1.0-1_amd64.deb: encryption_1.0-1_amd64/DEBIAN/control encryption_1.0-1_amd64/usr/bin encryption_1.0-1_amd64/usr/lib encryption_1.0-1_amd64/usr/include/Encryption
	dpkg-deb --build --root-owner-group encryption_1.0-1_amd64
encryption_1.0-1_amd64/DEBIAN/control:
	mkdir -p encryption_1.0-1_amd64/DEBIAN
	cp control encryption_1.0-1_amd64/DEBIAN
encryption_1.0-1_amd64/usr/bin: encrypt decrypt
	mkdir -p encryption_1.0-1_amd64/usr/bin
	cp decrypt encryption_1.0-1_amd64/usr/bin
	cp encrypt encryption_1.0-1_amd64/usr/bin
encryption_1.0-1_amd64/usr/lib: libencryption.a
	mkdir -p encryption_1.0-1_amd64/usr/lib
	cp libencryption.a encryption_1.0-1_amd64/usr/lib
encryption_1.0-1_amd64/usr/include/Encryption:
	mkdir -p encryption_1.0-1_amd64/usr/include
	cp -r include encryption_1.0-1_amd64/usr/include/Encryption
encrypt: ${OBJS} src/encryption.o
	g++ ${OBJS} src/encryption.o -o encrypt -Iinclude -lboost_program_options
decrypt: ${OBJS} src/decryption.o
	g++ ${OBJS} src/decryption.o -o decrypt -Iinclude -lboost_program_options
libencryption.a: ${OBJS}
	ar rcs libencryption.a ${OBJS}
clean:
	rm -f src/encryption.o src/decryption.o ${OBJS}
%.o: %.cpp
	g++ -c $< -o $@ -Iinclude -g -std=c++17