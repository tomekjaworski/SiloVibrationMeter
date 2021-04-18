using System;
using System.Collections.Generic;
using System.Text;

namespace UDPTerminal
{
    public class StringUtil
    {
        public static String ToString(double value, string unit)
        {
            if (unit == null)
                unit = string.Empty;

            char[] w1 = new char[] { 'k', 'M', 'G', 'T', 'P' };
            char[] w2 = new char[] { 'm', 'u', 'n', 'p', 'f', 'a' };

            int index = 0;
            if (value != 0)
            while ((Math.Abs(value) < 1) && (Math.Abs(value) <= 1000))
            {
                if (value < 1)
                {
                    value *= 1000;
                    index -= 1;
                }

                if (value >= 1000)
                {
                    value /= 1000;
                    index += 1;
                }

            }

            string p="";
            if (index > 0)
                p = new String(w1[index - 1], 1);
            if (index < 0)
                p = new String(w2[-index-1], 1);
            p = string.Format("{0:N2}{1}{2}", value, p, unit);
            return p;
        }
    }
}
