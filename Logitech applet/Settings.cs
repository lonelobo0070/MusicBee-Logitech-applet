﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace MusicBeePlugin
{
    public partial class Settings : Form
    {
        private string settingsPath_;
        private Logitech logitech_;

        public Settings(string settingspath, Logitech logitech)
        {
            settingsPath_ = settingspath;
            logitech_ = logitech;
            InitializeComponent();
        }

        public void openSettings()
        {
            bool alwaysOnTop = true;
           
            string[] lines = File.ReadAllLines(settingsPath_ + "LogitechLCDSettings.ini");

            if (lines.Length > 0)
            {
                string alwaysOnTopString = lines[0].Replace("alwaysOnTop: ", "");
                try
                {
                    alwaysOnTop = Convert.ToBoolean(alwaysOnTopString);
                }
                catch (FormatException)
                {
                    
                }
            }

            if (alwaysOnTop)
            {
                this.radioButton1.Checked = true;
            }
            else
            {
                this.radioButton1.Checked = false;
            }


            logitech_.settingsChanged(alwaysOnTop);
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            bool alwaysOnTop = radioButton1.Checked;

            string[] lines = { "alwaysOnTop: " + alwaysOnTop };
            File.WriteAllLines(settingsPath_ + "LogitechLCDSettings.ini", lines);

            logitech_.settingsChanged(alwaysOnTop);
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}