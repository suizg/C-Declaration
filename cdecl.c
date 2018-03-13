#include<stdio.h>
#include<ctype.h>
#include<assert.h>
#include<string.h>
#define MAXTOKENS 100
#define MAXTOKENLEN 64

enum type_tag {IDENTIFIER,QUALIFIER,TYPE};

struct Token {
	char type;
	char string[MAXTOKENLEN];
};

int top = -1;
struct Token stack[MAXTOKENLEN];
struct Token _this;

#define POP stack[top--]
#define PUSH(s) stack[++top]=s

int atoi(char str[])
{
	int i, n;
	n = 0;
	for (i = 0; str[i] >= '0' && str[i] <= '9'; ++i)
		n = 10 * n + (str[i] - '0');
	return n;
}

char* _strcpy(char *dest, char *src)
{
	assert(dest!=NULL && src!=NULL);
	char *res = dest;
	while ((*dest++ = *src++) != '\0')
		;
	return res;
}

enum type_tag classify_string()   //判断标识符的类型
{
	char *s = _this.string;
	if (!strcmp(s, "const")) {
		_strcpy(s,"read-only");
		return QUALIFIER;
	}
	if (!strcmp(s, "volatile"))return QUALIFIER;
	if (!strcmp(s, "void"))return TYPE;
	if (!strcmp(s, "char"))return TYPE;
	if (!strcmp(s, "signed"))return TYPE;
	if (!strcmp(s, "unsigned"))return TYPE;
	if (!strcmp(s, "short"))return TYPE;
	if (!strcmp(s, "int"))return TYPE;
	if (!strcmp(s, "long"))return TYPE;
	if (!strcmp(s, "float"))return TYPE;
	if (!strcmp(s, "double"))return TYPE;
	if (!strcmp(s, "struct"))return TYPE;
	if (!strcmp(s, "union"))return TYPE;
	if (!strcmp(s, "enum"))return TYPE;
	return IDENTIFIER;
}

void gettoken(void)
{
	char *p = _this.string;   //读入下一个标记，保存在“this”中
	while ((*p = getchar()) == ' ');
	if (isalnum(*p)) {
		while (isalnum(*++p = getchar()));
		ungetc(*p, stdin);
		*p = '\0';
		_this.type = classify_string();
		return;
	}

	if (*p == '*') {
		_strcpy(_this.string, "pointer to ");
		_this.type = '*';
		return;
	}

	_this.string[1] = '\0';
	_this.type = *p;
	return;
}

void read_to_first_identifer()
{
	gettoken();
	while (_this.type != IDENTIFIER) {
		PUSH(_this);
		gettoken();
	}
	printf("%s is ",_this.string);
	gettoken();
}

void deal_with_arrays() {
	while (_this.type == '[') {
		printf("array ");
		gettoken();
		if (isdigit(_this.string[0])) {
			printf("0..%d ",atoi(_this.string)-1);
			gettoken();
		}
		gettoken();
		printf("of ");
	}
}

void deal_with_function_args()
{
	while (_this.type != ')') {
		gettoken();
	}
	gettoken();
	printf("function return ");
}

void deal_with_pointers() {
	while (stack[top].type == '*') {
		printf("%s ",POP.string);
	}
}

void deal_with_declartor()
{
	switch (_this.type)
	{
	case '[':deal_with_arrays(); break;
	case '(':deal_with_function_args();
	}

	deal_with_pointers();

	while (top >= 0) {
		if (stack[top].type == '(') {
			POP;
			gettoken();
			deal_with_declartor();
		}
		else {
			printf("%s ",POP.string);
		}
	}
}

int main()
{
	read_to_first_identifer();
	deal_with_declartor();
	printf("\n");
	return 0;
}
