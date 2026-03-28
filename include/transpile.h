// Converts Nodes -> C
// Author: Evan Cassidy
// Date: 3/27/2026

// Imports
#include "helper.h"
#include "tokenize.h"
#include "parse.h"

// Converts custom type to C type
char *TypeToC(TokenSubtype type);

// Converts a Node to C code
char *NodeToC(Node *node);