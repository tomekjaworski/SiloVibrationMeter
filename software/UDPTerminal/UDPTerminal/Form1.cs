using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.Serialization.Formatters.Soap;
using System.IO;
using System.Net.Sockets;
using System.Net;

namespace UDPTerminal
{
    public partial class Form1 : Form
    {
        private UdpClient cli;

        private int cnt_sent_bytes, cnt_sent_packets, cnt_recv_bytes, cnt_recv_packets;

        public Form1()
        {
            this.InitializeComponent();

            if (File.Exists("udpterm.xml"))
                using (FileStream fs = new FileStream("udpterm.xml", FileMode.Open))
                {
                    SoapFormatter s = new SoapFormatter();
                    String[] hosts = (String[])s.Deserialize(fs);
                    String[] ports = (String[])s.Deserialize(fs);
                    String[] cmds = (String[])s.Deserialize(fs);

                    this.cbHost.Items.AddRange(hosts);
                    this.cbPort.Items.AddRange(ports);
                    this.cbCommand.Items.AddRange(cmds);

                    this.cbHost.Text = (String)s.Deserialize(fs);
                    this.cbPort.Text = (String)s.Deserialize(fs);
                    this.cbCommand.Text = (String)s.Deserialize(fs);

                }


            this.cli = null;

            this.UpdateGUI();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            using (FileStream fs = new FileStream("udpterm.xml", FileMode.Create))
            {
                SoapFormatter s = new SoapFormatter();

                String[] hosts = new String[this.cbHost.Items.Count];
                String[] ports = new String[this.cbPort.Items.Count];
                String[] cmds = new String[this.cbCommand.Items.Count];

                this.cbHost.Items.CopyTo(hosts, 0);
                this.cbPort.Items.CopyTo(ports, 0);
                this.cbCommand.Items.CopyTo(cmds, 0);


                s.Serialize(fs, hosts);
                s.Serialize(fs, ports);
                s.Serialize(fs, cmds);
                s.Serialize(fs, this.cbHost.Text);
                s.Serialize(fs, this.cbPort.Text);
                s.Serialize(fs, this.cbCommand.Text);
            }

            if (this.cli != null)
            {
                UdpClient temp = this.cli;
                this.cli = null;
                temp.Close();
            }
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            string cmd = this.cbCommand.Text;
            this.cbCommand.Text = "";

            this.Send(cmd);
            this.cbCommand.Focus();
        }

        private void Send(string cmd)
        {
            byte[] data = Encoding.ASCII.GetBytes(cmd);

            this.cli.Send(data, data.Length);
            this.cnt_sent_packets++;
            this.cnt_sent_bytes += data.Length;

            this.UpdateGUI();
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            try
            {
                if (this.cli == null)
                {
                    this.cli = new UdpClient(this.cbHost.Text, int.Parse(this.cbPort.Text));
                    if (this.cbHost.Items.IndexOf(this.cbHost.Text) == -1)
                        this.cbHost.Items.Insert(0, this.cbHost.Text);
                    if (this.cbPort.Items.IndexOf(this.cbPort.Text) == -1)
                        this.cbPort.Items.Insert(0, this.cbPort.Text);

                    this.textBox1.Clear();
                    this.cli.BeginReceive(new AsyncCallback(ReceiveCallback), this);

                    IPEndPoint ep = this.cli.Client.LocalEndPoint as IPEndPoint;
                    this.Send("REMOTE " + ep.ToString());
                }
                else
                {
                    UdpClient temp = this.cli;
                    this.cli = null;
                    temp.Close();
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show("Exception occured:\n" + ex.Message, Application.ProductName);
            }

            this.UpdateGUI();
        }

        public delegate void StringDelegate(string s);

        public static void ReceiveCallback(IAsyncResult ar)
        {
            Form1 form = ar.AsyncState as Form1;
            if (form.cli != null)
            {
                IPEndPoint ep = form.cli.Client.RemoteEndPoint as IPEndPoint;

                Byte[] receiveBytes = form.cli.EndReceive(ar, ref ep);
                string receiveString = Encoding.ASCII.GetString(receiveBytes);
                form.Invoke(new StringDelegate(form.Log), receiveString);
            } else
                form.Invoke(new StringDelegate(form.Log), "EOT.");
            
        }

        private void Log(string text)
        {
            this.textBox1.AppendText(text + "\n");
            this.textBox1.ScrollToCaret();
        }

        private void UpdateGUI()
        {
            this.btnConnect.Text = (this.cli == null) ? "Connect" : "Disconnect";
            this.btnSend.Enabled = (this.cli != null);
            this.cbCommand.Enabled = (this.cli != null);

            this.cbHost.Enabled = (this.cli == null);
            this.cbPort.Enabled = (this.cli == null);

            this.label7.Text = string.Format("{0}/{1}", this.cnt_sent_bytes, this.cnt_sent_packets);
            this.label8.Text = string.Format("{0}/{1}", this.cnt_recv_bytes, this.cnt_recv_packets);
        }

        private void cbCommand_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyValue == '\r')
                this.btnSend_Click(null, null);
        }
    }
}
