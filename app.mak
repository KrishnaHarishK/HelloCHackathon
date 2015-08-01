
MAKEFLAGS= --no-print-directory

DEPEND_KLUDGE=$(shell ls Depend 2>&1 | awk '/No such/ {print "${DEVBASE}/include/blank.mak"} /^Depend$$/ {print "Depend"}')

OBJ_DIR     = obj
SPLINT_DIR  = splint
DEPEND_FILE = Depend

BASE_DIR                          = $(DEVBASE)

IPTS_INC_DIR                      = $(BASE_DIR)/include
IPTS_SHINC_DIR                    = $(BASE_DIR)/src/ip_shared/include
IPTS_NG_TV_SHARED_DIR             = $(BASE_DIR)/iptsShared/src/include
IPTS_BIN_DIR                      = $(BASE_DIR)/bin
IPTS_LIB_DIR                      = $(BASE_DIR)/lib
IPTS_CPP_LIB_DIR                  = $(BASE_DIR)/cpplib
IPTS_SHARED_LIB_DIR               = $(IPTS_LIB_DIR)
IPTS_REAL_SHARED_LIB_DIR          = $(BASE_DIR)/shlib
IPTS_SUPPORT_PROD_BIN_DIR         = $(DEVBASE)/SupportTools/prod/bin
IPTS_SUPPORT_PROD_SCRIPT_DIR      = $(DEVBASE)/SupportTools/prod/scripts
IPTS_SUPPORT_NONPROD_BIN_DIR      = $(DEVBASE)/SupportTools/nonprod/bin
IPTS_SUPPORT_NONPROD_SCRIPT_DIR   = $(DEVBASE)/SupportTools/nonprod/scripts
IPTS_SHARED_INC_DIR               = $(BASE_DIR)/iptsShared/src/include
IPTS_NG_TV_SHARED_DIR             = $(BASE_DIR)/iptsShared/src/include
IPTSSHARED_DIR                    = $(BASE_DIR)/iptsShared
IPTSSHARED_LIBRARIES_DIR          = $(IPTSSHARED_DIR)/src/Libraries
IPTS_SHARED_LIBRARIES_INCLUDE_DIR = $(IPTSSHARED_DIR)/src/Libraries/Utility


ERRORFILE = error.lst

CC     = gcc
CXX    = g++
LD     = gcc
DEPCC  = gcc
DEPCXX = g++
 
SPLINT = splint
MV     = mv
AR     = /usr/bin/ar

# For distributed make
DMAKE_MODE = parallel
DMAKE_MAX_JOBS = 12

#  Shared Libraries
LIB_GLIB              = $(IPTS_LIB_DIR)/libglib.so
LIB_EXPAT             = $(IPTS_LIB_DIR)/libexpat.so
LIB_MESGLOG           = $(IPTS_REAL_SHARED_LIB_DIR)/libMesgLog.so
LIB_FILEUTIL          = $(IPTS_REAL_SHARED_LIB_DIR)/libFileUtil.so
LIB_THREADUTIL        = $(IPTS_REAL_SHARED_LIB_DIR)/libThreadUtil.so
LIB_SOCREATE          = $(IPTS_REAL_SHARED_LIB_DIR)/libcreate.so
LIB_P2CPASSTHRU       = $(IPTS_REAL_SHARED_LIB_DIR)/p2cpassthru.so
LIB_MATCHCPP          = $(IPTS_REAL_SHARED_LIB_DIR)/libmatchcpp.so
LIB_FASTTIME          = $(IPTS_REAL_SHARED_LIB_DIR)/libFastTime.so
LIB_FILELOCK          = $(IPTS_LIB_DIR)/libFileLock.so

# TV Shared Libraries
LIB_REPORTING         = $(IPTS_REAL_SHARED_LIB_DIR)/libReporting.so

# Static Libraries
LIB_UTILITY           = $(IPTS_LIB_DIR)/libutility.a
LIB_ZLIB              = $(IPTS_LIB_DIR)/libzlib.a
LIB_NGUTILITY         = $(IPTS_LIB_DIR)/libngutility.a
LIB_REFLOOKUP         = $(IPTS_LIB_DIR)/libRefLookup.a 
LIB_IOIPTS            = $(IPTS_LIB_DIR)/libioIPTS.a
LIB_CONVERSIONS       = $(IPTS_LIB_DIR)/libConversions.a
LIB_RECDUP            = $(IPTS_LIB_DIR)/libRecdup.a
LIB_RECYCLE           = $(IPTS_LIB_DIR)/librecycle.a
LIB_SOCKETW           = $(IPTS_LIB_DIR)/libSocketW.a
LIB_DRART             = $(IPTS_LIB_DIR)/libDRARTCommon.a
LIB_XMLPREPROC        = $(IPTS_LIB_DIR)/libXMLPreProc.a
LIB_TDRPREPROC_SHARED = $(IPTS_LIB_DIR)/libTDRPreProc.a
LIB_REFLOOKUP_SHARED  = $(IPTS_LIB_DIR)/libRefLookupShared.a
LIB_TDRV2             = $(IPTS_LIB_DIR)/libTDRv2.a
LIB_IPV6              = $(IPTS_LIB_DIR)/libIPV6.a
LIB_CCLBASE           = $(IPTS_LIB_DIR)/libcclbase.a
LIB_TDRSERVENT        = $(IPTS_LIB_DIR)/libtdrservent.a
LIB_PROCMGMT          = $(IPTS_LIB_DIR)/libprocmgmt.a

# TM Static Libraries
LIB_OMSUM             = $(IPTS_LIB_DIR)/libomsum.a

# TV Static Libraries
LIB_TVUTILITY         = $(IPTS_LIB_DIR)/libtvutility.a
LIB_CPP_UTILS         = $(IPTS_LIB_DIR)/libcpp_utils.a
LIB_HUBMSGQUEUE       = $(IPTS_LIB_DIR)/libHubMsgQueue.a
LIB_INDEXFILE         = $(IPTS_LIB_DIR)/libIndexFile.a
LIB_TABLEUTILS        = $(IPTS_LIB_DIR)/libTableUtils.a

# C++ static libraries
LIB_CPP_FILEUTILS     = $(IPTS_CPP_LIB_DIR)/libFileUtils.a
LIB_CPP_REFUTILS      = $(IPTS_CPP_LIB_DIR)/libRefUtils.a
LIB_CPP_BV_UTILS      = $(IPTS_CPP_LIB_DIR)/libBatchVector.a
LIB_ISHERIFF          = $(IPTS_CPP_LIB_DIR)/libiSheriffUtils.a
LIB_GBRDRIVER         = $(IPTS_CPP_LIB_DIR)/libGBRDriver.a
LIB_CREATEREF         = $(IPTS_CPP_LIB_DIR)/libCreateRef.a

#################### Default Targets ##################
LocalFiles: CleanErrorFile DefaultSubDirs BuildDepend DisplayStartMessage
	@$(MAKE) SourceTargets
	@$(MAKE) EndTarget
	@$(MAKE) DisplayErrorFile

$(LIB_GLIB) :
	@/bin/echo "Glib is missing.  Please build IPTS clean, including glib realclean." ; exit 1

$(LIB_EXPAT) :
	@/bin/echo "Expat is missing.  Please build IPTS clean, including expat realclean." ; exit 1

$(LIB_CONVERSIONS):
	@cd $(DEVBASE)/src/ip_shared/Conversions; $(MAKE)

$(LIB_REFLOOKUP):
	@cd $(DEVBASE)/src/ip_shared/RefLookup; $(MAKE)

$(LIB_IOIPTS):
	@cd $(DEVBASE)/src/ip_shared/ioIPTS; $(MAKE)

$(LIB_UTILITY):
	@cd $(DEVBASE)/src/ip_shared/Utility; $(MAKE)

$(LIB_TDRSERVENT):
	@cd $(DEVBASE)/src/Libraries/tdr_servent; $(MAKE)

$(LIB_RECDUP):
	@cd $(DEVBASE)/src/recdup/libs; $(MAKE)

$(LIB_PROCMGMT):
	@cd $(DEVBASE)/src/Libraries/procmgmt; $(MAKE)

$(LIB_SOCREATE):
	@cd $(DEVBASE)/src/create_downstream; $(MAKE)

$(LIB_P2CPASSTHRU):
	@cd $(DEVBASE)/src/p2cpassthru; $(MAKE)

$(LIB_SOCKETW):
	@cd $(DEVBASE)/iptsShared/src/LibSocketW; $(MAKE)

#$(LIB_MESLOG):
#	@cd $(DEVBASE)/src/Libraries/Common; $(MAKE)

#$(LIB_FILEUTIL):
#	@cd $(DEVBASE)/src/Libraries/lib; co -kb libFileUtil.so; cp libFileUtil.so $(IPTS_SHARED_LIB_DIR)/

#$(LIB_FILEUTIL):
#	@cd $(DEVBASE)/src/Libraries/Common; $(MAKE)

#$(LIB_THREADUTIL):
#	@cd $(DEVBASE)/src/Libraries/Common; $(MAKE)

#$(LIB_FASTTIME):
#	@cd $(DEVBASE)/src/Libraries/Common; $(MAKE)

$(LIB_TDRV2):
	@cd $(IPTSSHARED_LIBRARIES_DIR)/TDRv2; $(MAKE)

$(LIB_TABLEUTILS):
	@cd $(DEVBASE)/iptsShared/src/Libraries/TableUtils; $(MAKE)

.PHONY: DefaultSubDirs $(SUBDIRS)
DefaultSubDirs: ${SUBDIRS}
${SUBDIRS}:
#Surya	@mydirs="${SUBDIRS}"; for i in $$mydirs ; do (cd $$i; if test $$i = "p2cpassthru"; then /usr/local/bin/perl Makefile.PL; fi;  $(MAKE)); done
	@ $(MAKE) -C $@

SourceTargets: MakeBuildDirs $(SOURCE_TARGETS)
	@/bin/echo -n `/bin/echo $(SOURCE_TARGETS) | sed 's/.*//g'`

MakeBuildDirs:
	@mkdir -p $(SPLINT_DIR)
	@mkdir -p $(OBJ_DIR)

EndTarget: $(END_TARGET)
	@echo -n

all:
	@cd $(DEVBASE); $(MAKE)

everything:
	@cd $(DEVBASE); $(MAKE) clean
	@cd $(DEVBASE)/iptsShared/src/glib; $(MAKE) realclean; cd $(DEVBASE)
	@cd $(DEVBASE)/iptsShared/src/expat; $(MAKE) realclean; cd $(DEVBASE)
	@cd $(DEVBASE)/src/ip_shared/Common; ./make.sh clean; cd $(DEVBASE)
	@$(MAKE) all

allclean:
	@cd $(DEVBASE); $(MAKE) clean
	@$(MAKE) all

up:
	@cd ..; $(MAKE)

############### Extra Compiler/Linker Options  ###########
INCDIRS = $(EXTRA_INCLUDES) -I. -I$(IPTS_SHINC_DIR) -I$(IPTS_SHARED_INC_DIR) -I$(IPTS_INC_DIR)

# EXTRA_DEFINES can be defined in your local Makefile to pass different
# linker and compiler options.
DEFINES = $(EXTRA_DEFINES) -DLINUX -DVOIP_NG

#################### Makefile Settings ###################
SPLINTFLAGS = -weak $(INCDIRS)
CFLAGS      = -O2 -c -ggdb3 -Wall -Wextra -fPIC $(EXTRA_CFLAGS)   $(DEFINES) $(INCDIRS) 
CXXFLAGS    = -O2 -c -ggdb3 -Wall -Wextra       $(EXTRA_CXXFLAGS) $(DEFINES) $(INCDIRS)
LDFLAGS     = -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -m64 -mtune=generic $(EXTRA_LDFLAGS)

#################### Auto-(re-)generate iptsconfig vars from cfg file as needed

GEN_IPTSCONFIG_VARS   = $(IPTS_SUPPORT_NONPROD_SCRIPT_DIR)/gen-iptsconfig-vars
IPTSCONFIG_VARS_CFG   = iptsconfig_vars.cfg
IPTSCONFIG_VARS_C     = iptsconfig_vars.c
IPTSCONFIG_VARS_H     = iptsconfig_vars.h
CLEAN_IPTSCONFIG_VARS = $(IPTSCONFIG_VARS_C) $(IPTSCONFIG_VARS_H)

$(IPTSCONFIG_VARS_C): $(IPTSCONFIG_VARS_H)
	$(GEN_IPTSCONFIG_VARS)    < $(IPTSCONFIG_VARS_CFG) > $(IPTSCONFIG_VARS_C)

$(IPTSCONFIG_VARS_H): $(IPTSCONFIG_VARS_CFG) $(GEN_IPTS_VARS)
	$(GEN_IPTSCONFIG_VARS) -e < $(IPTSCONFIG_VARS_CFG) > $(IPTSCONFIG_VARS_H)


###################### Make Options ##################
#.c.ln: 
#$(LINT_DIR)/%.ln : %.c
#	@/bin/echo "*** $< ***" >> $(ERRORFILE)
#	@/bin/echo -n $< " "
#	@/bin/echo "Lint: $<" >> $(ERRORFILE)
#	-@$(LINT) -c -Xa $(LINTFLAGS) $< 2>> $(ERRORFILE)
#	@touch $*.ln
#	@mv $*.ln $(LINT_DIR)

.c.ln: 
$(SPLINT_DIR)/%.ln : %.c
	@#$(SPLINT) $(SPLINTFLAGS) $< 2>> $(ERRORFILE)
	@touch $*.ln
	@mv $*.ln $(SPLINT_DIR)

##
# Older style that suppressed build commands
##
#.C.o:
#$(OBJ_DIR)/%.o: %.C
#	@/bin/echo "*** $< ***" >> $(ERRORFILE)
#	@/bin/echo -n $< " "
#	-@rm -f $(OBJ_DIR)/$*.o
#	@/bin/echo "Compile: $*.C" >> $(ERRORFILE)
#	-@$(CXX) $(CXXFLAGS) -c $*.C -o $@ 2>> $(ERRORFILE)

.C.ln:
$(SPLINT_DIR)/%.ln : %.C
	@touch $(SPLINT_DIR)/$*.ln

.C.o:
$(OBJ_DIR)/%.o: %.C $(SPLINT_DIR)/%.ln
	-@rm -f $(OBJ_DIR)/$*.o
	$(CXX) $(CXXFLAGS) -c $*.C -o $@

##
# Older style that suppressed build commands
##
#.cpp.o:
#$(OBJ_DIR)/%.o: %.cpp
#	@/bin/echo "*** $< ***" >> $(ERRORFILE)
#	@/bin/echo -n $< " "
#	-@rm -f $(OBJ_DIR)/$*.o
#	@/bin/echo "Compile: $*.cpp" >> $(ERRORFILE)
#	-@$(CXX) $(CXXFLAGS) -c $*.cpp -o $@ 2>> $(ERRORFILE)

.cpp.ln:
$(SPLINT_DIR)/%.ln : %.cpp
	@touch $(SPLINT_DIR)/$*.ln

.cpp.o:
$(OBJ_DIR)/%.o: %.cpp $(SPLINT_DIR)/%.ln
	-@rm -f $(OBJ_DIR)/$*.o
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $@
	touch $(SPLINT_DIR)/%.ln

.cc.ln:
$(SPLINT_DIR)/%.ln : %.cc
	@touch $(SPLINT_DIR)/$*.ln

.cc.o:
$(OBJ_DIR)/%.o: %.cc $(SPLINT_DIR)/%.ln
	-@rm -f $(OBJ_DIR)/$*.o
	$(CXX) $(CXXFLAGS) -c $*.cc -o $@
	touch $(SPLINT_DIR)/%.ln


##
# Older style that suppressed build commands
##
#$(OBJ_DIR)/%.o : $(LINT_DIR)/%.ln
#	-@rm -f $(OBJ_DIR)/$*.o
#	@/bin/echo "Compile: $*.c" >> $(ERRORFILE)
#	-@$(CC) $(CFLAGS) -o $@ $*.c 2>> $(ERRORFILE)

$(OBJ_DIR)/%.o : $(SPLINT_DIR)/%.ln
	-@rm -f $(OBJ_DIR)/$*.o
	$(CC) $(CFLAGS) -o $@ $*.c

CleanErrorFile:
	@rm -f $(ERRORFILE)
	@/bin/echo "" >  $(ERRORFILE)

DisplayStartMessage:
	@printf "%s %-41s %s\n" "[ ------- Source building in" "`pwd | sed 's+'${DEVBASE}'/++g'`" "------- ]"

DisplayErrorFile:
	@cat $(ERRORFILE)
	@/bin/echo -n ""

clean:
	@printf "%s %-51s %s\n" "[ ------- Cleaning" "`pwd | sed 's+'${DEVBASE}'/++g'`" "------- ]"
	@mydirs="${SUBDIRS}"; for i in $$mydirs ; do (cd $$i; $(MAKE) clean); done
	@rm -f $(SOURCE_TARGETS) $(END_TARGET) $(SPLINT_DIR)/*.ln $(DEPEND_FILE) $(CLEAN_IPTSCONFIG_VARS) $(EXTRA_CLEAN)

BuildDepend: $(DEPEND_FILE)

$(DEPEND_FILE):
	@$(MAKE) depend

depend:
	@printf "%s %-41s %s\n" "[ ------- Depend building in" "`pwd | sed 's+'${DEVBASE}'/++g'`" "------- ]"
	@$(RM) -f $(DEPEND_FILE)
	@ ([ -e $(IPTSCONFIG_VARS_CFG) ] && $(MAKE) $(IPTSCONFIG_VARS_C)) || true
	@touch $(DEPEND_FILE)
	@mydirs="${SUBDIRS}"; for i in $$mydirs ; do \
	(ls -1 $$i/$(OBJ_DIR)/*.o 2>/dev/null |sed "s+^+$(END_TARGET): +" >> $(DEPEND_FILE); /bin/echo -n $$i " "); done
	@mydirs=`/bin/echo $(SOURCE_TARGETS) | sed "s/$(OBJ_DIR)\///g" | sed "s/\.o//g"` ; for i in $$mydirs ; do \
	(if [ -r $$i.c ] ; then $(DEPCC) -M -x c $(DEFINES) $(INCDIRS) $$i.c | sed "s/^  /$(SPLINT_DIR)\/$$i.o: /g" | sed "s/\.o[ ]*:/\.ln:/g" | sed "s/\\\\//g" >> $(DEPEND_FILE); /bin/echo -n $$i.c " "; fi;\
   if [ -r $$i.C ] ; then $(DEPCXX) -M -x c++ $(DEFINES) $(INCDIRS) $$i.C | sed "s/^  /$(SPLINT_DIR)\/$$i.o: /g" | sed "s/\.o[ ]*:/\.ln:/g" |grep -v Cstd | sed "s/\\\\//g" >> $(DEPEND_FILE); /bin/echo -n $$i.C " "; fi;\
   if [ -r $$i.cpp ] ; then $(DEPCXX) -M -x c++ $(DEFINES) $(INCDIRS) $$i.cpp | sed "s/^  /$(SPLINT_DIR)\/$$i.o: /g" | sed "s/\.o[ ]*:/\.ln:/g" |grep -v Cstd | sed "s/\\\\//g" >> $(DEPEND_FILE); /bin/echo -n $$i.cpp " "; fi;\
   if [ -r $$i.cc ] ; then $(DEPCXX) -M -x c++ $(DEFINES) $(INCDIRS) $$i.cc | sed "s/^  /$(SPLINT_DIR)\/$$i.o: /g" | sed "s/\.o[ ]*:/\.ln:/g" |grep -v Cstd | sed "s/\\\\//g" >> $(DEPEND_FILE); /bin/echo -n $$i.cc " "; fi;\
  ); done
	@/bin/echo ""
