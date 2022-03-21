using System;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Windows.Forms;
using ZedGraph;
using System.Windows.Forms;
using System.IO.Ports;
using ZedGraph;
using System.Text.RegularExpressions;




namespace ECE_323_LAB_10
{

    public partial class Form1 : Form
    {
        //Global Variables
        PointPairList list = new PointPairList();
        double temp;
        double temp1;
        int flag = 0;
        int flag_D = 0;
        double time = 0;
        int data_length = 0;
        string s = "";
        string soup;
        int counter = 0;
        int counts = 0;
        string sfrequency_string_global = "";
        string srate_string_global = "";
        string AverageNum_string_global = "";

        string[] fullstring = new string[58];
        int count_fullstring = 0;    //String length
        string string_extract = "";
        int y = 0;
        int f = 0;
        float collectperiod;
        int count_file = 1;
        int flag_D1=0;

        char[] note = new char[58];
        int count_value=0;
        int count_valuey = 0;


        //string[] temparray;   //Array for the temp
        //string[] timearray;   //Array for the time 



        //For Excel
        Excelclass choosing_data = new Excelclass();
        int time_length;
        int j;
        string[,] temparray = new string[1000, 1000];    //Total length o temp array is 1000
        string[] timearray = new string[1000];    //Total length of time array is 1000

        string[,] temparray2 = new string[1000, 1000];    //Total length o temp array is 1000
        string[] timearray2 = new string[1000];    //Total length of time array is 1000
        string[] currenttimearray = new string[1000];    //Total length of time array is 1000 represents the current time

        string m = "";



        int i;
        int time_count = 0;
        int i_count = 0;
        int total_length;
        char print_letter_flag;


        public delegate void AddDataDelegate(double temp);
        public AddDataDelegate myDelegate;



        private Bluetooth_Settings _setting = new Bluetooth_Settings();
        public Form1()
        {
            InitializeComponent();
            //_setting._serial.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
            // setsize();

        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
            int text_length = 0;
            text_length = richTextBox1.TextLength;
            char send_ch = richTextBox1.Text[text_length - 1]; // extracting the last character
            char[] ch = new char[1];
            ch[0] = send_ch;
            if (send_ch == '\n')
            {
                _setting._serial.Write("\r"); // sending carraige return 
            }
            else
            {

                _setting._serial.Write(ch, 0, 1); // sending char to microcontroller
            }
        }

        private void blueToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void toolStripProgressBar1_Click(object sender, EventArgs e)
        {

        }

        private void COMPortToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _setting.Show();
             _setting._serial.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
        }

        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            counter++;
            
            if (sp.BytesToRead > 0)
            {
                byte[] buffer = new byte[sp.BytesToRead];
                int maker =0+ sp.BytesToRead;
                if(maker>buffer.Length)
                { 

                }
                else
                {
                    int count = sp.Read(buffer, 0, buffer.Length);
                    //int count = sp.Read(buffer, 0, sp.BytesToRead);
                    Extract_each_Character(buffer, count);   //Display 5 tempertures

                }
                

            }


        }

        private void Extract_each_Character(byte[] buffer, int count)
        {
            byte data;
            for (int i = 0; i < count; i++)
            {
                data = buffer[i];
                this.Invoke(new EventHandler(update_richtextbox1), new object[] { data });

            }

        }

        private void update_richtextbox1(object sender, EventArgs e)
        {



            byte data = (byte)sender;
            char sin = Convert.ToChar(data);
            string sings = Convert.ToString(sin);
            //sp.DataReceived -= new SerialDataReceivedEventHandler(SerialPort_DataReceived);
            // sin.DataReceived -= new EventHandler(SerialPort_DataReceived);
            richTextBox1.TextChanged -= new EventHandler(richTextBox1_TextChanged); // removing text changed event 
            richTextBox1.AppendText(sin.ToString());
            getvalue(sin);
            //if (sings.Contains("&"))
            //{
            //    Take_string_To_graph(sin.ToString());   //Graphs and display value on chart
            //}

            if (flag_D == 23)
            {
                Take_string_To_graph(sin.ToString());   //Graphs and display value on chart
            }


            // SerialPort.DataReceived(s.ToString());
            // return soup;
            //Take_string_To_graph();
            richTextBox1.TextChanged += new EventHandler(richTextBox1_TextChanged); // adding text changed event
                                                                                    //sp.DataReceived += new SerialDataReceivedEventHandler(SerialPort_DataReceived);

        }


        public void getvalue(char s)
        {
            if (count_value < 40)
            {
                note[count_value] = s;
                count_value++;
                if (s == '\n')   //new line
                {
                    count_value = 0;
                    string str = new string(note);
                    if (str.Contains("Sampling freq:"))
                    {
                        str = Regex.Replace(str, "[^0-9 _]", "");
                        sfrequency_string_global = str;         //move the global varible so that it can be stored in excel
                        str = "";  //clear the string
                        Array.Clear(note, 0, note.Length);  //clear the string

                    }
                    else if (str.Contains("Display period:"))
                    {
                          str = Regex.Replace(str, "[^0-9 _]", "");
                        float str2 = float.Parse(str);
                        // int str2   = int.Parse(str1);
                        // float str3 = (float)(str2 * 0.1);
                        //collectperiod = str3;
                        //string str4 = str3.ToString();
                         collectperiod = str2;
                        srate_string_global = str;         //move the global varible so that it can be stored in excel
                        str = "";  //clear the string
                        //str3 = 0.0; // clear the float
                        Array.Clear(note, 0, note.Length);  //clear the string

                    }
                    else if (str.Contains("Average Number:"))
                    {
                        str = Regex.Replace(str, "[^0-9 _]", "");
                        AverageNum_string_global = str;         //move the global varible so that it can be stored in excel
                        str = "";  //clear the string
                        Array.Clear(note, 0, note.Length);  //clear the string

                    }

                    
                    else
                    {
                        count_value = 0;
                        Array.Clear(note, 0, note.Length);  //clear the string
                        str = "";
                    }
                }

            }

         
        }

        private void Form1_Load(object sender, EventArgs e)
        {
          
            // WriteData();
            //CreateGraph(zedGraphControl1);
        }



        private void CreateGraph(ZedGraphControl zedGraphControl1)
        {

            // Declare a new GraphPane Object 
            GraphPane myPane = zedGraphControl1.GraphPane;
            if (flag == 1)
            {
                // Set the titles and axis labels
                myPane.Title.Text = " Temperature Graph";
                myPane.XAxis.Title.Text = "Time, Seconds";
                myPane.YAxis.Title.Text = "Temprature in F";
            }

            else if (flag == 2)
            {

                // Set the titles and axis labels
                myPane.Title.Text = " Temp. Graph";
                myPane.XAxis.Title.Text = "Time, Seconds";
                myPane.Y2Axis.Title.Text = "Temprature in C";

                // Make up some data pointw based on the Sine function 
                // Note: All data being plotted by zedgraph have to be “Double” format.
                // Data should be saved as a PointPairList before plotting.
                // So “List.Add()” method should be called after you define or
                // change the data that are plotted.
                // Generate a red curve with diamond symbols, and "Alpha" in the legend



            }
            LineItem myCurve = myPane.AddCurve("", list, Color.Blue);
            myCurve.Symbol.Fill = new Fill(Color.White);

            myPane.XAxis.MajorGrid.IsVisible = true;
            // Make the Y axis scale red
            myPane.YAxis.Scale.FontSpec.FontColor = Color.Red;
            myPane.YAxis.Title.FontSpec.FontColor = Color.Red;
            // Don't display the Y zero line
            myPane.YAxis.MajorGrid.IsZeroLine = false;
            // Align the Y axis labels so they are flush to the axis
            myPane.YAxis.Scale.Align = AlignP.Inside;
            // Manually set the axis range
            //myPane.YAxis.Scale.Min = 0;
            //myPane.YAxis.Scale.Max = 500;
            // Fill the axis background with a gradient
            myPane.Chart.Fill = new Fill(Color.White, Color.LightGray, 45.0f);
            // Calculate the Axis Scale Ranges
            //Note: ZedGraphControl.AxisChange() command keep checking and
            // adjusting the display axis setting according to the List changes.
            zedGraphControl1.AxisChange();

            if (flag_D1 == 40)
            {
                //Clear the Zedgraph
                myCurve.Clear();
                //zedGraphControl1.GraphPane.CurveList.Clear();
               // zedGraphControl1.Refresh();
                zedGraphControl1.Invalidate();
                zedGraphControl1.AxisChange();
                time = 0;
                time_length = 0;
                flag_D1 = 0;
                i=0;
                j=0;
            }
           


        }

        public void Take_string_To_graph(string s)
        {

            //SerialPort info = (SerialPort)sender;

            // s = info.ReadLine();
            if (s.Length != 0)
            {
                if (s.Contains("F")) // Fahrenhiet 
                {


                    flag = 1;
                    // s = Regex.Replace(s, "[^0-9 _]", "");
                    //  AddTextToLabel(s);  //graph the number
                    Collectfullstring(s);


                }
                else if (s.Contains("C")) // Celcuis
                {

                    flag = 2;
                    // s = Regex.Replace(s, "[^0-9 _]", "");
                    // AddTextToLabel(s);
                    Collectfullstring(s);


                }

                else { Collectfullstring(s); }

                ////else if (s.Any(char.IsDigit)) //toggle
                ////{

                ////    flag = 3;
                ////    info.DataReceived += new SerialDataReceivedEventHandler(SerialPort_DataReceived);
                ////    AddTextToLabel(s);
                ////    Collectfullstring(s);

                ////}


            }
        }

       


        public void Collectfullstring(string str)
        {
            //Two varibales to compare y and f

            count_fullstring++;

            if (y == f)
            {
                if (count_fullstring == 57)
                {
                    Array.Clear(fullstring, 0,fullstring.Length);
                    count_fullstring = 0;
                    y = 0;
                    f = 0;


                }
                else { fullstring[y] = str; }
                
            
            }
            y++;
            f++;

            if (str.Contains("."))    //This number depends on the length of your string
            {
                count_valuey++;
               if (count_valuey == 2)
               {

                    //Change the string array in one whole string 
                    string totalstring = string.Join("", fullstring);

                    if (totalstring.Contains("Temp from Rate in Fahrenhiet:"))
                    {
                        //Extract just the numbers
                        string_extract = Regex.Replace(totalstring, "[^0-9. _]", "");   // Remove all letter 
                        string_extract = string_extract.Remove(string_extract.Length - 1, 1);   // Remove the last dot 
                        AddTextToLabel(string_extract);
                        Array.Clear(fullstring, 0, 58);
                        count_fullstring = 0;
                        y = 0;
                        f = 0;
                        count_valuey = 0;

                    }

                    else if (totalstring.Contains("Temp from Frequency in Fahrenhiet:"))
                    {
                        //Extract just the numbers
                        string_extract = Regex.Replace(totalstring, "[^0-9. _]", "");
                        string_extract = string_extract.Remove(string_extract.Length - 1, 1);
                        AddTextToLabel(string_extract);
                        Array.Clear(fullstring, 0, 58);
                        count_fullstring = 0;
                        y = 0;
                        f = 0;
                        count_valuey = 0;
                    }
                    else { Array.Clear(fullstring, 0, 58); }
               }

            }



            else
            {

            }


        }





        public void AddTextToLabel(string s)
        {
            if (s.Contains("*"))
            {

            }
            else
            {

                if (flag == 1) // Fahrenhiet
                {

                    temp = Double.Parse(s);

                    // Immediately after collect the current time
                    DateTime now = DateTime.Now;
                    string crazy_now = now.ToString("yyy:hh:mm:ss");

                    counts++;

                    data_length++;
                    time++;                                   //Time in Seconds

                    i++;
                    j++;

                    time_length = Convert.ToInt32(time);     //Convert the current time number to integer

                    double y = temp;

                    string Stemp = "";
                    string Stime = "";

                    //Creates  the time as a string
                    string temp_string = temp.ToString(Stemp);
                    string time_string = time.ToString(Stime);
                    //int i;
                    //int j;
                    //temparray = new string[1000];    //Total length oftemp array is 1000
                    //timearray = new string[1000];    //Total length of time array is 1000
                    if (i == time)
                    {
                        if (j == time)
                        {
                            temparray[0, i] = time_string;
                            temparray[1, j] = temp_string;
                            currenttimearray[i] = crazy_now;
                            
                        }


                        // timearray[i] = time_string;
                    }
                    //if (i_count==time_count)
                    //{

                    //    temparray[i] = temp_string;
                    //    timearray[i] = time_string;
                    //}

                    i_count++;
                    time_count++;
                    float time_s = time_length * collectperiod;

                    list.Add(time_s, y);
                    zedGraphControl1.Invalidate();
                    //zedGraphControl1.Refresh();    // Refresh  Graph
                    zedGraphControl1.AxisChange(); //Auto scale Axis
                    CreateGraph(zedGraphControl1);

                }



                else if (flag == 2) // Celcius
                {

                    //list Adding Function 1;
                    temp = Double.Parse(s);

                    time++;
                    double y = temp;

                    list.Add(time, y);
                    zedGraphControl1.Invalidate();
                    //zedGraphControl1.Refresh();    // Refresh  Graph
                    zedGraphControl1.AxisChange(); //Auto scale Axis
                    CreateGraph(zedGraphControl1);
                    counts = 0;

                }


            }


        }

        private void setsize()
        {
            zedGraphControl1.Location = new System.Drawing.Point(10, 10);

            zedGraphControl1.Size = new Size(this.ClientRectangle.Width - 20, this.ClientRectangle.Height - 20);
        }

        private void zedGraphControl1_Load(object sender, EventArgs e)

        {
            //CreateChart(zedGraphControl1);
            //int b = 0;
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
        public void Clear_fields()
        {
            //Clearing Textbox

            textBoxAverageNum.Text = String.Empty;
            textBoxSRate.Text = String.Empty;
            textBoxSfrequency.Text = String.Empty;
            MessageBox.Show("Text Cleared");
        }
        private void button6_Click(object sender, EventArgs e)
        {
            Clear_fields();
        }

        private void textBoxSfrequency_TextChanged(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Sending the Char A so that micro controller knows that You are entering sampling frequency 
            string A = "A";
            Char[] charA = A.ToCharArray();
            _setting._serial.Write(charA, 0, 1); // sending char to microcontroller
            _setting._serial.Write("\r"); // sending carraige return 

           

            print_letter_flag = 'A';

        }

        private void button2_Click(object sender, EventArgs e)
        {
            //Sending the Char B so that micro controller knows that You are entering sampling Rate

            print_letter_flag = 'B';

            string B = "B";
            Char[] charB = B.ToCharArray();
            _setting._serial.Write(charB, 0, 1); // sending char to microcontroller
            _setting._serial.Write("\r"); // sending carraige return 

           
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //Sending the Char C so that micro controller knows that You are entering Average Num

            print_letter_flag = 'C';
        
          string C = "C";
            Char[] charC = C.ToCharArray();
            _setting._serial.Write(charC, 0, 1); // sending char to microcontroller
            _setting._serial.Write("\r"); // sending carraige return 

            print_letter_flag = 'C';
        }

        private void button5_Click(object sender, EventArgs e)
        {
            // Sending the Char D so that micro controller knows that You are starting to collect the samples

            string D = "D";
            Char[] charD = D.ToCharArray();
            _setting._serial.Write(charD, 0, 1); // sending char to microcontroller

            _setting._serial.Write("\r"); // sending carraige return 



        }

        private void button6_Click_1(object sender, EventArgs e)
        {
            // Sending the Char T so that micro controller knows that You are sstopping the timer

            string T = "T";
            Char[] charT = T.ToCharArray();
            _setting._serial.Write(charT, 0, 1); // sending char to microcontroller
            _setting._serial.Write("\r"); // sending carraige return ("Enter") 


            richTextBox1.SelectAll();
            //richTextBox1.Sele.Text=""
            //richTextBox1.Text = String.Empty;
            richTextBox1.Clear();
            //richTextBox1.Focus();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            //Graph the temperature
            // Sending the Char  so that micro controller knows that You are sstopping the timer
            flag_D = 23;

            //zedGraphControl1.Refresh();    // Refresh  Graph
            zedGraphControl1.GraphPane.CurveList.Clear();
           


            //string Graph = "Graph";
            //Char[] Graph_1 = Graph.ToCharArray();
            //_setting._serial.Write(Graph_1, 0, 5); // sending char to microcontroller
            //_setting._serial.Write("\r"); // sending carraige return ("Enter") 

        }

        private void button7_Click(object sender, EventArgs e)
        {
            // Stop the timer and the Display on the terminal
            string T = "T";
            Char[] charT = T.ToCharArray();
            _setting._serial.Write(charT, 0, 1); // sending char to microcontroller
            _setting._serial.Write("\r"); // sending carraige return ("Enter") 

            flag_D = 0; // Reset graph

        }


        public void WriteData()
        {
            Excelclass names_of_excel = new Excelclass();
            //Excelclass name_excel = new Excelclass(@"Test.xlsx", 1);
           // names_of_excel.CreateNewFile();
            //names_of_excel.CreateNewsheet();
            names_of_excel.WriteTocell(0, 0, "Sample frequency");     //Cell A1  
            names_of_excel.WriteTocell(1, 0, "Sample Rate");         //Cell A2
            names_of_excel.WriteTocell(2, 0, "Average Number");      //Cell A3 
            names_of_excel.SaveAs(@"Test working Excel C# 3");
            names_of_excel.Close_file();
        }

        private void button9_Click(object sender, EventArgs e)
        {

            flag_D1 = 40; // Dont graph
            flag_D = 0; // Dont graph
            // Creating the file path and Initializations
            // Excelclass names_of_excel = new Excelclass(@"Test 24.xlsx",1);

            choosing_data.CreateNewFile();
            
            // choosing_data.CreateNewsheet();
            choosing_data.WriteTocell(0, 0, "Sample frequency");     //Cell A1  
            choosing_data.WriteTocell(1, 0, "Sample Rate");          //Cell A2
            choosing_data.WriteTocell(2, 0, "Average Number");       //CellA3
            choosing_data.WriteTocell(3, 3, "Data Point");         //Cell D4 
            choosing_data.WriteTocell(3, 4, "Temperature(F)");        //Cell E4
            choosing_data.WriteTocell(3, 5, "Time(seconds)");         //Cell F4 

            // WriteData();
            //Write the Set Parameter in the Cells in Excel
            //choosing_data.CreateNewFile();
            choosing_data.WriteTocell(0, 1, sfrequency_string_global);        //Cell B1
            choosing_data.WriteTocell(1, 1, srate_string_global);             //Cell B2
            choosing_data.WriteTocell(2, 1, AverageNum_string_global);       //Cell B3


            //year:hour:min:sec
            //choosing_data.WriteTocell(3, 6,crazy_now);       //Cell G4


            //Stop the timer to collect the Data points
            string T = "T";
            Char[] charT = T.ToCharArray();
            _setting._serial.Write(charT, 0, 1); // sending char to microcontroller
            _setting._serial.Write("\r"); // sending carraige return ("Enter") 

            total_length = 7 + time_length;
            //string[] temparray2 = new string[total_length-5];    //Total length o temp array is 1000
            //string[] timearray2 = new string[total_length - 5];    //Total length of time array is 1000
            int m;
            int k;
            int t;
            string me = "";
            for (m = 0; m <= time_length; m++)
            {
                for (k = 0; k < 2; k++)
                {
                    temparray2[k, m] = temparray[k, m];
                  
                }

                
                // temparray2[m-1,k] = temparray[m,k];
                //timearray2[m-1] = timearray[m];

                //choosing_data.WriteRange(5, total_length, timearray2);          //Uses Cell D (Display Time)
                //choosing_data.WriteRange2(5, total_length, temparray2);       //Uses Cell E (Display Temp)

            }
            int total = time_length + 1;
            string[] str_2 = new string[total];
            string str_21;
            string[] str_3 = new string[total];
           
            for (t = 1; t < total; t++)
            {
                str_21 = temparray2[0, t];                //Collect the Time of the temperature graphed

                //convert to variable
                 float var = float.Parse(str_21);   //put the number as an interger
                //float var = Convert.ToInt32(str_21.ToString());
                if (collectperiod > 0.0)
                {
                    float var1 = var * collectperiod;
                    string var3 = var1.ToString();
                    //ws.Cells[i, j].Value2 = s;
                    str_3[t] = var3;
                   
                }


            }

            choosing_data.particularcell(5, 6, total, str_3);        // Send the Time in  Col F

            choosing_data.Writeto_multiplecells(5, 4,5,6, total_length, temparray2,currenttimearray);
            //choosing_data.WriteRange3(4, 4, 5, total_length, temparray2);       //Uses Cell E (Display Temp)

            //Set the Time and the Temperature Value
            // choosing_data.WriteRange(5, total_length, timearray2);          //Uses Cell D (Display Time)
            // choosing_data.WriteRange2(5, total_length, temparray2);       //Uses Cell E (Display Temp)
            string Message = "Total data Temp collected is :" + time_length;
            //string Message1 = Message.Append<string>(time_length);
            //Show a message on the Scrren
            MessageBox.Show(Message);

           

            if (count_file > 6)
            {
                choosing_data.SaveAs(@"Test working Excel C# 6");
            }
            //choosing_data.WriteTocell(2, 1, AverageNum_string_global);       //Cell B3
            choosing_data.SaveAs(@"Test working Excel C# 5");
            choosing_data.Close_file();
            count_file++;
            
        }

        private void button8_Click(object sender, EventArgs e)
        {
            flag_D = 0;   //Reset graph

            // Button For Main Menu
            string ch = "*";
            Char[] charTh = ch.ToCharArray();
            _setting._serial.Write(charTh, 0, 1); // sending char to microcontroller
            _setting._serial.Write("\r"); // sending carraige return ("Enter") 


        }

        private void button11_Click(object sender, EventArgs e)
        {
            //Button that acts like the entered button sending values to the microcontroller
            if (print_letter_flag == 'A')
            {
                //Send the entered numbers as char to the micro controller
                int sfrequency = int.Parse(textBoxSfrequency.Text);   //put the number as an interger
                string sfrequency_string = sfrequency.ToString();
                sfrequency_string_global = sfrequency_string;         // Global string for Excel plotting
                int sfrequency_count = sfrequency_string.Length;
                char[] sfrequency_char = sfrequency_string.ToCharArray();

                _setting._serial.Write(sfrequency_char, 0, sfrequency_count); // sending char to microcontroller
                _setting._serial.Write("\r"); // sending carraige return 
                Array.Clear(sfrequency_char, 0, sfrequency_char.Length);
                sfrequency_count = 0;
                sfrequency = 0;
                sfrequency_string = "";
                //sfrequency_string_global = "";
            }

            else if (print_letter_flag == 'B')
            {
                //Send the entered numbers as char to the micro controller
                float srate = float.Parse(textBoxSRate.Text);   //put the number as an interger
                collectperiod = srate;
                string srate_string = srate.ToString();
                srate_string_global = srate_string;        //Global string for Excel
                int srate_count = srate_string.Length;
                char[] srate_char = srate_string.ToCharArray();

                _setting._serial.Write(srate_char, 0, srate_count); // sending char to microcontroller
                _setting._serial.Write("\r"); // sending carraige return 

                Array.Clear(srate_char, 0, srate_char.Length);
                srate_count = 0;
                srate = 0;
                srate_string = "";
               // srate_string_global = "";
            }
            else if (print_letter_flag =='C')
            {

                //Send the entered numbers as char to the micro controller
                int AverageNum = int.Parse(textBoxAverageNum.Text);   //put the number as an interger
                string AverageNum_string = AverageNum.ToString();
                AverageNum_string_global = AverageNum_string;            //Global string for Excel
                int AverageNum_count = AverageNum_string.Length;
                char[] AverageNum_char = AverageNum_string.ToCharArray();

                _setting._serial.Write(AverageNum_char, 0, AverageNum_count); // sending char to microcontroller 
                _setting._serial.Write("\r"); // sending carraige return
                Array.Clear(AverageNum_char, 0,AverageNum_char.Length);
                AverageNum_count = 0;
                AverageNum = 0;
                AverageNum_string = "";
               // AverageNum_string_global = "";
            }

            else { }

        }

         
        
    }



}

