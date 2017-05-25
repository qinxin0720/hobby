USE RES;
go

if not exists(select * from sysobjects where name='TESTDATA')
CREATE TABLE TESTDATA
(
	Datatime datetime NOT NULL,
	Point1 nvarchar(16) NOT NULL,
	Point2 nvarchar(16) NOT NULL,
	Uplimit nvarchar(32) NOT NULL,
	Lowlimit nvarchar(32) NOT NULL,
	Data nvarchar(32) NOT NULL,
	Result nvarchar(8) NOT NULL,
);
go