
  CC = g++


  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
  CFLAGS  = -g -Wall -std=c++17

  # the build target executable:
  TARGET = ProtobufFormatter

  all: $(TARGET)

  $(TARGET): $(TARGET).cpp
  	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

  clean:
  	$(RM) $(TARGET)
