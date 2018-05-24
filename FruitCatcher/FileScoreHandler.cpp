#include "FileScoreHandler.h"

FileScoreHandler::FileScoreHandler()
{

}

FileScoreHandler::~FileScoreHandler()
{

}
int FileScoreHandler::readFromfile(string filename) {
	ifstream myfile(filename);
	int value;
	if (myfile.is_open()) {
		string values;
		getline(myfile, values);
		value = stoi(values);
		myfile.close();
	}
	else {
		cout << "Unable to open File";
		exit(1);
	}
	return value;
}
void FileScoreHandler::writetoFile(string filename, string value) {
	ofstream myfile(filename);
	if (myfile.is_open()) {
		myfile << value;
		myfile.close();
	}
	else {
		cout << "Unable to open File";
		exit(1);
	}

}