# Simple-Console-Class
A simple and useful Win c++ console class

## Features
+ Edit console title
+ Edit text color
+ Output log file
+ Support Win x64/x86
+ Support Multi Byte and Unicode
+ Variadic function

## Samples
```C++
#include "Console.h"

int main()
{
  CConsole Console;

	Console.Init(true, true); //void Init(bool Console, bool LogFile);

	Console.SetDefaultColor(ConsoleColor::Gray);
  
  	Console.SetTitle(TEXT("C++ Console Class"));

	Console.Log(TEXT("Hello World!"));

	Console.Log(TEXT("%s, %i, %f"), TEXT("String"), (int)2 + 2, (float)123.456);

	Console.Log(ConsoleColor::Green, TEXT("Simple colored string"));

	return 0;
}
```
![Console](https://i.imgur.com/s3ycsmh.png)

## Unicode examples
Russian:
![Russian](https://i.imgur.com/TWEuMU3.png)
Português:
![Português](https://i.imgur.com/3FUtdFy.png)

## References
http://www.cplusplus.com/forum/articles/16820/

https://msdn.microsoft.com/pt-br/library/xa1a1a6z.aspx

https://bitismyth.wordpress.com/2017/04/30/charset-e-coisa-seria/
