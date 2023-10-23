using System;
using System.Runtime.InteropServices;

namespace JaProj
{

    class Program
    {
        [DllImport(@"D:\Studia\Projects\AssemblerProject\JaProj\x64\Debug\JaAsm.dll")]
        static extern int mmx(byte[] list1, byte[] list2, byte[] list3);

        [DllImport(@"D:\Studia\Projects\AssemblerProject\JaProj\x64\Debug\JaCpp.dll")]

        static extern int add(int a, int b);

        static void Main(string[] args)
        {
            byte[] list1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
            byte[] list2 = { 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
            byte[] list3 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            mmx(list1, list2, list3);
            for (int i = 0; i <= 15; i++)
            {
                Console.Write(list3[i] + ", ");
            }

            Console.Write(" " );
        }
    }
}