
SUBDIRS = 
SOURCE_TARGETS = \
	CompareRecordApp.o  \
	RecordComparator.o  \
	FixedRecord.o       \
	RecordDefinition.o  \
	RecordBase.o        \
	FileReader.o        \
	StringUtils.o       \
	CompareRecords.o

END_TARGET = CompareRecords

include app.mak

$(END_TARGET) : $(SOURCE_TARGETS) $(TARGET_LIBS)
	g++ -z muldefs -o $(END_TARGET) $(SOURCE_TARGETS)
	/bin/echo "$(END_TARGET) linked"

include $(DEPEND_KLUDGE)
