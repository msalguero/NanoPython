#include <sstream>
#include "tree.h"

map<string,int> mapper;
int current_temp_offset = 4;
int label_count = 0;

string NewLabel(){
	return "label_" + to_string(label_count++);
}

string NewTemp(){
	string new_temp = "dword[ebp - " + to_string(current_temp_offset) + "]";
	current_temp_offset += 4;
	return new_temp;
}

void ResetOffset(){
	current_temp_offset = 4;
}

string ResolveId(string id){
	if(mapper.find(id) != mapper.end()){
	  int offset = mapper[id];
	  string dir = "dword[ebp - " + to_string(offset) + "]";
	  return dir;
	}
	
	mapper[id] = current_temp_offset;
	string dir = "dword[ebp - " + to_string(current_temp_offset) + "]";
	current_temp_offset += 4;
	return dir;
}

void AddExpression::GenerateCode(string& code, string &place ){
  
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"add eax, " + place2 + "\n" +
		"mov " +place + ", eax \n";
}

void SubExpression::GenerateCode(string& code, string &place ){
	string code1 , place1;
	string code2 , place2;

	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"sub eax, " + place2 + "\n" +
		"mov " +place + ", eax \n";
}

void MultExpression::GenerateCode(string& code, string &place ){
  
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"imul eax, " + place2 + "\n" +
		"mov " +place + ", eax \n";
}

void DivExpression::GenerateCode(string& code, string &place ){
  
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"xor edx, edx\n" +
		"idiv " + place2 + "\n" +
		"mov " +place + ", eax \n";
}

void GreaterExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setg al \n" +
		"movzx " +place + ", al \n";
}

void LessExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setl al \n" +
		"movzx " +place + ", al \n";
}

void GreaterOrEqualExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setge al \n" +
		"movzx " +place + ", al \n";
}

void LessOrEqualExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setle al \n" +
		"movzx " +place + ", al \n";
}

void EqualExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"sete al \n" +
		"movzx " +place + ", al \n";
}

void NotEqualExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setne al \n" +
		"movzx " +place + ", al \n";
}

void NumExpression::GenerateCode(string& code, string &place){
	code = "";
	place = to_string(number);
}

void IdExpression::GenerateCode(string& code, string &place){
	code = "";
	place = ResolveId(id);
}

string AssignSentence::GenerateCode(){
    string code, place;
      assignExpression->GenerateCode(code,place);
      stringstream ss;
      string address = ResolveId(this->id);
      ss << code << endl
      << "mov eax, " << place << endl
      << "mov " << address << ", eax" << endl;
      return ss.str();
}

string PrintSentence::GenerateCode(){
      string code, place;
      printExpression->GenerateCode(code,place);
      stringstream ss;
      ss << code << endl
      << "push " << place << endl
      << "push paddr(0x10000000)" << endl
      << "call @libc.printf" << endl
      << "add esp, 8" << endl;
      return ss.str();
}

string IfSentence::GenerateCode(){
	string label_else = NewLabel();
	string label_endif = NewLabel();
	string code, place;
	condition->GenerateCode(code,place);
	stringstream ss;
	ss << code << endl
	<< "cmp " << place << ", 0" << endl
	<< "je " << label_else << endl
	<< this->trueBlock->GenerateCodeList() << endl
	<< "jmp " << label_endif << endl
	<< label_else << ":" << endl;
	if(falseBlock != NULL){
		ss << this->falseBlock->GenerateCodeList() << endl;
	}
	ss << label_endif << ":" << endl;
	return ss.str();
}

string WhileSentence::GenerateCode(){
	string label_while = NewLabel();
	string label_endwhile = NewLabel();
	string code, place;
	condition->GenerateCode(code,place);
	stringstream ss;
	ss << code << endl
	<< label_while << ":" << endl
	<< "cmp " << place << ", 0" << endl
	<< "je " << label_endwhile << endl
	<< this->block->GenerateCodeList() << endl
	<< "jmp " << label_while << endl
	<< label_endwhile << ":" << endl;
	return ss.str();
}