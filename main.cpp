#include <windows.h>

using namespace System;

namespace test {
   public
    ref class Arith {
       public:
        /// <summary>
        /// add two numbers
        /// </summary>
        static int Add(int a, int b) { return a + b; }
        static int Sub(int a, int b) { return a - b; }
        static int Mul(int a, int b) { return a * b; }
        static int Div(int a, int b) { return a / b; }
    };
}    // namespace test
