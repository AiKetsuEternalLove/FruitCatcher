#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class FileScoreHandler
{
public:
	FileScoreHandler();
	~FileScoreHandler();
	int readFromfile(string filename);
	void writetoFile(string filename, string value);
private:

};
