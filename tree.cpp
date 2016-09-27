#include <sstream>
#include "tree.h"

map<string,int> globalTable;
map<string, int> context;
int current_temp_offset = 4;
int label_count = 0;
int global_var = 10000000;

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

string InsertToSymbolTable(string id){
	context[id] = current_temp_offset;
	string dir = "dword[ebp - " + to_string(current_temp_offset) + "]";

	current_temp_offset += 4;
	return dir;
}

int SearchGlobal(string id){
	if(globalTable.find(id) != globalTable.end()){
	  return globalTable[id];
	}else{
		return -1;
	}
}

string ResolveId(string id){
	if(context.find(id) != context.end()){
		int offset = context[id];
		cout << offset<< endl;
		string sign = offset >= 0 ? "-" : "+";
		string dir = "dword[ebp " + sign + to_string(abs(offset)) + " ]";
		return dir;
	}
	int found = SearchGlobal(id);
	if(found != -1){
		string dir = "dword[esi + " + to_string(found) + "]";
		return dir;
	}

	return InsertToSymbolTable(id);
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

void MethodCallExpr::GenerateCode(string& code, string &place){
	int parameterSize = parameters != NULL ? parameters->size() : 0;
	stringstream ss;
	ss << GenerateParametersCode() << endl
	<< "call " + id << endl;
	if(parameterSize > 0){
		ss << "add esp, " << to_string(parameterSize * 4) << endl;
	}
	place = NewTemp();
	ss << "mov " << place << ", eax";
	code = ss.str();
}

string MethodCallExpr::GenerateParametersCode(){
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
      //<< "push " << place << endl
      //<< "push format_string" << endl
      //<< "call printf" << endl
      << "#show " << place << endl;
      //<< "add esp, 8" << endl;
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
	ss << label_while << ":" << endl
	<< code << endl
	<< "cmp " << place << ", 0" << endl
	<< "je " << label_endwhile << endl
	<< this->block->GenerateCodeList() << endl
	<< "jmp " << label_while << endl
	<< label_endwhile << ":" << endl;
	return ss.str();
}

string ForSentence::GenerateCode(){
	string label_for = NewLabel();
	string label_endfor = NewLabel();
	string code1, place1, code2, place2;
	from->GenerateCode(code1,place1);
	to->GenerateCode(code2, place2);
	stringstream ss;
	ss << code1 << code2 << endl
	<< label_for << ":" << endl
	<< "cmp " << place1 << ", " << place2 << endl
	<< "jge " << label_endfor << endl
	<< this->block->GenerateCodeList() << endl
	<< "inc " << place1 << endl
	<< "jmp " << label_for << endl
	<< label_endfor << ":" << endl;
	return ss.str();
}

string ReturnSentence::GenerateCode(){
	string code, place;
	returnValue->GenerateCode(code, place);
	stringstream ss;
	ss << code << endl
	<< "mov eax, " << place << endl;
	return ss.str();
}

string MethodCallSentence::GenerateCode(){
	int parameterSize = parameters != NULL ? parameters->size() : 0;
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
	//ss << "format ELF" << endl
	//<< "extrn printf" << endl
	//<< "public main" << endl
	//<< "section '.data'" << endl
	string code = "mov esi, " + to_string(global_var);
	//<< "format_string db \"%d \", 0" << endl
	ss << code << endl
	<< GenerateVariableCode() << endl
	<< "call main" << endl
	<< "#stop" << endl
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
	
	string place, code;
	expression->GenerateCode(code, place);
	code = code + "mov dword[0x"+to_string(global_var)+"], " + place + "\n";
	global_var += 4;
	return code;
}

void Method::RegisterParameters(){
	int position = 8;
	ParameterList::iterator iter = parameters->begin(); 
	while(iter != parameters->end()){
		Parameter* var = *iter;
		string id(var->id);
		context[id] = position * -1;
		position += 4;
		iter++;
	}
}

string Method::GenerateCode(){
	stringstream ss;
	map<string, int> methodTable;
	if(parameters != NULL)
		RegisterParameters();
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