
// File:   main.cpp
// Author: Jose Fraga
// Created on July 25, 2017, 12:48 PM

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include "Stack.h"
#include "Vector.h"
#include "Map.h"
using namespace std;


// Returns the precedence of a given string by variable reference

void currentPrecedence(string value, int& precedenceValue)
{
    if      (value == "(")      precedenceValue = 0;
    else if (value == "min")    precedenceValue = 3;
    else if (value == "max")    precedenceValue = 3;
    else if (value == "sin")    precedenceValue = 3;
    else if (value == "cos")    precedenceValue = 3;
    else if (value == "tan")    precedenceValue = 3;   
    else if (value == "*")      precedenceValue = 2;
    else if (value == "/")      precedenceValue = 2;
    else if (value == "+")      precedenceValue = 1;
    else if (value == "-")      precedenceValue = 1;
}

// Pops operators from the operation stack, and pushes them onto postfix 
// accordingly.

void movePrecedence(Stack<string>& opStack, Vector<string>& postfix, 
    int operation, int& precedence)
{
    // Initialization & declaration
    string value;
    opStack.top(value);   
    int precedenceValue = 0;
        
    // Pops one operator at a time accordingly   
    while ((!opStack.isEmpty()) && (value != "(") && (operation <= precedence))
    {
        opStack.pop(value);        
        
        if (value != "(")
            postfix.pushBack(value);
                  
        if (value == "(")
            opStack.push(value);
        
        if (!opStack.isEmpty())
        {           
            // Need to update the operator and precedence on the operation stack.
            opStack.top(value);
            currentPrecedence(value, precedenceValue);
            precedence = precedenceValue;
        }
        else
            precedence = 0;               
    }  
}

// Returns the two values that arithmetic will be performed on by variable reference.

bool operatorValues(Stack<double>& auxStack, double& value1, double& value2,
    int trigFunc)
{
    // Error checking for malformed expressions
    if (auxStack.size() >= 2 && trigFunc == 0)   
    {
        // pop 2nd value
        auxStack.pop(value2);

        // pop 1st value
        auxStack.pop(value1);

        return true;
    }
    else if (auxStack.size() >= 1 && trigFunc == 1)
    {
        // pop one value
        auxStack.pop(value1);
        
        return true;
    }
    else return false;
}

// This function will be given the infix expression provided by the user and will 
// attempt to convert it to an equivalent postfix expression, that will be stored 
// in ‘postfix’. This function will return true if we were able to perform the 
// conversion and false if the expression is malformed. Shunting Yard algorithm 
// will only fail if the parentheses are mismatched. It is possible for the 
// expression to be invalid, but still pass through the shunting yard.

bool shuntingYard(const Vector<string>& expression, const int startIndex, 
    Vector<string>& postfix)
{    
    // Creating the auxiliary track (opStack object).
    Stack<string> opStack;
    
    // Precedence levels    
    int precedence      =  0;
    int multiplication  =  2;
    int division        =  2;
    int addition        =  1;
    int subtraction     =  1;
    int nonOperator     = -1;
    
    for (int i = startIndex; i < expression.getSize(); i++)
    {            
        if (expression[i] == "(")
        {
            // Update precedence of operator being pushed onto the operation stack
            currentPrecedence(expression[i], precedence);
            opStack.push("(");
        }
        else if (expression[i] == ")")
        {                                    
            movePrecedence(opStack, postfix, nonOperator, precedence);
            
            string value;
            opStack.top(value);
            currentPrecedence(value, precedence);
            
            // Error checking for missing parenthesis.
            if (value == "(")
                opStack.pop(value);
            else return false;
            
            // Update the precedence if there are remaining operators on the
            // operation stack
            opStack.top(value);
            currentPrecedence(value, precedence);
        }
        else if (expression[i] == "min")
        {
            currentPrecedence(expression[i], precedence);            
            opStack.push("min");
        }
        else if (expression[i] ==  "max")
        {
            currentPrecedence(expression[i], precedence);           
            opStack.push("max");
        }
        else if (expression[i] == "sin")
        {
            currentPrecedence(expression[i], precedence);
            opStack.push("sin");
        }
        else if (expression[i] == "cos")
        {
            currentPrecedence(expression[i], precedence);
            opStack.push("cos");
        }
        else if (expression[i] == "tan")
        {
            currentPrecedence(expression[i], precedence);
            opStack.push("tan");
        }        
        else if (expression[i] == "*")
        {
            if (multiplication > precedence)
            {
                currentPrecedence(expression[i], precedence);
                opStack.push("*");
            }
            else
            {                
                movePrecedence(opStack, postfix, multiplication, precedence);
                
                currentPrecedence(expression[i], precedence);
                opStack.push("*");
            }
        }
        else if (expression[i] == "/")
        {        
            if (division > precedence)
            {
                currentPrecedence(expression[i], precedence);
                opStack.push("/");
            }
            else
            {                
                movePrecedence(opStack, postfix, division, precedence);
                
                currentPrecedence(expression[i], precedence);
                opStack.push("/");
            }
        }
        else if (expression[i] == "+")
        {
            if (addition > precedence)
            {
                currentPrecedence(expression[i], precedence);
                opStack.push("+");
            }
            else
            {                
                movePrecedence(opStack, postfix, addition, precedence);
                
                currentPrecedence(expression[i], precedence);
                opStack.push("+");
            }
        }
        else if (expression[i] == "-")
        {
            if (subtraction > precedence)
            {
                currentPrecedence(expression[i], precedence);
                opStack.push("-");
            }
            else
            {                   
                movePrecedence(opStack, postfix, subtraction, precedence);
                
                currentPrecedence(expression[i], precedence);
                opStack.push("-");
            }
        }
        else
        {            
            // Pushing numbers directly onto postfix
            postfix.pushBack(expression[i]);           
        }
    }
    
    // Push the final operator in the operation stack onto postfix
    movePrecedence(opStack, postfix, nonOperator, precedence);
    
    // Transforming infix to postfix was a success if opStack is empty
    if (opStack.isEmpty())
        return true;
    else return false;
}

// This function will be given the postfix expression produced by the 
// Shunting Yard algorithm as input. It will attempt to evaluate the expression, 
// returning true if evaluation was successful. If evaluation fails 
// (because the postfix expression was invalid), the function will return false 
// instead. The result will be saved in ‘result’ if the function returns true.
// Including variable map as an input. Values will be inserted into 
// the variable map in the driver, and they will be retrieved in 
// evaluatePostfix(). At any point in time, all variables that have been 
// assigned by the program so far will be stored in the variable map.

bool evaluatePostfix(const Vector<string>& postfix,
    const Map<string, double>& variables, double& result)
{   
    // Declaring & initializing
    double value1 = 0;
    double value2 = 0;
    // To differentiate between a trig function and non trig function string
    int trigFunc = 0;
    
    // Creating the auxiliary track (auxStack object).
    Stack<double> auxStack;
    
    for (int i = 0; i < postfix.getSize(); i++)
    {           
        if (postfix.get(i) == "min")
        {
            if (operatorValues(auxStack, value1, value2, trigFunc))
                // Compute arithmetic and push it onto the auxiliary stack
                auxStack.push(fmin (value1, value2));
            else return false;            
        }
        else if (postfix.get(i) == "max")
        {
            if (operatorValues(auxStack, value1, value2, trigFunc))
                // Compute arithmetic and push it onto the auxiliary stack
                auxStack.push(fmax (value1, value2));
            else return false;            
        }
        else if (postfix.get(i) == "sin")
        {
            trigFunc = 1;            
            if (operatorValues(auxStack, value1, value2, trigFunc))
                // Compute arithmetic and push it onto the auxiliary stack
                auxStack.push(sin (value1));
            else return false;
        }
        else if (postfix.get(i) == "cos")
        {
            trigFunc = 1;
            if (operatorValues(auxStack, value1, value2, trigFunc))
                // Compute arithmetic and push it onto the auxiliary stack
                auxStack.push(cos (value1));
            else return false;
        }
        else if (postfix.get(i) == "tan")
        {
            trigFunc = 1;
            if (operatorValues(auxStack, value1, value2, trigFunc))
                // Compute arithmetic and push it onto the auxiliary stack
                auxStack.push(tan (value1));
            else return false;
        }        
        else if (variables.search(postfix[i], value1))
        {
            auxStack.push(value1);
        }        
        else if (postfix.get(i) == "+")
        {         
            if (operatorValues(auxStack, value1, value2, trigFunc))
                // Compute arithmetic and push it onto the auxiliary stack
                auxStack.push(value1 + value2);
            else return false;
        } 
        else if (postfix.get(i) == "-")
        {
            if (operatorValues(auxStack, value1, value2, trigFunc))
                auxStack.push(value1 - value2);
            else return false;
        } 
        else if (postfix.get(i) == "*")
        {
            if (operatorValues(auxStack, value1, value2, trigFunc))
                auxStack.push(value1 * value2);
            else return false;
        } 
        else if (postfix.get(i) == "/")
        {
            if (operatorValues(auxStack, value1, value2, trigFunc))
                auxStack.push(value1 / value2);
            else return false;
        }
        else
        {
            // Converting string to double value.
            double value = atof(postfix[i].c_str());
 
            // Pushing the double value directly onto the auxiliary stack
            auxStack.push(value);
        }
    }
    
    // If there is only 1 element (result) in the auxiliary stack return true
    if (auxStack.size() == 1)
    {
        // Pop the result
        auxStack.pop(result);
        return true;
    }
    else return false;
}

// Driver
// Values will be inserted into the variable map in the driver, and they will 
// be retrieved in evaluatePostfix().

int main()
{  
    // Map data structure
    Map<string, double> variables;
    
    string str;    
    // Until user decides to quit the program
    while (str != "Q")
    {
        // reads an entire line into a string, and then tokenizes it separately
        Vector<string> expression;

        cout << "\nEnter infix expression (mathematical expression) with spaces in ";
        cout << "between elements.\nOr enter a variable assignment.\n";
        cout << "Then press Enter.\nPress 'Q' to quit the program.\n";

        // to read the line from cin and store the result in ‘str’.
        getline(cin, str);
        
        // special C++ object called a “string stream” to perform the 
        // tokenizing process.
        stringstream ss(str);

        while (ss >> str)
            expression.pushBack(str);        
        
        Vector<string> postfix;
        
        int startIndex = 0;
        // Assignment expression
        if (expression.getSize() >= 3 && expression[1] == "=")
        {
            startIndex = 2;           
            if (shuntingYard(expression, startIndex, postfix))
            {                                     
                cout << "Postfix: ";
                postfix.print();

                double result = 0;
                if (evaluatePostfix(postfix, variables, result))
                {
                    // Evaluate expression[2, infinity]
                    variables.insert(expression[0], result);
                                       
                    cout << "Result: " << result << endl;
                } 
                else
                    cout << "Expression was malformed.\n";       
            } 
            else
                cout << "Mismatched parentheses.\n";
        }
        // Regular expression
        else
        {
            if (shuntingYard(expression, startIndex, postfix))
            {                                     
                cout << "Postfix: ";
                postfix.print();

                double result = 0;
                if (evaluatePostfix(postfix, variables, result))
                    cout << "Result: " << result << endl; 
                else 
                    cout << "Expression was malformed.\n";        
            } 
            else 
                cout << "Mismatched parentheses.\n";
        }
    }    
    return 0;
}