source_files := $(wildcard src/*.cpp)
object_files := $(patsubst %.cpp,%.o,$(source_files))

CFLAGS := -fPIC -m32 -Isrc/ -lbyond -lext

%.o : %.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

foxmos.so: $(object_files)
	$(CC) -shared $(CFLAGS) $(CPPFLAGS) $(object_files) -o foxmos.so

all: foxmos.so
clean:
	rm $(object_files)
#gcc -shared -m32 -fPIC -lbyond .cpp -I./ -o ../foxmos.so