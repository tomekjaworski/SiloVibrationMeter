using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Net.Sockets;

namespace Native32
{
    public static class Winsock
    {
        private const string dll = "shell.dll";

        [DllImport(dll, SetLastError = true)]
        public static extern int select(
            [In] int nfds,
            [In, Out] IntPtr[] readfds,
            [In, Out] IntPtr[] writefds,
            [In, Out] IntPtr[] exceptfds,
            [In] ref TimeValue timeout);

        [DllImport(dll, SetLastError = true)]
        public static extern unsafe int send(
            [In] IntPtr socketHandle,
            [In] byte* pinnedBuffer,
            [In] int len,
            [In] SocketFlags socketFlags);

        [DllImport(dll, SetLastError = true)]
        public static extern IntPtr gethostbyaddr(
            [In] ref int addr,
            [In] int len,
            [In] ProtocolFamily type);

        [DllImport(dll, CharSet = CharSet.Ansi, SetLastError = true)]
        public static extern IntPtr gethostbyname([In] string host);

        [DllImport(dll, SetLastError = true)]
        public static extern SocketError getsockopt([In] IntPtr socketHandle, [In] SocketOptionLevel optionLevel, [In] SocketOptionName optionName, out int optionValue, [In, Out] ref int optionLength);
        
        [DllImport(dll, SetLastError = true)]
        public static extern SocketError getsockopt([In] IntPtr socketHandle, [In] SocketOptionLevel optionLevel, [In] SocketOptionName optionName, [Out] byte[] optionValue, [In, Out] ref int optionLength);
        
        [DllImport(dll, SetLastError = true)]
        public static extern SocketError getsockopt([In] IntPtr socketHandle, [In] SocketOptionLevel optionLevel, [In] SocketOptionName optionName, out Linger optionValue, [In, Out] ref int optionLength);
        
        [DllImport(dll, CharSet = CharSet.Ansi, SetLastError = true)]
        public static extern int inet_addr([In] string cp);

        [DllImport("ws2_32.dll", SetLastError = true)]
        public static extern SocketError ioctlsocket([In] IntPtr socketHandle, [In] int cmd, [In, Out] ref int argp);
        
        [DllImport(dll, SetLastError = true)]
        public static extern SocketError listen([In] IntPtr socketHandle, [In] int backlog);
        
        [DllImport(dll, SetLastError = true)]
        public static extern unsafe int recv([In] IntPtr socketHandle, [In] byte* pinnedBuffer, [In] int len, [In] SocketFlags socketFlags);
        
        [DllImport(dll, SetLastError = true)]
        public static extern unsafe int recvfrom([In] IntPtr socketHandle, [In] byte* pinnedBuffer, [In] int len, [In] SocketFlags socketFlags, [Out] byte[] socketAddress, [In, Out] ref int socketAddressSize);
        
        [DllImport(dll, SetLastError = true)]
        public static extern int select([In] int ignoredParameter, [In, Out] IntPtr[] readfds, [In, Out] IntPtr[] writefds, [In, Out] IntPtr[] exceptfds, [In] IntPtr nullTimeout);
        
        [DllImport(dll, SetLastError = true)]
        public static extern unsafe int sendto([In] IntPtr socketHandle, [In] byte* pinnedBuffer, [In] int len, [In] SocketFlags socketFlags, [In] byte[] socketAddress, [In] int socketAddressSize);
        
        [DllImport(dll, SetLastError = true)]
        public static extern SocketError setsockopt([In] IntPtr socketHandle, [In] SocketOptionLevel optionLevel, [In] SocketOptionName optionName, [In] ref IntPtr pointer, [In] int optionLength);
        
        [DllImport(dll, SetLastError = true)]
        public static extern SocketError setsockopt([In] IntPtr socketHandle, [In] SocketOptionLevel optionLevel, [In] SocketOptionName optionName, [In] byte[] optionValue, [In] int optionLength);
        
        [DllImport(dll, SetLastError = true)]
        public static extern SocketError setsockopt([In] IntPtr socketHandle, [In] SocketOptionLevel optionLevel, [In] SocketOptionName optionName, [In] ref int optionValue, [In] int optionLength);
        
        [DllImport(dll, SetLastError = true)]
        public static extern SocketError setsockopt([In] IntPtr socketHandle, [In] SocketOptionLevel optionLevel, [In] SocketOptionName optionName, [In] ref Linger linger, [In] int optionLength);
        
        [DllImport(dll, SetLastError = true)]
        public static extern SocketError shutdown([In] IntPtr socketHandle, [In] int how);


        [StructLayout(LayoutKind.Sequential)]
        public struct Linger
        {
            internal short OnOff;
            internal short Time;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct TimeValue
        {
            public int Seconds;
            public int Microseconds;
        }


    }
}
