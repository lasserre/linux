#include "logger.h"

// static singleton variable initializations

bool Logger::constructed = false;
Logger* Logger::instance = NULL;

Logger::Logger() : mode(RunModeOnly)
	, filename("")	
	, beginningNewLine(true)
{
}

Logger::~Logger()
{
	for (int i = 0; i < MAX_NUM_FILES; i++)
	{
		if (outputFiles[i].is_open())	
			outputFiles[i].close();
	}

	constructed = false;
	instance = NULL;
}

Logger* Logger::GetInstance()
{
	if (!constructed)
	{
		instance = new Logger();
		constructed = true;
	}

	return instance;
}

void Logger::SetMode(const LoggingMode& Mode, const string& Filename /* = "" */)
{
	this->mode = Mode;
	this->filename = Filename;
	if (filename != "")
	{
		if (outputFiles[0].is_open())
			outputFiles[0].close();

		outputFiles[0].open(filename.c_str(), ios::trunc);
	}
	return;
}

void Logger::SetFileX(const int& FileIndex, const string& Filename, const bool& AppendMode)
{
	if (FileIndex < MAX_NUM_FILES)
	{
		if (outputFiles[FileIndex].is_open())
			outputFiles[FileIndex].close();

		if (AppendMode)
			outputFiles[FileIndex].open(Filename.c_str(), ios::app);
		else
			outputFiles[FileIndex].open(Filename.c_str(), ios::trunc);
	}
	return;
}

void Logger::Log_Run(const string& Output, const bool& Bold /* = false*/)
{
	if (mode == RunModeOnly || mode == AllModesOn)
		Log(Output, false, 0, Bold);
}

void Logger::LogIndent_Run(const string& Output, const int& NumIndentions /* = 1*/)
{
	if (mode == RunModeOnly || mode == AllModesOn)
		LogIndent(Output, NumIndentions);
}

void Logger::LogLine_Run(const string& Line, const bool& Bold /* = false*/)
{
	if (mode == RunModeOnly || mode == AllModesOn)
		LogLine(Line, false, 0, Bold);
}

void Logger::Log_Csv(const string& Value, const int& FileIndex /* = 0 */)
{
	if (mode == CsvModeOnly || mode == AllModesOn)
	{
		bool writeToFile = false;

		if (filename != "" && outputFiles[FileIndex].is_open())
		{
			writeToFile = true;

			if (!beginningNewLine)
				outputFiles[FileIndex] << ",";			
		}
		else
		{
			if (!beginningNewLine)
				cout << ",";
		}

		Log(Value, writeToFile, FileIndex);
	}
	return;
}

void Logger::LogLine_Csv(const string& Line, const int& FileIndex /* = 0 */)
{
	if (mode == CsvModeOnly || mode == AllModesOn)
	{
		bool writeToFile = false;

		if (filename != "" && outputFiles[FileIndex].is_open())
		{
			writeToFile = true;

			if (!beginningNewLine)
				outputFiles[FileIndex] << ",";
		}
		else
		{
			if (!beginningNewLine)
				cout << ",";
		}
		
		LogLine(Line, writeToFile, FileIndex);
	}
	return;
}

void Logger::Log(const string& Output, const bool& ToFile /* = false */, const int& FileIndex /*= 0*/, const bool& BoldText /*= false*/)
{
	if (ToFile)
	{
		if (outputFiles[FileIndex].is_open())
		{
			outputFiles[FileIndex] << Output;
			beginningNewLine = false;
		}
	}
	else
	{
		PrintToCout(Output, BoldText);
		beginningNewLine = false;
	}
	return;
}

void Logger::LogLine(const string& Line /* = "" */, const bool& ToFile /* = false */, const int& FileIndex /*= 0*/, const bool& BoldText /*= false*/)
{
	if (ToFile)
	{
		if (outputFiles[FileIndex].is_open())
		{
			outputFiles[FileIndex] << Line << endl;
			beginningNewLine = true;
		}
	}
	else
	{
		PrintToCout(Line + "\n", BoldText);
		beginningNewLine = true;
	}
	return;
}

void Logger::LogIndent(const string& Output, const int& NumIndentions /* = 1 */)
{
	for (int i = 0; i < NumIndentions; i++)
		cout << "\t";
	cout << Output;
	beginningNewLine = false;
	return;
}

void Logger::LogIndentLine(const string& Output, const int& NumIndentions /* = 1 */)
{
	LogIndent(Output, NumIndentions);
	LogLine();
	return;
}

void Logger::PrintToCout(const string& OutputString, const bool& BoldText)
{
	if (BoldText)
	{
		printf("%c[1m", ESC);
		cout << OutputString;
		printf("%c[0m", ESC);
	}
	else
	{
		cout << OutputString;
	}	
	return;
}