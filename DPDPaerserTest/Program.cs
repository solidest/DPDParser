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
        private extern static bool start_parse(int taskid);


        static void Main(string[] args)
        {

            var db = CreateParserDB();

            //创建分析任务
            var taskid = CreateUTF8FileTask(@"C:\Users\solidest\Desktop\pdb.txt", db);
            Console.WriteLine("Create task:" + taskid.ToString() + "\n");
            //var taskid = CreateProtocolTask("Protocol testtest Segment segoftest Array End", db);
            //var taskid = CreateSegmentTask("Segment seg1 Array Segment seg2 StandardUInt8 ByteOrder=Big", db);

            //调用dll启动分析任务
            start_parse(taskid);

            //读取分析结果
            ReadTaskResult(taskid, db);

            //清楚全部任务数据
            ClearTask(taskid, db);

            //释放数据库
            ReleaseParserDB(db);
            string c = Console.ReadLine();

        }

        //读取分析结果
        static void  ReadTaskResult(int taskid, SQLiteBase db)
        {
            var t = db.ExecuteQuery("SELECT protocolname FROM predpd_protocol WHERE  taskid = " + taskid.ToString());
            foreach (DataRow r in t.Rows)
            {
                Console.WriteLine(r["protocolname"].ToString() + "\n");
            }

            Console.WriteLine("Get Protocol Count:" + t.Rows.Count + "\n");
        }

        //创建一条文件分析任务
        static int CreateUTF8FileTask(string filename, SQLiteWrapper.SQLiteBase db)
        {
            string sql = String.Format("INSERT INTO predpd_task (tasktype, taskstate, dpdcode) VALUES({0}, {1}, \"{2}\");", 1, 1, filename);
            db.ExecuteNonQuery(sql);
            return db.GetLastRowId();
        }

        //创建一条协议分析任务
        static int CreateProtocolTask(string code, SQLiteBase db)
        {
            UTF8Encoding utf8 = new UTF8Encoding();
            code = utf8.GetString(utf8.GetBytes(code));
            string sql = String.Format("INSERT INTO predpd_task (tasktype, taskstate, dpdcode) VALUES({0}, {1}, \"{2}\");", 2, 1, code);
            db.ExecuteNonQuery(sql);
            return db.GetLastRowId();
        }

        //创建一条字段分析任务
        static int CreateSegmentTask(string code, SQLiteBase db)
        {
            UTF8Encoding utf8 = new UTF8Encoding();
            code = utf8.GetString(utf8.GetBytes(code));
            string sql = String.Format("INSERT INTO predpd_task (tasktype, taskstate, dpdcode) VALUES({0}, {1}, \"{2}\");", 3, 1, code);
            db.ExecuteNonQuery(sql);
            return db.GetLastRowId();
        }

        //清除任务数据
        static void ClearTask(int taskid, SQLiteBase db)
        {
            db.ExecuteNonQuery("DELETE FROM predpd_error WHERE taskid = " + taskid.ToString());
            db.ExecuteNonQuery("DELETE FROM predpd_notes WHERE taskid = " + taskid.ToString());
            db.ExecuteNonQuery("DELETE FROM predpd_property WHERE taskid = " + taskid.ToString());
            db.ExecuteNonQuery("DELETE FROM predpd_segment WHERE taskid = " + taskid.ToString());
            db.ExecuteNonQuery("DELETE FROM predpd_symbol WHERE taskid = " + taskid.ToString());
            db.ExecuteNonQuery("DELETE FROM predpd_protocol WHERE taskid = " + taskid.ToString());
            db.ExecuteNonQuery("DELETE FROM predpd_task WHERE rowid =" + taskid.ToString());
        }

        //关闭数据库
        static void ReleaseParserDB(SQLiteBase db)
        {
            db.CloseDatabase();
        }

        //创建数据库结构
        static SQLiteBase CreateParserDB()
        {
            string sqlCmd =
               @"--
                -- File generated with SQLiteStudio v3.2.1 on 周三 8月 22 09:42:57 2018
                --
                -- Text encoding used: System
                --
                PRAGMA foreign_keys = off;
                BEGIN TRANSACTION;

                -- Table: dic_error
                CREATE TABLE dic_error (
                    error_code  INTEGER,
                    error_info  TEXT,
                    error_level INTEGER,
                    notes       TEXT
                );

                INSERT INTO dic_error (
                                            error_code,
                                            error_info,
                                            error_level,
                                            notes
                                        )
                                        VALUES (
                -                         1,
                                            '拼写错误',
                                            1,
                                            '词法分析器发现无法识别的符号'
                                        );

                INSERT INTO dic_error (
                                            error_code,
                                            error_info,
                                            error_level,
                                            notes
                                        )
                                        VALUES (
                -                         2,
                                            '语法错误',
                                            1,
                                            '语法分析过程中出现错误'
                                        );


                -- Table: predpd_error
                CREATE TABLE predpd_error (
                    taskid      INTEGER,
                    errorcode   INTEGER,
                    firstsymbol INTEGER,
                    lastsymbol  INTEGER
                );


                -- Table: predpd_notes
                CREATE TABLE predpd_notes (
                    notesymbol  INTEGER,
                    aftersymbol INTEGER
                );


                -- Table: predpd_property
                CREATE TABLE predpd_property (
                    segment       INTEGER,
                    propertyname  INTEGER,
                    propertyvalue INTEGER,
                    propertytype  INTEGER
                );

                INSERT INTO predpd_property (
                                                segment,
                                                propertyname,
                                                propertyvalue,
                                                propertytype
                                            )
                                            VALUES (
                                                2,
                                                3,
                                                4,
                                                5
                                            );


                -- Table: predpd_protocol
                CREATE TABLE predpd_protocol (
                    taskid       INTEGER,
                    protocolname INTEGER
                );

                INSERT INTO predpd_protocol (
                                                taskid,
                                                protocolname
                                            )
                                            VALUES (
                                                1,
                                                0
                                            );


                -- Table: predpd_segment
                CREATE TABLE predpd_segment (
                    protocol    INTEGER,
                    segmentname INTEGER,
                    segmenttype INTEGER
                );

                INSERT INTO predpd_segment (
                                                protocol,
                                                segmentname,
                                                segmenttype
                                            )
                                            VALUES (
                                                1,
                                                1,
                                                13
                                            );

                INSERT INTO predpd_segment (
                                                protocol,
                                                segmentname,
                                                segmenttype
                                            )
                                            VALUES (
                                                1,
                                                2,
                                                1
                                            );


                -- Table: predpd_symbol
                CREATE TABLE predpd_symbol (
                    taskid      INTEGER,
                    symbol      TEXT,
                    lineno      INTEGER,
                    firstcolumn INTEGER,
                    lastcolumn  INTEGER
                );

                INSERT INTO predpd_symbol (
                                                taskid,
                                                symbol,
                                                lineno,
                                                firstcolumn,
                                                lastcolumn
                                            )
                                            VALUES (
                                                1,
                                                'seg1',
                                                1,
                                                9,
                                                12
                                            );

                INSERT INTO predpd_symbol (
                                                taskid,
                                                symbol,
                                                lineno,
                                                firstcolumn,
                                                lastcolumn
                                            )
                                            VALUES (
                                                1,
                                                'seg2',
                                                1,
                                                28,
                                                31
                                            );

                INSERT INTO predpd_symbol (
                                                taskid,
                                                symbol,
                                                lineno,
                                                firstcolumn,
                                                lastcolumn
                                            )
                                            VALUES (
                                                1,
                                                'ByteOrder',
                                                1,
                                                47,
                                                55
                                            );

                INSERT INTO predpd_symbol (
                                                taskid,
                                                symbol,
                                                lineno,
                                                firstcolumn,
                                                lastcolumn
                                            )
                                            VALUES (
                                                1,
                                                'Big',
                                                1,
                                                57,
                                                59
                                            );


                -- Table: predpd_task
                CREATE TABLE predpd_task (
                    tasktype  INTEGER,
                    taskstate INTEGER,
                    dpdcode   TEXT
                );

                INSERT INTO predpd_task (
                                            tasktype,
                                            taskstate,
                                            dpdcode
                                        )
                                        VALUES (
                                            3,
                                            3,
                                            'Segment seg1 Array Segment seg2 StandardUInt8 ByteOrder=Big'
                                        );


                COMMIT TRANSACTION;
                PRAGMA foreign_keys = on;


            ";

            var db = new SQLiteWrapper.SQLiteBase();
            db.OpenDatabase(@"file:C:\Users\solidest\Desktop\parser.db");
            //db.OpenDatabase(@"file::memory:?cache=shared");
            //db.ExecuteNonQuery(sqlCmd);
            return db;
        }

    }

}
