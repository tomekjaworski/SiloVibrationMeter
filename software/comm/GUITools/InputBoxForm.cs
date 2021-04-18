using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Globalization;

namespace GUITools
{
    partial class InputBoxForm : Form
    {
        private Regex reg_validator;

        public InputBoxForm(string caption, string prompt, string regex_validator)
        {
            this.InitializeComponent();
            this.Text = caption;
            this.label1.Text = prompt;

            if (String.IsNullOrEmpty(regex_validator))
                regex_validator = ".*";
            this.reg_validator = new Regex(regex_validator);

        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void InputBoxForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (this.DialogResult == DialogResult.Cancel)
                return;

            if (this.reg_validator.IsMatch(this.textBox1.Text))
                return;

            e.Cancel = true;
            string msg = "The entered value is invalid.\n Should match to the following regular expression: " + this.reg_validator.ToString();
            this.errorProvider1.SetError(this.textBox1, msg);
            Application.DoEvents();
            MessageBox.Show(msg, Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
            this.textBox1.Focus();
            this.textBox1.SelectAll();
        }

        public void SetString(string str)
        {
            this.textBox1.Text = str;
        }

        public string GetString()
        {
            return this.textBox1.Text;
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            this.errorProvider1.Clear();
        }
    }

    public class InputBox
    {
        public static bool InputSring(string caption, string prompt, ref string value)
        {
            using (InputBoxForm wnd = new InputBoxForm(caption, prompt, "^.*$"))
            {
                wnd.SetString(value);
                wnd.ShowDialog();
                if (wnd.DialogResult == DialogResult.OK)
                {
                    value = wnd.GetString();
                    return true;
                }
                return false;
            }
        }

        public static bool InputInteger(string caption, string prompt, ref int value)
        {
            using (InputBoxForm wnd = new InputBoxForm(caption, prompt, "^[+-]?[0-9]+$"))
            {
                wnd.SetString(value.ToString());
                wnd.ShowDialog();
                if (wnd.DialogResult == DialogResult.OK)
                {
                    value = int.Parse(wnd.GetString());
                    return true;
                }
                return false;
            }
        }

        public static bool InputFloat(string caption, string prompt, ref float value)
        {
            using (InputBoxForm wnd = new InputBoxForm(caption, prompt, "^[+-]?[0-9]+([.,][0-9]+)?(e[+-]?[0-9]+)$"))
            {
                wnd.SetString(value.ToString());
                wnd.ShowDialog();
                if (wnd.DialogResult == DialogResult.OK)
                {
                    value = int.Parse(wnd.GetString());
                    return true;
                }
                return false;
            }
        }
    }
}
