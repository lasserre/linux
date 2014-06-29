// logger.h

#pragma once

#include <iostream>
#include <fstream>

using namespace std;

#define MAX_NUM_FILES 3

enum LoggingMode
{
	AllModesOn,		
	RunModeOnly,	// Show formatted (readable) output
	CsvModeOnly		// Print csv-formatted output to stdout or filename if specified 
					// (if stdout, you can use with tee to construct .csv file)
};

class Logger
{
public:	

	static Logger* GetInstance();

	void SetMode(const LoggingMode& Mode, const string& Filename = "");	// If no filename specified, output > stdout
	void SetFileX(const int& FileIndex, const string& Filename, const bool& AppendMode);

	// Executed unconditionally
	void Log(const string& Output, const bool& ToFile = false, const int& FileIndex = 0, const bool& BoldText = false);
	void LogLine(const string& Line = "", const bool& ToFile = false, const int& FileIndex = 0, const bool& BoldText = false);
	void LogIndent(const string& Output, const int& NumIndentions = 1);
	void LogIndentLine(const string& Output, const int& NumIndentions = 1);

	// Executed based on current mode
	void Log_Run(const string& Output, const bool& Bold = false);
	void LogLine_Run(const string& Line, const bool& Bold = false);
	void LogIndent_Run(const string& Output, const int& NumIndentions = 1);
	void Log_Csv(const string& Value, const int& FileIndex = 0);
	void LogLine_Csv(const string& Line, const int& FileIndex = 0);

protected:
	LoggingMode mode;
	string filename;
	ofstream outputFiles[MAX_NUM_FILES];
	bool beginningNewLine;

	void PrintToCout(const string& OutputString, const bool& BoldText);

private:
	Logger();
	~Logger();
	static bool constructed;
	static Logger* instance;
	static const char ESC = 27;
};