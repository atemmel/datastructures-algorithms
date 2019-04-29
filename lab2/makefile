TARGET=lab2
SRCS=*.cpp
FLAGS=-pedantic -Wall -Werror

$(TARGET): $(SRCS)
	g++ -o $(TARGET) $(SRCS) $(FLAGS)

test: $(TARGET)
	./$(TARGET)
