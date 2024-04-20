# compiler and compilation options
CXX ?= g++
CXXFLAGS ?= -std=c++20
CPPFLAGS ?= -O3 -Wall -I. -Wno-conversion-null -Wno-deprecated-declarations -I../pacs-examples/Examples/include 

# executable name 
EXEC     = main
LDFLAGS ?= -L../pacs-examples/Examples/lib -W

# source files
SRCS = main.cpp 
OBJS = $(SRCS:.cpp=.o)

# header files
HDRS = Sparse_matrix.hpp Sparse_matrix_impl.hpp 

# rules
all: $(EXEC)

%.o: %.cpp $(HDRS)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
 
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	$(RM) *.o