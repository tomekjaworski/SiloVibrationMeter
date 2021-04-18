using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Security;

namespace Native32
{
    public static class Kernel32
    {
        private const string dll = "kernel32.dll";

        [DllImport(dll), SuppressUnmanagedCodeSecurity]
        public static extern bool QueryPerformanceCounter(out long lpPerformanceCount);

        [DllImport(dll), SuppressUnmanagedCodeSecurity]
        public static extern bool QueryPerformanceFrequency(out long frequency);

        [DllImport(dll)]
        public static extern UInt32 GetTickCount();

        [DllImport(dll)]
        public static extern UInt64 GetTickCount64();


        [DllImport(dll, SetLastError = true)]
        public static extern IntPtr LoadLibrary(string lpFileName);

        [DllImport(dll, SetLastError = true)]
        public static extern bool FreeLibrary(IntPtr hModule);

        [DllImport(dll, CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string procName);
    }
}
