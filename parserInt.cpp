/* Implementation of Recursive-Descent Parser
 * parser.cpp
 * Programming Assignment 2
 * Spring 2023
 */
#include "parserInt.h"

#include <vector>
map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults;
std::vector<Value> ValueList;
namespace Parser
{
    bool pushed_back = false;
    LexItem pushed_token;
    static LexItem GetNextToken(istream &in, int &line)
    {
        if (pushed_back)
        {
            pushed_back = false;
            return pushed_token;
        }
        return getNextToken(in, line);
    }
    static void PushBackToken(LexItem &t)
    {
        if (pushed_back)
        {
            abort();
        }
        pushed_back = true;
        pushed_token = t;
    }
}
static int error_count = 0;
int ErrCount()
{
    return error_count;
}
void ParseError(int line, string msg)
{
    ++error_count;
    cout << error_count << ". Line # " << line << ": " << msg << endl;
}
bool Prog(istream &in, int &line)
{
    bool status = StmtList(in, line);
    if (!status)
    {
        ParseError(line, "Missing Program");
        return false;
    }
    cout << endl
         << "(DONE)" << endl;
    return true;
}
int if_stmt_depth = 0;
bool bad_stmt = false;
bool StmtList(istream &in, int &line)
{
    bool status = false;
    status = Stmt(in, line);
    if (!status)
    {
        if (in.eof())
        {
            return true;
        }
        LexItem tok = Parser::GetNextToken(in, line);
        if ((tok.GetToken() == RBRACES) && if_stmt_depth > 0 && !bad_stmt)
        {
            if_stmt_depth--;
            return true;
        }
        ParseError(line, "Syntactic error in Program Body.");
        return false;
    }
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok.GetLexeme()[0] == ';')
    {
        status = StmtList(in, line);
    }
    else
    {
        Parser::PushBackToken(tok);
        ParseError(line, "Syntactic error in Program Body.");
        return false;
    }
    return status;
}
bool Stmt(istream &in, int &line)
{
    if (AssignStmt(in, line))
    {
        return true;
    }
    else if (bad_stmt)
    {
        return false;
    }
    if (WritelnStmt(in, line))
    {
        return true;
    }
    else if (bad_stmt)
    {
        return false;
    }
    if (IfStmt(in, line))
    {
        return true;
    }
    else if (bad_stmt)
    {
        return false;
    }
    return false;
}
bool WritelnStmt(istream &in, int &line)
{
    LexItem t;
    t = Parser::GetNextToken(in, line);
    if (t != WRITELN)
    {
        Parser::PushBackToken(t);
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if (t != LPAREN)
    {
        ParseError(line, "Missing Left Parenthesis of Writeln Statement");
        bad_stmt = true;
        return false;
    }
    bool ex = ExprList(in, line);
    if (!ex)
    {
        ParseError(line, "Missing expression list after Print");
        bad_stmt = true;
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if (t != RPAREN)
    {
        ParseError(line, "Missing Right Parenthesis of Writeln Statement");
        bad_stmt = true;
        return false;
    }
    for (auto i = ValueList.cbegin(); i != ValueList.cend(); ++i)
    {
        Value content = *i;
        cout << fixed << showpoint;
        if (content.IsReal())
        {
            cout << setprecision(1) << content.GetReal();
        }
        else if (content.IsString())
        {
            cout << content.GetString();
        }
        else
        {
            cout << content;
        }
    }
    cout << endl;
    ValueList.clear();
    return true;
} // End of WritelnStmt
bool IfStmt(istream &in, int &line)
{
    LexItem t;
    // if
    t = Parser::GetNextToken(in, line);
    if (t != IF)
    {
        Parser::PushBackToken(t);
        return false;
    }
    // '(' Expr ')'
    t = Parser::GetNextToken(in, line);
    if (t != LPAREN)
    {
        ParseError(line, "Missing Left Parenthesis of Writeln Statement");
        ParseError(line, "Incorrect If-Statement.");
        bad_stmt = true;
        return false;
    }
    Value exprVal;
    bool ex = Expr(in, line, exprVal);
    if (!ex)
    {
        ParseError(line, "Missing if statement Logic Expression");
        ParseError(line, "Incorrect If-Statement.");
        bad_stmt = true;
        return false;
    }
    if (exprVal.GetType() != VBOOL)
    {
        ParseError(line, "Illegal Type for If statement condition.");
        ParseError(line, "Incorrect If-Statement.");
        bad_stmt = true;
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if (t != RPAREN)
    {
        ParseError(line, "Missing Right Parenthesis of If Statement");
        ParseError(line, "Incorrect If-Statement.");
        bad_stmt = true;
        return false;
    }
    // '{' StmtList '}'
    t = Parser::GetNextToken(in, line);
    if (t != LBRACES)
    {
        ParseError(line, "Missing Left Brace of If Statement");
        ParseError(line, "Incorrect If-Statement.");
        bad_stmt = true;
        return false;
    }
    if (exprVal.GetBool())
    {
        if_stmt_depth++;
        bool stmtls = StmtList(in, line);
        if (!stmtls)
        {
            ParseError(line, "Missing Statement for If-Stmt Clause");
            ParseError(line, "Incorrect If-Statement.");
            bad_stmt = true;
            return false;
        }
    }
    else // Skip statements in if block
    {
        while (t.GetToken() != RBRACES)
        {
            t = Parser::GetNextToken(in, line);
            if (in.eof())
            {
                ParseError(line, "Missing Right Brace of If Statement");
                ParseError(line, "Incorrect If-Statement.");
                bad_stmt = true;
                return false;
            }
        }
    }
    // else
    t = Parser::GetNextToken(in, line);
    if (t == ELSE)
    {
        // '{' StmtList '}'
        t = Parser::GetNextToken(in, line);
        if (t != LBRACES)
        {
            ParseError(line, "Missing Left Brace of Else Statement");
            ParseError(line, "Incorrect Else-Statement.");
            bad_stmt = true;
            return false;
        }
        if (!exprVal.GetBool())
        {
            if_stmt_depth++;
            bool stmtls = StmtList(in, line);
            if (!stmtls)
            {
                ParseError(line, "Missing Statement for Else-Stmt Clause");
                ParseError(line, "Incorrect Else-Statement.");
                bad_stmt = true;
                return false;
            }
        }
        else // Skip statements in else block
        {
            while (t.GetToken() != RBRACES)
            {
                t = Parser::GetNextToken(in, line);
                if (in.eof())
                {
                    ParseError(line, "Missing Right Brace of If Statement");
                    ParseError(line, "Incorrect If-Statement.");
                    bad_stmt = true;
                    return false;
                }
            }
        }
    }
    else
    {
        Parser::PushBackToken(t);
    }
    return true;
}
bool AssignStmt(istream &in, int &line)
{
    LexItem idtok;
    if (!Var(in, line, idtok))
    {
        Parser::PushBackToken(idtok);
        return false;
    }
    LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() != ASSOP)
    {
        ParseError(line, "Missing Assignment Operator");
        ParseError(line, "Incorrect Assignment Statement.");
        bad_stmt = true;
        Parser::PushBackToken(t);
        return false;
    }
    Value exprVal;
    if (!Expr(in, line, exprVal))
    {
        ParseError(line, "Missing Expression in Assignment Statement");
        ParseError(line, "Incorrect Assignment Statement.");
        bad_stmt = true;
        return false;
    }
    if (exprVal.IsBool())
    {
        ParseError(line, "Illegal Assignment of a boolean value to a numeric or string variable.");
        ParseError(line, "Incorrect Assignment Statement.");
        bad_stmt = true;
        return false;
    }
    // Type conversion
    Value varVal;
    string emp = "";
    Value empty = Value(emp);
    switch (idtok.GetToken())
    {
    case NIDENT:
        varVal = Value(0) + exprVal;
        break;
    case SIDENT:
        varVal = empty.Catenate(exprVal);
        break;
    default:
        ParseError(line, "Incorrect Assignment Statement.");
        bad_stmt = true;
        return false;
    }
    if (varVal.IsErr())
    {
        ParseError(line, "Invalid assignment statement conversion of a string value to a double variable.");
        ParseError(line, "Incorrect Assignment Statement.");
        bad_stmt = true;
        return false;
    }
    TempsResults[idtok.GetLexeme()] = varVal;
    return true;
}
bool Var(istream &in, int &line, LexItem &idtok)
{
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == NIDENT || tok.GetToken() == SIDENT)
    {
        SymTable[tok.GetLexeme()] = tok.GetToken();
        idtok = tok;
        return true;
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    idtok = tok;
    return false;
}
// ExprList:= Expr {,Expr}
bool ExprList(istream &in, int &line)
{
    bool status = false;
    Value exprVal;
    status = Expr(in, line, exprVal);
    if (!status)
    {
        ParseError(line, "Missing Expression");
        return false;
    }
    ValueList.push_back(exprVal);
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok == COMMA)
    {
        status = ExprList(in, line);
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    else
    {
        Parser::PushBackToken(tok);
        return true;
    }
    return status;
} // End of ExprList
bool Expr(istream &in, int &line, Value &retVal)
{
    Value relVal1;
    if (!RelExpr(in, line, relVal1))
    {
        return false;
    }
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == SEQ || tok.GetToken() == NEQ)
    {
        Value relVal2;
        if (AddExpr(in, line, relVal2))
        {
            switch (tok.GetToken())
            {
            case SEQ:
                retVal = relVal1.SEqual(relVal2);
                if (retVal.GetType() == Value().GetType())
                {
                    ParseError(line, "Illegal operand type for the operation.");
                    return false;
                }
                break;
            case NEQ:
                retVal = relVal1 == relVal2;
                if (retVal.GetType() == Value().GetType())
                {
                    ParseError(line, "Illegal operand type for the operation.");
                    return false;
                }
                break;
            default:
                ParseError(line, "Incorrect Expression.");
                return false;
            }
            return true;
        }
        else
        {
            // ParseError(line, "Missing Relational Expression");
            return false;
        }
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    retVal = relVal1;
    Parser::PushBackToken(tok);
    return true;
}
bool RelExpr(istream &in, int &line, Value &retVal)
{
    Value addVal1;
    if (!AddExpr(in, line, addVal1))
    {
        // ParseError(line, "Missing Addition Expression");
        return false;
    }
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == SLTHAN || tok.GetToken() == SGTHAN || tok.GetToken() == NLTHAN || tok.GetToken() == NGTHAN)
    {
        Value addVal2;
        if (AddExpr(in, line, addVal2))
        {
            switch (tok.GetToken())
            {
            case SLTHAN:
                retVal = addVal1.SLthan(addVal2);
                break;
            case SGTHAN:
                retVal = addVal1.SGthan(addVal2);
                break;
            case NLTHAN:
                retVal = addVal1 < addVal2;
                break;
            case NGTHAN:
                retVal = addVal1 > addVal2;
                break;
            default:
                ParseError(line, "Incorrect Relational Expression.");
                return false;
            }
            if (retVal.IsErr())
            {
                ParseError(line, "Illegal Relational operation.");
                return false;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    retVal = addVal1;
    Parser::PushBackToken(tok);
    return true;
}
Token funta;
Token funtm;
bool AddExpr(istream &in, int &line, Value &retVal)
{
    bool status = false;
    Value multVal;
    status = MultExpr(in, line, multVal);
    if (!status)
    {
        return false;
    }
    if (funta == PLUS)
    {
        retVal = retVal + multVal;
    }
    else if (funta == MINUS)
    {
        retVal = retVal - multVal;
    }
    else if (funta == CAT)
    {
        retVal = retVal.Catenate(multVal);
    }
    else
    {
        retVal = multVal;
    }
    if (retVal.IsErr())
    {
        ParseError(line, "Illegal operand type for the operation.");
        return false;
    }
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == PLUS || tok.GetToken() == MINUS || tok.GetToken() == CAT)
    {
        funta = tok.GetToken();
        status = AddExpr(in, line, retVal);
        funta = ERR;
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    else
    {
        Parser::PushBackToken(tok);
        return true;
    }
    return status;
}
bool MultExpr(istream &in, int &line, Value &retVal)
{
    bool status = false;
    Value exponVal;
    status = ExponExpr(in, line, exponVal);
    if (!status)
    {
        // ParseError(line, "Missing Exponential Expression");
        return false;
    }
    if (funtm == SREPEAT)
    {
        retVal = retVal.Repeat(exponVal);
    }
    else if (funtm == MULT)
    {
        retVal = retVal * exponVal;
    }
    else if (funtm == DIV)
    {
        double denum = exponVal.GetReal();
        if (denum == 0)
        {
            ParseError(line, "Illegal operand type for the operation.");
            ParseError(line, "Missing operand after operator");
            return false;
        }
        retVal = retVal / exponVal;
    }
    else
    {
        retVal = exponVal;
    }
    if (retVal.IsErr())
    {
        ParseError(line, "Illegal operand type for the operation.");
        return false;
    }
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == MULT || tok.GetToken() == DIV || tok.GetToken() == SREPEAT)
    {
        funtm = tok.GetToken();
        status = MultExpr(in, line, retVal);
        funtm = ERR;
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    else
    {
        Parser::PushBackToken(tok);
        return true;
    }
    return status;
}
vector<Value> ExponList;
bool ExponExpr(istream &in, int &line, Value &retVal)
{
    bool status = false;
    Value unaryVal;
    status = UnaryExpr(in, line, unaryVal);
    if (!status)
    {
        return false;
    }
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == EXPONENT)
    {
        ExponList.push_back(unaryVal);
        status = ExponExpr(in, line, retVal);
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    else
    {
        for (auto i = ExponList.rbegin(); i != ExponList.rend(); ++i)
        {
            Value expo = *i;
            unaryVal = expo ^ unaryVal;
        }
        retVal = unaryVal;
        if (retVal.IsErr())
        {
            ParseError(line, "Illegal exponentiation operation.");
            ParseError(line, "Missing operand after operator");
            return false;
        }
        ExponList.clear();
        Parser::PushBackToken(tok);
        return true;
    }
    return status;
}
Token funt;
bool UnaryExpr(istream &in, int &line, Value &retVal)
{
    if (PrimaryExpr(in, line, 1000, retVal))
    {
        return true;
    }
    if (funt == CAT)
    {
        ParseError(line, "Missing operand after operator");
        return false;
    }
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == PLUS || tok.GetToken() == MINUS)
    {
        if (PrimaryExpr(in, line, tok.GetToken(), retVal))
        {
            return true;
        }
        else
        {
            Parser::GetNextToken(in, line);
            return false;
        }
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    return false;
}
bool PrimaryExpr(istream &in, int &line, int sign, Value &retVal)
{
    LexItem tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == IDENT || tok.GetToken() == NIDENT || tok.GetToken() == SIDENT)
    {
        if (TempsResults.find(tok.GetLexeme()) == TempsResults.end())
        {
            ParseError(line, "Usage of Uninitialized Variable");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            Parser::PushBackToken(tok);
            return false;
        }
        Value initVal = TempsResults[tok.GetLexeme()];
        if (tok.GetToken() != SIDENT)
        {
            retVal = (sign == PLUS || sign == 1000 ? (Value(0) + initVal) : (Value(0) - initVal));
            return true;
        }
        else if (sign == PLUS || sign == MINUS)
        {
            ParseError(line, "Illegal Operand Type for Sign Operator");
            Parser::PushBackToken(tok);
            return false;
        }
        else
        {
            retVal = initVal;
            return true;
        }
    }
    else if (tok.GetToken() == RCONST || tok.GetToken() == ICONST || tok.GetToken() == SCONST)
    {
        Value initVal = Value(tok.GetLexeme());
        if (tok.GetToken() != SCONST)
        {
            retVal = (sign == PLUS || sign == 1000 ? (Value(0) + initVal) : (Value(0) - initVal));
            return true;
        }
        else if (sign == PLUS || sign == MINUS)
        {
            ParseError(line, "Illegal Operand Type for Sign Operator");
            Parser::PushBackToken(tok);
            return false;
        }
        else
        {
            retVal = initVal;
            return true;
        }
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        funt = CAT;
        return false;
    }
    else if (tok.GetToken() == LPAREN)
    {
        vector<Token> prev_state;
        prev_state.push_back(funta);
        funta = ERR;
        prev_state.push_back(funtm);
        funtm = ERR;
        Value exprVal;
        if (!Expr(in, line, exprVal))
        {
            return false;
        }
        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() != RPAREN)
        {
            ParseError(line, "Missing Right Parenthesis");
            Parser::PushBackToken(tok);
            return false;
        }
        funtm = prev_state.back();
        prev_state.pop_back();
        funta = prev_state.back();
        prev_state.pop_back();
        retVal = exprVal;
        if (retVal.IsErr())
        {
            ParseError(line, "Illegal operand type for the operation.");
            return false;
        }
        return true;
    }
    Parser::PushBackToken(tok);
    return false;
}