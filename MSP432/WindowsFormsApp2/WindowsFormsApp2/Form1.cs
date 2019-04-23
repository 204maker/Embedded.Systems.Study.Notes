using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace WindowsFormsApp2
{
    public partial class Form1 : Form
    {
        public int count = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Enabled = false;
        }

        private void BigBadButton_Click(object sender, EventArgs e)
        {
            //textBox_BIG.Text = "TIME, 'X' was sent to UART\n";
            textBox_BIG.BackColor = Color.Black;
            textBox_BIG.ForeColor = Color.Yellow;

            BigBadButton.BackColor = Color.Black;
            RedButton.BackColor = Color.DarkGray;
            GreenButton.BackColor = Color.DarkGray;
            BlueButton.BackColor = Color.DarkGray;

            timer1.Enabled = true;
            if(serialPort1.IsOpen == false)
            {
                //serialPort1.BaudRate = 9600;
                serialPort1.PortName = "COM4";
                serialPort1.Open();
            }
            //serialPort1.WriteLine("Hello --> Serial");
            serialPort1.WriteLine("X");
            textBox_BIG.Text = "'X' was sent to UART"+ Environment.NewLine + serialPort1.ReadExisting() + Environment.NewLine;            
        }

        
        private void timer1_Tick(object sender, EventArgs e)
        {
            /*int minutes = 0;
            int seconds = 0;
            count++;
            seconds = count % 60;
            minutes = count / 60;
            textBox_BIG.Text = Convert.ToString(minutes) + ":" + Convert.ToString(seconds);
            //.Text = serialPort1.ReadExisting() + textBox_BIG.Text;
            textBox_BIG.Text = serialPort1.ReadExisting() + textBox_BIG.Text;
            if (serialPort1.BytesToRead > 0)
            {
                //DateTime;
                textBox_BIG.AppendText(DateTime.Now.ToString("HH:mm:ss.fff"));
                textBox_BIG.AppendText(",  ");
                textBox_BIG.AppendText("42");
                textBox_BIG.AppendText(serialPort1.ReadExisting());
                textBox_BIG.AppendText("\n");
            }*/

        }
        

        private void SaveButton_Click(object sender, EventArgs e)
        {
    //        if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                using (var sfd = new SaveFileDialog())
                {
                    sfd.FilterIndex = 2;
                    sfd.Filter = "CSV Files | *.csv";
                    if (sfd.ShowDialog() == DialogResult.OK)
                    {
                        File.WriteAllText(sfd.FileName, textBox_BIG.Text);
                    }
                }
            }

        }

        private void RedButton_Click(object sender, EventArgs e)
        {
            //textBox_BIG.Text = "TIME, 'R' was sent to UART\n";
            textBox_BIG.BackColor = Color.Red;
            textBox_BIG.ForeColor = Color.Yellow;

            BigBadButton.BackColor = Color.DarkGray;
            RedButton.BackColor = Color.Red;
            GreenButton.BackColor = Color.DarkGray;
            BlueButton.BackColor = Color.DarkGray;

            timer1.Enabled = true;
            if (serialPort1.IsOpen == false)
            {
                //serialPort1.BaudRate = 9600;
                serialPort1.PortName = "COM4";
                serialPort1.Open();
            }
            serialPort1.WriteLine("R");
            textBox_BIG.Text = "'R' was sent to UART" + Environment.NewLine + serialPort1.ReadExisting() + Environment.NewLine;
        }

        private void GreenButton_Click(object sender, EventArgs e)
        {
            //textBox_BIG.Text = "TIME, 'G' was sent to UART\n";
            textBox_BIG.BackColor = Color.Green;
            textBox_BIG.ForeColor = Color.Yellow;

            BigBadButton.BackColor = Color.DarkGray;
            RedButton.BackColor = Color.DarkGray;
            GreenButton.BackColor = Color.Green;
            BlueButton.BackColor = Color.DarkGray;

            timer1.Enabled = true;
            if (serialPort1.IsOpen == false)
            {
                //serialPort1.BaudRate = 9600;
                serialPort1.PortName = "COM4";
                serialPort1.Open();
            }
            serialPort1.WriteLine("G");
            textBox_BIG.Text = "'G' was sent to UART" + Environment.NewLine + serialPort1.ReadExisting() + Environment.NewLine;
        }

        private void BlueButton_Click(object sender, EventArgs e)
        {
            //textBox_BIG.Text = "TIME, 'B' was sent to UART\n";
            textBox_BIG.BackColor = Color.Blue;
            textBox_BIG.ForeColor = Color.Yellow;

            BigBadButton.BackColor = Color.DarkGray;
            RedButton.BackColor = Color.DarkGray;
            GreenButton.BackColor = Color.DarkGray;
            BlueButton.BackColor = Color.Blue;

            timer1.Enabled = true;
            if (serialPort1.IsOpen == false)
            {
                //serialPort1.BaudRate = 9600;
                serialPort1.PortName = "COM4";
                serialPort1.Open();
            }
            serialPort1.WriteLine("B");
            textBox_BIG.Text = "'B' was sent to UART" + Environment.NewLine + serialPort1.ReadExisting() + Environment.NewLine;
        }       
    }
}
