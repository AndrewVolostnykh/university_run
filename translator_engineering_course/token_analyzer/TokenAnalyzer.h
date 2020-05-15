#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

struct Token {
	int code;
	int line;
	int column;

	string name;
};

class TokenAnalyzer {
public:

	int token_counter;
	int line_counter;
	int save_line;
	int column_counter;
	int save_column;
	int token_error_code;

	int idn_counter;
	int min_idn;

	int digit_counter;
	int min_digit;
	int digit_position;
	int keywords_min;

	int file_size;

	int ascii_arr[128];
	Token buffer;
	vector<Token> tokens;
	vector<string> idn_table;
	vector<string> numbers_table;
	vector<string> standart_keywords{ "PROGRAM", "BEGIN", "END", "CONST" };
	vector<Token> key_words;
	vector<string> mdm{ ":=" };
	vector<Token> mdm_table;
	
	vector<int> com_err_codes;

	int position;
	char sbuff;
	fstream file;

	TokenAnalyzer();
	void Analyzer(string filename);
	void AddToken(int Code, int Line, int Column, string Name);
	bool SizeOut();
	void INP();
	void SPACE();
	void Digit();
	void IDN();
	void BCOM();
	void COM(string Buf);
	void ECOM(string Buf);
	void MDM();
	void DM();
	void ERR(string pt);
	int SearchIdent(string Ident);
	int SearchStandartIdent(string Ident);
	int SearchDigit(string Digit);
	int SearchMDM(string MDM);
	int SearchErrors(string ERR);
	void MakeListing(string file);
	void setAscii();
};