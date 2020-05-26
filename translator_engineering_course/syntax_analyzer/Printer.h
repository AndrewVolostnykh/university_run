#include "TokenAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>
using namespace std;

void PrintSyntaxAnalyzerTree(const string &path, list<SynTree> syntax_tree);

void PrintSynErrors(string path, string error);

void PrintTokenErrors(string path, vector<Token> tokens_errors);


