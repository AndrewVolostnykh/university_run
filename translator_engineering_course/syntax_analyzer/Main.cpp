#include "Printer.h"
using namespace std;

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
	cout << "Starting token analyzing...\n";
	TokenAnalyzer analyzer;
	analyzer.Analyzer(File + input_path);
	cout << "Token analyzer finished.\n";
	
	if (analyzer.isSuccess()){
		cout << "Starting syntax analyzing...\n";
		SyntaxAnalyzer syntaxAnalyzer(analyzer.getTokensTable());
		if(syntaxAnalyzer.analyze_success()){
			PrintSyntaxAnalyzerTree(File + input_path, syntaxAnalyzer.getTree());
		} else {
			cout << "Syntax analyzer have find errors, check log file: " << File << input_path << "errors.txt";
			PrintSyntaxAnalyzerTree(File + input_path, syntaxAnalyzer.getTree());
			PrintSynErrors(File + input_path, syntaxAnalyzer.getError());
		}
	} else { 
		cout << "Token Analyzer have errors";
		PrintTokenErrors(File + input_path, analyzer.getErrors());
	}

 	return 0;
}