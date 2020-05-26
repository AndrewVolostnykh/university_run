#include "TokenAnalyzer.h"
#include <vector>
#include <list>
#include <string>
using namespace std;

struct SynTree{
    int id;
    string name;
    int deep; 
};

class SyntaxAnalyzer{
	public:
		SyntaxAnalyzer(const vector<Token> &tokens);
		string getError();
		list<SynTree> getTree();
		bool analyze_success();
	private: 
		bool signal_program();
		bool program(int &token_index, int deep);
		bool block(int &token_index, int deep);
		bool constant_declarations(int &token_index, int deep);
		bool constant_declaration_list(int &token_index, int deep);
		bool constant_declaration(int &token_index, int deep);
		bool statements_list(int &token_index, int deep);
		bool statement(int &token_index, int deep);
		bool constant(int &token_index, int deep);
		bool identifier(int &token_index, int deep);
		bool unsigned_integer(int &token_index, int deep);
		
		void writeError(int row, int column, string expected, string found);
	
		bool success;
		vector<Token> tokens;
		string error;
		list<SynTree> tree;
};