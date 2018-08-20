--
-- File generated with SQLiteStudio v3.2.1 on ÷‹»’ 8‘¬ 19 20:28:30 2018
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
