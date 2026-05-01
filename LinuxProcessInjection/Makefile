CC = gcc

injector := injector
target := target

all: injector target

$(injector): $(injector).c
	$(CC) $(injector).c -o $(injector)


$(target): $(target).c
	$(CC) $(target).c -o $(target)

clean:
	rm $(injector)
	rm $(target)
