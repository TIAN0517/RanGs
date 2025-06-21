USE [master]
GO
/****** Object:  Database [RanLog]    Script Date: 2025/6/21 下午 05:03:15 ******/
CREATE DATABASE [RanLog]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'RanLog', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL15.MSSQLSERVER\MSSQL\DATA\RanLog.mdf' , SIZE = 5120KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'RanLog_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL15.MSSQLSERVER\MSSQL\DATA\RanLog_log.ldf' , SIZE = 26816KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
 WITH CATALOG_COLLATION = DATABASE_DEFAULT
GO
ALTER DATABASE [RanLog] SET COMPATIBILITY_LEVEL = 110
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [RanLog].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [RanLog] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [RanLog] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [RanLog] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [RanLog] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [RanLog] SET ARITHABORT OFF 
GO
ALTER DATABASE [RanLog] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [RanLog] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [RanLog] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [RanLog] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [RanLog] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [RanLog] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [RanLog] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [RanLog] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [RanLog] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [RanLog] SET  DISABLE_BROKER 
GO
ALTER DATABASE [RanLog] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [RanLog] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [RanLog] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [RanLog] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [RanLog] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [RanLog] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [RanLog] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [RanLog] SET RECOVERY FULL 
GO
ALTER DATABASE [RanLog] SET  MULTI_USER 
GO
ALTER DATABASE [RanLog] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [RanLog] SET DB_CHAINING OFF 
GO
ALTER DATABASE [RanLog] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [RanLog] SET TARGET_RECOVERY_TIME = 0 SECONDS 
GO
ALTER DATABASE [RanLog] SET DELAYED_DURABILITY = DISABLED 
GO
ALTER DATABASE [RanLog] SET ACCELERATED_DATABASE_RECOVERY = OFF  
GO
EXEC sys.sp_db_vardecimal_storage_format N'RanLog', N'ON'
GO
ALTER DATABASE [RanLog] SET QUERY_STORE = OFF
GO
USE [RanLog]
GO
/****** Object:  Table [dbo].[HackProgramList]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[HackProgramList](
	[HackProgramNum] [int] NOT NULL,
	[HackProgramName] [varchar](100) NOT NULL,
 CONSTRAINT [PK_HackProgramList] PRIMARY KEY CLUSTERED 
(
	[HackProgramNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogHackProgram]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogHackProgram](
	[HackNum] [int] IDENTITY(1,1) NOT NULL,
	[UserNum] [int] NOT NULL,
	[ChaNum] [int] NOT NULL,
	[SGNum] [int] NOT NULL,
	[SvrNum] [int] NOT NULL,
	[HackProgramNum] [int] NOT NULL,
	[HackDate] [datetime] NOT NULL,
	[HackComment] [varchar](512) NOT NULL,
 CONSTRAINT [PK_LogHackProgram] PRIMARY KEY CLUSTERED 
(
	[HackNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  View [dbo].[viewLogHackProgram]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE VIEW [dbo].[viewLogHackProgram]
AS
SELECT   TOP 100 PERCENT dbo.LogHackProgram.HackNum, 
                dbo.LogHackProgram.UserNum, dbo.LogHackProgram.ChaNum, 
                dbo.LogHackProgram.SGNum, dbo.LogHackProgram.SvrNum, 
                dbo.LogHackProgram.HackProgramNum, dbo.LogHackProgram.HackDate, 
                dbo.LogHackProgram.HackComment, 
                dbo.HackProgramList.HackProgramName
FROM      dbo.LogHackProgram LEFT OUTER JOIN
                dbo.HackProgramList ON 
                dbo.LogHackProgram.HackProgramNum = dbo.HackProgramList.HackProgramNum




GO
/****** Object:  Table [dbo].[LogItemExchange]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogItemExchange](
	[ExchangeNum] [bigint] IDENTITY(1,1) NOT NULL,
	[NIDMain] [int] NULL,
	[NIDSub] [int] NULL,
	[SGNum] [int] NULL,
	[SvrNum] [int] NULL,
	[FldNum] [int] NULL,
	[MakeType] [int] NOT NULL,
	[MakeNum] [money] NOT NULL,
	[ItemAmount] [int] NOT NULL,
	[ItemFromFlag] [int] NOT NULL,
	[ItemFrom] [int] NOT NULL,
	[ItemToFlag] [int] NOT NULL,
	[ItemTo] [int] NOT NULL,
	[ExchangeFlag] [int] NOT NULL,
	[ExchangeDate] [datetime] NOT NULL,
	[Damage] [int] NOT NULL,
	[Defense] [int] NOT NULL,
	[Fire] [int] NOT NULL,
	[Ice] [int] NOT NULL,
	[Poison] [int] NOT NULL,
	[Electric] [int] NOT NULL,
	[Spirit] [int] NOT NULL,
	[CostumeMID] [int] NOT NULL,
	[CostumeSID] [int] NOT NULL,
	[TradePrice] [money] NOT NULL,
 CONSTRAINT [PK_LogItemExchange_200511] PRIMARY KEY NONCLUSTERED 
(
	[ExchangeNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  View [dbo].[viewLogItemExchangeMax]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO
-------------------------------------------------------------------------------
-- viewLogItemExchangeMax
CREATE VIEW [dbo].[viewLogItemExchangeMax]
AS
SELECT   TOP 100 PERCENT MAX(MakeNum) AS MaxNum, NIDMain, NIDSub, MakeType, 
                SGNum, SvrNum, FldNum
FROM      dbo.LogItemExchange
GROUP BY SGNum, SvrNum, FldNum, MakeType, NIDMain, NIDSub
ORDER BY SGNum, SvrNum, FldNum, MakeType, NIDMain, NIDSub, MaxNum




GO
/****** Object:  Table [dbo].[LogAction]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogAction](
	[ActionNum] [bigint] IDENTITY(1,1) NOT NULL,
	[ChaNum] [int] NOT NULL,
	[Type] [int] NOT NULL,
	[TargetNum] [int] NOT NULL,
	[TargetType] [int] NOT NULL,
	[BrightPoint] [int] NOT NULL,
	[LifePoint] [int] NOT NULL,
	[ExpPoint] [money] NOT NULL,
	[ActionMoney] [money] NOT NULL,
	[ActionDate] [datetime] NOT NULL,
 CONSTRAINT [PK_LogAction] PRIMARY KEY CLUSTERED 
(
	[ActionNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogExchangeFlag]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogExchangeFlag](
	[ExchangeFlag] [int] NOT NULL,
	[ExchangeName] [varchar](50) NULL,
 CONSTRAINT [PK_LogExchangeFlag] PRIMARY KEY CLUSTERED 
(
	[ExchangeFlag] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogItemMax]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogItemMax](
	[NIDMain] [int] NOT NULL,
	[NIDSub] [int] NOT NULL,
	[SGNum] [int] NOT NULL,
	[SvrNum] [int] NOT NULL,
	[FldNum] [int] NOT NULL,
	[MakeType] [int] NOT NULL,
	[MaxNum] [money] NULL,
 CONSTRAINT [PK_LogItemMax] PRIMARY KEY CLUSTERED 
(
	[NIDMain] ASC,
	[NIDSub] ASC,
	[SGNum] ASC,
	[SvrNum] ASC,
	[FldNum] ASC,
	[MakeType] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogItemRandom]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogItemRandom](
	[RandomNum] [bigint] IDENTITY(1,1) NOT NULL,
	[NIDMain] [smallint] NULL,
	[NIDSub] [smallint] NULL,
	[SGNum] [tinyint] NULL,
	[SvrNum] [tinyint] NULL,
	[FldNum] [tinyint] NULL,
	[MakeType] [tinyint] NOT NULL,
	[MakeNum] [money] NOT NULL,
	[RandomType1] [tinyint] NOT NULL,
	[RandomValue1] [smallint] NOT NULL,
	[RandomType2] [tinyint] NOT NULL,
	[RandomValue2] [smallint] NOT NULL,
	[RandomType3] [tinyint] NOT NULL,
	[RandomValue3] [smallint] NOT NULL,
	[RandomType4] [tinyint] NOT NULL,
	[RandomValue4] [smallint] NOT NULL,
	[RandomDate] [smalldatetime] NOT NULL,
 CONSTRAINT [PK_LogItemRandom] PRIMARY KEY CLUSTERED 
(
	[RandomNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogMakeType]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogMakeType](
	[MakeType] [int] NOT NULL,
	[MakeName] [varchar](20) NULL,
 CONSTRAINT [PK_LogMakeType] PRIMARY KEY CLUSTERED 
(
	[MakeType] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogPetAction]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogPetAction](
	[ActionNum] [bigint] IDENTITY(1,1) NOT NULL,
	[PetNum] [int] NOT NULL,
	[ItemMID] [smallint] NOT NULL,
	[ItemSID] [smallint] NOT NULL,
	[ActionType] [smallint] NOT NULL,
	[PetFull] [int] NOT NULL,
	[LogDate] [datetime] NOT NULL,
 CONSTRAINT [PK_LogPetAction] PRIMARY KEY CLUSTERED 
(
	[ActionNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogPetActionFlag]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogPetActionFlag](
	[PetActionFlag] [int] NOT NULL,
	[PetActionName] [varchar](50) NOT NULL,
 CONSTRAINT [PK_LogPetActinoFlag] PRIMARY KEY CLUSTERED 
(
	[PetActionFlag] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogServerState]    Script Date: 2025/6/21 下午 05:03:15 ******/
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
/****** Object:  Table [dbo].[LogVehicleAction]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogVehicleAction](
	[ActionNum] [bigint] IDENTITY(1,1) NOT NULL,
	[VehicleNum] [int] NOT NULL,
	[ItemMID] [smallint] NOT NULL,
	[ItemSID] [smallint] NOT NULL,
	[ActionType] [smallint] NOT NULL,
	[VehicleBattery] [int] NOT NULL,
	[LogDate] [datetime] NOT NULL,
 CONSTRAINT [PK_LogVehicleAction] PRIMARY KEY CLUSTERED 
(
	[ActionNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogVehicleActionFlag]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogVehicleActionFlag](
	[VehicleActionFlag] [int] NOT NULL,
	[VehicleActionName] [varchar](50) NOT NULL,
 CONSTRAINT [PK_LogVehicleActinoFlag] PRIMARY KEY CLUSTERED 
(
	[VehicleActionFlag] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[HackProgramList] ADD  CONSTRAINT [DF_HackProgramList_HackProgramName]  DEFAULT ('') FOR [HackProgramName]
GO
ALTER TABLE [dbo].[LogAction] ADD  CONSTRAINT [DF_LogAction_ChaNum]  DEFAULT ((0)) FOR [ChaNum]
GO
ALTER TABLE [dbo].[LogAction] ADD  CONSTRAINT [DF_LogAction_Type]  DEFAULT ((0)) FOR [Type]
GO
ALTER TABLE [dbo].[LogAction] ADD  CONSTRAINT [DF_LogAction_TargetNum]  DEFAULT ((0)) FOR [TargetNum]
GO
ALTER TABLE [dbo].[LogAction] ADD  CONSTRAINT [DF_LogAction_TargetType]  DEFAULT ((0)) FOR [TargetType]
GO
ALTER TABLE [dbo].[LogAction] ADD  CONSTRAINT [DF_LogAction_BrightPoint]  DEFAULT ((0)) FOR [BrightPoint]
GO
ALTER TABLE [dbo].[LogAction] ADD  CONSTRAINT [DF_LogAction_LifePoint]  DEFAULT ((0)) FOR [LifePoint]
GO
ALTER TABLE [dbo].[LogAction] ADD  CONSTRAINT [DF_LogAction_ExpPoint]  DEFAULT ((0)) FOR [ExpPoint]
GO
ALTER TABLE [dbo].[LogAction] ADD  CONSTRAINT [DF_LogAction_ActionMoney]  DEFAULT ((0)) FOR [ActionMoney]
GO
ALTER TABLE [dbo].[LogAction] ADD  CONSTRAINT [DF_LogAction_ActionDate]  DEFAULT (getdate()) FOR [ActionDate]
GO
ALTER TABLE [dbo].[LogHackProgram] ADD  CONSTRAINT [DF_LogHackProgram_UserNum]  DEFAULT ((0)) FOR [UserNum]
GO
ALTER TABLE [dbo].[LogHackProgram] ADD  CONSTRAINT [DF_LogHackProgram_ChaNum]  DEFAULT ((0)) FOR [ChaNum]
GO
ALTER TABLE [dbo].[LogHackProgram] ADD  CONSTRAINT [DF_LogHackProgram_SvrGrp]  DEFAULT ((0)) FOR [SGNum]
GO
ALTER TABLE [dbo].[LogHackProgram] ADD  CONSTRAINT [DF_LogHackProgram_SvrNum]  DEFAULT ((0)) FOR [SvrNum]
GO
ALTER TABLE [dbo].[LogHackProgram] ADD  CONSTRAINT [DF_LogHackProgram_HackProgramNum]  DEFAULT ((0)) FOR [HackProgramNum]
GO
ALTER TABLE [dbo].[LogHackProgram] ADD  CONSTRAINT [DF_LogHackProgram_HackDate]  DEFAULT (getdate()) FOR [HackDate]
GO
ALTER TABLE [dbo].[LogHackProgram] ADD  CONSTRAINT [DF__LogHackPr__HackC__6D0D32F4]  DEFAULT ('') FOR [HackComment]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_NIDMain_200511]  DEFAULT ((0)) FOR [NIDMain]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_NIDSub_200511]  DEFAULT ((0)) FOR [NIDSub]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_SGNum_200511]  DEFAULT ((0)) FOR [SGNum]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_SvrNum_200511]  DEFAULT ((0)) FOR [SvrNum]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_FldNum_200511]  DEFAULT ((0)) FOR [FldNum]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_MakeType_200511]  DEFAULT ((0)) FOR [MakeType]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_MakeNum_200511]  DEFAULT ((0)) FOR [MakeNum]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_ItemAmount_200511]  DEFAULT ((0)) FOR [ItemAmount]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_ItemFromFlag_200511]  DEFAULT ((0)) FOR [ItemFromFlag]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_ItemFrom_200511]  DEFAULT ((0)) FOR [ItemFrom]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_ItemToFlag_200511]  DEFAULT ((0)) FOR [ItemToFlag]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_ItemTo_200511]  DEFAULT ((0)) FOR [ItemTo]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_ExchangeFlag_200511]  DEFAULT ((0)) FOR [ExchangeFlag]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_ExchangeDate_200511]  DEFAULT (getdate()) FOR [ExchangeDate]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_Damage_200511]  DEFAULT ((0)) FOR [Damage]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_Defense_200511]  DEFAULT ((0)) FOR [Defense]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_Fire_200511]  DEFAULT ((0)) FOR [Fire]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_Ice_200511]  DEFAULT ((0)) FOR [Ice]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_Poison_200511]  DEFAULT ((0)) FOR [Poison]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_Electric_200511]  DEFAULT ((0)) FOR [Electric]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_Spirit_200511]  DEFAULT ((0)) FOR [Spirit]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_CostumeMID_200511]  DEFAULT ((0)) FOR [CostumeMID]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_CostumeSID_200511]  DEFAULT ((0)) FOR [CostumeSID]
GO
ALTER TABLE [dbo].[LogItemExchange] ADD  CONSTRAINT [DF_LogItemExchange_TradePrice_200511]  DEFAULT ((0)) FOR [TradePrice]
GO
ALTER TABLE [dbo].[LogItemMax] ADD  CONSTRAINT [DF_LogItemMax_NIDMain]  DEFAULT ((0)) FOR [NIDMain]
GO
ALTER TABLE [dbo].[LogItemMax] ADD  CONSTRAINT [DF_LogItemMax_NIDSub]  DEFAULT ((0)) FOR [NIDSub]
GO
ALTER TABLE [dbo].[LogItemMax] ADD  CONSTRAINT [DF_LogItemMax_SGNum]  DEFAULT ((0)) FOR [SGNum]
GO
ALTER TABLE [dbo].[LogItemMax] ADD  CONSTRAINT [DF_LogItemMax_SvrNum]  DEFAULT ((0)) FOR [SvrNum]
GO
ALTER TABLE [dbo].[LogItemMax] ADD  CONSTRAINT [DF_LogItemMax_FldNum]  DEFAULT ((0)) FOR [FldNum]
GO
ALTER TABLE [dbo].[LogItemMax] ADD  CONSTRAINT [DF_LogItemMax_MakeType]  DEFAULT ((0)) FOR [MakeType]
GO
ALTER TABLE [dbo].[LogItemMax] ADD  CONSTRAINT [DF_LogItemMax_MaxNum]  DEFAULT ((0)) FOR [MaxNum]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_NIDMain]  DEFAULT ((0)) FOR [NIDMain]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_NIDSub]  DEFAULT ((0)) FOR [NIDSub]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_SGNum]  DEFAULT ((0)) FOR [SGNum]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_SvrNum]  DEFAULT ((0)) FOR [SvrNum]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_FldNum]  DEFAULT ((0)) FOR [FldNum]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_MakeType]  DEFAULT ((0)) FOR [MakeType]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_MakeNum]  DEFAULT ((0)) FOR [MakeNum]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_RandomType1]  DEFAULT ((0)) FOR [RandomType1]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_RandomValue1]  DEFAULT ((0)) FOR [RandomValue1]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_RandomType2]  DEFAULT ((0)) FOR [RandomType2]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_RandomValue2]  DEFAULT ((0)) FOR [RandomValue2]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_RandomType3]  DEFAULT ((0)) FOR [RandomType3]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_RandomValue3]  DEFAULT ((0)) FOR [RandomValue3]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_RandomType4]  DEFAULT ((0)) FOR [RandomType4]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_RandomValue4]  DEFAULT ((0)) FOR [RandomValue4]
GO
ALTER TABLE [dbo].[LogItemRandom] ADD  CONSTRAINT [DF_LogItemRandom_RandomDate]  DEFAULT (getdate()) FOR [RandomDate]
GO
ALTER TABLE [dbo].[LogPetAction] ADD  CONSTRAINT [DF_LogPetAction_PetNum]  DEFAULT ((0)) FOR [PetNum]
GO
ALTER TABLE [dbo].[LogPetAction] ADD  CONSTRAINT [DF_LogPetAction_ItemMID]  DEFAULT ((0)) FOR [ItemMID]
GO
ALTER TABLE [dbo].[LogPetAction] ADD  CONSTRAINT [DF_LogPetAction_ItemSID]  DEFAULT ((0)) FOR [ItemSID]
GO
ALTER TABLE [dbo].[LogPetAction] ADD  CONSTRAINT [DF_LogPetAction_ActionType]  DEFAULT ((0)) FOR [ActionType]
GO
ALTER TABLE [dbo].[LogPetAction] ADD  CONSTRAINT [DF_LogPetAction_PetFull]  DEFAULT ((0)) FOR [PetFull]
GO
ALTER TABLE [dbo].[LogPetAction] ADD  CONSTRAINT [DF_LogPetAction_LogDate]  DEFAULT (getdate()) FOR [LogDate]
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
ALTER TABLE [dbo].[LogVehicleAction] ADD  CONSTRAINT [DF_LogVehicleAction_VehicleNum]  DEFAULT ((0)) FOR [VehicleNum]
GO
ALTER TABLE [dbo].[LogVehicleAction] ADD  CONSTRAINT [DF_LogVehicleAction_ItemMID]  DEFAULT ((0)) FOR [ItemMID]
GO
ALTER TABLE [dbo].[LogVehicleAction] ADD  CONSTRAINT [DF_LogVehicleAction_ItemSID]  DEFAULT ((0)) FOR [ItemSID]
GO
ALTER TABLE [dbo].[LogVehicleAction] ADD  CONSTRAINT [DF_LogVehicleAction_ActionType]  DEFAULT ((0)) FOR [ActionType]
GO
ALTER TABLE [dbo].[LogVehicleAction] ADD  CONSTRAINT [DF_LogVehicleAction_VehicleBattery]  DEFAULT ((0)) FOR [VehicleBattery]
GO
ALTER TABLE [dbo].[LogVehicleAction] ADD  CONSTRAINT [DF_LogVehicleAction_LogDate]  DEFAULT (getdate()) FOR [LogDate]
GO
/****** Object:  StoredProcedure [dbo].[InsertLogHackProgram]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[InsertLogHackProgram]
	@nSGNum int,
	@nSvrNum int,
	@nUserNum int,
	@nChaNum int,
	@nHackProgramNum int,
	@strComment varchar (512),
	@nReturn int OUTPUT
AS	
	DECLARE
		@error_var int, 
		@rowcount_var int
		
	SET NOCOUNT ON
	
	SET @nReturn = 0

    INSERT INTO LogHackProgram (SGNum,   SvrNum,   UserNum,   ChaNum,   HackProgramNum,   HackComment) 
    VALUES (@nSGNum, @nSvrNum, @nUserNum, @nChaNum, @nHackProgramNum, @strComment)
    
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
/****** Object:  StoredProcedure [dbo].[log_serverstate]    Script Date: 2025/6/21 下午 05:03:15 ******/
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
/****** Object:  StoredProcedure [dbo].[sp_LogAction_Insert]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_LogAction_Insert]
	@nChaNum int,
	@nType int,
    @nTargetNum int,
    @nTargetType int,
    @nExpPoint money,
    @nBrightPoint int,
    @nLifePoint int,
    @nMoney money
AS	
	SET NOCOUNT ON

	INSERT INTO LogAction (ChaNum,   Type,   TargetNum,   TargetType,   ExpPoint,   BrightPoint,   LifePoint,   ActionMoney) 
    VALUES    (@nChaNum, @nType, @nTargetNum, @nTargetType, @nExpPoint, @nBrightPoint, @nLifePoint, @nMoney)

	SET NOCOUNT OFF




GO
/****** Object:  StoredProcedure [dbo].[sp_logitemexchange_insert]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[sp_logitemexchange_insert]
    @NIDMain int,
	@NIDSub int,
	@SGNum int,
	@SvrNum int,
	@FldNum int,
	
	@MakeType int,
	@MakeNum money,
	@ItemAmount int,
	@ItemFromFlag int,
	@ItemFrom int,
	
	@ItemToFlag int,
	@ItemTo int,
	@ExchangeFlag int,
	@Damage int,
	@Defense int,
	
	@Fire int,
	@Ice int,
	@Poison int,
	@Electric int,
	@Spirit int,
	
	@CostumeMID int,
	@CostumeSID int,
	@TradePrice money,    
	@nReturn int OUTPUT
AS
	DECLARE 	
		@error_var int -- Declare variables used in error checking.

	SET NOCOUNT ON

	INSERT INTO LogItemExchange (NIDMain, NIDSub, SGNum, SvrNum, FldNum,
	MakeType, MakeNum, ItemAmount, ItemFromFlag, ItemFrom, 
	ItemToFlag, ItemTo, ExchangeFlag, Damage, Defense,
	Fire, Ice, Poison, Electric, Spirit,
	CostumeMID, CostumeSID, TradePrice) VALUES 
	(@NIDMain, @NIDSub, @SGNum, @SvrNum, @FldNum,
	@MakeType, @MakeNum, @ItemAmount, @ItemFromFlag, @ItemFrom,
	@ItemToFlag, @ItemTo, @ExchangeFlag, @Damage, @Defense,
	@Fire, @Ice, @Poison, @Electric, @Spirit,
	@CostumeMID, @CostumeSID, @TradePrice)

	SELECT @error_var = @@ERROR
	IF @error_var <> 0 
	BEGIN
	    -- 火涝角菩
	    SET @nReturn = 0	    
	END
        ELSE
        BEGIN
	    -- 沥惑利栏肺 火涝 己傍
	    SET @nReturn = 1
        END

	SET NOCOUNT OFF

	RETURN @nReturn




GO
/****** Object:  StoredProcedure [dbo].[sp_LogItemRandom_Insert]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- Return
-- DB_ERROR -1
-- DB_OK 0

CREATE Procedure [dbo].[sp_LogItemRandom_Insert]
	@NIDMain int,
	@NIDSub int,
	@SGNum int,
	@SvrNum int,
	@FldNum int,
	@MakeType int,
	@MakeNum money,
	@RandomType1 int, 
	@RandomValue1 int, 
	@RandomType2 int, 
	@RandomValue2 int, 
	@RandomType3 int, 
	@RandomValue3 int, 
	@RandomType4 int, 
	@RandomValue4 int,
	@nReturn int OUTPUT
AS
	DECLARE
		@error_var int, 
		@rowcount_var int
	SET NOCOUNT ON

	SET @nReturn = 0

	BEGIN TRAN

	Insert Into LogItemRandom (
		NIDMain, NIDSub, SGNum, SvrNum, FldNum, MakeType, MakeNum
		, RandomType1, RandomValue1, RandomType2, RandomValue2
		, RandomType3, RandomValue3, RandomType4, RandomValue4)
	Values (
		@NIDMain, @NIDSub, @SGNum, @SvrNum, @FldNum, @MakeType, @MakeNum
		, @RandomType1, @RandomValue1, @RandomType2, @RandomValue2
		, @RandomType3, @RandomValue3, @RandomType4, @RandomValue4
	)

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
/****** Object:  StoredProcedure [dbo].[sp_LogPetAction_Insert]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- Return
-- DB_ERROR -1
-- DB_OK 0

CREATE Procedure [dbo].[sp_LogPetAction_Insert]
	@PetNum int,
	@ItemMID int,
	@ItemSID int,
	@ActionType int,	
	@PetFull int,
	@nReturn int OUTPUT
AS
	DECLARE
	@error_var int,
	@rowcount_var int
	SET NOCOUNT ON

	SET @nReturn=0

	BEGIN TRAN

	Insert Into LogPetAction( PetNum, ItemMID, ItemSID, ActionType, PetFull )
	Values( @PetNum, @ItemMID, @ItemSID, @ActionType, @PetFull )

	SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
	IF @error_var<>0 OR @rowcount_var=0
	BEGIN
		ROLLBACK TRAN
		SET @nReturn=-1
	END
	ELSE
	BEGIN
		COMMIT TRAN
		SET @nReturn=0
	END

	SET NOCOUNT OFF

	RETURN @nReturn




GO
/****** Object:  StoredProcedure [dbo].[sp_LogVehicleAction_Insert]    Script Date: 2025/6/21 下午 05:03:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- Return
-- DB_ERROR -1
-- DB_OK 0

CREATE Procedure [dbo].[sp_LogVehicleAction_Insert]
	@VehicleNum int,
	@ItemMID int,
	@ItemSID int,
	@ActionType int,	
	@VehicleBattery int,
	@nReturn int OUTPUT
AS
	DECLARE
	@error_var int,
	@rowcount_var int
	SET NOCOUNT ON

	SET @nReturn=0

	BEGIN TRAN

	Insert Into LogVehicleAction( VehicleNum, ItemMID, ItemSID, ActionType, VehicleBattery )
	Values( @VehicleNum, @ItemMID, @ItemSID, @ActionType, @VehicleBattery )

	SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
	IF @error_var<>0 OR @rowcount_var=0
	BEGIN
		ROLLBACK TRAN
		SET @nReturn=-1
	END
	ELSE
	BEGIN
		COMMIT TRAN
		SET @nReturn=0
	END

	SET NOCOUNT OFF

	RETURN @nReturn




GO
USE [master]
GO
ALTER DATABASE [RanLog] SET  READ_WRITE 
GO
