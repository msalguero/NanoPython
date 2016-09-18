#include <iostream>
#include <string>
#include <map>
#include <list>
using namespace std;
class GlobalVariable;
class Method;
class Expr;
typedef list<GlobalVariable *> GlobalVariableList;
typedef list<Method *> MethodList;
typedef list<Expr *> ExprList;

class Expr {
public:
	virtual void GenerateCode(string& code, string &place) = 0;
};

class AddExpression : public Expr
{
public:
	AddExpression(Expr* left, Expr* right){
		this->left = left;
		this->right = right;
	}
	void GenerateCode(string& code, string &place);
private:
	Expr* left;
	Expr* right;
};

class MultExpression : public Expr
{
public:
	MultExpression(Expr* left, Expr* right){
		this->left = left;
		this->right = right;
	}
	void GenerateCode(string& code, string &place);
private:
	Expr* left;
	Expr* right;
};

class SubExpression : public Expr
{
public:
	SubExpression(Expr* left, Expr* right){
		this->left = left;
		this->right = right;
	}
	void GenerateCode(string& code, string &place);
private:
	Expr* left;
	Expr* right;
};

class DivExpression : public Expr{
public:
	DivExpression(Expr* left, Expr* right){
		this->left = left;
		this->right = right;
	}
	void GenerateCode(string& code, string &place);
private:
	Expr* left;
	Expr* right;
};

class GreaterExpression : public Expr
{
public:
	GreaterExpression(Expr* left, Expr* right){
		this->left = left;
		this->right = right;
	}
	void GenerateCode(string& code, string &place);
private:
	Expr* left;
	Expr* right;
};

class LessExpression : public Expr
{
public:
	LessExpression(Expr* left, Expr* right){
		this->left = left;
		this->right = right;
	}
	void GenerateCode(string& code, string &place);
private:
	Expr* left;
	Expr* right;
};

class EqualExpression : public Expr
{
public:
	EqualExpression(Expr* left, Expr* right){
		this->left = left;
		this->right = right;
	}
	void GenerateCode(string& code, string &place);
private:
	Expr* left;
	Expr* right;
};

class NotEqualExpression : public Expr
{
public:
	NotEqualExpression(Expr* left, Expr* right){
		this->left = left;
		this->right = right;
	}
	void GenerateCode(string& code, string &place);
private:
	Expr* left;
	Expr* right;
};

class GreaterOrEqualExpression : public Expr
{
public:
	GreaterOrEqualExpression(Expr* left, Expr* right){
		this->left = left;
		this->right = right;
	}
	void GenerateCode(string& code, string &place);
private:
	Expr* left;
	Expr* right;
};

class LessOrEqualExpression : public Expr
{
public:
	LessOrEqualExpression(Expr* left, Expr* right){
		this->left = left;
		this->right = right;
	}
	void GenerateCode(string& code, string &place);
private:
	Expr* left;
	Expr* right;
};

class NumExpression : public Expr
{
public:
	NumExpression(int number){
		this->number = number;
	}
	void GenerateCode(string& code, string &place);
private:
	int number;
};

class IdExpression : public Expr
{
public:
	IdExpression(char* id){
		this->id = id;
	}
	void GenerateCode(string& code, string &place);
private:
	char* id;
};

class Sentence {
public:
	virtual string GenerateCode() = 0;
	string GenerateCodeList(){
		string code = this->GenerateCode();
		if (this->next != NULL)
			code += this->next->GenerateCodeList();
		return code;
	}
	Sentence* next = NULL;
};

class PrintSentence : public Sentence{
public:
	PrintSentence(Expr* printExpression){
		this->printExpression = printExpression;
		this->next = NULL;
	}
	string GenerateCode();
private:
	Expr* printExpression;
};

class AssignSentence : public Sentence{
public:
	AssignSentence(Expr* assignExpression, char* id){
		this->assignExpression = assignExpression;
		std::string str(id);
		this->id = str;
		this->next = NULL;
	}
	string GenerateCode();
private:
	Expr* assignExpression;
	string id;
};

class IfSentence : public Sentence{
public:
	IfSentence(Expr* condition, Sentence* trueBlock, Sentence* falseBlock){
		this->condition = condition;
		this->trueBlock = trueBlock;
		this->falseBlock = falseBlock;
		this->next = NULL;
	}
	string GenerateCode();
private:
	Expr* condition;
	Sentence* trueBlock = NULL;
	Sentence* falseBlock = NULL;
};

class WhileSentence : public Sentence{
public:
	WhileSentence(Expr* condition, Sentence* block){
		this->condition = condition;
		this->block = block;
		this->next = NULL;
	}
	string GenerateCode();
private:
	Expr* condition;
	Sentence* block = NULL;
};

class MethodCallSentence : public Sentence{
public:
	MethodCallSentence(string id, ExprList* parameters){
		this->id = id;
		this->parameters = parameters;
		this->next = NULL;
	}
	string GenerateCode();
	string GenerateParametersCode();
private:
	string id;
	ExprList* parameters;
};

class Parameter{
public:
	Parameter(){

	}
	//string GenerateCode();
private:
	string id;
	string type;
};

class ClassDefinition {
public:
	ClassDefinition(GlobalVariableList* variableList, MethodList* methodList){
		this->variableList = variableList;
		this->methodList = methodList;
	}
	string GenerateCode();
	string GenerateVariableCode();
	string GenerateMethodCode();
private:
	GlobalVariableList* variableList;
	MethodList* methodList;
};

class GlobalVariable{
public:
	GlobalVariable(Expr* condition, string id){
		this->condition = condition;
		this->id = id;
	}
	string GenerateCode();
	string id;
	Expr* condition;
};

class Method{
public:
	Method(string id, Sentence* block){
		this->id = id;
		this->block = block;
	}
	string GenerateCode();
private:
	string id;
	Sentence* block = NULL;
};