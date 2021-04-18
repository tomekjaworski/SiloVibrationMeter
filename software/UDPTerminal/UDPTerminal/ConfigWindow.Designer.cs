namespace UDPTerminal
{
    partial class ConfigWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.rbXTAL = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.lblTad = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cbMultiplier = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.rbADRC = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.lblTconv = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.lblTsampl = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.cbSamples = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // rbXTAL
            // 
            this.rbXTAL.AutoSize = true;
            this.rbXTAL.Location = new System.Drawing.Point(14, 43);
            this.rbXTAL.Name = "rbXTAL";
            this.rbXTAL.Size = new System.Drawing.Size(108, 17);
            this.rbXTAL.TabIndex = 1;
            this.rbXTAL.Text = "XTAL (Tcy=25ns)";
            this.rbXTAL.UseVisualStyleBackColor = true;
            this.rbXTAL.CheckedChanged += new System.EventHandler(this.rbADRC_CheckedChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.lblTad);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.cbMultiplier);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.rbADRC);
            this.groupBox1.Controls.Add(this.rbXTAL);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(162, 125);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Czas wzorcowy Tad";
            // 
            // lblTad
            // 
            this.lblTad.AutoSize = true;
            this.lblTad.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.lblTad.Location = new System.Drawing.Point(39, 91);
            this.lblTad.Name = "lblTad";
            this.lblTad.Size = new System.Drawing.Size(41, 13);
            this.lblTad.TabIndex = 5;
            this.lblTad.Text = "label3";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(11, 91);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(32, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Tad=";
            // 
            // cbMultiplier
            // 
            this.cbMultiplier.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbMultiplier.FormattingEnabled = true;
            this.cbMultiplier.Location = new System.Drawing.Point(82, 62);
            this.cbMultiplier.Name = "cbMultiplier";
            this.cbMultiplier.Size = new System.Drawing.Size(61, 21);
            this.cbMultiplier.TabIndex = 3;
            this.cbMultiplier.SelectedIndexChanged += new System.EventHandler(this.cbMultiplier_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(29, 65);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(50, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Mnożnik:";
            // 
            // rbADRC
            // 
            this.rbADRC.AutoSize = true;
            this.rbADRC.Checked = true;
            this.rbADRC.Location = new System.Drawing.Point(14, 20);
            this.rbADRC.Name = "rbADRC";
            this.rbADRC.Size = new System.Drawing.Size(121, 17);
            this.rbADRC.TabIndex = 0;
            this.rbADRC.TabStop = true;
            this.rbADRC.Text = "ADRC  (Tad=250ns)";
            this.rbADRC.UseVisualStyleBackColor = true;
            this.rbADRC.CheckedChanged += new System.EventHandler(this.rbADRC_CheckedChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.lblTconv);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.lblTsampl);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.cbSamples);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Location = new System.Drawing.Point(180, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(162, 125);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Czas próbkowania i konwersji";
            // 
            // lblTconv
            // 
            this.lblTconv.AutoSize = true;
            this.lblTconv.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.lblTconv.Location = new System.Drawing.Point(56, 65);
            this.lblTconv.Name = "lblTconv";
            this.lblTconv.Size = new System.Drawing.Size(41, 13);
            this.lblTconv.TabIndex = 9;
            this.lblTconv.Text = "label3";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(15, 65);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(44, 13);
            this.label6.TabIndex = 8;
            this.label6.Text = "Tconv=";
            // 
            // lblTsampl
            // 
            this.lblTsampl.AutoSize = true;
            this.lblTsampl.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.lblTsampl.Location = new System.Drawing.Point(56, 47);
            this.lblTsampl.Name = "lblTsampl";
            this.lblTsampl.Size = new System.Drawing.Size(41, 13);
            this.lblTsampl.TabIndex = 7;
            this.lblTsampl.Text = "label3";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(15, 47);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(45, 13);
            this.label5.TabIndex = 6;
            this.label5.Text = "Tsamp=";
            // 
            // cbSamples
            // 
            this.cbSamples.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbSamples.FormattingEnabled = true;
            this.cbSamples.Location = new System.Drawing.Point(87, 19);
            this.cbSamples.Name = "cbSamples";
            this.cbSamples.Size = new System.Drawing.Size(61, 21);
            this.cbSamples.TabIndex = 5;
            this.cbSamples.SelectedIndexChanged += new System.EventHandler(this.cbMultiplier_SelectedIndexChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 24);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(66, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Uśrednianie:";
            // 
            // ConfigWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(448, 331);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "ConfigWindow";
            this.Text = "ConfigWindow";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RadioButton rbXTAL;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton rbADRC;
        private System.Windows.Forms.ComboBox cbMultiplier;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblTad;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label lblTsampl;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox cbSamples;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label lblTconv;
        private System.Windows.Forms.Label label6;
    }
}