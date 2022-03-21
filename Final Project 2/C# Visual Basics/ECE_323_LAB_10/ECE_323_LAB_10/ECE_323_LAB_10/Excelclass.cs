using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Office.Interop.Excel;
using _Excel = Microsoft.Office.Interop.Excel;


namespace ECE_323_LAB_10
{
    class Excelclass
    {
        //For Excel
        string path = "";
        _Application excel = new _Excel.Application();
        Workbook wb;
        Worksheet ws;

        public Excelclass()
        {

        }
        public  Excelclass(string path, int Sheet)
        {
             this.path = path;
             wb = excel.Workbooks.Open(path);
             ws = wb.Worksheets[Sheet];

        }

        public void WriteTocell(int i, int j, string s)
        {
            //write to excel file cells
            i++;     // like rows 
            j++;   //Like columns A,B,C 

            ws.Cells[i, j].Value2 = s;
        }
        public void particularcell(int i, int j,int endrow, string[] str)
        {
            int y;

            for (y = 0; y < endrow; y++)
            {
                ws.Cells[i, j].Value2 = str[y];
                i++;
               

            }
            
        }

        public void SaveAs(string path)
        {
           // wb.Save();
            wb.SaveAs(path);
        }
        public void Close_file()
        {
            wb.Close();
        }

        public void CreateNewFile()
        {
            
            this.wb = excel.Workbooks.Add(XlWBATemplate.xlWBATWorksheet);
            this.ws = wb.Worksheets[1]; 

        }

        public void CreateNewsheet()
        {

            // Creating a new file in the Location
            //string Location = @"C:\Users\Joel\Documents\Excel C#";
            //string fielname = System.IO.Path.Combine(Location, path);
            //if (!System.IO.File.Exists(fielname))
            //{
            //    System.IO.File.Create(fielname);
            //    MessageBox.Show("Excel file created Open");

            //}
            //else
            //{
            //    MessageBox.Show("AlreadyExist");
            //}

             Worksheet temptsheet = wb.Worksheets.Add(After: ws); 
        }
        public void WriteRange(int starti, int endi, string[] writestring)

        {
            //Uses Cell D
            Range range = (Range)ws.Range[ws.Cells[starti, 4], ws.Cells[endi,starti]];
            range.Value2 = writestring;
           
        }

        public void WriteRange2(int starti, int endi, string[] writestring)

        {
            //usess Cell E

            Range range = (Range)ws.Range[ws.Cells[starti, 5], ws.Cells[endi, 5]];
            range.Value2 = writestring;

        }

        public void WriteRange3(int starti, int starty, int endi, int endy, string[,] writestring)

        {
            //Uses Cell D
            Range range = (Range)ws.Range[ws.Cells[starti, starty], ws.Cells[endi, endy]];
            range.Value2 = writestring;

        }

        public void Writeto_multiplecells(int startRow, int coln,int coln2,int coln3, int endRow, string[,] str,string[] timestr)
        {
            // seperate into two strings
            int m;
            int j;
           string [] str_1 = new string[endRow-startRow];
           string [] str_2 = new string[endRow-startRow];
           string[] str_3 = new string[endRow - startRow];
           //int [] var = new int[endRow - startRow];
            for (m =0; m < endRow-startRow; m++)
            {
                str_1[m] = str[1, m];       //Collect the Temperature graphed

            }
            
            for (j =0; j < endRow - startRow; j++)
            {
                str_2[j] = str[0, j];      //Collect the Time of the temperature graphed

            }
            m = 0; j = 0;   //reset the counters to be used again
            //int k;

            //Put the string into given cells
            for (m = 0; m < endRow-startRow; m++)
            {
              
                    ws.Cells[m+startRow, coln2].Value2 = str_1[m];   //Put the temperature in the second coln
            }

            for (j = 0; j < endRow-startRow; j++)
            {
                ws.Cells[j+startRow, coln].Value2 = str_2[j];    //Put the time in the first coln
                ws.Cells[j+startRow, coln3].Value2 = timestr[j];    //Put the time in the Last coln


            }

           
            


        }
    }
}
