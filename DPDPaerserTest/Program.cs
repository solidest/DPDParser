using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using SQLiteWrapper;

namespace DPDPaerserTest
{
    class Program
    {
        [DllImport("DPDParser")]
        private extern static bool StartParse(int taskid);


        static void Main(string[] args)
        {

            var db = CreateParserDB();

            //创建分析任务
            var taskid = CreateUTF8FileTask("");

            //调用dll启动分析任务
            StartParse(taskid);

            //读取分析结果
            ReadTaskResult(taskid);

            //释放数据库
            ReleaseParserDB(db);
            string c = Console.ReadLine();

        }

        //读取分析结果
        static void  ReadTaskResult(int taskid)
        {
            ;
        }

        //创建数据库结构
        static SQLiteBase CreateParserDB()
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

            var db = new SQLiteWrapper.SQLiteBase();
            db.OpenDatabase("file::memory:?cache=shared");
            db.ExecuteNonQuery(sqlCmd);
            return db;
        }

        //创建一条文件分析任务
        static int CreateUTF8FileTask(string filename)
        {
            return -1;
        }

        //创建一条协议分析任务
        static int CreateProtocolTask(string protoCode)
        {
            return -1;
        }

        //创建一条字段分析任务
        static int CreateSegmentTask(string segCode)
        {
            return -1;
        }

        //关闭数据库
        static void ReleaseParserDB(SQLiteBase db)
        {
            db.CloseDatabase();
        }

    }

}
