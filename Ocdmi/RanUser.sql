USE [master]
GO
/****** Object:  Database [RanUser]    Script Date: 2025/6/21 下午 05:02:30 ******/
CREATE DATABASE [RanUser]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'RanUser', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL15.MSSQLSERVER\MSSQL\DATA\RanUser.mdf' , SIZE = 185344KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'RanUser_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL15.MSSQLSERVER\MSSQL\DATA\RanUser_1.ldf' , SIZE = 3136KB , MAXSIZE = UNLIMITED, FILEGROWTH = 10%)
 WITH CATALOG_COLLATION = DATABASE_DEFAULT
GO
ALTER DATABASE [RanUser] SET COMPATIBILITY_LEVEL = 110
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [RanUser].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [RanUser] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [RanUser] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [RanUser] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [RanUser] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [RanUser] SET ARITHABORT OFF 
GO
ALTER DATABASE [RanUser] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [RanUser] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [RanUser] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [RanUser] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [RanUser] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [RanUser] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [RanUser] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [RanUser] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [RanUser] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [RanUser] SET  DISABLE_BROKER 
GO
ALTER DATABASE [RanUser] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [RanUser] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [RanUser] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [RanUser] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [RanUser] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [RanUser] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [RanUser] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [RanUser] SET RECOVERY FULL 
GO
ALTER DATABASE [RanUser] SET  MULTI_USER 
GO
ALTER DATABASE [RanUser] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [RanUser] SET DB_CHAINING OFF 
GO
ALTER DATABASE [RanUser] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [RanUser] SET TARGET_RECOVERY_TIME = 0 SECONDS 
GO
ALTER DATABASE [RanUser] SET DELAYED_DURABILITY = DISABLED 
GO
ALTER DATABASE [RanUser] SET ACCELERATED_DATABASE_RECOVERY = OFF  
GO
EXEC sys.sp_db_vardecimal_storage_format N'RanUser', N'ON'
GO
ALTER DATABASE [RanUser] SET QUERY_STORE = OFF
GO
USE [RanUser]
GO
/****** Object:  UserDefinedFunction [dbo].[fn_md5]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE FUNCTION [dbo].[fn_md5] (@data VARCHAR(10), @data2 VARCHAR(10))
RETURNS BINARY(16) AS
BEGIN
DECLARE @hash BINARY(16)
EXEC master.dbo.XP_MD5_EncodeKeyVal @data, @data2, @hash OUT
RETURN @hash
END
GO
/****** Object:  Table [dbo].[LogGameTime]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogGameTime](
	[GameTimeNum] [bigint] IDENTITY(1,1) NOT NULL,
	[LogDate] [datetime] NOT NULL,
	[GameTime] [int] NOT NULL,
	[UserID] [varchar](20) NULL,
	[UserNum] [int] NULL,
	[SGNum] [int] NULL,
	[SvrNum] [int] NULL,
	[ChaNum] [int] NULL,
 CONSTRAINT [PK_LogGameTime] PRIMARY KEY CLUSTERED 
(
	[GameTimeNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  View [dbo].[viewLogGameTime]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-------------------------------------------------------------------------------
-- viewLogGameTime
-------------------------------------------------------------------------------
CREATE VIEW [dbo].[viewLogGameTime] AS
SELECT GameTimeNum, LogDate, GameTime, UserID, UserNum, SGNum, SvrNum, ChaNum
FROM LogGameTime




GO
/****** Object:  Table [dbo].[LogLogin]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogLogin](
	[LoginNum] [bigint] IDENTITY(1,1) NOT NULL,
	[UserNum] [int] NOT NULL,
	[UserID] [varchar](20) NOT NULL,
	[LogInOut] [int] NOT NULL,
	[LogDate] [datetime] NULL,
	[LogIpAddress] [varchar](23) NULL,
 CONSTRAINT [PK_LogLogin] PRIMARY KEY CLUSTERED 
(
	[LoginNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  View [dbo].[viewLogLogin]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-------------------------------------------------------------------------------
-- viewLogLogin
-------------------------------------------------------------------------------
Create view [dbo].[viewLogLogin] AS
SELECT  LoginNum, UserNum, UserID, LogInOut, LogDate, LogIpAddress
FROM LogLogin




GO
/****** Object:  Table [dbo].[ServerGroup]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ServerGroup](
	[SGNum] [int] NOT NULL,
	[SGName] [varchar](50) NOT NULL,
	[OdbcName] [varchar](50) NOT NULL,
	[OdbcUserID] [varchar](20) NOT NULL,
	[OdbcPassword] [varchar](20) NOT NULL,
	[OdbcLogName] [varchar](50) NOT NULL,
	[OdbcLogUserID] [varchar](20) NOT NULL,
	[OdbcLogPassword] [varchar](20) NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[SGNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ServerInfo]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ServerInfo](
	[SGNum] [int] NOT NULL,
	[SvrNum] [int] NOT NULL,
	[SvrType] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[SvrNum] ASC,
	[SGNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  View [dbo].[viewServerList]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO
-------------------------------------------------------------------------------
-- view_ServerList.sql
-------------------------------------------------------------------------------

CREATE VIEW [dbo].[viewServerList]
AS
SELECT  A.SGNum, A.SvrNum, A.SvrType, B.SGName, 
        B.OdbcName, B.OdbcUserID, B.OdbcPassword,
        B.OdbcLogName, B.OdbcLogUserID, B.OdbcLogPassword
FROM    dbo.ServerInfo A LEFT OUTER JOIN
        dbo.ServerGroup B ON A.SGNum = B.SGNum
WHERE   (A.SvrType = 4)




GO
/****** Object:  Table [dbo].[BlockAddress]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[BlockAddress](
	[BlockIdx] [int] IDENTITY(1,1) NOT NULL,
	[BlockAddress] [varchar](23) NOT NULL,
	[BlockReason] [varchar](256) NULL,
	[BlockDate] [datetime] NULL,
 CONSTRAINT [PK_BlockAddress] PRIMARY KEY CLUSTERED 
(
	[BlockIdx] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[CheckId]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[CheckId](
	[Num] [int] IDENTITY(1,1) NOT NULL,
	[Id] [varchar](18) NOT NULL,
	[CreateDate] [datetime] NULL,
 CONSTRAINT [PK_CheckId] PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[FullUserInfo]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[FullUserInfo](
	[UserNum] [int] IDENTITY(1,1) NOT NULL,
	[UserName] [varchar](20) NOT NULL,
	[UserID] [varchar](20) NOT NULL,
	[UserPass] [varchar](20) NOT NULL,
	[UserPass2] [varchar](20) NOT NULL,
	[BodyID] [varchar](18) NOT NULL,
	[Sex] [varchar](2) NOT NULL,
	[Email] [varchar](50) NOT NULL,
	[BirthY] [varchar](4) NOT NULL,
	[BirthM] [varchar](2) NOT NULL,
	[BirthD] [varchar](2) NOT NULL,
	[TEL] [varchar](15) NOT NULL,
	[Mobile] [varchar](13) NULL,
	[QQ] [varchar](13) NULL,
	[MSN] [varchar](50) NULL,
	[City1] [varchar](20) NOT NULL,
	[City2] [varchar](20) NOT NULL,
	[Post] [varchar](6) NULL,
	[Address] [varchar](60) NULL,
	[SafeId] [varchar](12) NOT NULL,
	[BodyID2] [varchar](18) NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[gmc]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[gmc](
	[username] [nvarchar](50) NULL,
	[session] [varchar](max) NULL,
	[sesexp] [varchar](max) NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[IPInfo]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[IPInfo](
	[IpAddress] [varchar](23) NOT NULL,
	[UserNum] [int] NOT NULL,
	[IdxIP] [int] IDENTITY(1,1) NOT NULL,
	[UseAvailable] [int] NULL,
 CONSTRAINT [PK_IPInfo] PRIMARY KEY CLUSTERED 
(
	[IdxIP] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogGmCmd]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogGmCmd](
	[GmCmdNum] [int] IDENTITY(1,1) NOT NULL,
	[LogDate] [datetime] NULL,
	[GmCmd] [varchar](200) NULL,
	[UserNum] [int] NULL,
 CONSTRAINT [PK_LogGmCmd] PRIMARY KEY CLUSTERED 
(
	[GmCmdNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogServerState]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogServerState](
	[SvrStateNum] [int] IDENTITY(1,1) NOT NULL,
	[LogDate] [datetime] NOT NULL,
	[UserNum] [int] NOT NULL,
	[UserMaxNum] [int] NOT NULL,
	[SvrNum] [int] NOT NULL,
	[SGNum] [int] NOT NULL,
 CONSTRAINT [PK_LogServerState] PRIMARY KEY CLUSTERED 
(
	[SvrStateNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[newcheckid]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[newcheckid](
	[Id] [varchar](18) NOT NULL,
	[CreateDate] [datetime] NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[StatGameTime]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[StatGameTime](
	[GYear] [int] NOT NULL,
	[GMonth] [int] NOT NULL,
	[GDay] [int] NOT NULL,
	[GTime] [int] NULL,
 CONSTRAINT [PK_StatGameTime] PRIMARY KEY CLUSTERED 
(
	[GYear] ASC,
	[GMonth] ASC,
	[GDay] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[StatLogin]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[StatLogin](
	[LYear] [int] NOT NULL,
	[LMonth] [int] NOT NULL,
	[LDay] [int] NOT NULL,
	[LHour] [int] NOT NULL,
	[LCount] [int] NULL,
 CONSTRAINT [PK_StatLogin] PRIMARY KEY CLUSTERED 
(
	[LYear] ASC,
	[LMonth] ASC,
	[LDay] ASC,
	[LHour] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[TopUp]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[TopUp](
	[ID] [bigint] IDENTITY(1,1) NOT NULL,
	[CardID] [varchar](10) NULL,
	[CardCODE] [varchar](10) NULL,
	[CardVALUE] [bigint] NOT NULL,
	[CardSTATUS] [int] NOT NULL,
	[DateGenerated] [datetime] NOT NULL,
	[UserAgentID] [int] NOT NULL,
	[UserTopUp] [varchar](50) NULL,
	[DateTopUp] [datetime] NULL,
	[CardVALUECP] [bigint] NOT NULL,
	[CardType] [int] NOT NULL,
	[CardPrice] [int] NOT NULL,
 CONSTRAINT [PK_TopUp] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[UserInfo]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[UserInfo](
	[UserNum] [int] IDENTITY(1,1) NOT NULL,
	[UserName] [varchar](20) NOT NULL,
	[UserID] [varchar](20) NOT NULL,
	[UserPass] [varchar](20) NOT NULL,
	[UserPass_N] [varchar](50) NOT NULL,
	[UserPass2] [varchar](20) NOT NULL,
	[UserPass2_N] [varchar](50) NOT NULL,
	[UserType] [int] NOT NULL,
	[UserLoginState] [int] NOT NULL,
	[UserAvailable] [int] NULL,
	[CreateDate] [datetime] NOT NULL,
	[LastLoginDate] [datetime] NOT NULL,
	[SGNum] [int] NULL,
	[SvrNum] [int] NULL,
	[ChaName] [varchar](33) NULL,
	[UserBlock] [int] NOT NULL,
	[UserBlockDate] [datetime] NOT NULL,
	[ChaRemain] [int] NOT NULL,
	[ChaTestRemain] [int] NOT NULL,
	[PremiumDate] [datetime] NOT NULL,
	[ChatBlockDate] [datetime] NOT NULL,
	[UserPoint] [int] NOT NULL,
	[UserPoint2] [int] NOT NULL,
	[WebLoginState] [varchar](50) NULL,
	[UserAge] [int] NOT NULL,
	[OfflineTime] [bigint] NOT NULL,
	[GameTime] [bigint] NOT NULL,
	[UserIP] [varchar](50) NULL,
	[PlayTime] [bigint] NOT NULL,
	[UserEmail] [varchar](50) NOT NULL,
	[NameFirst] [varchar](50) NOT NULL,
	[NameLast] [varchar](50) NOT NULL,
	[PhoneNumber] [varchar](50) NOT NULL,
	[UserSA] [varchar](50) NOT NULL,
	[UserType2] [int] NOT NULL,
 CONSTRAINT [PK_UserInfo] PRIMARY KEY CLUSTERED 
(
	[UserNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY],
 CONSTRAINT [IX_UserInfoUserID] UNIQUE NONCLUSTERED 
(
	[UserID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[UserKey]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[UserKey](
	[Num] [int] IDENTITY(1,1) NOT NULL,
	[PCIDKey] [varchar](33) NOT NULL,
	[PCIDUserNum] [int] NOT NULL,
	[PCIDUserID] [varchar](20) NOT NULL,
	[PCIDSGNum] [int] NOT NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Vote]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Vote](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[UserID] [varchar](20) NOT NULL,
	[last_vote] [int] NULL,
	[date] [datetime] NOT NULL,
	[hits] [int] NULL
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[BlockAddress] ADD  CONSTRAINT [DF_BlockAddress_BlockDate]  DEFAULT (getdate()) FOR [BlockDate]
GO
ALTER TABLE [dbo].[FullUserInfo] ADD  CONSTRAINT [DF_FullUserInfo_UserName]  DEFAULT ('') FOR [UserName]
GO
ALTER TABLE [dbo].[FullUserInfo] ADD  CONSTRAINT [DF_FullUserInfo_UserID]  DEFAULT ('') FOR [UserID]
GO
ALTER TABLE [dbo].[FullUserInfo] ADD  CONSTRAINT [DF_FullUserInfo_UserPass]  DEFAULT ('') FOR [UserPass]
GO
ALTER TABLE [dbo].[FullUserInfo] ADD  CONSTRAINT [DF_FullUserInfo_UserPass2]  DEFAULT ('') FOR [UserPass2]
GO
ALTER TABLE [dbo].[FullUserInfo] ADD  CONSTRAINT [DF_FullUserInfo_BodyID]  DEFAULT ('') FOR [BodyID]
GO
ALTER TABLE [dbo].[LogGameTime] ADD  CONSTRAINT [DF_LogGameTime_LogDate]  DEFAULT (getdate()) FOR [LogDate]
GO
ALTER TABLE [dbo].[LogGameTime] ADD  CONSTRAINT [DF_LogGameTime_GameTime]  DEFAULT ((0)) FOR [GameTime]
GO
ALTER TABLE [dbo].[LogGameTime] ADD  CONSTRAINT [DF_LogGameTime_UserNum]  DEFAULT ((0)) FOR [UserNum]
GO
ALTER TABLE [dbo].[LogGameTime] ADD  CONSTRAINT [DF_LogGameTime_SGNum]  DEFAULT ((0)) FOR [SGNum]
GO
ALTER TABLE [dbo].[LogGameTime] ADD  CONSTRAINT [DF_LogGameTime_SvrNum]  DEFAULT ((0)) FOR [SvrNum]
GO
ALTER TABLE [dbo].[LogGameTime] ADD  CONSTRAINT [DF_LogGameTime_ChaNum]  DEFAULT ((0)) FOR [ChaNum]
GO
ALTER TABLE [dbo].[LogGmCmd] ADD  CONSTRAINT [DF_LogGmCmd_LogDate]  DEFAULT (getdate()) FOR [LogDate]
GO
ALTER TABLE [dbo].[LogLogin] ADD  CONSTRAINT [DF_LogLogin_LogInOut]  DEFAULT ((0)) FOR [LogInOut]
GO
ALTER TABLE [dbo].[LogLogin] ADD  CONSTRAINT [DF_LogLogin_LogDate]  DEFAULT (getdate()) FOR [LogDate]
GO
ALTER TABLE [dbo].[LogServerState] ADD  CONSTRAINT [DF_LogServer_LogDate]  DEFAULT (getdate()) FOR [LogDate]
GO
ALTER TABLE [dbo].[LogServerState] ADD  CONSTRAINT [DF_LogServer_UserNum]  DEFAULT ((0)) FOR [UserNum]
GO
ALTER TABLE [dbo].[LogServerState] ADD  CONSTRAINT [DF_LogServer_UserMaxNum]  DEFAULT ((0)) FOR [UserMaxNum]
GO
ALTER TABLE [dbo].[LogServerState] ADD  CONSTRAINT [DF_LogServer_SvrNum]  DEFAULT ((0)) FOR [SvrNum]
GO
ALTER TABLE [dbo].[LogServerState] ADD  CONSTRAINT [DF_LogServer_SGNum]  DEFAULT ((0)) FOR [SGNum]
GO
ALTER TABLE [dbo].[ServerGroup] ADD  CONSTRAINT [DF_ServerGroup_SGName]  DEFAULT ('') FOR [SGName]
GO
ALTER TABLE [dbo].[ServerGroup] ADD  CONSTRAINT [DF_ServerGroup_OdbcName]  DEFAULT ('') FOR [OdbcName]
GO
ALTER TABLE [dbo].[ServerGroup] ADD  CONSTRAINT [DF_ServerGroup_OdbcUserID]  DEFAULT ('') FOR [OdbcUserID]
GO
ALTER TABLE [dbo].[ServerGroup] ADD  CONSTRAINT [DF_ServerGroup_OdbcPassword]  DEFAULT ('') FOR [OdbcPassword]
GO
ALTER TABLE [dbo].[ServerGroup] ADD  CONSTRAINT [DF_ServerGroup_OdbcLogName]  DEFAULT ('') FOR [OdbcLogName]
GO
ALTER TABLE [dbo].[ServerGroup] ADD  CONSTRAINT [DF_ServerGroup_OdbcLogUserID]  DEFAULT ('') FOR [OdbcLogUserID]
GO
ALTER TABLE [dbo].[ServerGroup] ADD  CONSTRAINT [DF_ServerGroup_OdbcLogPassword]  DEFAULT ('') FOR [OdbcLogPassword]
GO
ALTER TABLE [dbo].[StatGameTime] ADD  CONSTRAINT [DF_StatGameTime_GYear]  DEFAULT (datepart(year,getdate())) FOR [GYear]
GO
ALTER TABLE [dbo].[StatGameTime] ADD  CONSTRAINT [DF_StatGameTime_GMonth]  DEFAULT (datepart(month,getdate())) FOR [GMonth]
GO
ALTER TABLE [dbo].[StatGameTime] ADD  CONSTRAINT [DF_StatGameTime_GDay]  DEFAULT (datepart(day,getdate())) FOR [GDay]
GO
ALTER TABLE [dbo].[StatGameTime] ADD  CONSTRAINT [DF_StatGameTime_GCount]  DEFAULT ((0)) FOR [GTime]
GO
ALTER TABLE [dbo].[StatLogin] ADD  CONSTRAINT [DF_StatLogin_LYear]  DEFAULT (datepart(year,getdate())) FOR [LYear]
GO
ALTER TABLE [dbo].[StatLogin] ADD  CONSTRAINT [DF_StatLogin_LMonth]  DEFAULT (datepart(month,getdate())) FOR [LMonth]
GO
ALTER TABLE [dbo].[StatLogin] ADD  CONSTRAINT [DF_StatLogin_LDay]  DEFAULT (datepart(day,getdate())) FOR [LDay]
GO
ALTER TABLE [dbo].[StatLogin] ADD  CONSTRAINT [DF_StatLogin_LHour]  DEFAULT (datepart(hour,getdate())) FOR [LHour]
GO
ALTER TABLE [dbo].[StatLogin] ADD  CONSTRAINT [DF_StatLogin_LCount]  DEFAULT ((1)) FOR [LCount]
GO
ALTER TABLE [dbo].[TopUp] ADD  CONSTRAINT [DF_TopUp_CardVALUE]  DEFAULT ((0)) FOR [CardVALUE]
GO
ALTER TABLE [dbo].[TopUp] ADD  CONSTRAINT [DF_TopUp_CardSTATUS]  DEFAULT ((0)) FOR [CardSTATUS]
GO
ALTER TABLE [dbo].[TopUp] ADD  CONSTRAINT [DF_TopUp_DateGenerated]  DEFAULT (getdate()) FOR [DateGenerated]
GO
ALTER TABLE [dbo].[TopUp] ADD  CONSTRAINT [DF_TopUp_UserAgentID]  DEFAULT ((0)) FOR [UserAgentID]
GO
ALTER TABLE [dbo].[TopUp] ADD  CONSTRAINT [DF_TopUp_CardVALUECP]  DEFAULT ((0)) FOR [CardVALUECP]
GO
ALTER TABLE [dbo].[TopUp] ADD  CONSTRAINT [DF_TopUp_CardType]  DEFAULT ((0)) FOR [CardType]
GO
ALTER TABLE [dbo].[TopUp] ADD  CONSTRAINT [DF_TopUp_CardPrice]  DEFAULT ((0)) FOR [CardPrice]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserName]  DEFAULT ('') FOR [UserName]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserID]  DEFAULT ('') FOR [UserID]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserPass]  DEFAULT ('') FOR [UserPass]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserPass_N]  DEFAULT ('empty') FOR [UserPass_N]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserPass2]  DEFAULT ('') FOR [UserPass2]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserPass2_N]  DEFAULT ('empty') FOR [UserPass2_N]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserType]  DEFAULT ((1)) FOR [UserType]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserLoginState]  DEFAULT ((0)) FOR [UserLoginState]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserAvailable]  DEFAULT ((1)) FOR [UserAvailable]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_CreateDate]  DEFAULT (getdate()) FOR [CreateDate]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_LastLoginDate]  DEFAULT (getdate()) FOR [LastLoginDate]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_SGNum]  DEFAULT ((0)) FOR [SGNum]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_SvrNum]  DEFAULT ((0)) FOR [SvrNum]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_ChaName]  DEFAULT ('') FOR [ChaName]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserBlock]  DEFAULT ((0)) FOR [UserBlock]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserBlockDate]  DEFAULT (getdate()) FOR [UserBlockDate]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_ChaRemain]  DEFAULT ((2)) FOR [ChaRemain]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_ChaTestRemain]  DEFAULT ((4)) FOR [ChaTestRemain]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_PremiumDate]  DEFAULT ('2020-02-01') FOR [PremiumDate]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_ChatBlockDate]  DEFAULT ('1970-02-01') FOR [ChatBlockDate]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserPoint]  DEFAULT ((0)) FOR [UserPoint]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserPoint2]  DEFAULT ((0)) FOR [UserPoint2]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserAge]  DEFAULT ((18)) FOR [UserAge]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_OfflineTime]  DEFAULT ((-999999999)) FOR [OfflineTime]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_GameTime]  DEFAULT ((-999999999)) FOR [GameTime]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_PlayTime]  DEFAULT ((0)) FOR [PlayTime]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_NameFirst]  DEFAULT ('empty') FOR [NameFirst]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_NameLast]  DEFAULT ('empty') FOR [NameLast]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_PhoneNumber]  DEFAULT ('empty') FOR [PhoneNumber]
GO
ALTER TABLE [dbo].[UserInfo] ADD  CONSTRAINT [DF_UserInfo_UserSA]  DEFAULT ('empty') FOR [UserSA]
GO
ALTER TABLE [dbo].[UserKey] ADD  CONSTRAINT [DF_UserKey_MacKey]  DEFAULT ('') FOR [PCIDKey]
GO
ALTER TABLE [dbo].[UserKey] ADD  CONSTRAINT [DF_UserKey_UserNum]  DEFAULT ((0)) FOR [PCIDUserNum]
GO
ALTER TABLE [dbo].[UserKey] ADD  CONSTRAINT [DF_UserKey_MacUserID]  DEFAULT ('') FOR [PCIDUserID]
GO
ALTER TABLE [dbo].[UserKey] ADD  CONSTRAINT [DF_UserKey_PCIDSGNum]  DEFAULT ((0)) FOR [PCIDSGNum]
GO
ALTER TABLE [dbo].[Vote] ADD  CONSTRAINT [DF_VoteTime_UserName]  DEFAULT ('') FOR [UserID]
GO
ALTER TABLE [dbo].[Vote] ADD  CONSTRAINT [DF_VoteTime_date]  DEFAULT ('1970-02-01') FOR [date]
GO
/****** Object:  StoredProcedure [dbo].[gm_login]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO
-------------------------------------------------------------------------------
-- sp_gm_login.sql
-------------------------------------------------------------------------------
CREATE PROCEDURE [dbo].[gm_login]
	@userId		char(50),
	@userPass 	char(50),
	@authenticate char(50),
	@nReturn 	int	OUTPUT
AS
	DECLARE @nUserNum	int,
		@nUserType	int

	SET NOCOUNT ON

	SET @nReturn = 0
	SET @nUserNum = 0
	
	SELECT @nUserNum = UserInfo.UserNum, @nUserType=UserInfo.UserType 
	FROM UserInfo 
	WHERE UserID = @userId AND UserPass = @userPass AND UserAvailable = 1 AND UserType>=20 

	-- ID / PWD ??...
	IF @nUserNum = 0
	BEGIN
		-- ID / PWD ??? ?? ???????
		SET @nReturn = 0
	END
	ELSE
	BEGIN
		-- ID / PWD ??
		SET @nReturn = @nUserType
		-- ???? ???
		INSERT INTO LogGmCmd (UserNum, GmCmd) 
		VALUES (@nUserNum, 'LOGIN GMTOOL UserID:' + @userId)
	END	

	SET NOCOUNT OFF	
	
	RETURN @nReturn




GO
/****** Object:  StoredProcedure [dbo].[log_serverstate]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
Create Procedure [dbo].[log_serverstate]	
	@usernum int,
	@usermax int,
    @svrnum   int,
	@sgnum    int
AS
	SET NOCOUNT ON

	INSERT INTO LogServerState (UserNum, UserMaxNum, SvrNum, SGNum) 
	VALUES (@usernum, @usermax, @svrnum, @sgnum)

	SET NOCOUNT OFF




GO
/****** Object:  StoredProcedure [dbo].[selchar_changemail]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[selchar_changemail]
    @userId			varchar(25),
    @userPass		varchar(25),
    @userPin		varchar(25),
    @userMailOld	varchar(50),
    @userMailNew	varchar(50),
    @nReturn		int    OUTPUT

AS        
    DECLARE 
        @nUserNum    int,
        @nUserMailCheck int
    SET NOCOUNT ON

    SET @nReturn = 0
    SET @nUserNum = 0
    SET @nUserMailCheck = 0
    
    --User ID Check begin--    
    SELECT @nUserMailCheck=UserInfo.UserNum
    FROM UserInfo
    WHERE UserEmail = @userMailNew
    
    SELECT @nUserNum=UserInfo.UserNum
    FROM UserInfo
    WHERE UserID = @userId and UserPass = @userPass and UserPass2 = @userPin and UserEmail = @userMailOld
	
	IF @nUserMailCheck <> 0
	BEGIN
		SET @nReturn = 3
		RETURN @nReturn
	END
	
    IF @nUserNum = 0
    BEGIN        
        SET @nReturn = 1
        RETURN @nReturn
    END
    ELSE
    BEGIN        
        SET @nReturn = 2   
    END

    IF (@nReturn = 2)
    BEGIN
		UPDATE UserInfo
		SET UserEmail = @userMailNew
		WHERE UserID = @userId
    END

    SET NOCOUNT OFF    
    
    RETURN @nReturn







GO
/****** Object:  StoredProcedure [dbo].[selchar_changepass]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[selchar_changepass]
    @userId			varchar(25),
    @userPass		varchar(25),
    @userPincode	varchar(25),
    @userNewPass	varchar(25),
    @nReturn		int    OUTPUT

AS        
    DECLARE 
        @nUserNum    int  
    SET NOCOUNT ON

    SET @nReturn = 0
    SET @nUserNum = 0
    
    --User ID Check begin--
    SELECT @nUserNum=UserInfo.UserNum
    FROM UserInfo
    WHERE UserID = @userId and UserPass = @userPass and UserPass2 = @userPincode
	
    IF @nUserNum = 0
    BEGIN        
        SET @nReturn = 1
        RETURN @nReturn
    END
    ELSE
    BEGIN        
        SET @nReturn = 2   
    END

    IF (@nReturn = 2)
    BEGIN
		UPDATE UserInfo
		SET UserPass = @userNewPass
		WHERE UserID = @userId
    END

    SET NOCOUNT OFF    
    
    RETURN @nReturn







GO
/****** Object:  StoredProcedure [dbo].[selchar_changepin]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[selchar_changepin]
    @userId			varchar(25),
    @userPass		varchar(25),
    @userPincode	varchar(25),
    @userNewPincode	varchar(25),
    @nReturn		int    OUTPUT

AS        
    DECLARE 
        @nUserNum    int  
    SET NOCOUNT ON

    SET @nReturn = 0
    SET @nUserNum = 0
    
    --User ID Check begin--
    SELECT @nUserNum=UserInfo.UserNum
    FROM UserInfo
    WHERE UserID = @userId and UserPass = @userPass and UserPass2 = @userPincode
	
    IF @nUserNum = 0
    BEGIN        
        SET @nReturn = 1
        RETURN @nReturn
    END
    ELSE
    BEGIN        
        SET @nReturn = 2   
    END

    IF (@nReturn = 2)
    BEGIN
		UPDATE UserInfo
		SET UserPass2 = @userNewPincode
		WHERE UserID = @userId
    END

    SET NOCOUNT OFF    
    
    RETURN @nReturn







GO
/****** Object:  StoredProcedure [dbo].[selchar_resetpin]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[selchar_resetpin]
    @userId			varchar(25),
    @userPass		varchar(25),
    @userMail		varchar(50),
    @userNewPincode	varchar(25),
    @nReturn		int    OUTPUT

AS        
    DECLARE 
        @nUserNum    int  
    SET NOCOUNT ON

    SET @nReturn = 0
    SET @nUserNum = 0
    
    --User ID Check begin--
    SELECT @nUserNum=UserInfo.UserNum
    FROM UserInfo
    WHERE UserID = @userId and UserPass = @userPass and UserEmail = @userMail
	
    IF @nUserNum = 0
    BEGIN        
        SET @nReturn = 1
        RETURN @nReturn
    END
    ELSE
    BEGIN        
        SET @nReturn = 2   
    END

    IF (@nReturn = 2)
    BEGIN
		UPDATE UserInfo
		SET UserPass2 = @userNewPincode
		WHERE UserID = @userId
    END

    SET NOCOUNT OFF    
    
    RETURN @nReturn







GO
/****** Object:  StoredProcedure [dbo].[selchar_topup]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[selchar_topup]
    @userId			varchar(25),
    @cardID			varchar(16),
    @cardCode		varchar(8),
    @nReturn		int    OUTPUT

AS        
    DECLARE 
		@nCardPrice	   int,
        @nCardValue    int,
        @nCardValueCP  int,
        @nCardType int,
        @nClaim int,
        @nToSend int,
        @nSendCount int
    SET NOCOUNT ON

    SET @nReturn = 0
    SET @nCardValue = 0
    SET @nCardValueCP = 0
    SET @nCardType = 0
    SET @nClaim = 0
    SET @nToSend = 0
    SET @nSendCount = 0
    SET @nCardPrice = 0
    
    --Topup Check begin--
    SELECT @nCardValue = CardVALUE,
    @nCardValueCP = CardVALUECP,
    @nCardType = CardType,
    @nCardPrice = CardPrice
    FROM TopUp
    WHERE CardID = @cardID and CardCODE = @cardCode and CardSTATUS = 0
    
    IF @nCardValue = 0 and @nCardValueCP = 0
    BEGIN        
        SET @nReturn = 1
    END
    ELSE
    BEGIN        
        SET @nReturn = 2   
    END
	
    IF (@nReturn = 2)
    BEGIN
		UPDATE UserInfo
		SET UserPoint = UserPoint + @nCardValue,
		UserPoint2 = UserPoint2 + @nCardValueCP
		WHERE UserID = @userId
			
		UPDATE TopUp
		SET UserTopUp = @userId, CardSTATUS = 1, DateTopUp=GETDATE()
		WHERE CardID = @cardID
		and CardCODE = @cardCode
		and CardSTATUS = 0
    END

    SET NOCOUNT OFF    
    
    RETURN @nReturn








GO
/****** Object:  StoredProcedure [dbo].[sp_GenerateTopUpCard]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



CREATE PROCEDURE [dbo].[sp_GenerateTopUpCard]
	@nEpValue			int,
	@nCpValue			int,
	@nPrice				int,
    @nUserID			int,
    @nReturn		int    OUTPUT

AS        
    DECLARE 
    @nCodeCounter int, 
    @nCodeGen int, 
    @nCardFound int, 
    @nCardId varchar(10),
    @nCardCode varchar(10),
    @nCardPrice int
    SET NOCOUNT ON
    
	SET @nCodeCounter = 0 
	SET @nCodeGen = 1
	SET @nCardFound = 0
	SET @nReturn = -1

	--dummy codes generate
	SET @nCardId = char(90)+char(82)+SUBSTRING(CONVERT(varchar(100), NEWID()),0,8)
	SET @nCardCode = char(rand()*26+65)+char(rand()*26+65)+char(rand()*26+65)+char(rand()*26+65)+char(rand()*26+65)+char(rand()*26+65) 
	SELECT @nCardFound = ID
	FROM TopUp
	WHERE CardID = @nCardId

	IF @nCardFound = 0 
	BEGIN
		INSERT INTO TopUp( CardId, CardCode, CardValue, CardVALUECP, UserAgentID,CardType,CardPrice)
		values( @nCardId,@nCardCode,@nEpValue,@nCpValue,@nUserID,0,@nPrice)		
		
		SELECT @nCardFound = ID
		FROM TopUp
		WHERE CardID = @nCardId	
		
		IF @nCardFound > 0 
		BEGIN
			SET @nReturn = @nCardFound
		END
	END
	
	SET NOCOUNT OFF   
    
    
    RETURN @nReturn
    
    










GO
/****** Object:  StoredProcedure [dbo].[sp_ReqGameTimeConvert]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[sp_ReqGameTimeConvert]
    @userId			varchar(25),
	/*@nType			int,
	@nVal			int,*/
    @nReturn		int    OUTPUT

AS        
    DECLARE 
        @nUserNum		int,
        @nGameTimeDivisor int,
        @nGameTime int,
		@nUserPoint int,
        @nRewardEP int,
        @nRewardCP int,
		@nValueEpToGP int
    SET NOCOUNT ON

	SET @nReturn = 0
    SET @nUserNum = 0
	--GameTime to GamePoint Settings--
    SET @nGameTimeDivisor = 60
    SET @nRewardCP = 2

	--EP TO GP SETTINGS--
	SET @nValueEpToGP = 1
    
    
    --User ID Check begin--
    SELECT 
    @nUserNum = UserInfo.UserNum,
    @nGameTime = UserInfo.PlayTime,
	@nUserPoint = UserInfo.UserPoint
    FROM UserInfo
    WHERE UserID = @userId 
    
	
    IF @nUserNum = 0
    BEGIN        
        SET @nReturn = -1
        RETURN @nReturn
    END
    ELSE
    BEGIN    
		SET @nReturn = @nGameTime / @nGameTimeDivisor
		IF @nReturn = 0
		BEGIN
			SET @nReturn = -1
			RETURN @nReturn
		END
		ELSE
		BEGIN
				UPDATE UserInfo
				SET PlayTime = PlayTime-(@nReturn*@nGameTimeDivisor)
				, UserPoint2 = UserPoint2 + (@nReturn*@nRewardCP)
				WHERE UserInfo.userID = @userId
				and PlayTime >= @nReturn*@nGameTimeDivisor
				
				SET @nReturn = @nReturn*@nRewardCP
				RETURN @nReturn
		END     
		/*
		IF @nType = 0
		BEGIN
			SET @nReturn = @nGameTime / @nGameTimeDivisor
			IF @nReturn = 0
			BEGIN
				SET @nReturn = -1
				RETURN @nReturn
			END
			ELSE
			BEGIN
					UPDATE UserInfo
					SET PlayTime = PlayTime-(@nReturn*@nGameTimeDivisor)
					, UserPoint2 = UserPoint2 + (@nReturn*@nRewardCP)
					WHERE UserInfo.userID = @userId
					and PlayTime >= @nReturn*@nGameTimeDivisor
				
					SET @nReturn = @nReturn*@nRewardCP
					RETURN @nReturn
			END
		END
		
		ELSE
		BEGIN
			if @nUserPoint >= @nVal
			BEGIN
				UPDATE UserInfo
				SET UserPoint = UserPoint-@nVal
				, UserPoint2 = UserPoint2 + (@nVal*@nValueEpToGP)
				WHERE UserInfo.userID = @userId
				and UserPoint >= @nVal
				
				SET @nReturn = @nVal*@nValueEpToGP
				RETURN @nReturn
			END
			ELSE
			BEGIN	
				SET @nReturn = -1
				RETURN @nReturn
			END
		END
		*/
		
    END
	
	SET NOCOUNT OFF   
     
    SET @nReturn = -1
    RETURN @nReturn








GO
/****** Object:  StoredProcedure [dbo].[sp_SelectVehicle]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


CREATE PROCEDURE [dbo].[sp_SelectVehicle]
	@nVehicleNum		int,
	@nVehicleChaNum		int
AS
	SET NOCOUNT ON
	
	SELECT VehicleChaNum, VehicleCardMID, VehicleCardSID,
	VehicleType, VehicleBattery, VehicleBooster FROM VehicleInfo WHERE VehicleNum = @nVehicleNum And VehicleChaNum = @nVehicleChaNum

	SET NOCOUNT OFF	





GO
/****** Object:  StoredProcedure [dbo].[sp_UpdateMoneyPoint]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create PROCEDURE [dbo].[sp_UpdateMoneyPoint]
	@nPremium			int,
	@userId				varchar(25),
	@nReturn			int OUTPUT
AS	

	SET NOCOUNT ON;
	
	BEGIN TRAN	
	
	UPDATE [UserInfo]
	SET [UserPoint] += @nPremium
	WHERE [UserName] = @userId
	
	IF @@ERROR = 0 
		BEGIN 
			COMMIT TRAN;
			SET @nReturn = 0;
		END
	SET NOCOUNT OFF;

RETURN @nReturn
GO
/****** Object:  StoredProcedure [dbo].[sp_UpdatePetDualSkill]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



-- Return
-- DB_ERROR -1
-- DB_OK 0
CREATE PROCEDURE [dbo].[sp_UpdatePetDualSkill]
	@nChaNum	int,
	@nPetNum	int,
	@nPetDualSkill	int,
	@nReturn	int OUTPUT
AS	
	DECLARE
		@error_var int, 
		@rowcount_var int
		
	SET NOCOUNT ON
	
	SET @nReturn = 0	
	
	BEGIN TRAN

	UPDATE PetInfo
	SET PetDualSkill=@nPetDualSkill
	WHERE PetNum=@nPetNum And PetChaNum=@nChaNum

	SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT    
	IF @error_var <> 0 OR @rowcount_var = 0
	BEGIN
		ROLLBACK TRAN
		SET @nReturn = -1
	END
	ELSE
	BEGIN
		COMMIT TRAN
		SET @nReturn = 0
	END    

	SET NOCOUNT OFF
	RETURN @nReturn






GO
/****** Object:  StoredProcedure [dbo].[sp_UpdateVehicleBooster]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


-- Return
-- DB_ERROR -1
-- DB_OK 0
CREATE PROCEDURE [dbo].[sp_UpdateVehicleBooster]
	@nVehicleNum	 int,
	@nVehicleChaNum	 int,
	@nVehicleBooster int,
	@nReturn		int OUTPUT
AS	
	DECLARE
		@error_var int, 
		@rowcount_var int
		
	SET NOCOUNT ON
	
	SET @nReturn = 0	
	
	BEGIN TRAN

	UPDATE VehicleInfo
	SET VehicleBooster=@nVehicleBooster
	WHERE VehicleNum=@nVehicleNum AND VehicleChaNum = @nVehicleChaNum

	SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
	IF @error_var <> 0 OR @rowcount_var = 0
	BEGIN
		ROLLBACK TRAN
		SET @nReturn = -1
	END
	ELSE
	BEGIN
		COMMIT TRAN
		SET @nReturn = 0
	END    

	SET NOCOUNT OFF
	RETURN @nReturn





GO
/****** Object:  StoredProcedure [dbo].[UpdateChaName]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[UpdateChaName]
	@nUserNum int,
	@szChaName varchar (33)
AS
	SET NOCOUNT ON
	
	UPDATE UserInfo
	SET ChaName=@szChaName
	WHERE UserNum=@nUserNum
	
	SET NOCOUNT OFF




GO
/****** Object:  StoredProcedure [dbo].[UpdateChaNumDecrease]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[UpdateChaNumDecrease]
	@nUserNum int,
	@nReturn int OUTPUT
AS	
	DECLARE
		@error_var int, 
		@rowcount_var int
		
	SET NOCOUNT ON
	
	SET @nReturn = 0
	
	UPDATE UserInfo
	SET ChaRemain=ChaRemain-1
	WHERE UserNum=@nUserNum
	
	SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
	IF @error_var <> 0 OR @rowcount_var = 0
	BEGIN
       	SET @nReturn = -1
		SET NOCOUNT OFF
		RETURN @nReturn
	END
	ELSE
	BEGIN
       	SET @nReturn = 0
		SET NOCOUNT OFF
		RETURN @nReturn
	END




GO
/****** Object:  StoredProcedure [dbo].[UpdateChaNumIncrease]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[UpdateChaNumIncrease]
	@nUserNum int,
	@nReturn int OUTPUT
AS	
	DECLARE
		@error_var int, 
		@rowcount_var int
		
	SET NOCOUNT ON
	
	SET @nReturn = 0
	
	UPDATE UserInfo
	SET ChaRemain=ChaRemain+1
	WHERE UserNum=@nUserNum	
	
	SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT    
	IF @error_var <> 0 OR @rowcount_var = 0
	BEGIN
       	SET @nReturn = -1
		SET NOCOUNT OFF
		RETURN @nReturn
	END
	ELSE
	BEGIN
       	SET @nReturn = 0
		SET NOCOUNT OFF
		RETURN @nReturn
	END




GO
/****** Object:  StoredProcedure [dbo].[UpdateTestChaNumDecrease]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[UpdateTestChaNumDecrease]
	@nUserNum int,
	@nReturn int OUTPUT
AS	
	DECLARE
		@error_var int, 
		@rowcount_var int
		
	SET NOCOUNT ON
	
	SET @nReturn = 0
	
	UPDATE UserInfo
	SET ChaTestRemain=ChaTestRemain-1
	WHERE UserNum=@nUserNum	
	
	SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT    
	IF @error_var <> 0 OR @rowcount_var = 0
	BEGIN
       	SET @nReturn = -1
		SET NOCOUNT OFF
		RETURN @nReturn
	END
	ELSE
	BEGIN
       	SET @nReturn = 0
		SET NOCOUNT OFF
		RETURN @nReturn
	END




GO
/****** Object:  StoredProcedure [dbo].[UpdateTestChaNumIncrease]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[UpdateTestChaNumIncrease]
	@nUserNum int,
	@nReturn int OUTPUT
AS	
	DECLARE
		@error_var int, 
		@rowcount_var int
		
	SET NOCOUNT ON
	
	SET @nReturn = 0
	
	UPDATE UserInfo
	SET ChaTestRemain=ChaTestRemain+1
	WHERE UserNum=@nUserNum	
	
	SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT    
	IF @error_var <> 0 OR @rowcount_var = 0
	BEGIN
       	SET @nReturn = -1
		SET NOCOUNT OFF
		RETURN @nReturn
	END
	ELSE
	BEGIN
       	SET @nReturn = 0
		SET NOCOUNT OFF
		RETURN @nReturn
	END




GO
/****** Object:  StoredProcedure [dbo].[user_cha_remain]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[user_cha_remain]
	@nUserNum	int,
	@nReturn 	int	OUTPUT
AS	    
	DECLARE @nChaRemain int

	SET NOCOUNT ON

	SET @nReturn = 0
	SET @nChaRemain = 0
	
	SELECT @nChaRemain = UserInfo.ChaRemain
	FROM UserInfo
	WHERE UserNum = @nUserNum
	
	SET @nReturn = @nChaRemain

	SET NOCOUNT OFF	
	
	RETURN @nReturn




GO
/****** Object:  StoredProcedure [dbo].[user_cha_test_remain]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[user_cha_test_remain]
	@nUserNum	int,
	@nReturn 	int	OUTPUT
AS	    
	DECLARE @nChaRemain int

	SET NOCOUNT ON

	SET @nReturn = 0
	
	SELECT @nChaRemain = UserInfo.ChaTestRemain
	FROM UserInfo
	WHERE UserNum = @nUserNum
	
	SET @nReturn = @nChaRemain

	SET NOCOUNT OFF	
	
	RETURN @nReturn




GO
/****** Object:  StoredProcedure [dbo].[user_changepass]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[user_changepass]
    @userId			varchar(25),
    @userPass		varchar(25),
    @userPass2		varchar(25),
    @nReturn		int    OUTPUT

AS        
    DECLARE 
        @nUserNum    int  
    SET NOCOUNT ON

    SET @nReturn = 0
    SET @nUserNum = 0
    
    --User ID Check begin--
    SELECT @nUserNum=UserInfo.UserNum
    FROM UserInfo
    WHERE UserID = @userId and UserPass2 = @userPass2
	
    IF @nUserNum = 0
    BEGIN        
        SET @nReturn = 1
        RETURN @nReturn
    END
    ELSE
    BEGIN        
        SET @nReturn = 2   
    END

    IF (@nReturn = 2)
    BEGIN
		UPDATE UserInfo
		SET UserInfo.UserPass = @userPass
		WHERE UserInfo.userID = @userId
    END

    SET NOCOUNT OFF    
    
    RETURN @nReturn


GO
/****** Object:  StoredProcedure [dbo].[user_forgotpass]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[user_forgotpass]
    @userId			varchar(25),
    @userPass		varchar(25),
    @nReturn		int    OUTPUT

AS        
    DECLARE 
        @nUserNum    int  
    SET NOCOUNT ON

    SET @nReturn = 0
    SET @nUserNum = 0
    
    --User ID Check begin--
    SELECT @nUserNum=UserInfo.UserNum
    FROM UserInfo
    WHERE UserID = @userId and UserPass2 = @userPass
	
    IF @nUserNum = 0
    BEGIN        
        SET @nReturn = 1
        RETURN @nReturn
    END
    ELSE
    BEGIN        
        SET @nReturn = 2   
    END

    IF (@nReturn = 2)
    BEGIN
		UPDATE UserInfo
		SET UserInfo.UserSA = @userId
		WHERE UserInfo.userID = @userId
    END

    SET NOCOUNT OFF    
    
    RETURN @nReturn


GO
/****** Object:  StoredProcedure [dbo].[user_gettype]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[user_gettype]
	@nUserNum	int,
	@nReturn 	int	OUTPUT
AS	
    
	DECLARE @nUserType int

	SET NOCOUNT ON

	SET @nReturn = 0
	SET @nUserType = 0
	
	SELECT @nUserType = UserInfo.UserType 
	FROM UserInfo
	WHERE UserNum = @nUserNum
	
	SET @nReturn = @nUserType

	SET NOCOUNT OFF	
	
	RETURN @nReturn




GO
/****** Object:  StoredProcedure [dbo].[user_logout]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO




CREATE Procedure [dbo].[user_logout]
    @userId     char(25),
    @usernum int,
    @gametime int,
    @chanum   int,
    @svrgrp   int,
    @svrnum   int,
    @extra   int,
    @nReturn  int OUTPUT

AS
    SET NOCOUNT ON

    DECLARE 
        -- Declare variables used in error checking.
        @error_var int, 
        @rowcount_var int
    
    -- ???, ??? ????? ??
    UPDATE UserInfo
    SET UserLoginState=0, LastLoginDate=getdate() 
    WHERE UserNum = @usernum
    
    -- ???? ??? 1 : ??? 0 : ????
    INSERT INTO LogLogin (UserNum, UserID, LogInOut) 
    VALUES (@usernum, @userId, 0)
    
    -- ???? ??? 
    INSERT INTO LogGameTime (UserNum, UserID, GameTime, ChaNum, SGNum, SvrNum) 
    VALUES (@usernum, @userId, @gametime, @chanum, @svrgrp, @svrnum)


	UPDATE UserInfo
    SET PlayTime=PlayTime+@gametime 
    WHERE usernum = @usernum
	
	---PCID
	DELETE FROM UserKey 
	WHERE PCIDUserNum=@usernum

    -- ???? ?? ????
    UPDATE StatGameTime
    SET GTime=GTime+@gametime 
    WHERE GYear=Year(GetDate()) AND GMonth=Month(GetDate()) AND GDay=Day(GetDate())

    SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
    IF @error_var <> 0 OR @rowcount_var = 0
    BEGIN
        INSERT INTO StatGameTime (GYear, GMonth, GDay, GTime)  
        VALUES (Year(GetDate()), Month(GetDate()), Day(GetDate()), @gametime)
    END

    SET NOCOUNT OFF  







GO
/****** Object:  StoredProcedure [dbo].[user_register]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



CREATE PROCEDURE [dbo].[user_register]
    @userId				varchar(25),
    @userPass1			varchar(25),
	@userPass2			varchar(25),
	@userSA				varchar(25),
	@userMail			varchar(50),
    @SvrGrpNum			int,
    @SvrNum				int,   
    @nReturn			int    OUTPUT

AS        
    DECLARE 
		@nUserNum int,
        @error_var int,
        @rowcount_var int

    SET NOCOUNT ON

    SET @nReturn = 0
    SET @nUserNum = 0
    
    SELECT @nUserNum=UserInfo.UserNum
           
    FROM UserInfo
    WHERE UserID = @userId

    IF @nUserNum > 0
    BEGIN    
		 /*user exist*/    
        SET @nReturn = 1       
        RETURN @nReturn 
    END
	ELSE
		BEGIN TRAN

		INSERT INTO UserInfo ( UserName, UserID, UserPass, UserPass2, UserSA, UserEmail, SGNum, SvrNum, UserType2 )
		VALUES (	@userId, @userId, @userPass1, @userPass2, @userSA, @userMail, @SvrGrpNum, @SvrNum, 0 )
	
		SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT    
		IF @error_var <> 0 OR @rowcount_var = 0
			BEGIN
			ROLLBACK TRAN
			SET @nReturn = -1
			END
		ELSE
			BEGIN
			COMMIT TRAN
			END

   
    SET NOCOUNT OFF    
    
    RETURN @nReturn  







GO
/****** Object:  StoredProcedure [dbo].[user_verify]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



CREATE PROCEDURE [dbo].[user_verify]
    @userId        char(25),
    @userPass     char(25),
    @userIp        char(25),
    @SvrGrpNum    int,
    @SvrNum        int,   
    @proPass    varchar(6), -- Ran dynamic password
    @proNum     varchar(2), -- Ran dynamic password random number
   -- @pcID  varchar(33), --PCID
    @nReturn     int    OUTPUT

AS        
    DECLARE 
        @nAvailable    int,
        @nUserNum    int,
        @nState int,
        -- Declare variables used in error checking.
        @error_var int, 
        @rowcount_var int,
        @nBlock int,
        @BlockDate datetime,
        @EndDate datetime,
        @StrSql nvarchar(100),
        @LastLoginDate datetime,
        @UserLevel int
 
    SET NOCOUNT ON

    SET @nReturn = 0
    SET @nUserNum = 0
    
    SELECT @nUserNum=UserInfo.UserNum, 
           @nState=UserInfo.UserLoginState, 
           @nBlock=UserInfo.UserBlock, 
           @BlockDate=UserInfo.UserBlockDate,
           @LastLoginDate=UserInfo.LastLoginDate,
           @UserLevel = UserInfo.UserType

    FROM UserInfo
    WHERE UserID = @userId AND UserPass = @userPass AND UserAvailable = 1


    -----------------------------------------------------------------
-- ID / PWD check...
    IF @nUserNum = 0
    BEGIN        
        SET @nReturn = 0        
        RETURN @nReturn
    END
    ELSE
    BEGIN        
        SET @nReturn = 1        
        IF @nState = 1
        BEGIN
            SET @nReturn = 5
             RETURN @nReturn 
--	update userinfo set userloginstate =0
--	where usernum =@nUserNum 
       END
    END

	---------
	---Login delay check
	---works only on non user gm
	IF @UserLevel < 20
	BEGIN
		IF (SELECT DATEDIFF(SECOND, @LastLoginDate, GETDATE()) ) < 20
		BEGIN
			SET @nReturn = 30
			RETURN @nReturn
		END
	END 
	
	---PCID
	---comment this lines to allow multi login per pc
	/*
	IF (SELECT COUNT(*) FROM UserKey WITH (NOLOCK) WHERE PCIDKey = @pcID ) > 0
	BEGIN
		-- pc active
		SET @nReturn = 31
		RETURN @nReturn
	END
	
	INSERT INTO UserKey (PCIDKey,PCIDUserNum,PCIDUserID,PCIDSGNum) VALUES (@pcID, @nUserNum, @userId, @SvrGrpNum )
	*/
	---END PCID
	
	
	IF (SELECT COUNT(*) FROM BlockAddress WITH (NOLOCK) WHERE BlockAddress = @userIp) > 0
    BEGIN
        declare @params as nvarchar(100)        
        SET @StrSQL = 'SELECT @ordercnt=count(userid) FROM Randpass WHERE userid='''+@userId+''' AND pwd'+@proNum + '='''+@proPass + ''''
        SET @params = '@ordercnt as int OUTPUT' -- OUTPUT ???? ??
        
        exec sp_executesql @StrSQL, @params, @ordercnt = @rowcount_var OUTPUT

        SELECT @error_var = @@ERROR
        IF @error_var = 0 AND @rowcount_var = 1
        BEGIN
            SET @nReturn = 1 -- Dynamic password correct
        END
        ELSE
        BEGIN
            SET @nReturn = 7 -- Dynamic password error            
            RETURN @nReturn            
        END        
    END


    -----------------------------------------------------------------
    -- IP Address ??
    IF (SELECT COUNT(*) FROM IPInfo WITH (NOLOCK) WHERE ipAddress = @userIp) > 0
    -- IP ?? ??
    BEGIN
        SELECT @nAvailable = useAvailable 
        FROM IPInfo 
        WHERE ipAddress = @userIp
        
        IF @nAvailable = 1 
        BEGIN            
            SET @nReturn = 2 -- ID/PWD ? ????, IP ? ????            
        END
        ELSE
        BEGIN            
            SET @nReturn = 4 -- ID/PWD ? ????, IP ? ????? ??            
        END
    END
    ELSE
    -- IP ?? ??
    BEGIN
        SET @nReturn = 3     -- ID/PWD ? ????, IP ??? ??        
    END

    -----------------------------------------------------------------
    -- Block ?? ??
    IF (@nBlock = 1)
    BEGIN
        IF (@BlockDate > GetDate())
        BEGIN
            SET @nReturn = 6
        END
        ELSE
        BEGIN
            UPDATE UserInfo 
            SET UserBlock=0 
            WHERE UserNum = @nUserNum
            
            SET @nReturn  = 2
        END 
    END    
    
    -----------------------------------------------------------------
    -- ??? ???? ??    
    IF (@nReturn = 1) OR (@nReturn = 2) OR (@nReturn = 3)
    BEGIN
        -- ???, ??? ????? ??
        UPDATE UserInfo
        SET UserLoginState=1, LastLoginDate=getdate(), SGNum=@SvrGrpNum, SvrNum=@SvrNum 
        WHERE UserNum = @nUserNum
        
        -- ???? ???
        INSERT INTO LogLogin (UserNum, UserID, LogInOut, LogIpAddress) 
        VALUES (@nUserNum, @userId, 1, @userIp)    
        
        -- ?? ????
        UPDATE StatLogin
        SET LCount = LCount+1 
        WHERE LYear=Year(GetDate()) AND LMonth=Month(GetDate()) AND LDay=Day(GetDate()) AND LHour=DatePart(hour, GetDate())
        
        SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
        IF @error_var <> 0 OR @rowcount_var = 0
        BEGIN
            INSERT INTO StatLogin (LYEAR) 
            VALUES (YEAR(GetDate()))
        END
    END

    SET NOCOUNT OFF    
    
    RETURN @nReturn  







GO
/****** Object:  StoredProcedure [dbo].[UserLogoutSimple]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[UserLogoutSimple]
	@szUserID varchar(20)
AS
	SET NOCOUNT ON
	
	UPDATE UserInfo
	SET UserLoginState=0 
	WHERE UserID=@szUserID

	---PCID
	DELETE FROM UserKey 
	WHERE PCIDUserID=@szUserID
	
	SET NOCOUNT OFF




GO
/****** Object:  StoredProcedure [dbo].[UserLogoutSimple2]    Script Date: 2025/6/21 下午 05:02:30 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[UserLogoutSimple2]
	@nUserNum int
AS
	SET NOCOUNT ON
	
	UPDATE UserInfo
	SET UserLoginState=0 
	WHERE UserNum=@nUserNum

	---PCID
	DELETE FROM UserKey 
	WHERE PCIDUserNum=@nUserNum
	
	SET NOCOUNT OFF




GO
USE [master]
GO
ALTER DATABASE [RanUser] SET  READ_WRITE 
GO
