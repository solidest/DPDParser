using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using SQLiteWrapper;

namespace DPDPaerserTest
{
    class Program
    {
        static void Main(string[] args)
        {
            string sqlCmd =
                @"--
                -- File generated with SQLiteStudio v3.2.1 on 周日 8月 19 20:28:30 2018
                --
                -- Text encoding used: System
                --
                PRAGMA foreign_keys = off;
                BEGIN TRANSACTION;

                -- Table: pre_notes
                CREATE TABLE pre_notes (
                    note      TEXT,
                    lineno    INTEGER,
                    tokentype INTEGER,
                    tokenid   INTEGER
                );


                -- Table: pre_protocol
                CREATE TABLE pre_protocol (
                    pretaskid INTEGER,
                    name      CHAR (40),
                    lineno    INTEGER
                );


                -- Table: predpd_task
                CREATE TABLE predpd_task (
                    codeversion INTEGER,
                    taskstate   INTEGER,
                    dpdcode     TEXT
                );


                COMMIT TRANSACTION;
                PRAGMA foreign_keys = on;
            ";

            //创建数据库结构
            var db = new SQLiteWrapper.SQLiteBase();
            db.OpenDatabase("file:memdb1?mode=memory&cache=shared");
            db.ExecuteNonQuery(sqlCmd);
            var ts = db.GetTables();
            foreach(var t in ts)
            {
                Console.WriteLine(t);
            }

            //调用dll


            //关闭数据库
            db.CloseDatabase();
            string c = Console.ReadLine();        
        }

    }
}
