using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace UDPTerminal
{
    public partial class ConfigWindow : Form
    {
        private bool gui_updatable;
        public ConfigWindow()
        {
            this.InitializeComponent();
            this.gui_updatable = false;

            for (int i = 64; i >= 1; i--)
                this.cbMultiplier.Items.Add("x"+i.ToString());
            for (int i = 1; i <= 31; i++)
                this.cbSamples.Items.Add("x" + i.ToString());
            
            this.cbMultiplier.SelectedIndex = 0;
            this.cbSamples.SelectedIndex = 0;

            this.gui_updatable = true;
            this.UpdateGUI();
        }

        private void UpdateGUI()
        {
            if (!this.gui_updatable)
                return;

            double Tad = 0, mp = 0;
            double Tad_min = 117.6e-9; // z dokumentaci dspic33fj128mc706a

            this.cbMultiplier.Enabled = this.rbXTAL.Checked;

            if (this.rbADRC.Checked)
            {
                Tad = 250e-9;
                mp = 1;
            }

            if (this.rbXTAL.Checked)
            {
                Tad = 25e-9;
                mp = double.Parse(this.cbMultiplier.Text.Substring(1));
            }

            Tad = Tad * mp;
            this.lblTad.Text = StringUtil.ToString(Tad, "s");
            if (Tad < Tad_min)
                this.lblTad.ForeColor = Color.Red;
            else
                this.lblTad.ForeColor = SystemColors.ControlText;

            //////////////
            mp = double.Parse(this.cbSamples.Text.Substring(1));
            double Tsamp = mp * Tad;
            double Tconv = 14 * Tad;

            if (mp < 3)
                this.lblTsampl.ForeColor = Color.Red;
            else
                this.lblTsampl.ForeColor = SystemColors.ControlText;

            this.lblTsampl.Text = StringUtil.ToString(Tsamp, "s");
            this.lblTconv.Text = StringUtil.ToString(Tconv, "s");
        }

        private void rbADRC_CheckedChanged(object sender, EventArgs e)
        {
            this.UpdateGUI();
        }

        private void cbMultiplier_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.UpdateGUI();
        }

    }
}
