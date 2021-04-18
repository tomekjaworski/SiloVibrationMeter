using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Native32.Joystick
{
    public static class Flags
    {
        public const int JOY_RETURNALL = (JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR | JOY_RETURNU | JOY_RETURNV | JOY_RETURNPOV | JOY_RETURNBUTTONS);
        public const int JOY_RETURNBUTTONS = 0x80;
        public const int JOY_RETURNCENTERED = 0x400;
        public const int JOY_RETURNPOV = 0x40;
        public const int JOY_RETURNPOVCTS = 0x200;
        public const int JOY_RETURNR = 0x8;
        public const int JOY_RETURNRAWDATA = 0x100;
        public const int JOY_RETURNU = 0x10;
        public const int JOY_RETURNV = 0x20;
        public const int JOY_RETURNX = 0x1;
        public const int JOY_RETURNY = 0x2;
        public const int JOY_RETURNZ = 0x4;

        public const int JOY_CAL_READ3 = 0x40000;
        public const int JOY_CAL_READ4 = 0x80000;
        public const int JOY_CAL_READ5 = 0x400000;
        public const int JOY_CAL_READ6 = 0x800000;
        public const int JOY_CAL_READALWAYS = 0x10000;
        public const int JOY_CAL_READRONLY = 0x2000000;
        public const int JOY_CAL_READUONLY = 0x4000000;
        public const int JOY_CAL_READVONLY = 0x8000000;
        public const int JOY_CAL_READXONLY = 0x100000;
        public const int JOY_CAL_READXYONLY = 0x20000;
        public const int JOY_CAL_READYONLY = 0x200000;
        public const int JOY_CAL_READZONLY = 0x1000000;
    }

    public static class Buttons
    {
        public const int JOY_BUTTON1 = 0x1;
        public const int JOY_BUTTON10 = 0x200;
        public const int JOY_BUTTON11 = 0x400;
        public const int JOY_BUTTON12 = 0x800;
        public const int JOY_BUTTON13 = 0x1000;
        public const int JOY_BUTTON14 = 0x2000;
        public const int JOY_BUTTON15 = 0x4000;
        public const int JOY_BUTTON16 = 0x8000;
        public const int JOY_BUTTON17 = 0x10000;
        public const int JOY_BUTTON18 = 0x20000;
        public const int JOY_BUTTON19 = 0x40000;
        public const int JOY_BUTTON1CHG = 0x100;
        public const int JOY_BUTTON2 = 0x2;
        public const int JOY_BUTTON20 = 0x80000;
        public const int JOY_BUTTON21 = 0x100000;
        public const int JOY_BUTTON22 = 0x200000;
        public const int JOY_BUTTON23 = 0x400000;
        public const int JOY_BUTTON24 = 0x800000;
        public const int JOY_BUTTON25 = 0x1000000;
        public const int JOY_BUTTON26 = 0x2000000;
        public const int JOY_BUTTON27 = 0x4000000;
        public const int JOY_BUTTON28 = 0x8000000;
        public const int JOY_BUTTON29 = 0x10000000;
        public const int JOY_BUTTON2CHG = 0x200;
        public const int JOY_BUTTON3 = 0x4;
        public const int JOY_BUTTON30 = 0x20000000;
        public const int JOY_BUTTON31 = 0x40000000;
        public const int JOY_BUTTON32 = unchecked((int)0x80000000);
        public const int JOY_BUTTON3CHG = 0x400;
        public const int JOY_BUTTON4 = 0x8;
        public const int JOY_BUTTON4CHG = 0x800;
        public const int JOY_BUTTON5 = 0x10;
        public const int JOY_BUTTON6 = 0x20;
        public const int JOY_BUTTON7 = 0x40;
        public const int JOY_BUTTON8 = 0x80;
        public const int JOY_BUTTON9 = 0x100;
    }

    public static class Errors
    {
        public const int JOYERR_BASE = 160;
        public const int JOYERR_NOCANDO = (JOYERR_BASE + 6);
        public const int JOYERR_NOERROR = (0);
        public const int JOYERR_PARMS = (JOYERR_BASE + 5);
        public const int JOYERR_UNPLUGGED = (JOYERR_BASE + 7);
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct JOYCAPS
    {
        public short wMid;
        public short wPid;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string szPname;
        public short wXmin;
        public short wXmax;
        public short wYmin;
        public short wYmax;
        public short wZmin;
        public short wZmax;
        public short wNumButtons;
        public short wPeriodMin;
        public short wPeriodMax;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct JOYINFO
    {
        public int wXpos;
        public int wYpos;
        public int wZpos;
        public int wButtons;
    }

    /// <summary>
    /// Value type containing joystick position information.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct JOYINFOEX
    {
        /// <summary>Size of structure, in bytes.</summary>
        public int Size;
        /// <summary>Flags to indicate what information is valid for the device.</summary>
        public int flags;
        /// <summary>X axis.</summary>
        public int X;
        /// <summary>Y axis.</summary>
        public int Y;
        /// <summary>Z axis.</summary>
        public int Z;
        /// <summary>Rudder position.</summary>
        public int Rudder;
        /// <summary>5th axis position.</summary>
        public int Axis5;
        /// <summary>6th axis position.</summary>
        public int Axis6;
        /// <summary>State of buttons.</summary>
        public int buttons;
        /// <summary>Currently pressed button.</summary>
        public int ButtonNumber;
        /// <summary>Angle of the POV hat, in degrees (0 - 35900, divide by 100 to get 0 - 359 degrees.</summary>
        public int POV;
        /// <summary>Reserved.</summary>
        int Reserved1;
        /// <summary>Reserved.</summary>
        int Reserved2;
    }

}
