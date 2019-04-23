namespace WindowsFormsApp2
{
    partial class Form1
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
            this.components = new System.ComponentModel.Container();
            this.textBox_BIG = new System.Windows.Forms.TextBox();
            this.BigBadButton = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.SaveButton = new System.Windows.Forms.Button();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.RedButton = new System.Windows.Forms.Button();
            this.GreenButton = new System.Windows.Forms.Button();
            this.BlueButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBox_BIG
            // 
            this.textBox_BIG.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(128)))));
            this.textBox_BIG.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox_BIG.Location = new System.Drawing.Point(426, 36);
            this.textBox_BIG.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_BIG.Multiline = true;
            this.textBox_BIG.Name = "textBox_BIG";
            this.textBox_BIG.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_BIG.Size = new System.Drawing.Size(684, 349);
            this.textBox_BIG.TabIndex = 0;
            this.textBox_BIG.Text = "This is my Text Window";
            // 
            // BigBadButton
            // 
            this.BigBadButton.Font = new System.Drawing.Font("Arial Rounded MT Bold", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BigBadButton.ForeColor = System.Drawing.Color.Red;
            this.BigBadButton.Location = new System.Drawing.Point(46, 36);
            this.BigBadButton.Margin = new System.Windows.Forms.Padding(4);
            this.BigBadButton.Name = "BigBadButton";
            this.BigBadButton.Size = new System.Drawing.Size(343, 144);
            this.BigBadButton.TabIndex = 1;
            this.BigBadButton.Text = "PRESS ME";
            this.BigBadButton.UseVisualStyleBackColor = true;
            this.BigBadButton.Click += new System.EventHandler(this.BigBadButton_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 500;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // serialPort1
            // 
            this.serialPort1.PortName = "COM6";
            // 
            // SaveButton
            // 
            this.SaveButton.Location = new System.Drawing.Point(46, 337);
            this.SaveButton.Margin = new System.Windows.Forms.Padding(4);
            this.SaveButton.Name = "SaveButton";
            this.SaveButton.Size = new System.Drawing.Size(343, 48);
            this.SaveButton.TabIndex = 2;
            this.SaveButton.Text = "Save Data";
            this.SaveButton.UseVisualStyleBackColor = true;
            this.SaveButton.Click += new System.EventHandler(this.SaveButton_Click);
            // 
            // RedButton
            // 
            this.RedButton.BackColor = System.Drawing.SystemColors.ControlDark;
            this.RedButton.Font = new System.Drawing.Font("Arial", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RedButton.Location = new System.Drawing.Point(46, 201);
            this.RedButton.Margin = new System.Windows.Forms.Padding(4);
            this.RedButton.Name = "RedButton";
            this.RedButton.Size = new System.Drawing.Size(106, 112);
            this.RedButton.TabIndex = 1;
            this.RedButton.Text = "Red";
            this.RedButton.UseVisualStyleBackColor = false;
            this.RedButton.Click += new System.EventHandler(this.RedButton_Click);
            // 
            // GreenButton
            // 
            this.GreenButton.BackColor = System.Drawing.SystemColors.ControlDark;
            this.GreenButton.Font = new System.Drawing.Font("Arial", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.GreenButton.Location = new System.Drawing.Point(165, 201);
            this.GreenButton.Margin = new System.Windows.Forms.Padding(4);
            this.GreenButton.Name = "GreenButton";
            this.GreenButton.Size = new System.Drawing.Size(106, 112);
            this.GreenButton.TabIndex = 1;
            this.GreenButton.Text = "Green";
            this.GreenButton.UseVisualStyleBackColor = false;
            this.GreenButton.Click += new System.EventHandler(this.GreenButton_Click);
            // 
            // BlueButton
            // 
            this.BlueButton.BackColor = System.Drawing.SystemColors.ControlDark;
            this.BlueButton.Font = new System.Drawing.Font("Arial", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BlueButton.Location = new System.Drawing.Point(288, 201);
            this.BlueButton.Margin = new System.Windows.Forms.Padding(4);
            this.BlueButton.Name = "BlueButton";
            this.BlueButton.Size = new System.Drawing.Size(101, 112);
            this.BlueButton.TabIndex = 1;
            this.BlueButton.Text = "Blue";
            this.BlueButton.UseVisualStyleBackColor = false;
            this.BlueButton.Click += new System.EventHandler(this.BlueButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1144, 428);
            this.Controls.Add(this.SaveButton);
            this.Controls.Add(this.BlueButton);
            this.Controls.Add(this.GreenButton);
            this.Controls.Add(this.RedButton);
            this.Controls.Add(this.BigBadButton);
            this.Controls.Add(this.textBox_BIG);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox_BIG;
        private System.Windows.Forms.Button BigBadButton;
        private System.Windows.Forms.Timer timer1;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button SaveButton;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.Button RedButton;
        private System.Windows.Forms.Button GreenButton;
        private System.Windows.Forms.Button BlueButton;
    }
}

