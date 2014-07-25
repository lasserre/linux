# ---------- Linked Libraries ----------
# PERFDIR = ../../libs/perflib
# PERFINCLUDEDIR = $(PERFDIR)/include
# PERFLIBDIR = $(PERFDIR)/lib
# PERFLIBNAME = PerfLib
# PERFLIBFILENAME = $(PERFDIR)/lib/lib$(PERFLIBNAME).a

# ---------- Compiler settings ----------
CXX = g++
OPTIMIZATION_FLAG = -O0
CXX_FLAGS = -Wall $(OPTIMIZATION_FLAG)

SOURCEDIR = src

# ---------- Executable ----------
HEADERS = ../common_include/experimentDebug.h

SOURCES =  $(SOURCEDIR)/simple_workload.cpp

INCLUDE_FLAGS = -I ../common_include

DEPENDENCIES = ../common_include/experimentDebug.h

TARGET = simple_workload

# ---------- Target Rules ----------
$(TARGET) : $(HEADERS) $(SOURCES) $(DEPENDENCIES)
	$(CXX) $(CXX_FLAGS) -o $(TARGET) $(SOURCES) $(INCLUDE_FLAGS) $(LIB_FLAGS)

# ---------- Clean Rule ----------
.PHONY : clean
clean:
	rm $(TARGET)

.PHONY : clean_all
clean_all:
	rm $(TARGET)