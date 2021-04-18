using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Security;

namespace Native32
{
    public static class User32
    {
        private const string dll = "user32.dll";
        public const int WM_SETREDRAW = 11;

        [DllImport(dll), SuppressUnmanagedCodeSecurity]
        public static extern short GetKeyState(Keys vKey);


        [DllImport(dll), SuppressUnmanagedCodeSecurity]
        public static extern IntPtr CreateIconIndirect(
            ref IconInfo icon);

        [DllImport(dll), SuppressUnmanagedCodeSecurity]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool GetIconInfo(IntPtr hIcon,
            ref IconInfo pIconInfo);


        [DllImport(dll), SuppressUnmanagedCodeSecurity]
        public static extern int SendMessage(IntPtr hWnd, Int32 wMsg, bool wParam, Int32 lParam);
        [DllImport(dll), SuppressUnmanagedCodeSecurity]
        public static extern int SendMessage(IntPtr hWnd, Int32 wMsg, Int32 wParam, Int32 lParam);


        public struct IconInfo
        {
            public bool fIcon;
            public int xHotspot;
            public int yHotspot;
            public IntPtr hbmMask;
            public IntPtr hbmColor;
        }

        [Flags]
        public enum MenuFlags : uint
        {
            MF_STRING = 0,
            MF_BYPOSITION = 0x400,
            MF_SEPARATOR = 0x800,
            MF_REMOVE = 0x1000,
        }

        [DllImport(dll), SuppressUnmanagedCodeSecurity]
        public static extern IntPtr GetSystemMenu(IntPtr hWnd, bool bRevert);

        [DllImport(dll), SuppressUnmanagedCodeSecurity]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool AppendMenu(IntPtr hMenu, MenuFlags Flagsw, uint IDNewItem, string lpNewItem);

        [Flags]
        public enum LISTVIEW_STYLES_EX
        {
            LVS_EX_GRIDLINES = 0x00000001,
            LVS_EX_SUBITEMIMAGES = 0x00000002,
            LVS_EX_CHECKBOXES = 0x00000004,
            LVS_EX_TRACKSELECT = 0x00000008,
            LVS_EX_HEADERDRAGDROP = 0x00000010,
            LVS_EX_FULLROWSELECT = 0x00000020,
            LVS_EX_ONECLICKACTIVATE = 0x00000040,
            LVS_EX_TWOCLICKACTIVATE = 0x00000080,
            LVS_EX_FLATSB = 0x00000100,
            LVS_EX_REGIONAL = 0x00000200,
            LVS_EX_INFOTIP = 0x00000400,
            LVS_EX_UNDERLINEHOT = 0x00000800,
            LVS_EX_UNDERLINECOLD = 0x00001000,
            LVS_EX_MULTIWORKAREAS = 0x00002000,
            LVS_EX_LABELTIP = 0x00004000,
            LVS_EX_BORDERSELECT = 0x00008000,
            LVS_EX_DOUBLEBUFFER = 0x00010000,
            LVS_EX_HIDELABELS = 0x00020000,
            LVS_EX_SINGLEROW = 0x00040000,
            LVS_EX_SNAPTOGRID = 0x00080000,
            LVS_EX_SIMPLESELECT = 0x00100000
        }

        [Flags]
        public enum LISTVIEW_MODES
        {
            LVM_FIRST = 0x1000,
            LVM_SETEXTENDEDLISTVIEWSTYLE = (LVM_FIRST + 54),
            LVM_GETEXTENDEDLISTVIEWSTYLE = (LVM_FIRST + 55),
        }


    }
}
