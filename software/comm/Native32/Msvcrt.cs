using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Native32
{
    public static class Msvcrt
    {
        [DllImport("msvcrt.dll", EntryPoint = "memcpy", CallingConvention = CallingConvention.Cdecl, SetLastError = false)]
        public static extern IntPtr memcpy(IntPtr dest, IntPtr src, UIntPtr count);
        [DllImport("msvcrt.dll", EntryPoint = "memcpy", CallingConvention = CallingConvention.Cdecl, SetLastError = false)]
        public unsafe static extern IntPtr memcpy(void* dest, void* src, uint count);
    }
}
