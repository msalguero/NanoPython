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
		"xor ebx, ebx\n" +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setg bl \n" +
		"mov " +place + ", ebx \n";
}

void LessExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"xor ebx, ebx\n" +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setl bl \n" +
		"mov " +place + ", ebx \n";
}

void GreaterOrEqualExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"xor ebx, ebx\n" +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setge bl \n" +
		"mov " +place + ", ebx \n";
}

void LessOrEqualExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"xor ebx, ebx\n" +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setle bl \n" +
		"mov " +place + ", ebx \n";
}

void EqualExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"xor ebx, ebx\n" +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"sete bl \n" +
		"mov " +place + ", ebx \n";
}

void NotEqualExpression::GenerateCode(string& code, string &place){
	string code1 , place1;
	string code2 , place2;
	
	place = NewTemp();

	left->GenerateCode(code1, place1);
	right->GenerateCode(code2, place2);

	code = code1 + code2 +
		"xor ebx, ebx\n" +
		"mov eax, " + place1 + "\n" +
		"cmp eax, " + place2 + "\n" +
		"setne bl \n" +
		"mov " +place + ", ebx \n";
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
      << "push format_string" << endl
      << "call printf" << endl
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

string MethodCallSentence::GenerateCode(){
	int parameterSize = parameters->size();
	stringstream ss;
	ss << GenerateParametersCode() << endl
	<< "call " + id << endl;
	if(parameterSize > 0){
		ss << "add esp, " << to_string(parameterSize * 4) << endl;
	}
	return ss.str();
}

string MethodCallSentence::GenerateParametersCode(){
	if(parameters == NULL){
		return "";
	}
	stringstream ss;
	ExprList::iterator iter = parameters->begin(); 
	while(iter != parameters->end()){
		Expr* var = *iter;
		string code, place;
		var->GenerateCode(code, place);
		ss << code << endl;
		ss << "push " << place << endl;
		iter++;
	}
	return ss.str();
}

string ClassDefinition::GenerateCode(){
	stringstream ss;
	ss << "format ELF" << endl
	<< "extrn printf" << endl
	<< "public main" << endl
	<< "section '.data'" << endl
	<< "format_string db \"%d \", 0" << endl
	<< GenerateVariableCode() << endl
	<< "section '.text'" << endl
	<< GenerateMethodCode() << endl;
	return ss.str();
}

string ClassDefinition::GenerateVariableCode(){
	stringstream ss;
	GlobalVariableList::iterator iter = variableList->begin(); 
	while(iter != variableList->end()){
		GlobalVariable* var = *iter;
		ss << var->GenerateCode() << endl;
		iter++;
	}
	return ss.str();
}

string ClassDefinition::GenerateMethodCode(){
	stringstream ss;
	MethodList::iterator iter = methodList->begin(); 
	while(iter != methodList->end()){
		Method* var = *iter;
		ss << var->GenerateCode() << endl;
		iter++;
	}
	return ss.str();
}

string GlobalVariable::GenerateCode(){
	
	return this->id + " dd 5";
}

string Method::GenerateCode(){
	stringstream ss;
	string code = this->block->GenerateCodeList();
	ss << this->id << ":" << endl
	<< "push ebp" <<  endl
    << "mov ebp, esp" << endl
    << "sub esp, " << to_string(current_temp_offset - 4) << endl
    << code 
    << "leave" << endl
    << "ret" << endl;
    ResetOffset();
	return ss.str();
}