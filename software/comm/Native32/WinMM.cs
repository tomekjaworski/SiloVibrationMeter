using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using Native32.Joystick;

namespace Native32
{
    public static class WinMM
    {
        [DllImport("WinMM.dll"), System.Security.SuppressUnmanagedCodeSecurity]
        public static extern int joyGetNumDevs();

        [DllImport("WinMM.dll"), System.Security.SuppressUnmanagedCodeSecurity]
        public static extern int joyGetPosEx(
            [In] int uJoyID,
            [MarshalAs(UnmanagedType.Struct)] ref JOYINFOEX p);

        [DllImport("WinMM.dll"), System.Security.SuppressUnmanagedCodeSecurity]
        public static extern int joyGetDevCaps(int id, ref JOYCAPS lpCaps, int uSize);

        [DllImport("WinMM.dll"), System.Security.SuppressUnmanagedCodeSecurity]
        public static extern int joyGetPos(int uJoyID, ref JOYINFO pji);
    }
}
