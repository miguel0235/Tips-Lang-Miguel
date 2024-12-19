PROGRAM SLOPE;
{ A Program to Calculate the Slope of a Line Given Two Points }

VAR
    X1: INTEGER;
    Y1: INTEGER; 
    X2: INTEGER;
    Y2: INTEGER;
    SLOPE: INTEGER;

BEGIN
    WRITE('Slope Calculator.');

    { Input the first point }
    WRITE('Enter the x-coordinate of the first point (X1): ');
    READ(X1);
    WRITE('Enter the y-coordinate of the first point (Y1): ');
    READ(Y1);

    { Input the second point }
    WRITE('Enter the x-coordinate of the second point (X2): ');
    READ(X2);
    WRITE('Enter the y-coordinate of the second point (Y2): ');
    READ(Y2);

    BEGIN
        { Calculate the slope }
        SLOPE := (Y2 - Y1) / (X2 - X1);

        { Print the result }
        WRITE('The slope of the line passing through ');
        WRITE(X1);
        WRITE(',');
        WRITE(Y1);
        WRITE(' and ');
        WRITE('');
        WRITE(X2);
        WRITE(', ');
        WRITE(Y2);
        WRITE(' is: ');
        WRITE(SLOPE);
    END;
END.