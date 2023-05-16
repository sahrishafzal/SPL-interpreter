#include <iostream>
#include "val.h"
#include <sstream>
#include <string>
using namespace std;

/*// numeric overloaded add op to this
    Value operator+(const Value& op) const;
    
    // numeric overloaded subtract op from this
    Value operator-(const Value& op) const;
    
    // numeric overloaded multiply this by op
    Value operator*(const Value& op) const;
    
    // numeric overloaded divide this by op
    Value operator/(const Value& op) const;
    //numeric overloaded equality operator of this with op
    Value operator==(const Value& op) const;
	//numeric overloaded greater than operator of this with op
	Value operator>(const Value& op) const;
	//numeric overloaded less than operator of this with op
	Value operator<(const Value& op) const;
	
	//Numeric exponentiation operation this raised to the power of op 
	Value operator^(const Value& oper) const;
	
	//string concatenation operation of this with op
	Value Catenate(const Value& oper) const;
	//string repetition operation of this with op
	Value Repeat(const Value& oper) const;
	//string equality (-eq) operator of this with op
	Value SEqual(const Value& oper) const;
	//string greater than (-gt) operator of this with op
	Value SGthan(const Value& oper) const;
	//string less than operator of this with op
	Value SLthan(const Value& oper) const;
	    */

Value Value::SLthan(const Value &oper) const
{
    if (IsString())
    {
        if (oper.IsString())
        {
            return Value(Stemp < oper.GetString());
        }
        else if (!oper.IsErr())
        {
            double number;
            if (oper.IsReal())
            {
                number = oper.GetReal();
            }
            else if (oper.IsInt())
            {
                number = oper.GetInt();
            }
            else if (oper.IsBool())
            {
                number = oper.GetBool();
            }

            stringstream st;
            st << showpoint << setprecision(2) << number;
            return Value(Stemp < st.str());
        }
        else
        {
            return Value();
        }
    }
    else if (!oper.IsErr() && !IsErr())
    {

        double number_1 = Itemp + Btemp + Rtemp;
        stringstream str1;
        str1 << number_1;
        if (oper.IsString())
        {
            return Value(str1.str() < oper.GetString());
        }
        else
        {
            double number_2;
            if (oper.IsInt())
            {
                number_2 = oper.GetInt();
            }
            else if (oper.IsBool())
            {
                number_2 = oper.GetBool();
            }
            else if (oper.IsReal())
            {
                number_2 = oper.GetReal();
            }
            stringstream str2;
            str2 << number_2;
            return Value(str1.str() < str2.str());
        }
    }
    else
    {
        return Value();
    }
}
Value Value::SGthan(const Value &oper) const
{
    if (IsString())
    {
        if (oper.IsString())
        {
            return Value(Stemp > oper.GetString());
        }
        else if (!oper.IsErr())
        {
            double number;
            if (oper.IsReal())
            {
                number = oper.GetReal();
            }
            else if (oper.IsInt())
            {
                number = oper.GetInt();
            }
            else if (oper.IsBool())
            {
                number = oper.GetBool();
            }

            stringstream st;
            st << showpoint << setprecision(2) << number;
            return Value(Stemp > st.str());
        }
        else
        {
            return Value();
        }
    }
    else if (!oper.IsErr() && !IsErr())
    {

        double number_1 = Itemp + Btemp + Rtemp;
        stringstream str1;
        str1 << number_1;
        if (oper.IsString())
        {
            return Value(str1.str() > oper.GetString());
        }
        else
        {
            double number_2;
            if (oper.IsInt())
            {
                number_2 = oper.GetInt();
            }
            else if (oper.IsBool())
            {
                number_2 = oper.GetBool();
            }
            else if (oper.IsReal())
            {
                number_2 = oper.GetReal();
            }
            stringstream str2;
            str2 << number_2;
            return Value(str1.str() > str2.str());
        }
    }
    else
    {
        return Value();
    }
}
Value Value::SEqual(const Value &oper) const
{
    if (IsString())
    {
        if (oper.IsString())
        {
            return Value(Stemp == oper.GetString());
        }
        else if (!oper.IsErr())
        {
            double number;
            if (oper.IsReal())
            {
                number = oper.GetReal();
            }
            else if (oper.IsInt())
            {
                number = oper.GetInt();
            }
            else if (oper.IsBool())
            {
                number = oper.GetBool();
            }

            stringstream st;
            st << showpoint << setprecision(2) << number;
            return Value(Stemp == st.str());
        }
        else
        {
            return Value();
        }
    }
    else if (!oper.IsErr() && !IsErr())
    {

        double number_1 = Itemp + Btemp + Rtemp;
        stringstream str1;
        str1 << number_1;
        if (oper.IsString())
        {
            return Value(str1.str() == oper.GetString());
        }
        else
        {
            double number_2;
            if (oper.IsInt())
            {
                number_2 = oper.GetInt();
            }
            else if (oper.IsBool())
            {
                number_2 = oper.GetBool();
            }
            else if (oper.IsReal())
            {
                number_2 = oper.GetReal();
            }
            stringstream str2;
            str2 << number_2;
            return Value(str1.str() == str2.str());
        }
    }
    else
    {
        return Value();
    }
}
Value Value::operator==(const Value &op) const
{
    if (!IsErr() && !op.IsErr())
    {
        double number_1;
        double number_2;
        if (!IsString())
        {
            number_1 = Itemp + Btemp + Rtemp;
        }
        else
        {
            try
            {
                number_1 = stod(GetString());
            }
            catch (...)
            {
                cout << "Invalid conversion from string to double." << endl;
                return Value();
            }
        }
        if (!op.IsString())
        {
            if (op.IsInt())
            {
                number_2 = op.GetInt();
            }
            else if (op.IsBool())
            {
                number_2 = op.GetBool();
            }
            else if (op.IsReal())
            {
                number_2 = op.GetReal();
            }
        }
        else
        {
            try
            {
                number_2 = stod(op.GetString());
            }
            catch (...)
            {
                cout << "Invalid conversion from string to double." << endl;
                return Value();
            }
        }
        return Value(number_1 == number_2);
    }
    else
    {
        return Value();
    }
}
//expon  operator
Value Value::operator^(const Value &oper) const
{
    if (!oper.IsString() && !IsString() && !oper.IsErr() && !IsErr())
    {
        double number_1 = Itemp + Rtemp + Btemp;
        double number_2;
        if (IsInt())
        {
            number_2 = oper.GetInt();
        }
        else if (IsBool())
        {
            number_2 = oper.GetBool();
        }
        else if (IsReal())
        {
            number_2 = oper.GetReal();
        }
        return Value(pow(number_1, number_2));
    }
    else
    {
        return Value();
    }
}
//mult* operator
Value Value::operator*(const Value &op) const
{
    if (op.IsString() && !IsErr())
    {
        try
        {
            double str_num2 = stod(op.GetString());
            if (IsString())
            {
                double str_num1 = stod(GetString());
                return Value(str_num1 * str_num2);
            }
            else if (!IsString())
            {
                return Value((Itemp + Rtemp + Btemp) * str_num2);
            }
            else
            {
                return Value();
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (IsString() && !op.IsErr())
    {
        try
        {
            double str_num1 = stod(GetString());
            if (op.IsReal())
            {
                return Value(str_num1 * op.GetReal());
            }
            else if (op.IsInt())
            {
                return Value(str_num1 * op.GetInt());
            }
            else if (op.IsBool())
            {
                return Value(str_num1 * op.GetBool());
            }
            else if (op.IsString())
            {
                double str_num2 = stod(GetString());
                return Value(str_num1 * str_num2);
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (!IsErr() && op.IsInt())
    {
        return Value((Itemp + Btemp + Rtemp) * op.GetInt());
    }
    else if (!IsErr() && op.IsBool())
    {
        return Value((Itemp + Btemp + Rtemp) * op.GetBool());
    }
    else if (!IsErr() && op.IsReal())
    {
        return Value((Itemp + Btemp + Rtemp) * op.GetReal());
    }
    return Value();
}
// div/ operator
Value Value::operator/(const Value &op) const
{
    if (op.IsString() && !IsErr())
    {
        try
        {
            double str_num2 = stod(op.GetString());
            if (IsString())
            {
                double str_num1 = stod(GetString());
                return Value(str_num1 / str_num2);
            }
            else if (!IsString())
            {
                return Value((Itemp + Rtemp + Btemp) / str_num2);
            }
            else
            {
                return Value();
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (IsString() && !op.IsErr())
    {
        try
        {
            double str_num1 = stod(GetString());
            if (op.IsReal())
            {
                return Value(str_num1 / op.GetReal());
            }
            else if (op.IsInt())
            {
                return Value(str_num1 / op.GetInt());
            }
            else if (op.IsBool())
            {
                return Value(str_num1 / op.GetBool());
            }
            else if (op.IsString())
            {
                double str_num2 = stod(GetString());
                return Value(str_num1 / str_num2);
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (!IsErr() && op.IsInt())
    {
        return Value((Itemp + Btemp + Rtemp) / op.GetInt());
    }
    else if (!IsErr() && op.IsBool())
    {
        return Value((Itemp + Btemp + Rtemp) / op.GetBool());
    }
    else if (!IsErr() && op.IsReal())
    {
        return Value((Itemp + Btemp + Rtemp) / op.GetReal());
    }
    return Value();
}    //numberic lthan
Value Value::operator<(const Value &op) const
{
    if (op.IsString() && !IsErr())
    {
        try
        {
            double str_num2 = stod(op.GetString());
            if (IsString())
            {
                double str_num1 = stod(GetString());
                return Value(str_num1 < str_num2);
            }
            else if (!IsString())
            {
                return Value((Itemp + Rtemp + Btemp) < str_num2);
            }
            else
            {
                return Value();
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (IsString() && !op.IsErr())
    {
        try
        {
            double str_num1 = stod(GetString());
            if (op.IsReal())
            {
                return Value(str_num1 < op.GetReal());
            }
            else if (op.IsInt())
            {
                return Value(str_num1 < op.GetInt());
            }
            else if (op.IsBool())
            {
                return Value(str_num1 < op.GetBool());
            }
            else if (op.IsString())
            {
                double str_num2 = stod(GetString());
                return Value(str_num1 < str_num2);
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (!IsErr() && op.IsInt())
    {
        return Value((Itemp + Btemp + Rtemp) < op.GetInt());
    }
    else if (!IsErr() && op.IsBool())
    {
        return Value((Itemp + Btemp + Rtemp) < op.GetBool());
    }
    else if (!IsErr() && op.IsReal())
    {
        return Value((Itemp + Btemp + Rtemp) < op.GetReal());
    }
    return Value();
}
    // > greater oprerator
Value Value::operator>(const Value &op) const
{
    if (op.IsString() && !IsErr())
    {
        try
        {
            double str_num2 = stod(op.GetString());
            if (IsString())
            {
                double str_num1 = stod(GetString());
                return Value(str_num1 > str_num2);
            }
            else if (!IsString())
            {
                return Value((Itemp + Rtemp + Btemp) > str_num2);
            }
            else
            {
                return Value();
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (IsString() && !op.IsErr())
    {
        try
        {
            double str_num1 = stod(GetString());
            if (op.IsReal())
            {
                return Value(str_num1 > op.GetReal());
            }
            else if (op.IsInt())
            {
                return Value(str_num1 > op.GetInt());
            }
            else if (op.IsBool())
            {
                return Value(str_num1 > op.GetBool());
            }
            else if (op.IsString())
            {
                double str_num2 = stod(GetString());
                return Value(str_num1 > str_num2);
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (!IsErr() && op.IsInt())
    {
        return Value((Itemp + Btemp + Rtemp) > op.GetInt());
    }
    else if (!IsErr() && op.IsBool())
    {
        return Value((Itemp + Btemp + Rtemp) > op.GetBool());
    }
    else if (!IsErr() && op.IsReal())
    {
        return Value((Itemp + Btemp + Rtemp) > op.GetReal());
    }
    return Value();
}
// - oprator function
Value Value::operator-(const Value &op) const
{
    if (op.IsString() && !IsErr())
    {
        try
        {
            double str_num2 = stod(op.GetString());
            if (IsString())
            {
                double str_num1 = stod(GetString());
                return Value(str_num1 - str_num2);
            }
            else if (!IsString())
            {
                return Value((Itemp + Rtemp + Btemp) - str_num2);
            }
            else
            {
                return Value();
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (IsString() && !op.IsErr())
    {
        try
        {
            double str_num1 = stod(GetString());
            if (op.IsReal())
            {
                return Value(str_num1 - op.GetReal());
            }
            else if (op.IsInt())
            {
                return Value(str_num1 - op.GetInt());
            }
            else if (op.IsBool())
            {
                return Value(str_num1 - op.GetBool());
            }
            else if (op.IsString())
            {
                double str_num2 = stod(GetString());
                return Value(str_num1 - str_num2);
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (!IsErr() && op.IsInt())
    {
        return Value((Itemp + Btemp + Rtemp) - op.GetInt());
    }
    else if (!IsErr() && op.IsBool())
    {
        return Value((Itemp + Btemp + Rtemp) - op.GetBool());
    }
    else if (!IsErr() && op.IsReal())
    {
        return Value((Itemp + Btemp + Rtemp) - op.GetReal());
    }
    return Value();
}
// plus + operator
Value Value::operator+(const Value &op) const
{
    if (op.IsString() && !IsErr())
    {
        try
        {
            double str_num2 = stod(op.GetString());
            if (IsString())
            {
                double str_num1 = stod(GetString());
                return Value(str_num1 + str_num2);
            }
            else if (!IsString())
            {
                return Value((Itemp + Rtemp + Btemp) + str_num2);
            }
            else
            {
                return Value();
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (IsString() && !op.IsErr())
    {
        try
        {
            double str_num1 = stod(GetString());
            if (op.IsReal())
            {
                return Value(str_num1 + op.GetReal());
            }
            else if (op.IsInt())
            {
                return Value(str_num1 + op.GetInt());
            }
            else if (op.IsBool())
            {
                return Value(str_num1 + op.GetBool());
            }
            else if (op.IsString())
            {
                double str_num2 = stod(GetString());
                return Value(str_num1 + str_num2);
            }
        }
        catch (...)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    }
    else if (!IsErr() && op.IsInt())
    {
        return Value((Itemp + Btemp + Rtemp) + op.GetInt());
    }
    else if (!IsErr() && op.IsBool())
    {
        return Value((Itemp + Btemp + Rtemp) + op.GetBool());
    }
    else if (!IsErr() && op.IsReal())
    {
        return Value((Itemp + Btemp + Rtemp) + op.GetReal());
    }
    return Value();
}

Value Value::Catenate(const Value &oper) const
{
    if (IsString())
    {
        if (oper.IsString())
        {
            return Value(Stemp + oper.GetString());
        }
        else if (!oper.IsErr())
        {
            double number;
            if (oper.IsInt())
            {
                number = oper.GetInt();
            }
            else if (oper.IsBool())
            {
                number = oper.GetBool();
            }
            else if (oper.IsReal())
            {
                number = oper.GetReal();
            }
            stringstream st;
            st << number;
            return Value(Stemp + st.str());
        }
        else
        {
            return Value();
        }
    }
    else if (!oper.IsErr() && !IsErr())
    {

        double number_1 = Itemp + Btemp + Rtemp;
        stringstream str1;
        str1 << number_1;
        if (oper.IsString())
        {
            return Value(str1.str() + oper.GetString());
        }
        else
        {
            double number_2;
            if (oper.IsInt())
            {
                number_2 = oper.GetInt();
            }
            else if (oper.IsBool())
            {
                number_2 = oper.GetBool();
            }
            else if (oper.IsReal())
            {
                number_2 = oper.GetReal();
            }
            stringstream str2;
            str2 << number_2;
            return Value(str1.str() + str2.str());
        }
    }
    else
    {
        return Value();
    }
}


//repeat string of this object op number of times
Value Value::Repeat(const Value& op) const {
    //First operands is a string and the second operand is an integer
    double oper;
    if(IsString() && op.IsString()){
        //second operand must be converted to a numeric
        try {
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        int times = oper;
        string str = GetString(), newstr = "";
        for(int i = 1; i <= times; i++){
            newstr += str;
        }
        return Value(newstr);
    }
    else if(IsReal() && op.IsString()){
        //Both operands to be converted
        //first operand must be converted to a string
        ostringstream outStr1;
        outStr1 << GetReal();
        string stroper1 = outStr1.str(), newstr = "";
        //second operand must be converted to a numeric
        try {
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        int times = oper;
        for(int i = 1; i <= times; i++){
            newstr += stroper1;
        }
        return Value(newstr);
    }
    else if (IsReal() && op.IsReal()) {
        //Other cases to follow
        ostringstream outStr1;
        outStr1 << GetReal();
        string stroper1 = outStr1.str(), newstr = "";
        oper = op.GetReal();
        int times = oper;

        for(int i = 1; i <= times; i++){
            newstr += stroper1;
        }
        return Value(newstr);
    }
    else if (IsString() && op.IsReal()){
        oper = op.GetReal();
        int times = oper;
        string str = GetString(), newstr = "";
        for(int i = 1; i <= times; i++){
            newstr += str;
        }
        return Value(newstr);
    }
    else {
        return Value();
    }
}
