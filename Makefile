source_files := $(wildcard src/*.cpp)
object_files := $(patsubst %.cpp,%.o,$(source_files))

CFLAGS := -m32 -Isrc/ -lbyond

%.o : %.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

foxmos.so: $(object_files)
	$(CC) -shared -fPIC $(CFLAGS) $(CPPFLAGS) -o foxmos.so

all: foxmos.so
clean:
	rm $(object_files)
#gcc -shared -m32 -fPIC -lbyond .cpp -I./ -o ../foxmos.so