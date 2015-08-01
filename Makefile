END_TARGET1  = HelloHackathon
END_TARGET2  = test

SOURCE_TARGETS1 =                \
    HelloHackathon.o

SOURCE_TARGETS2 =                \
    HelloHackathon_test.o

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

$(END_TARGET1): $(SOURCE_TARGETS1) $(TARGET_LIBS)
	@/bin/echo "";
	$(CXX) $(LDFLAGS) $(DEFINES) -z muldefs -o $(END_TARGET1) $(SOURCE_TARGETS1) 2>&1 | tee -a error.lst
	@/bin/echo ""; /bin/echo $(END_TARGET1) linked.

$(END_TARGET2): $(SOURCE_TARGETS2) $(TARGET_LIBS)
	@/bin/echo "";
	$(CXX) $(LDFLAGS) $(DEFINES) -z muldefs -o $(END_TARGET2) $(SOURCE_TARGETS2) 2>&1 | tee -a error.lst
	@/bin/echo ""; /bin/echo $(END_TARGET2) linked.