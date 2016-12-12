#include "Alpha.h"

Alpha_Cord Alpha::AlphaCheckBoard(char Text)
{
	switch (Text)
	{
	case 'A':return{ 0,0 }; break;
	case 'B':return{ 0,1 }; break;
	case 'C':return{ 0,2 }; break;
	case 'D':return{ 0,3 }; break;
	case 'E':return{ 0,4 }; break;
	case 'F':return{ 0,5 }; break;
	case 'G':return{ 0,6 }; break;
	case 'H':return{ 0,7 }; break;
	case 'I':return{ 0,8 }; break;
	case 'J':return{ 0,9 }; break;
	case 'K':return{ 0,10 }; break;
	case 'L':return{ 0,11 }; break;
	case 'M':return{ 0,12 }; break;

	case 'N':return{ 1,0 }; break;
	case 'O':return{ 1,1 }; break;
	case 'P':return{ 1,2 }; break;
	case 'Q':return{ 1,3 }; break;
	case 'R':return{ 1,4 }; break;
	case 'S':return{ 1,5 }; break;
	case 'T':return{ 1,6 }; break;
	case 'U':return{ 1,7 }; break;
	case 'V':return{ 1,8 }; break;
	case 'W':return{ 1,9 }; break;
	case 'X':return{ 1,10 }; break;
	case 'Y':return{ 1,11 }; break;
	case 'Z':return{ 1,12 }; break;

	case 'a':return{ 2,0 }; break;
	case 'b':return{ 2,1 }; break;
	case 'c':return{ 2,2 }; break;
	case 'd':return{ 2,3 }; break;
	case 'e':return{ 2,4 }; break;
	case 'f':return{ 2,5 }; break;
	case 'g':return{ 2,6 }; break;
	case 'h':return{ 2,7 }; break;
	case 'i':return{ 2,8 }; break;
	case 'j':return{ 2,9 }; break;
	case 'k':return{ 2,10 }; break;
	case 'l':return{ 2,11 }; break;
	case 'm':return{ 2,12 }; break;

	case 'n':return{ 3,0 }; break;
	case 'o':return{ 3,1 }; break;
	case 'p':return{ 3,2 }; break;
	case 'q':return{ 3,3 }; break;
	case 'r':return{ 3,4 }; break;
	case 's':return{ 3,5 }; break;
	case 't':return{ 3,6 }; break;
	case 'u':return{ 3,7 }; break;
	case 'v':return{ 3,8 }; break;
	case 'w':return{ 3,9 }; break;
	case 'x':return{ 3,10 }; break;
	case 'y':return{ 3,11 }; break;
	case 'z':return{ 3,12 }; break;

	case '0':return{ 4,0 }; break;
	case '1':return{ 4,1 }; break;
	case '2':return{ 4,2 }; break;
	case '3':return{ 4,3 }; break;
	case '4':return{ 4,4 }; break;
	case '5':return{ 4,5 }; break;
	case '6':return{ 4,6 }; break;
	case '7':return{ 4,7 }; break;
	case '8':return{ 4,8 }; break;
	case '9':return{ 4,9 }; break;
	case '+':return{ 4,10 }; break;
	case '-':return{ 4,11 }; break;
	case '=':return{ 4,12 }; break;

	case '.':return{ 5,0 }; break;
	case ',':return{ 5,1 }; break;
	case '!':return{ 5,2 }; break;
	case '?':return{ 5,3 }; break;
	case '@':return{ 5,4 }; break;
	case ':':return{ 5,5 }; break;
	case ';':return{ 5,6 }; break;
	case '[':return{ 5,7 }; break;
	case ']':return{ 5,8 }; break;
	case '(':return{ 5,9 }; break;
	case ')':return{ 5,10 }; break;
	case '_':return{ 5,11 }; break;
	case '/':return{ 5,12 }; break;

	case '{':return{ 6,0 }; break;
	case '}':return{ 6,1 }; break;
	case '|':return{ 6,2 }; break;
	case '~':return{ 6,3 }; break;
	case '^':return{ 6,4 }; break;
	case '#':return{ 6,5 }; break;
	case '$':return{ 6,6 }; break;
	case '%':return{ 6,7 }; break;
	case '&':return{ 6,8 }; break;
	case '*':return{ 6,9 }; break;

	default:
		return{ 0,13 };
		break;
	}
}