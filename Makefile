
SUBDIRS = 
SOURCE_TARGETS1 = \
	CompareRecordApp.o  \
	RecordComparator.o  \
	FixedRecord.o       \
	RecordDefinition.o  \
	RecordBase.o        \
	FileReader.o        \
	StringUtils.o       \
	CompareRecords.o

SOURCE_TARGETS2 = \
	HelloHackathon_test.o

END_TARGET1 = ./HelloHackathon.sh
END_TARGET2 = test

include app.mak

$(END_TARGET1) : $(SOURCE_TARGETS1) $(TARGET_LIBS)
	g++ -z muldefs -o $(END_TARGET1) $(SOURCE_TARGETS1)
	/bin/echo "$(END_TARGET1) linked"
	
$(END_TARGET2) : $(SOURCE_TARGETS2) $(TARGET_LIBS)
	g++ -z muldefs -o $(END_TARGET2) $(SOURCE_TARGETS2)
	/bin/echo "$(END_TARGET2) linked"

include $(DEPEND_KLUDGE)
