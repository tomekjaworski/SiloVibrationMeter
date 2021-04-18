using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.ComponentModel;
using System.IO;
using System.Threading;

namespace comm
{

    public class Device : IDisposable
    {

        public IPAddress LocalIPAddress;
        public UInt32 RandomSeed;

        /// <summary>
        /// Adres sieciowy akcelerometru
        /// </summary>
        public IPAddress DeviceIPAddress;

        /// <summary>
        /// Adres bramy wyjściowej sieci w której znajduje się akcelerometr
        /// </summary>
        public IPAddress IPGateway;

        /// <summary>
        /// Maska sieci w której znajduje się akcelerometr
        /// </summary>
        public IPAddress IPNetmask;

        /// <summary>
        /// Unikalny identyfikator urządzenia (mam nadzieję, że unikalny)
        /// </summary>
        public UInt16 DeviceID;

        /// <summary>
        /// Adres sprzętowy interfejsu sieciowego (MAC)
        /// </summary>
        public byte[] HardwareAddress;

        /// <summary>
        /// Data i czas kompilacji firmware'u
        /// </summary>
        [Category("Information"), Description("A timestamp of the firmware's compilation")]
        public string Timestamp
        {
            get { return this.timestamp; }
        }

        /// <summary>
        /// Wersja firware'u
        /// </summary>
        [Category("Information"), Description("Version of the installed firmware")]
        public String Version
        {
            get { return this.version; }
        }

        internal string version;
        internal string timestamp;
        private string name;

        public object Tag;

        public int BytesSent;
        public int BytesReceived;

        private Socket control_socket;
        private Socket data_socket;

        private UInt16 x_dac;
        private UInt16 y_dac;
        private UInt16 z_dac;
        private UInt16 x_gain;
        private UInt16 y_gain;
        private UInt16 z_gain;

        private UInt16 zero_gain;

        private IPEndPoint control_channel;
        private IPEndPoint data_channel;

        private int measures_per_packet;
        private int max_measures_per_packet;
        private double frequency;

        bool analog_configuration_modified;

        private string dump_file_name;
        private Thread dump_thread;
        private bool dump_done;


        private int avg_x_sum;
        private int avg_x_position;
        private int[] avg_x_array;
        private int avg_y_sum;
        private int avg_y_position;
        private int[] avg_y_array;
        private int avg_z_sum;
        private int avg_z_position;
        private int[] avg_z_array;

        [Category("Podgląd - uśrednianie"), Description("Oś X: ilość elementów branych do średniej kroczącej")]
        public int XAvgLimit
        {
            get { return this.avg_x_array.Length; }
            set
            {
                lock (this)
                {
                    this.avg_x_position = 0;
                    this.avg_x_sum = 0;
                    this.avg_x_array = new int[value];
                }
            }
        }

        [Category("Podgląd - uśrednianie"), Description("Oś Y: ilość elementów branych do średniej kroczącej")]
        public int YAvgLimit
        {
            get { return this.avg_y_array.Length; }
            set
            {
                lock (this)
                {
                    this.avg_y_position = 0;
                    this.avg_y_sum = 0;
                    this.avg_y_array = new int[value];
                }
            }
        }

        [Category("Podgląd - uśrednianie"), Description("Oś Z: ilość elementów branych do średniej kroczącej")]
        public int ZAvgLimit
        {
            get { return this.avg_z_array.Length; }
            set
            {
                lock (this)
                {
                    this.avg_z_position = 0;
                    this.avg_z_sum = 0;
                    this.avg_z_array = new int[value];
                }
            }
        }


        [Browsable(false)]
        public int XAvg
        { get { return this.avg_x_sum / this.avg_x_array.Length; } }

        [Browsable(false)]
        public int YAvg
        { get { return this.avg_y_sum / this.avg_y_array.Length; } }

        [Browsable(false)]
        public int ZAvg
        { get { return this.avg_z_sum / this.avg_z_array.Length; } }

        [Browsable(false)]
        public float XAvgV
        { get { return ((float)(this.avg_x_sum / this.avg_x_array.Length) * 3.3f) / 4096.0f; } }

        [Browsable(false)]
        public float YAvgV
        { get { return ((float)(this.avg_y_sum / this.avg_y_array.Length) * 3.3f) / 4096.0f; } }

        [Browsable(false)]
        public float ZAvgV
        { get { return ((float)(this.avg_z_sum / this.avg_z_array.Length) * 3.3f) / 4096.0f; } }

        /// <summary>
        /// Wartość napięcia odniesienia dla osi X
        /// </summary>
        [Category("Stopień wejściowy - napięcia"), Description("Napięcie odniesienia wzmacniacza dla osi X, w woltach")]
        public float XOffsetVoltage
        {
            get { return this.GetDACVoltage(Axis.X); }
            set { this.SetDACVoltage(Axis.X, value); }
        }

        /// <summary>
        /// Wartość napięcia odniesienia dla osi Y
        /// </summary>
        [Category("Stopień wejściowy - napięcia"), Description("Napięcie odniesienia wzmacniacza dla osi Y, w woltach")]
        public float YOffsetVoltage
        {
            get { return this.GetDACVoltage(Axis.Y); }
            set { this.SetDACVoltage(Axis.Y, value); }
        }

        /// <summary>
        /// Wartość napięcia odniesienia dla osi Z
        /// </summary>
        [Category("Stopień wejściowy - napięcia"), Description("Napięcie odniesienia wzmacniacza dla osi Z, w woltach")]
        public float ZOffsetVoltage
        {
            get { return this.GetDACVoltage(Axis.Z); }
            set { this.SetDACVoltage(Axis.Z, value); }
        }



        /// <summary>
        /// Wartość wzmocnienia dla osi X
        /// </summary>
        [Category("Stopień wejściowy - wzmocnienia"), Description("Wzmocnienie stopnia wejściowego osi X")]
        public GainLevel XGain
        {
            get { return this.GetGainLevel(Axis.X); }
            set { this.SetGainLevel(Axis.X, value); }
        }

        /// <summary>
        /// Wartość wzmocnienia dla osi Y
        /// </summary>
        [Category("Stopień wejściowy - wzmocnienia"), Description("Wzmocnienie stopnia wejściowego osi Y")]
        public GainLevel YGain
        {
            get { return this.GetGainLevel(Axis.Y); }
            set { this.SetGainLevel(Axis.Y, value); }
        }


        /// <summary>
        /// Wartość wzmocnienia dla osi Z
        /// </summary>
        [Category("Stopień wejściowy - wzmocnienia"), Description("Wzmocnienie stopnia wejściowego osi Z")]
        public GainLevel ZGain
        {
            get { return this.GetGainLevel(Axis.Z); }
            set { this.SetGainLevel(Axis.Z, value); }
        }

        [Category("Stopień pomiarowy"), Description("Ilość pomiarów na pakiet")]
        public int MeasuresPerPacket
        {
            get { return measures_per_packet; }
            set
            {
                if (value <= 0)
                    throw new ArgumentOutOfRangeException("Ilość pomiarów na jeden pakiet UDP musi być większa od zera!");
                if (value > this.max_measures_per_packet)
                    throw new ArgumentOutOfRangeException(String.Format("Ilość pomiarów na jeden pakiet UDP musi być mniejsza bądź równa {0}!", this.max_measures_per_packet));
                this.measures_per_packet = value;
            }

        }

        [Category("Stopień pomiarowy"), Description("Częstotliwość wykonywania pomiarów [Hz]")]
        public double Frequency
        {
            get { return this.frequency; }
            set
            {
                int dummy1, dummy2;
                if (!this.TrySetFrequency(value, out dummy1, out dummy2))
                    throw new ArgumentOutOfRangeException("Częstotliwość pomiarów nie w zakresie!\nMinimum: 2,4Hz; Maximum: 1MHz");
                this.frequency = value;
            }
        }

        [Description("Nazwa pliku z pomiarami")]
        public string FileName
        {
            get { return this.dump_file_name; }
            set { this.dump_file_name = value; }
        }



        public Device()
        {
            this.control_socket = null;
            this.analog_configuration_modified = false;

            this.frequency = 10;
            this.measures_per_packet = 10;
            this.max_measures_per_packet = 127;

            this.avg_x_array = new int[8];
            this.avg_y_array = new int[8];
            this.avg_z_array = new int[8];

            this.dump_file_name = "";
            //Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            //s.EnableBroadcast = true;
            //byte[] b = new byte[1515-42];
            //int bytes  = s.SendTo(b, new IPEndPoint(IPAddress.Parse("255.255.255.255"), 12345));
        }

        public void Close()
        {
            this.dump_done = true;
        }

        public void ReadConfiguration()
        {
            this.CheckSocket();

            this.SendCommand('C');
            byte[] buffer = this.WaitForReply(2000);

            if (buffer == null)
                throw new Exception("ReadConfiguration: Timeout 1");

            BinaryReader br = new BinaryReader(new MemoryStream(buffer));

            uint magic = br.ReadUInt32();
            uint command = br.ReadUInt16();

            if (magic != 0x01CAFE02 || command != 'C')
                throw new Exception("ReadConfiguration: Recv 1");

            int analog_config_size = br.ReadUInt16();

            this.x_dac = br.ReadUInt16();
            this.y_dac = br.ReadUInt16();
            this.z_dac = br.ReadUInt16();

            this.x_gain = br.ReadUInt16();
            this.y_gain = br.ReadUInt16();
            this.z_gain = br.ReadUInt16();

            this.zero_gain = br.ReadUInt16();

            this.analog_configuration_modified = false;
        }

        public void WriteConfiguration()
        {
            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);

            ushort size = 2 * 3 + 2 * 3 + 2 * 1;
            bw.Write(size);

            bw.Write(this.z_dac);
            bw.Write(this.x_dac);
            bw.Write(this.y_dac);

            bw.Write(this.z_gain);
            bw.Write(this.x_gain);
            bw.Write(this.y_gain);

            bw.Write(this.zero_gain);


            this.SendCommand('D', ms.ToArray());

            byte[] resp = this.WaitForReply(1000);

            BinaryReader br = new BinaryReader(new MemoryStream(resp));

            uint magic = br.ReadUInt32();
            uint command = br.ReadUInt16();

            if (magic != 0x01CAFE02 || command != 'D')
                throw new Exception("ReadConfiguration: Recv 1");

            this.analog_configuration_modified = false;

        }
        #region komunikacja
        private byte[] WaitForReply(int timeout)
        {
            UInt32 start_time = Native32.Kernel32.GetTickCount();

            List<Socket> read_fds = new List<Socket>();
            while (true)
            {
                read_fds.Clear();
                read_fds.Add(this.control_socket);

                Socket.Select(read_fds, null, null, 10 * 1000);
                if (read_fds.Contains(this.control_socket))
                {
                    EndPoint resp = new IPEndPoint(0, 0);
                    byte[] recv_buffer = new byte[1024 * 4];
                    int recvd = this.control_socket.ReceiveFrom(recv_buffer, ref resp);
                    if (recvd == 0)
                        continue;

                    byte[] b = new byte[recvd];
                    Array.Copy(recv_buffer, b, recvd);

                    Interlocked.Add(ref this.BytesReceived, recvd);

                    return b;
                }

                if (Native32.Kernel32.GetTickCount() - start_time > timeout)
                    break;
            }

            return null; // timeout
        }


        private void SendCommand(char cmd, byte[] data)
        {
            if (data == null)
                data = new byte[0];

            byte[] b = new byte[2 + data.Length];
            b[0] = (byte)cmd;
            b[1] = 0;
            Array.Copy(data, 0, b, 2, data.Length);

            int r = this.control_socket.SendTo(b, this.control_channel);
            if (r != b.Length)
                throw new Exception("SendCommand: SendTo");

            Interlocked.Add(ref this.BytesSent, r);

        }

        private void SendCommand(char cmd)
        {
            this.SendCommand(cmd, null);
        }
#endregion

        private void AnalogConfigurationModified()
        {
            this.analog_configuration_modified = true;
            this.WriteConfiguration();
        }


        private void CheckSocket()
        {
            if (this.control_socket != null)
                return;

            this.control_channel = new IPEndPoint(this.DeviceIPAddress, 7000);
            this.data_channel = new IPEndPoint(this.DeviceIPAddress, 8000);

            this.control_socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

        }

        #region offset i wzmocnienie
        private float GetDACVoltage(Axis axis)
        {
            uint cfg = 0;
            if (axis == Axis.X) cfg = this.x_dac;
            if (axis == Axis.Y) cfg = this.y_dac;
            if (axis == Axis.Z) cfg = this.z_dac;

            cfg = cfg & 0x0FFF; // 12 najmlodszych bitów
            return 5.0f * ((float)cfg / 4095.0f);
        }
        private void SetDACVoltage(Axis axis, float voltage)
        {
            if (this.dump_thread != null)
                throw new InvalidOperationException("Strumień danych musi być zatrzymany");

            int level = (int)Math.Round((voltage / 5.0f) * 4095.0f);

            UInt16 cfg = 0;
            if (axis == Axis.X) cfg = this.x_dac;
            if (axis == Axis.Y) cfg = this.y_dac;
            if (axis == Axis.Z) cfg = this.z_dac;

            level = level & 0x0FFF;
            cfg = (ushort)(cfg & 0xF000);
            cfg = (UInt16)(cfg | level);

            if (axis == Axis.X) this.x_dac = cfg;
            if (axis == Axis.Y) this.y_dac = cfg;
            if (axis == Axis.Z) this.z_dac = cfg;

            this.AnalogConfigurationModified();
        }

        private GainLevel GetGainLevel(Axis axis)
        {
            uint cfg = 0;
            if (axis == Axis.X) cfg = this.x_gain;
            if (axis == Axis.Y) cfg = this.y_gain;
            if (axis == Axis.Z) cfg = this.z_gain;

            cfg = cfg & 0x0007; // 12 najmlodszych bitów
            return (GainLevel)cfg;
        }
        private void SetGainLevel(Axis axis, GainLevel gain)
        {
            if (this.dump_thread != null)
                throw new InvalidOperationException("Strumień danych musi być zatrzymany");

            int level = (int)gain;

            UInt16 cfg = 0;
            if (axis == Axis.X) cfg = this.x_gain;
            if (axis == Axis.Y) cfg = this.y_gain;
            if (axis == Axis.Z) cfg = this.z_gain;

            level = level & 0x0007;
            cfg = (UInt16)(cfg & 0xFFF8);
            cfg = (UInt16)(cfg | level);

            if (axis == Axis.X) this.x_gain = cfg;
            if (axis == Axis.Y) this.y_gain = cfg;
            if (axis == Axis.Z) this.z_gain = cfg;

            this.AnalogConfigurationModified();
        }
        #endregion

        private bool TrySetFrequency(double new_freq, out int divider1, out int divider2)
        {
            double Fosc = 80e6;
            double Fcy = Fosc / 2.0;

            new_freq = new_freq * 2; // na jeden odczyt potrzeba dwóch cykli zegara

            double P1 = (1.0 / 1.0) * (Fcy / new_freq) - 1.0;
            double P8 = (1.0 / 8.0) * (Fcy / new_freq) - 1.0;
            double P64 = (1.0 / 64.0) * (Fcy / new_freq) - 1.0;
            double P256 = (1.0 / 256.0) * (Fcy / new_freq) - 1.0;

            if (P1 < 65535)
            {
                divider1 = 0;
                divider2 = (int)Math.Round(P1);
                return true;
            }

            if (P8 < 65535)
            {
                divider1 = 1;
                divider2 = (int)Math.Round(P8);
                return true;
            }

            if (P64 < 65535)
            {
                divider1 = 2;
                divider2 = (int)Math.Round(P64);
                return true;
            }

            if (P256 < 65535)
            {
                divider1 = 3;
                divider2 = (int)Math.Round(P256);
                return true;
            }

            divider1 = 0;
            divider2 = 0;
            return false;

        }


        public void Start()
        {
            if (this.dump_thread != null)
                return;
                //throw new InvalidOperationException("Strumień danych musi być zatrzymany");

            this.CheckSocket();

            this.avg_x_position = 0;
            this.avg_x_sum = 0;
            Array.Clear(this.avg_x_array, 0, this.avg_x_array.Length);
            this.avg_y_position = 0;
            this.avg_y_sum = 0;
            Array.Clear(this.avg_y_array, 0, this.avg_y_array.Length);
            this.avg_z_position = 0;
            this.avg_z_sum = 0;
            Array.Clear(this.avg_z_array, 0, this.avg_z_array.Length);


            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);

            this.data_socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            this.data_socket.SendTo(new byte[0], data_channel);
            this.data_socket.ReceiveBufferSize = 64 * 1024;

            IPEndPoint ep = this.data_socket.LocalEndPoint as IPEndPoint;
            bw.Write((uint)this.LocalIPAddress.Address);
            bw.Write(IPAddress.HostToNetworkOrder((short)ep.Port));

            int div1, div2;
            bool q = this.TrySetFrequency(this.frequency, out div1, out div2);
            if (!q)
                throw new ArgumentOutOfRangeException("Niewłaściwa wartość częstotliwości");

            bw.Write((ushort)div1);
            bw.Write((ushort)div2);
            bw.Write((float)this.frequency);
            bw.Write((ushort)this.measures_per_packet);

            this.SendCommand('E', ms.ToArray());

            this.dump_done = false;
            this.dump_thread = new Thread(new ParameterizedThreadStart(DumpThread));

            this.dump_thread.Start(this);
        }

        public void Stop()
        {
            if (this.dump_thread == null)
                return; // throw new InvalidOperationException("Strumień danych nie może być zatrzymany");

            // stop!
            this.SendCommand('F');

            this.dump_done = true;
            this.dump_thread.Join();
            
            this.dump_thread = null;
            
            this.data_socket.Close();
            this.data_socket = null;
        }

        unsafe private static void DumpThread(object arg)
        {
            Device dev = arg as Device;
            uint time_start = Native32.Kernel32.GetTickCount();

            if (dev.FileName == string.Empty)
                dev.FileName = dev.DeviceID.ToString();

            DateTime dt = DateTime.Now;
            string file_name = String.Format("{6}_D{0:00}{1:00}{2:0000}_T{3:00}{4:00}{5:00}.txt",
                dt.Day, dt.Month, dt.Year,
                dt.Hour, dt.Minute, dt.Second,
                dev.FileName);
            

            using (StreamWriter dump_stream = new StreamWriter(new FileStream(file_name, FileMode.Create, FileAccess.Write, FileShare.Read)))
                try
                {
                    dump_stream.WriteLine("# Registration started at {0}", dt.ToString());
                    dump_stream.WriteLine("#"); 
                    dump_stream.WriteLine("# Hardware IP: {0}/{1}", dev.DeviceIPAddress, dev.IPNetmask);
                    dump_stream.WriteLine("# Firmware timestamp: {0}/{2}/ID={1}", dev.Timestamp, dev.DeviceID, dev.Version);
                    dump_stream.WriteLine("#"); 
                    dump_stream.WriteLine("# Measures per packet: {0}", dev.measures_per_packet);
                    dump_stream.WriteLine("# Measurement frequency: {0:N2}Hz", dev.Frequency);
                    dump_stream.WriteLine("# X Gain/Offset: {0}/{1:N5}V", dev.XGain, dev.XOffsetVoltage);
                    dump_stream.WriteLine("# Y Gain/Offset: {0}/{1:N5}V", dev.YGain, dev.YOffsetVoltage);
                    dump_stream.WriteLine("# Z Gain/Offset: {0}/{1:N5}V", dev.ZGain, dev.ZOffsetVoltage);
                    dump_stream.WriteLine("#");
                    dump_stream.WriteLine(""); 

                    List<Socket> rd = new List<Socket>();
                    byte[] buff = new byte[dev.measures_per_packet * (2 + 2 + 2 + 2) + 2];
                    while (!dev.dump_done)
                    {

                        uint time = Native32.Kernel32.GetTickCount();
                        if (time - time_start > 2500)
                        {
                            time_start = time;
                            dev.SendCommand('G');
                        }



                        rd.Clear();
                        rd.Add(dev.data_socket);
                        Socket.Select(rd, null, null, 25 * 1000);

                        if (rd.Count == 0)
                            continue;

                        //Console.Write(" {0}", dev.data_socket.Available);

                        EndPoint ep = new IPEndPoint(0, 0);
                        fixed (byte* b = buff)
                        {
                            int recv = dev.data_socket.ReceiveFrom(buff, ref ep);
                            Interlocked.Add(ref dev.BytesReceived, recv);
                            ushort* wdata = (ushort*)b;

                            // timestamp
                            dump_stream.WriteLine(string.Format("{0} {1}", b[0], dev.measures_per_packet));

                            int offset = dev.measures_per_packet * (2);
                            for (int i = 0; i < dev.measures_per_packet; i++)
                            {
                                int z = wdata[1 + i * 2 + 0];
                                int x = wdata[1 + i * 2 + 1];
                                int y = wdata[1 + offset + i * 2 + 0];
                                int w = wdata[1 + offset + i * 2 + 1];

                                dump_stream.WriteLine(string.Format("{0} {1} {2}", x, y, z));

                                lock (dev)
                                {
                                    dev.avg_x_sum -= dev.avg_x_array[dev.avg_x_position];
                                    dev.avg_x_sum += x;
                                    dev.avg_x_array[dev.avg_x_position++] = x;

                                    dev.avg_y_sum -= dev.avg_y_array[dev.avg_y_position];
                                    dev.avg_y_sum += y;
                                    dev.avg_y_array[dev.avg_y_position++] = y;

                                    dev.avg_z_sum -= dev.avg_z_array[dev.avg_z_position];
                                    dev.avg_z_sum += z;
                                    dev.avg_z_array[dev.avg_z_position++] = z;

                                    dev.avg_x_position = (dev.avg_x_position >= dev.avg_x_array.Length) ? 0 : dev.avg_x_position;
                                    dev.avg_y_position = (dev.avg_y_position >= dev.avg_y_array.Length) ? 0 : dev.avg_y_position;
                                    dev.avg_z_position = (dev.avg_z_position >= dev.avg_z_array.Length) ? 0 : dev.avg_z_position;

                                }
                            }

                            dump_stream.Flush();
                        }
                    }
                }
                catch (Exception ex)
                {
                }
        }

        public void Dispose()
        {
            this.Stop();
        }
    }
}
