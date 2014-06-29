# ---------- Linked Libraries ----------
PERFDIR = ../../libs/perflib
PERFINCLUDEDIR = $(PERFDIR)/include
PERFLIBDIR = $(PERFDIR)/lib
PERFLIBNAME = PerfLib
PERFLIBFILENAME = $(PERFDIR)/lib/lib$(PERFLIBNAME).a

# ---------- Compiler settings ----------
CXX = g++
OPTIMIZATION_FLAG = -O0
CXX_FLAGS = -Wall $(OPTIMIZATION_FLAG)

SOURCEDIR = src

# ---------- Executable ----------
HEADERS = $(SOURCEDIR)/migrator.h \
   ../../common_include/workload.h \
   ../../common_include/logger/logger.h \
   $(SOURCEDIR)/massiveArray.h

SOURCES =  $(SOURCEDIR)/migrator.cpp \
   ../../common_include/logger/logger.cpp \
   $(SOURCEDIR)/cache.cpp 

INCLUDE_FLAGS = -I ../../common_include \
        -I $(PERFINCLUDEDIR)

LIB_FLAGS = -L $(PERFLIBDIR) -Wl,-Bstatic -l$(PERFLIBNAME) \
            -Wl,-Bdynamic	# KEEP THIS LAST to link as normal (dynamically) with system libraries

DEPENDENCIES = \
        $(PERFLIBFILENAME) \
        ../../common_include/experimentDebug.h
TARGET = CacheMemoryExperiment

# ---------- Target Rules ----------
$(TARGET) : $(HEADERS) $(SOURCES) $(DEPENDENCIES)
	$(CXX) $(CXX_FLAGS) -o $(TARGET) $(SOURCES) $(INCLUDE_FLAGS) $(LIB_FLAGS)

# ---------- Clean Rule ----------
.PHONY : clean
clean:
	rm $(TARGET)

.PHONY : clean_all
clean_all:
	rm $(PERFLIBFILENAME)
	rm $(TARGET)

# ---------- Make all Rule ----------
.PHONY : all
all:
	$(MAKE) -C $(PERFDIR)
	$(MAKE) -C $(shell pwd)   # Make this project!