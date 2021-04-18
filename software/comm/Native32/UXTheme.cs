using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Native32
{
    public static class UXTheme
    {
        [DllImport("uxtheme.dll")]
        public static extern int SetWindowTheme(
            [In] IntPtr hwnd,
            [In, MarshalAs(UnmanagedType.LPWStr)] string pszSubAppName,
            [In, MarshalAs(UnmanagedType.LPWStr)] string pszSubIdList
            );

    }
}
