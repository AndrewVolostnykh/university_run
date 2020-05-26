#include "SyntaxAnalyzer.h"

string SyntaxAnalyzer::getError(){
	return error;
}

list<SynTree> SyntaxAnalyzer::getTree(){
	return tree;
}

bool SyntaxAnalyzer::analyze_success(){
	return success;
}

void SyntaxAnalyzer::writeError(int row, int column, string expected, string found) {
    error = "SyntaxAnalyzer: Error (line " + to_string(row) + ", column " + to_string(column) + "): " +
            "'" + expected + "' expected but '" + found + "' found";
}

SyntaxAnalyzer::SyntaxAnalyzer(const vector<Token> &tokens_) {
    tokens = tokens_;
    if (!tokens.empty()) {
        tokens.push_back({0, tokens[tokens.size() - 1].line, 
							tokens[tokens.size() - 1].column,
							"end of file"});
    } else {
        return;
    }
	success = signal_program();
}

bool SyntaxAnalyzer::signal_program() {
    int token_index = 0, deep = 1;
    tree.push_back({0, "<signal-proogram>", 0});
    if (!program(token_index, deep))
        return false;
	
    if (tokens[++token_index].name != "end of file") {
        writeError(tokens[token_index].line, tokens[token_index].column,
                       "end of file", tokens[token_index].name);
        return false;
    }

    return true;
}

bool SyntaxAnalyzer::program(int &token_index, int deep){
	tree.push_back({0, "<program>", deep});
	deep++;
	
	if (tokens[token_index].name == "PROGRAM"){
		tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
		
		tree.push_back({0, "<procedure-identifier>", deep});
		deep++;
		bool procedure_identifier = identifier(++token_index, deep);
		if (procedure_identifier) {
			deep--;
			if (tokens[++token_index].name == ";"){
				tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
				
				if (block(++token_index, deep)) {
					
					if (tokens[++token_index].name == ".") {
						tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
						
						return true;
					} else {
						writeError(tokens[token_index].line, tokens[token_index].column
									, ".", tokens[token_index].name);
					}
				}
			} else {
				writeError(tokens[token_index].line, tokens[token_index].column
							, ";", tokens[token_index].name);
			}
		} else {
			writeError(tokens[token_index].line, tokens[token_index].column
						, "<procedure-identifier>", tokens[token_index].name);
		}
	} else {
		writeError(tokens[token_index].line, tokens[token_index].column
					, "PROGRAM", tokens[token_index].name);
	}
	
	return false;
}

bool SyntaxAnalyzer::block(int &token_index, int deep){
	tree.push_back({0, "<block>", deep});
	deep++;
	
	bool declarations = constant_declarations(token_index, deep);
	if (declarations) {
		
		if (tokens[++token_index].name == "BEGIN") {
			tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
			
			tree.push_back({0, "<statements-list>", deep});
			deep++;
			if (statements_list(++token_index, deep)) {
				deep--;
				if(tokens[++token_index].name == "END") {
					tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
					
					return true;
				} else {
					writeError(tokens[token_index].line, tokens[token_index].column
								, "END", tokens[token_index].name);
				}
			}
		} else {
			writeError(tokens[token_index].line, tokens[token_index].column
						, "BEGIN", tokens[token_index].name);
		}
	}
	
	return false;
}

bool SyntaxAnalyzer::constant_declarations(int &token_index, int deep) {
	tree.push_back({0, "<declarations>", deep});
	deep++;
	
	tree.push_back({0, "<constant-declarations>", deep});
	deep++;
	
	if (tokens[token_index].name == "CONST") {
	tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
	
			
			tree.push_back({0, "<constant-declaration-list>", deep});
			deep++;
			if (constant_declaration_list(++token_index, deep)) {
			
				return true;
			} else {
				return false;
			}
	} else if (tokens[token_index].name == "BEGIN") {
		tree.push_back({0, "<empty>", deep});
		--token_index;
		return true;
	} else {
		writeError(tokens[token_index].line, tokens[token_index].column
						, "<declarations>", tokens[token_index].name);
		return false;
	}
}

bool SyntaxAnalyzer::constant_declaration_list(int &token_index, int deep) {
	
	if (constant_declaration(token_index, deep)) {
		
		if (constant_declaration_list(++token_index, deep)) {
			return true;
		} else {
			return false;
		}
	} else if (tokens[token_index].name == "BEGIN"){
		tree.push_back({0, "<empty>", deep});
		--token_index;
		return true;
	}
	else {
		writeError(tokens[token_index].line, tokens[token_index].column
						, "<constant-declaration>", tokens[token_index].name);
		return false;
	}
}

bool SyntaxAnalyzer::constant_declaration(int &token_index, int deep) {
	tree.push_back({0, "<constant-declaration>", deep});
	deep++;
	
	tree.push_back({0, "<constant-identifier>", deep});
	deep++;
	
	bool constant_identifier = identifier(token_index, deep);
	
	if (constant_identifier) {
		deep--;
		if (tokens[++token_index].name == "=") {

			tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
			
			if (constant(++token_index, deep)) {

				if(tokens[++token_index].name == ";") {

					tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
					return true;
				} else {
					return false;
					writeError(tokens[token_index].line, tokens[token_index].column
						, ";", tokens[token_index].name);
				}
			} else {
				return false;
				writeError(tokens[token_index].line, tokens[token_index].column
						, "<constant>", tokens[token_index].name);
			}
		} else {
			return false;
			writeError(tokens[token_index].line, tokens[token_index].column
						, "=", tokens[token_index].name);
		}
	} else if (tokens[token_index].name != "BEGIN") {
		writeError(tokens[token_index].line, tokens[token_index].column
						, "<constant-identifier>", tokens[token_index].name);
		return false;
	}
	
	tree.pop_back();
	tree.pop_back();
	return false;
}

bool SyntaxAnalyzer::statements_list(int &token_index, int deep) {
	
	if (statement(token_index, deep)) {
		
		if (statements_list(++token_index, deep)) {
			return true;
		} else {
			return false;
		}
	} else if (tokens[token_index].name == "END"){
		tree.push_back({0, "<empty>", deep});
		--token_index;
		return true;
	}
	else {
		writeError(tokens[token_index].line, tokens[token_index].column
						, "<statement>", tokens[token_index].name);
		return false;
	}
}

bool SyntaxAnalyzer::statement(int &token_index, int deep) {
	tree.push_back({0, "<statement>", deep});
	deep++;
	
	tree.push_back({0, "<variable-identifier>", deep});
	deep++;
	
	bool variable_identifier = identifier(token_index, deep);
	
	if (variable_identifier) {
		deep--;
		if (tokens[++token_index].name == ":=") {
			tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
			
			if (constant(++token_index, deep)) {
				
				if(tokens[++token_index].name == ";") {
					tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
					return true;
				} else {
					writeError(tokens[token_index].line, tokens[token_index].column
						, ";", tokens[token_index].name);
				}
			} else {
				writeError(tokens[token_index].line, tokens[token_index].column
						, "<constant>", tokens[token_index].name);
			}
		} else {
			writeError(tokens[token_index].line, tokens[token_index].column
						, ":=", tokens[token_index].name);
		}
	} else if (tokens[token_index].name != "END"){
		writeError(tokens[token_index].line, tokens[token_index].column
						, "<variable-identifier>", tokens[token_index].name);
		return false;
	}
	
	tree.pop_back();
	tree.pop_back();
	return false;
}


bool SyntaxAnalyzer::identifier(int &token_index, int deep) {
	tree.push_back({0, "<identifier>", deep});
	deep++;
	
	if (tokens[token_index].code > 1000) {
		tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
		return true;
	}
	
	tree.pop_back();
	return false;
}

bool SyntaxAnalyzer::constant(int &token_index, int deep){
	tree.push_back({0, "<constant>", deep});
	deep++;
	
	if (tokens[token_index].name == "-") {
		tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
		 
		 if (unsigned_integer(++token_index, deep)) {
			 return true;
		 } else {
			 writeError(tokens[token_index].line, tokens[token_index].column
						, "<unsigned-integer>", tokens[token_index].name);
		 }
	} else if (unsigned_integer(token_index, deep)) {
		return true;
		
		} else {
			writeError(tokens[token_index].line, tokens[token_index].column
						, "<unsigned-integer>", tokens[token_index].name);
						
		}
		
	return false;
}

bool SyntaxAnalyzer::unsigned_integer(int &token_index, int deep) {
	tree.push_back({0, "<unsigned-integer>", deep});
	deep++;
	
	if (tokens[token_index].code > 500 && tokens[token_index].code <=1000) {
		tree.push_back({tokens[token_index].code, tokens[token_index].name, deep});
		return true;
	}
	
	return false;
}


	
				
		
		
	
	
	
		


		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		