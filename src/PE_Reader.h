#ifndef PE_Reader_H
#define PE_Reader_H
/**
 @class Reader
	Abstract class 
*/ 
class Reader{
protected:
	char* filename; ///name of the input file
	FILE* fp; /// file pointer
public:
/**
    Load file in buffer for Static Analise
	@param buf buffer
	@param len buffer size
	@param start entry point =0 in network traffic search
*/
	virtual void LoadForStaticAnalise(char* & buf, unsigned int & len, unsigned int & start)=0; 
/**
    Load entire file in buffer for Dynamic Analise
	@param buf buffer
	@param len buffer size
	@param start entry point  
	@param base base=0x20000000L
	@param offset  
*/												
	virtual void LoadForDynamicAnalise(char* &buf, unsigned int& len, 
		unsigned int & start, unsigned int &base, unsigned int & offset)=0;
};
/**
 @class Raw_Reader
 Reads file in *.Raw format. Format is used in network traffic search.
*/ 

class Raw_Reader:public Reader{
public:
/**
  Constructor
  @param name file name
*/	 
	Raw_Reader(const char* name);
/**
    Load entire file in buffer for Static Analise
	@param buf buffer
	@param len buffer size
	@param start entry point =0 in network traffic search
*/ 
	virtual void LoadForStaticAnalise(char* & buf, unsigned int & len, 
												unsigned int & start);
/**
    Load entire file in buffer for Dynamic Analise
	@param buf buffer
	@param len buffer size
	@param start entry point =0 in network traffic search
	@param base is 0x20000000L
	@param offset is 0 in network traffic search 
*/	
	virtual void LoadForDynamicAnalise(char* & buf, unsigned int & len, 
		unsigned int & start, unsigned int & base, unsigned int & offset);
/**
	Destructor
*/	
	virtual ~Raw_Reader();
};
/**
 @class PE_Reader 
	Reader for Portable Executable format
*/ 
class PE_Reader:public Reader{
	int header_offset; /// offest of PE header
	int table_offset; /// offest of PE table
	int number_of_entries; /// number of sections
	unsigned int text_offset; /// offset of text section
	unsigned int data_offset; /// offset of data section
	unsigned int text_size; /// size of text section
	unsigned int data_size; /// size of data section
	unsigned int entry_point_rva; /// rva address of entry point
	unsigned int image_base; /// image base
	unsigned int text_address_rva; /// rva address of text sectioon
public:
/**
 Constructor
 @param name file name
*/ 
	PE_Reader(const char* name);
/**
 Copy Constructor
 @param pe_reader
*/ 
	PE_Reader(const PE_Reader* pe_reader);
/**
 Destructor
*/ 
	virtual ~PE_Reader();
/**
 Load .text section for Static Analise
 @param buf buffer
 @param len size of buffer
 @param start physical entry point offset from code section
*/ 
    virtual void LoadForStaticAnalise(char* &buf, unsigned int& len, 
												unsigned int& start);
/**
 Load entire file for Dynamic Analise
 @param buf buffer
 @param len size of buffer
 @param start physical original entry point offset 
 @param base is 0x20000000L
 @param offset physical text section offset
*/
	virtual void LoadForDynamicAnalise(char* &buf, unsigned int& len, 
										unsigned int & start, 
										unsigned int &base, 
										unsigned int & offset);
/**
 Finds offset offset of PE header
*/ 
	void PE_HeaderOffset();
/**
 Finds offset offset of PE table
*/
	void PE_TableOffset();
/**
 Controls Format of PE file. If (format !=PE) then exit(1) 
*/ 
	void PE_FileType() const;
/**
 Finds number of sections in table
*/ 	
	void PE_NumberOfEntries();
/**
 Finds entry point rva
*/ 
	void PE_EntryPoint();
/**
 Finds image base
*/ 
	void PE_ImageBase();
/**
 Finds code section. Many obphuscated files doesn't contain .text or.CODE section. In this case we suppose that code section contains entry point address.
*/ 	
	void PE_ObjectTable();
/**
 Works with code section in PE table
 @param section_offset code section rva
*/ 
	void PE_CodeSection(int section_offset);
/**
 Works with data section in PE table
 @param data_offset data section rva
*/
	void PE_DataSection(int data_offset);
};

#endif
