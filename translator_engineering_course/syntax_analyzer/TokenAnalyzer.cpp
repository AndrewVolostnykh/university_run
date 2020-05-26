#include "TokenAnalyzer.h"
#include <algorithm>

vector<Token> TokenAnalyzer::getTokensTable(){
	return tokens;	
}

vector<Token> TokenAnalyzer::getErrors(){
	return errors;
}

bool TokenAnalyzer::isSuccess(){
	return success;
}

void TokenAnalyzer::setAscii() {
	for (int i = 0; i < 128; i++) {
		if ((i == 9) || (i == 10) || (i == 32) || (i == 12) || (i == 13)) {
			ascii_arr[i] = 0;
			continue;
		}

		if ((i < 91) && (i > 64)) {
			ascii_arr[i] = 1;
			continue;
		}

		if ((i < 58) && (i > 47) || (i == 45)) {
			ascii_arr[i] = 2;
			continue;
		}

		if (i == ':') {
			ascii_arr[i] = 3;
			continue;
		}

		if ((i == ')') || (i == '(') || (i == '*') || (i == '.') || (i == ';') || (i == '=')) {
			ascii_arr[i] = 4;
			continue;
		}

		ascii_arr[i] = 5;
	}

}

TokenAnalyzer::TokenAnalyzer()
{
	// initialize required veriables 
	token_counter = 1;
	line_counter = 1;
	column_counter = 1;
	token_error_code = 2000;
	min_idn = 1001;
	min_digit = 501;
	keywords_min = 401;
	idn_counter = min_idn;
	digit_counter = min_digit;

	// adding our keywords to vector
	buffer.name = "PROGRAM";
	buffer.code = 401;
	key_words.push_back(buffer);

	buffer.name = "BEGIN";
	buffer.code = 402;
	key_words.push_back(buffer);

	buffer.name = "END";
	buffer.code = 403;
	key_words.push_back(buffer);

	buffer.name = "CONST";
	buffer.code = 404;
	key_words.push_back(buffer);

	buffer.name = ":=";
	buffer.code = 301;
	mdm_table.push_back(buffer);

	setAscii();
}

void TokenAnalyzer::AddToken(int code, int line, int column, string name)
{
	Token buf;
	buf.code = code;
	buf.line = line;
	buf.column = column;
	buf.name = name;
	tokens.push_back(buf);
}

void TokenAnalyzer::AddErrors(int code, int line, int column, string name){
	Token err;
	err.code = code;
	err.line = line;
	err.column = column;
	err.name = name;
	errors.push_back(err);
}

bool TokenAnalyzer::SizeOut()
{
	return !file.eof();
}

void TokenAnalyzer::Analyzer(string filename)
{
	file.open(filename + "input.sig");
	if (!file.is_open()) {
		std::cout << "failed to open " << filename << '\n';
	}
	position = 0;
	line_counter = 1;
	column_counter = 1;
	sbuff = file.get();
	if (SizeOut())
		INP();
	MakeListing(filename);
}

void TokenAnalyzer::INP()
{
	while (SizeOut())
	{
		switch (ascii_arr[sbuff])
		{
		case 0:
			SPACE();
			break;
		case 1:
			IDN();
			break;
		case 2:
			Digit();
			break;
		case 3:
			MDM();
			break;
		case 4:
			DM();
			break;
		case 5:
			ERR("");
			break;
		}
	}

}

void TokenAnalyzer::SPACE()
{
	if (SizeOut())
	{
		while ((SizeOut()) && (ascii_arr[sbuff] == 0))
		{
			if (sbuff == 10)
			{
				position++; column_counter++;
				sbuff = file.get();
				line_counter++;
				column_counter = 1;
			}
			else {
				position++; column_counter++;
				sbuff = file.get();
			}
		}
		return;
	}
}

void TokenAnalyzer::Digit() {
	save_line = line_counter;
	save_column = column_counter;
	string buf = "";
	while ((SizeOut()) && (ascii_arr[sbuff] == 2))
	{
		if(sbuff == 45)
		{
			AddToken(sbuff, save_line, save_column, "-"); // finding and saving - as DM 
			position++;
			sbuff = file.get();
			column_counter++;
			continue;
		}
		buf += sbuff;
		position++;
		sbuff = file.get();
		column_counter++;
	}

	switch (ascii_arr[sbuff])
	{
	case 0:
	case 3:
	case 4:
	{
		int n = SearchDigit(buf);
		if (n == -1)
		{
			AddToken(digit_counter, save_line, save_column, buf);
			numbers_table.push_back(buf);
			digit_counter++;
		}
		else
		{
			AddToken(n + min_digit, save_line, save_column, buf);
		}
		buf = "";
		return;
	}
	break;
	case 1:
	case 5:
		ERR(buf);
		break;
	}
}

void TokenAnalyzer::IDN() {
	int n;
	save_line = line_counter;
	save_column = column_counter;
	string Buf = "";
	while ((SizeOut()) && ((ascii_arr[sbuff] == 2) || (ascii_arr[sbuff] == 1)))
	{
		Buf += sbuff;
		position++;
		sbuff = file.get();
		column_counter++;
	}

	if (ascii_arr[sbuff] == 5)
	{
		ERR(Buf);
	}
	else
	{
		n = SearchStandartIdent(Buf);
		if (n == -1) {
			n = SearchIdent(Buf); 
			if (n == -1)
			{
				AddToken(idn_counter, save_line, save_column, Buf);
				idn_table.push_back(Buf);
				idn_counter++;
			}
			else
			{
				AddToken(n + min_idn, save_line, save_column, Buf);
			}
		}
		else AddToken(key_words[n].code, save_line, save_column, key_words[n].name);
		Buf = "";
		return;
	}
}

void TokenAnalyzer::BCOM()
{
	position++;
	sbuff = file.get();
	column_counter++;
	COM("(*");
}

void TokenAnalyzer::COM(string Buf)
{
	while (SizeOut() && (sbuff != '*'))
	{
		if (sbuff == '\n')
		{
			line_counter++;
			column_counter = 1;
			position++;
			sbuff = file.get();
		}
		else {
			Buf += sbuff;
			position++;
			sbuff = file.get();
			column_counter++;
		}
	}
	if (!SizeOut()) {
		ERR(Buf);
	}
	else {
		ECOM(Buf);
	}
}

void TokenAnalyzer::ECOM(string Buf)
{
	sbuff = file.get();
	//Buf += sbuff;
	position++;
	column_counter++;
	if ((SizeOut()) && (sbuff == ')'))
	{
		position++;
		sbuff = file.get();
		column_counter++;
		return;
	}
	else if (!SizeOut()) {
		ERR(Buf);
	}
	else {
		COM(Buf);
	}
}

void TokenAnalyzer::MDM()
{
	int n;
	save_line = line_counter;
	save_column = column_counter;
	string Buf = "";
	if ((SizeOut()) && ((ascii_arr[sbuff] == 3) || (ascii_arr[sbuff] == 4)))
	{
		Buf += sbuff;
		position++;
		sbuff = file.get();
		column_counter++;
		if ((SizeOut()) && ((sbuff == ':') || (sbuff == '=')))
		{
		  Buf += sbuff;
		  position++;
		  sbuff = file.get();
		  column_counter++;
		}
	}

	if (ascii_arr[sbuff] == 5)
	{
		ERR(Buf);
	}
	else
	{
		n = SearchMDM(Buf);
		if (n == -1) {
			ERR(Buf);
		}
		else AddToken(mdm_table[n].code, save_line, save_column, mdm_table[n].name);
		Buf = "";
		return;
	}
}

void TokenAnalyzer::DM()
{
	save_line = line_counter;
	save_column = column_counter;
	string Buf = "";

	if (sbuff == '(')
	{
		Buf += sbuff;
		position++;
		sbuff = file.get();
		column_counter++;
		if (sbuff == '*') {
			BCOM();
		}
		else {
			//AddToken('(', line_counter, column_counter - 1, Buf);
			ERR(Buf);
			return;
		}
	}
	else {
		if (sbuff == '*')
		{
			Buf += sbuff;
			ERR(Buf);
		}
		else
		{
			Buf += sbuff;
			AddToken(sbuff, line_counter, column_counter, Buf);
			position++;
			sbuff = file.get();
			column_counter++;
			return;
		}
	}
}

void TokenAnalyzer::ERR(string pt)
{

	string Buf = pt;
	
	while ((ascii_arr[sbuff] != 0) && (ascii_arr[sbuff] != 3) && (SizeOut()))
	{
		Buf += sbuff;
		position++;
		sbuff = file.get();
		column_counter++;
	}
	
	if(Buf.at(0) == '(' && Buf.at(1) == '*')
	{
		com_err_codes.push_back(token_error_code);
		Buf = "Unclosed comment: " + Buf;
		//AddErrors(token_error_code, save_line, save_column, Buf);
	}
	
	AddErrors(token_error_code, save_line, save_column, Buf);
	token_error_code++;
	Buf = "";
	INP();
}

int TokenAnalyzer::SearchIdent(string Ident) {
	for (int i = 0; i < idn_table.size(); i++)
	{
		if (idn_table[i] == Ident)
			return i;
	}
	return -1;
}

int TokenAnalyzer::SearchStandartIdent(string Ident) {
	for (int i = 0; i < key_words.size(); i++)
	{
		if (key_words[i].name == Ident)
			return i;
	}
	return -1;
}

int TokenAnalyzer::SearchDigit(string Digit) {
	for (int i = 0; i < numbers_table.size(); i++)
	{
		if (numbers_table[i] == Digit)
			return i;
	}
	return -1;
}

int TokenAnalyzer::SearchMDM(string MDM) {
	for (int i = 0; i < mdm_table.size(); i++)
	{
		if (mdm_table[i].name == MDM)
			return i;
	}
	return -1;
}



void TokenAnalyzer::MakeListing(string file) {
	file = file + "generated.txt";
	ofstream f(file);
	f << setw(10) << "code" << setw(10) << "line" << setw(10) << "column" << setw(20) << "name" << endl;
	f << endl;
	for (int i = 0; i < tokens.size(); i++)
	{
		f << setw(10) << tokens[i].code << setw(10) << tokens[i].line << setw(10) << tokens[i].column << setw(20) << tokens[i].name << endl;
	}
	f << endl;
	f << endl;
	int p = 0;
	string message;
	if(!errors.empty()){
		success = false;
		for(int i = 0; i < errors.size(); i++)
		{
			if (errors[i].code >= 2000) {
				if(find(com_err_codes.begin(), com_err_codes.end(), errors[i].code) != com_err_codes.end()){ // finds comment errors end change error message 
					message = "Unclosed comment, error: ";
				} else {
					message = "Undefined token, error ";
				}
				f << message << errors[i].code <<  " in line: " << errors[i].line << " column " << errors[i].column << ": " << errors[i].name << endl;
				p++;
			}
		}
	}
	f.close();
	if (p == 0) {
		cout << "Token analysing finished" << endl;
	}
	else {
		for (int i = 0; i < errors.size(); i++)
		{
			cout << errors[i].code <<  " in line: " << errors[i].line << " column " << errors[i].column << ": " << errors[i].name << endl;

		}
	}
}