typedef struct Variable
{
	const char *name;
	unsigned int scope;
	unsigned int line;
} Variable;

typedef struct Function
{
	const char *name;
	unsigned int scope;
	unsigned int line;
} Function;

enum SymbolType
{
	GLOBAL,
	LOCAL,
	FORMAL,
	USERFUNC,
	LIBFUNC
};

typedef struct SymbolTableEntry
{
	bool isActive;
	union
	{
		Variable *varVal;
		Function *funcVal;
	} value;
	
	enum SymbolType type;
	
} SymbolTableEntry;
