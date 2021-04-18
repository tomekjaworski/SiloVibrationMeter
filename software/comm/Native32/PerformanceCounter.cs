using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace Native32
{
    public class QueryPerfCounter
    {
        private long start;
        private long stop;
        private long frequency;
        Decimal multiplier = new Decimal(1.0e9);

        public QueryPerfCounter()
        {
            if (Native32.Kernel32.QueryPerformanceFrequency(out frequency) == false)
            {
                // Frequency not supported
                throw new Win32Exception();
            }
        }

        public void Start()
        {
            Native32.Kernel32.QueryPerformanceCounter(out start);
        }

        public void Stop()
        {
            Native32.Kernel32.QueryPerformanceCounter(out stop);
        }

        public double Duration(int iterations)
        {
            return ((((double)(stop - start) * (double)multiplier) / (double)frequency) / iterations);
        }

        /// <summary>
        /// Returns elapsed time in milliseconds
        /// </summary>
        /// <returns></returns>
        public double Duration()
        {
            return (double)(stop - start) / (double)frequency;
        }
    }

}
