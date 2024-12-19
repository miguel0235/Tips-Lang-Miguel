PROGRAM ADDITION;
{ A Simple Calculator to Add Two Numbers }
VAR
    NUMBER1: INTEGER;
    NUMBER2: INTEGER;
    RESULT: INTEGER;

BEGIN
    { Explain the program }
    WRITE('Addition Calculator.');
    
    { Get the first number }
    WRITE('Enter the first number: ');
    READ(NUMBER1);
    
    { Get the second number }
    WRITE('Enter the second number: ');
    READ(NUMBER2);
    
    { Calculate the sum }
    RESULT := NUMBER1 + NUMBER2;
    
    { Print the result }
    WRITE('The sum of ');
    WRITE(NUMBER1);
    WRITE(' and ');
    WRITE(NUMBER2);
    WRITE(' is: ');
    WRITE(RESULT);
END