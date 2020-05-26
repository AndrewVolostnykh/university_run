#include "Printer.h"
using namespace std;

void PrintSyntaxAnalyzerTree(const string &path, list<SynTree> syntax_tree){
	ofstream fout(path + "/generated.txt");
	
	for (const auto syn_branch : syntax_tree) {
		for (int i = 0; i < syn_branch.deep; i++) {
			fout << "|   ";
		}
		if (syn_branch.id != 0) {
			fout << setw(6) << left << syn_branch.id;
		}
		fout << syn_branch.name << endl;
	}
	fout << endl << endl;
	
	fout.close();
}

void PrintSynErrors(string path, string error){
	ofstream fout(path + "/errors.txt");
	fout << error;
	fout << endl << endl;
	
	fout.close();
}

void PrintTokenErrors(string path, vector<Token> tokens_errors){
	ofstream fout(path + "/errors.txt");
	
	for(auto error : tokens_errors){
		fout << "Token Analyzer: " << error.code << ", at line: " << error.line << ", at column: " << error.column << ", error: " << error.name << endl;
	}
	fout << endl << endl;
	
	fout.close();
}
