using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;
using System.IO;
using System.Xml;

namespace comm
{
    public partial class MainWindow : Form
    {
        List<Device> devices;
        public MainWindow()
        {
            this.InitializeComponent();
            this.devices = new List<Device>();

            this.DoubleBuffered = true;

            Native32.User32.LISTVIEW_STYLES_EX styles = (Native32.User32.LISTVIEW_STYLES_EX)Native32.User32.SendMessage(this.listView1.Handle, (int)Native32.User32.LISTVIEW_MODES.LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
            styles |= Native32.User32.LISTVIEW_STYLES_EX.LVS_EX_DOUBLEBUFFER | Native32.User32.LISTVIEW_STYLES_EX.LVS_EX_BORDERSELECT;
            Native32.User32.SendMessage(this.listView1.Handle, (int)Native32.User32.LISTVIEW_MODES.LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (int)styles);


        }


        private void button1_Click(object sender, EventArgs e)
        {
            Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            //IPEndPoint ep = new IPEndPoint(IPAddress.Parse("212.191.89.67"), 7000);
            IPEndPoint ep = new IPEndPoint(IPAddress.Parse("192.168.0.100"), 7000);

            byte[] b = {(byte)'C'};
            int r = s.SendTo(b, ep);

            byte[] recv_buffer = new byte[1024];
            EndPoint rep = new IPEndPoint(0, 0);
            r = s.ReceiveFrom(recv_buffer, ref rep);

            MemoryStream ms = new MemoryStream(recv_buffer);
            BinaryReader br = new BinaryReader(ms);

            uint magic = br.ReadUInt32();
            uint command = br.ReadUInt16();

            if (magic != 0x01CAFE02 || command!=(uint)'C')
                return;

            uint size = br.ReadUInt16();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Application.UseWaitCursor = true;
            Application.DoEvents();
            try
            {
                this.listView1.Items.Clear();
                foreach(Device dev in this.devices)
                    dev.Close(); 
                this.devices.Clear();

                Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                s.EnableBroadcast = true;

                IPEndPoint ep = new IPEndPoint(IPAddress.Broadcast, 7000);
                int r = s.SendTo(new byte[] { (byte)'!' }, ep);

                int timeout = 2000;
                UInt32 start_time = Native32.Kernel32.GetTickCount();

                List<Socket> read_fds = new List<Socket>();
                while (true)
                {
                    read_fds.Clear();
                    read_fds.Add(s);

                    Socket.Select(read_fds, null, null, 10 * 1000);
                    if (read_fds.Contains(s))
                    {
                        EndPoint resp = new IPEndPoint(0, 0);
                        byte[] recv_buffer = new byte[1024];
                        int recvd = s.ReceiveFrom(recv_buffer, ref resp);

                        MemoryStream ms = new MemoryStream(recv_buffer);
                        BinaryReader br = new BinaryReader(ms);
                        //ms.Seek(12, SeekOrigin.Begin);

                        Device dev = new Device();

                        uint magic = br.ReadUInt32();
                        uint command = br.ReadUInt16();

                        if (magic != 0x01CAFE02)
                            continue;

                        dev.RandomSeed = br.ReadUInt32();
                        dev.DeviceID = br.ReadUInt16();
                        dev.LocalIPAddress = new IPAddress(br.ReadUInt32());
                        dev.DeviceIPAddress = new IPAddress(br.ReadUInt32());
                        dev.IPGateway = new IPAddress(br.ReadUInt32());
                        dev.IPNetmask = new IPAddress(br.ReadUInt32());
                        dev.HardwareAddress = br.ReadBytes(6);
                        dev.timestamp = ReadASCIIZ(ms);
                        dev.version = ReadASCIIZ(ms);

                        this.devices.Add(dev);
                    }

                    if (Native32.Kernel32.GetTickCount() - start_time > timeout)
                        break;
                }

                foreach (Device dev in this.devices)
                    dev.ReadConfiguration();

                float zero = 0.0f;
                foreach (Device dev in this.devices)
                {
                    ListViewItem i = new ListViewItem();

                    i.Text = dev.DeviceID.ToString();
                    i.SubItems.Add(dev.DeviceIPAddress.ToString());
                    i.SubItems.Add(BitConverter.ToString(dev.HardwareAddress));
                    //i.SubItems.Add(dev.Timestamp);
                    //i.SubItems.Add(dev.Version);
                    i.SubItems.Add("0");
                    i.SubItems.Add("0");

                    i.SubItems.Add(zero.ToString("N3")); // xavg
                    i.SubItems.Add(zero.ToString("N3")); // yavg
                    i.SubItems.Add(zero.ToString("N3")); // zavg
                    i.Tag = dev;
                    dev.Tag = i;

                    this.listView1.Items.Add(i);

                }

                this.propertyGrid1.SelectedObject = null;
                            
            }
            catch (Exception ex)
            {
                Application.UseWaitCursor = false;

                MessageBox.Show("Wyjątek: " + ex.Message, Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }


            try
            {
                // wczytanie nazw
                if (File.Exists("config.xml"))
                {
                    XmlDocument doc = new XmlDocument();
                    doc.Load("config.xml");

                    foreach (Device dev in this.devices)
                    {
                        String hw = "MAC-" + BitConverter.ToString(dev.HardwareAddress);
                        XmlElement xml_dev = doc.SelectSingleNode("/root/" + hw) as XmlElement;

                        dev.FileName = xml_dev.SelectSingleNode("FileName").InnerText;
                    }
                }
            }
            catch (Exception ex)
            {
                // jesli cos sie stanie zlego, to po prostu wywal plik konfiguracyjny
                File.Delete("config.xml");
            }

            Application.UseWaitCursor = false;
        }


        private static String ReadASCIIZ(Stream ms)
        {
            String s = "";
            while (true)
            {
                int c = ms.ReadByte();
                if (c <= 0)
                    break;

                s += (char)c;
            }

            return s;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //MessageBox.Show("Było klikać???");
            //using (AboutBox1 w = new AboutBox1())
               // w.ShowDialog();
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.listView1.SelectedItems.Count == 0)
            {
                this.propertyGrid1.SelectedObject = null;
                return;
            }

            List<Device> devs = new List<Device>();

            foreach (ListViewItem i in this.listView1.SelectedItems)
                devs.Add(i.Tag as Device);

            this.propertyGrid1.SelectedObjects = devs.ToArray();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in this.listView1.SelectedItems)
            {
                Device dev = item.Tag as Device;
                dev.Start();
            }

            this.timer1.Start();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in this.listView1.SelectedItems)
            {
                Device dev = item.Tag as Device;
                dev.Stop();
            }

            this.timer1.Stop();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            this.listView1.BeginUpdate();

            foreach (Device dev in this.devices)
            {
                ListViewItem liv = dev.Tag as ListViewItem;
                liv.SubItems[3].Text = dev.BytesSent.ToString();
                liv.SubItems[4].Text = dev.BytesReceived.ToString();

                liv.SubItems[5].Text = dev.XAvgV.ToString("N3");
                liv.SubItems[6].Text = dev.YAvgV.ToString("N3");
                liv.SubItems[7].Text = dev.ZAvgV.ToString("N3");

                //liv.SubItems[6].Text = dev.FileName;
            }

            this.listView1.EndUpdate();
        }

        private void btnStartAll_Click(object sender, EventArgs e)
        {
            foreach (Device dev in this.devices)
                dev.Start();

            this.timer1.Start();
        }

        private void btnStopAll_Click(object sender, EventArgs e)
        {
            foreach (Device dev in this.devices)
                dev.Stop();

            this.timer1.Stop();

        }

        private void MainWindow_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.btnStopAll_Click(null, null);

            // zapisz do xmla
            XmlDocument doc = new XmlDocument();
            if (!File.Exists("config.xml"))
            {
                doc.AppendChild(doc.CreateXmlDeclaration("1.0", "utf-8", "yes"));
                doc.AppendChild(doc.CreateElement("root"));
            }
            else
                doc.Load("config.xml");

            foreach (Device dev in this.devices)
            {
                String hw = "MAC-" + BitConverter.ToString(dev.HardwareAddress);
                XmlElement xml_dev = doc.DocumentElement.SelectSingleNode(hw) as XmlElement;

                XmlElement xml_name, xml_lastip;
                if (xml_dev == null)
                {
                    xml_dev = doc.DocumentElement.AppendChild(doc.CreateElement(hw)) as XmlElement;

                    xml_name = xml_dev.AppendChild(doc.CreateElement("FileName")) as XmlElement;
                    xml_lastip = xml_dev.AppendChild(doc.CreateElement("LastIP")) as XmlElement;
                }
                else
                {
                    xml_name = xml_dev.SelectSingleNode("FileName") as XmlElement;
                    xml_lastip = xml_dev.SelectSingleNode("LastIP") as XmlElement;
                }

                xml_name.InnerText = dev.FileName;
                xml_lastip.InnerText = dev.DeviceIPAddress.ToString();
            }

            doc.Save("config.xml");
        }

        private void cmdZmienNazweAkcelerometru_Click(object sender, EventArgs e)
        {
            if (this.listView1.SelectedItems.Count != 1)
            {
                MessageBox.Show("Zaznacz tylko jeden akcelerometr", Application.ProductName);
                return;
            }
            
            Device dev = this.listView1.SelectedItems[0].Tag as Device;
            string nazwa = dev.FileName;
            if (!GUITools.InputBox.InputSring("Nazwa akcelerometru", "Podaj nową lub zmień istniejącą nazwę pliku do zapisu danych z akcelerometru:", ref nazwa))
                return;

            dev.FileName = nazwa;
        }
    }




}
