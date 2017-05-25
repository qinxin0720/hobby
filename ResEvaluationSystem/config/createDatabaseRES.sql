use master;
go

if not exists (SELECT * FROM sysdatabases WHERE NAME='RES')
create database RES;

go