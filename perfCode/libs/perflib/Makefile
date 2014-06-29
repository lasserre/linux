# ---------- TARGET SETTINGS----------
TARGETNAME =  PerfLib

DEPENDENCIES =

# ---------- Build settings ----------
# Compilation
CXX = g++
OPTIMIZATION_FLAG =
CXX_FLAGS = -c -Wall $(OPTIMIZATION_FLAG)

INCLUDE_DIR = include
SOURCEDIR = src

INCLUDE_FLAGS = -I $(INCLUDE_DIR) \
     -I ../../common_include
    

# Linking/Archiving
ARCHIVER = ar rcs

LIBDIR = lib
OBJDIR = obj

OBJECT_TARGET = $(PERF_SOURCES).o
ACTUAL_TARGET = $(LIBDIR)/lib$(TARGETNAME).a

# ---------- Sources  ----------
PERF_HEADERS =  \
    ../../common_include/experimentDebug.h \
    $(INCLUDE_DIR)/perfmanager.h \
    $(INCLUDE_DIR)/perfeventmap.h \
    $(INCLUDE_DIR)/perfeventmapentry.h \
    $(INCLUDE_DIR)/perfevent.h

PERF_SOURCES =  \
    $(SOURCEDIR)/perfmanager.cpp \
    $(SOURCEDIR)/perfeventmap.cpp \
    $(SOURCEDIR)/perfeventmapentry.cpp \
    $(SOURCEDIR)/perfevent.cpp

# ---------- Target Rules ----------
$(TARGETNAME) : $(PERF_HEADERS) $(PERF_SOURCES) $(DEPENDENCIES)	
	$(CXX) $(CXX_FLAGS) $(INCLUDE_FLAGS) $(PERF_SOURCES)
	$(ARCHIVER) $(ACTUAL_TARGET) *.o

# ---------- Clean Rule ----------
.PHONY : clean
clean:
	rm *.o
	rm $(LIBDIR)/*.a