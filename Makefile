
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
	HelloHackathon_test.cpp

END_TARGET1 = HelloHackathon
END_TARGET2	= test

include app.mak

$(END_TARGET1) : $(SOURCE_TARGETS1) $(TARGET_LIBS)
	$(CXX) $(LDFLAGS) -z muldefs -o $(END_TARGET1) $(SOURCE_TARGETS1)
	/bin/echo "$(END_TARGET) linked"
	
$(END_TARGET2) : $(SOURCE_TARGETS2) $(TARGET_LIBS)
	$(CXX) $(LDFLAGS) -z muldefs -o $(END_TARGET2) $(SOURCE_TARGETS2)
	/bin/echo "$(END_TARGET) linked"
