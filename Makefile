SRC := cts_log_filter.cpp
TARGET := cts_log_filter
GCC := gcc
CLEAN := clean
CPPFLAGS := 

$(TARGET):$(SRC)
	$(GCC) -o $(TARGET) $(CPPFLAGS) $(SRC)

$(CLEAN):
	rm -rf ./$(TARGET)
