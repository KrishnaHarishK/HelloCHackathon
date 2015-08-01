END_TARGET  = HelloHackathon

SOURCE_TARGETS =                \
    HelloHackathon.o

# If you need valgrind, you need this:
#    -lglib-2.0
# as well as
#export G_DEBUG=gc-friendly G_SLICE=always-malloc

EXTRA_DEFINES  = \
    -Wall                                       \
    -Wunused-variable                           \
    -Werror                                     \
    -Wextra                                     \
    -Wfatal-errors

EXTRA_CXXFLAGS          = -ggdb3 -O2 -std=c++98
EXTRA_CFLAGS            = -ggdb3 -O2

EXTRA_CXXLDFLAGS        = --fatal-warnings
EXTRA_CLDFLAGS          = --fatal-warnings

tags:  $(SOURCE_TARGETS) $(TARGET_LIBS)
        mkctags

$(END_TARGET): $(SOURCE_TARGETS) $(TARGET_LIBS)
	@/bin/echo "";
	set -o pipefail; $(CXX) $(LDFLAGS) $(DEFINES) -z muldefs -o $(END_TARGET) $(SOURCE_TARGETS) 2>&1 | tee -a error.lst
	@/bin/echo ""; /bin/echo $(END_TARGET) linked.