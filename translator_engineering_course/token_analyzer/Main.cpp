#include "TokenAnalyzer.h"

int main(int argc, char **argv)
{
	string input_path;

	if (argv[1] == nullptr) {
		input_path = "";
	}
	else {
		input_path = argv[1];
		input_path += "/";
	}

	string File = "./";
	TokenAnalyzer analyzer;
	analyzer.Analyzer(File + input_path);
	return 0;
}