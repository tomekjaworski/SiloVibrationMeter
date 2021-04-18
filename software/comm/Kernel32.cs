using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace comm
{
    public static class Windows
    {
        [DllImport("kernel32.dll")]
        public static extern uint GetTickCount();
    }
}
