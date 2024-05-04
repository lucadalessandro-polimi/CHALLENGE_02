# compiler and compilation options
CXX ?= g++
CXXFLAGS ?= -std=c++20
CPPFLAGS ?= -O3 -Wall -I. -Wno-conversion-null -Wno-deprecated-declarations 

# executable name 
EXEC     = main


# source files
SRCS = main.cpp 
OBJS = $(SRCS:.cpp=.o)

# header files
HDRS = Sparse_matrix.hpp Sparse_matrix_impl.hpp chrono.hpp

# rules
all: $(EXEC)

%.o: %.cpp $(HDRS)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
 
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	$(RM) *.o