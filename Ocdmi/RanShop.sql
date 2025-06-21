USE [master]
GO
/****** Object:  Database [RanShop]    Script Date: 2025/6/21 下午 05:01:54 ******/
CREATE DATABASE [RanShop]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'RanShop', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL15.MSSQLSERVER\MSSQL\DATA\RanShop.mdf' , SIZE = 7168KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'RanShop_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL15.MSSQLSERVER\MSSQL\DATA\RanShop_log.ldf' , SIZE = 57664KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
 WITH CATALOG_COLLATION = DATABASE_DEFAULT
GO
ALTER DATABASE [RanShop] SET COMPATIBILITY_LEVEL = 110
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [RanShop].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [RanShop] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [RanShop] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [RanShop] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [RanShop] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [RanShop] SET ARITHABORT OFF 
GO
ALTER DATABASE [RanShop] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [RanShop] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [RanShop] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [RanShop] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [RanShop] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [RanShop] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [RanShop] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [RanShop] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [RanShop] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [RanShop] SET  DISABLE_BROKER 
GO
ALTER DATABASE [RanShop] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [RanShop] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [RanShop] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [RanShop] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [RanShop] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [RanShop] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [RanShop] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [RanShop] SET RECOVERY FULL 
GO
ALTER DATABASE [RanShop] SET  MULTI_USER 
GO
ALTER DATABASE [RanShop] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [RanShop] SET DB_CHAINING OFF 
GO
ALTER DATABASE [RanShop] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [RanShop] SET TARGET_RECOVERY_TIME = 0 SECONDS 
GO
ALTER DATABASE [RanShop] SET DELAYED_DURABILITY = DISABLED 
GO
ALTER DATABASE [RanShop] SET ACCELERATED_DATABASE_RECOVERY = OFF  
GO
EXEC sys.sp_db_vardecimal_storage_format N'RanShop', N'ON'
GO
ALTER DATABASE [RanShop] SET QUERY_STORE = OFF
GO
USE [RanShop]
GO
/****** Object:  Table [dbo].[ShopPurchase]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ShopPurchase](
	[PurKey] [bigint] IDENTITY(1,1) NOT NULL,
	[UserUID] [varchar](20) NOT NULL,
	[ProductNum] [int] NOT NULL,
	[PurPrice] [int] NOT NULL,
	[PurFlag] [int] NOT NULL,
	[PurDate] [datetime] NOT NULL,
	[PurChgDate] [datetime] NULL,
	[ItemMain] [int] NULL,
	[ItemSub] [int] NULL,
	[GiftedBy] [varchar](20) NULL,
 CONSTRAINT [PK_ShopPurchase] PRIMARY KEY CLUSTERED 
(
	[PurKey] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ShopPurFlag]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ShopPurFlag](
	[PurFlag] [int] NOT NULL,
	[PurFlagName] [varchar](20) NULL,
 CONSTRAINT [PK_ShopPurFlag] PRIMARY KEY CLUSTERED 
(
	[PurFlag] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ShopItemMap]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ShopItemMap](
	[ProductNum] [int] IDENTITY(1,1) NOT NULL,
	[ItemMain] [int] NULL,
	[ItemSub] [int] NULL,
	[ItemName] [varchar](100) NULL,
	[ItemStock] [int] NOT NULL,
	[ItemPrice] [int] NULL,
	[ItemSection] [int] NULL,
	[ItemCurrency] [int] NULL,
	[ItemDiscount] [int] NOT NULL,
	[ItemList] [smallint] NULL,
	[ItemOrder] [int] NOT NULL,
	[ItemVisible] [int] NOT NULL,
	[Duration] [varchar](50) NULL,
	[Category] [varchar](50) NULL,
	[ItemImage] [varchar](300) NULL,
	[ItemMoney] [int] NULL,
	[ItemComment] [varchar](50) NULL,
 CONSTRAINT [PK_ShopItemMap] PRIMARY KEY CLUSTERED 
(
	[ProductNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  View [dbo].[viewShopPurchase]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-------------------------------------------------------------------------------
-- viewShopPurchase
-------------------------------------------------------------------------------

CREATE VIEW [dbo].[viewShopPurchase]
AS
SELECT   dbo.ShopPurchase.PurKey, dbo.ShopPurchase.UserUID, 
         dbo.ShopPurchase.ProductNum, dbo.ShopPurchase.PurPrice, 
         dbo.ShopPurchase.PurFlag, dbo.ShopPurchase.PurDate, 
         dbo.ShopPurchase.PurChgDate, dbo.ShopItemMap.ItemMain, 
         dbo.ShopItemMap.ItemSub, dbo.ShopItemMap.ItemName, 
         dbo.ShopPurFlag.PurFlagName
FROM     dbo.ShopPurchase INNER JOIN
         dbo.ShopItemMap ON 
         dbo.ShopPurchase.ProductNum = dbo.ShopItemMap.ProductNum LEFT OUTER JOIN
         dbo.ShopPurFlag ON dbo.ShopPurchase.PurFlag = dbo.ShopPurFlag.PurFlag




GO
/****** Object:  View [dbo].[viewShopPurchaseItem]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-------------------------------------------------------------------------------
-- view_ShopPurchaseItem.sql
-------------------------------------------------------------------------------

CREATE VIEW [dbo].[viewShopPurchaseItem]
AS
SELECT TOP 100 PERCENT SUM(A.PurPrice) AS tPrice, A.ItemMain, A.ItemSub, B.ItemName
FROM   dbo.viewShopPurchase A INNER JOIN
       dbo.ShopItemMap B ON A.ProductNum = B.ProductNum
GROUP BY A.ItemMain, A.ItemSub, B.ItemName
ORDER BY A.ItemMain, A.ItemSub




GO
/****** Object:  Table [dbo].[ItemMallLogBuy]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ItemMallLogBuy](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[UserID] [varchar](50) NULL,
	[ProductNum] [varchar](50) NULL,
	[PremPoints] [varchar](50) NULL,
	[AfterPremPoints] [varchar](50) NULL,
	[VotePoints] [varchar](50) NULL,
	[AfterVotePoints] [varchar](50) NULL,
	[DateBought] [datetime] NULL,
	[GiftedTo] [varchar](50) NULL,
	[ItemDiscount] [int] NULL,
	[CurrencyType] [int] NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[LogShopPurchase]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[LogShopPurchase](
	[PurNum] [bigint] IDENTITY(1,1) NOT NULL,
	[PurKey] [varchar](21) NOT NULL,
	[PurFlag] [int] NULL,
	[PurDate] [datetime] NULL,
 CONSTRAINT [PK_LogShopPurchase] PRIMARY KEY CLUSTERED 
(
	[PurNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ShopPurchaseHackLogs]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ShopPurchaseHackLogs](
	[UserID] [varchar](20) NOT NULL,
	[UserNum] [bigint] NOT NULL,
	[PurType] [int] NOT NULL,
	[ProductNum] [bigint] NOT NULL,
	[PurchaseDate] [datetime] NOT NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ShopPurchaseLogs]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ShopPurchaseLogs](
	[UserID] [varchar](20) NOT NULL,
	[UserNum] [bigint] NOT NULL,
	[PurType] [int] NOT NULL,
	[Points] [bigint] NOT NULL,
	[PointsNew] [bigint] NOT NULL,
	[ProductNum] [bigint] NOT NULL,
	[ProductName] [varchar](25) NOT NULL,
	[ProductPrice] [bigint] NOT NULL,
	[PurchaseDate] [datetime] NOT NULL
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[LogShopPurchase] ADD  CONSTRAINT [DF_LogShopPurchase_PurDate]  DEFAULT (getdate()) FOR [PurDate]
GO
ALTER TABLE [dbo].[ShopItemMap] ADD  CONSTRAINT [DF__ShopItemM__ItemS__38996AB5]  DEFAULT ((0)) FOR [ItemStock]
GO
ALTER TABLE [dbo].[ShopItemMap] ADD  CONSTRAINT [DF_ShopItemMap_ItemDiscount]  DEFAULT ((0)) FOR [ItemDiscount]
GO
ALTER TABLE [dbo].[ShopItemMap] ADD  CONSTRAINT [DF_ShopItemMap_ItemList]  DEFAULT ((1)) FOR [ItemList]
GO
ALTER TABLE [dbo].[ShopItemMap] ADD  CONSTRAINT [DF_ShopItemMap_ItemOrder]  DEFAULT ((99)) FOR [ItemOrder]
GO
ALTER TABLE [dbo].[ShopItemMap] ADD  CONSTRAINT [DF_ShopItemMap_ItemVisible]  DEFAULT ((1)) FOR [ItemVisible]
GO
ALTER TABLE [dbo].[ShopPurchase] ADD  CONSTRAINT [DF_ShopPurchase_PurPrice]  DEFAULT ((0)) FOR [PurPrice]
GO
ALTER TABLE [dbo].[ShopPurchase] ADD  CONSTRAINT [DF_ShopPurchase_PurFlag]  DEFAULT ((0)) FOR [PurFlag]
GO
ALTER TABLE [dbo].[ShopPurchase] ADD  CONSTRAINT [DF_ShopPurchase_PurDate]  DEFAULT (getdate()) FOR [PurDate]
GO
ALTER TABLE [dbo].[ShopPurchaseHackLogs] ADD  CONSTRAINT [DF_ShopPurchaseHackLogs_UserID]  DEFAULT ('') FOR [UserID]
GO
ALTER TABLE [dbo].[ShopPurchaseHackLogs] ADD  CONSTRAINT [DF_ShopPurchaseHackLogs_UserNum]  DEFAULT ((0)) FOR [UserNum]
GO
ALTER TABLE [dbo].[ShopPurchaseHackLogs] ADD  CONSTRAINT [DF_ShopPurchaseHackLogs_ProductNum]  DEFAULT ((0)) FOR [ProductNum]
GO
ALTER TABLE [dbo].[ShopPurchaseHackLogs] ADD  CONSTRAINT [DF_ShopPurchaseHackLogs_PurchaseDate]  DEFAULT (getdate()) FOR [PurchaseDate]
GO
ALTER TABLE [dbo].[ShopPurchaseLogs] ADD  CONSTRAINT [DF_ShopPurchaseLogs_UserID]  DEFAULT ('') FOR [UserID]
GO
ALTER TABLE [dbo].[ShopPurchaseLogs] ADD  CONSTRAINT [DF_ShopPurchaseLogs_UserNum]  DEFAULT ((0)) FOR [UserNum]
GO
ALTER TABLE [dbo].[ShopPurchaseLogs] ADD  CONSTRAINT [DF_ShopPurchaseLogs_PurType]  DEFAULT ((0)) FOR [PurType]
GO
ALTER TABLE [dbo].[ShopPurchaseLogs] ADD  CONSTRAINT [DF_Table_1_UserPoint]  DEFAULT ((0)) FOR [Points]
GO
ALTER TABLE [dbo].[ShopPurchaseLogs] ADD  CONSTRAINT [DF_ShopPurchaseLogs_PointsNew]  DEFAULT ((0)) FOR [PointsNew]
GO
ALTER TABLE [dbo].[ShopPurchaseLogs] ADD  CONSTRAINT [DF_ShopPurchaseLogs_ProductNum]  DEFAULT ((0)) FOR [ProductNum]
GO
ALTER TABLE [dbo].[ShopPurchaseLogs] ADD  CONSTRAINT [DF_ShopPurchaseLogs_ProductName]  DEFAULT ('') FOR [ProductName]
GO
ALTER TABLE [dbo].[ShopPurchaseLogs] ADD  CONSTRAINT [DF_ShopPurchaseLogs_PurchaseDate]  DEFAULT (getdate()) FOR [PurchaseDate]
GO
/****** Object:  StoredProcedure [dbo].[sp_InsertItem]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_InsertItem]
	@PurKey	varchar(100),
	@UserId varchar(50),
	@MID int,
	@SID int,
	@nReturn int OUTPUT
AS	
	DECLARE
		@ProductNum int;

	SET NOCOUNT ON;
	
	SET @nReturn = -1;
	
	SELECT @ProductNum = ShopItemMap.ProductNum 
	FROM ShopItemMap
	WHERE ItemMain = @MID and ItemSub = @SID;

	IF @@ERROR <> 0 OR @@ROWCOUNT = 0
		BEGIN
			--item not found
			SET @nReturn = -1;
			SET NOCOUNT OFF
			RETURN @nReturn	
		END
	ELSE
		BEGIN
			INSERT INTO ShopPurchase ( UserUID, ProductNum, PurFlag, PurDate )
			VALUES( @UserId, @ProductNum, 0, GETDATE())
			IF @@ERROR = 0
			BEGIN 
				SET @nReturn = 0;
			END
		END
		
	SET NOCOUNT OFF;
	
	RETURN @nReturn




GO
/****** Object:  StoredProcedure [dbo].[sp_ItemMallBuy]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[sp_ItemMallBuy]
	@nProductNum	int,
	@nPrice			int,
	@nType			int,
	@UserID			varchar(25),
	@bGift			int,
	@GiftUserId		varchar(25),
	@nReturn		int OUTPUT
AS	
	DECLARE
		@UserName varchar(50),
		@CurrencyType int,
		@nUserNum int,
		@ItemPrice int,
		@EPoint int,
		@GPoint int,
		@nUserID varchar(25),
		@nItemDiscount int,
		@DiscountedPrice int

	SET NOCOUNT ON;

	SET @nReturn = -1

	SELECT @nUserNum = UserNum , @UserName = UserName , @EPoint = UserPoint
	From RanUser.dbo.UserInfo 
	where UserName = @UserID

	IF  @nUserNum != 0 
	BEGIN
		Select @nUserID=UserID from RanGame1.[dbo].ChaInfo tab1
		join RanUser.[dbo].UserInfo tab2 on tab1.UserNum = tab2.UserNum WHERE tab1.ChaNum=@GiftUserId

		SELECT @CurrencyType=ItemCurrency , @ItemPrice=ItemPrice, @nItemDiscount=ItemDiscount
		FROM ShopItemMap 
		WHERE ProductNum=@nProductNum


		IF @CurrencyType = 0 
		BEGIN
			IF @EPoint !< 0
			BEGIN
				IF @nItemDiscount > 0
				BEGIN
					IF @EPoint >= @ItemPrice - ((@ItemPrice*@nItemDiscount)/100)
					BEGIN
						UPDATE RanUser.dbo.UserInfo SET UserPoint = UserPoint - (@ItemPrice - (@ItemPrice*@nItemDiscount)/100) Where UserID = @UserId
						UPDATE ShopItemMap SET Itemstock = Itemstock - 1 WHERE ProductNum=@nProductNum
						
						IF @bGift = 1
						BEGIN
							INSERT INTO ShopPurchase(
								UserUID,
								ProductNum,
								PurFlag,
								PurDate,
								PurChgDate,
								GiftedBy
							) VALUES (
								@nUserID,
								@nProductNum,
								0,
								GETDATE(),
								GETDATE(),
								@UserName
							)

							INSERT INTO ItemMallLogBuy(
								UserID,
								ProductNum,
								PremPoints,
								AfterPremPoints,
								VotePoints,
								AfterVotePoints,
								DateBought,
								GiftedTo,
								ItemDiscount,
								CurrencyType
							) VALUES (
								@UserName,
								@nProductNum,
								@EPoint,
								@EPoint - @ItemPrice - ((@ItemPrice*@nItemDiscount)/100),
								@GPoint,
								@GPoint,
								GETDATE(),
								@nUserID,
								@nItemDiscount,
								@CurrencyType

							)
						END
						ELSE
						BEGIN
							INSERT INTO ShopPurchase(
								UserUID,
								ProductNum,
								PurFlag,
								PurDate,
								PurChgDate
							) VALUES (
								@UserName,
								@nProductNum,
								0,
								GETDATE(),
								GETDATE()
							)

							INSERT INTO ItemMallLogBuy(
								UserID,
								ProductNum,
								PremPoints,
								AfterPremPoints,
								VotePoints,
								AfterVotePoints,
								DateBought,
								CurrencyType
							) VALUES (
								@UserName,
								@nProductNum,
								@EPoint,
								@EPoint - @ItemPrice,
								@GPoint,
								@GPoint,
								GETDATE(),
								@CurrencyType
							)
						END

						SET @nReturn = 1;
						RETURN @nReturn;
					END
					ELSE 
					BEGIN
						SET @nReturn = -1;
						RETURN @nReturn;
					END
				END
				ELSE
				BEGIN
					IF @EPoint >= @ItemPrice
					BEGIN
						UPDATE RanUser.dbo.UserInfo SET UserPoint = UserPoint - @ItemPrice Where UserID = @UserId
						UPDATE ShopItemMap SET Itemstock = Itemstock - 1 WHERE ProductNum=@nProductNum

						IF @bGift = 1
						BEGIN
							INSERT INTO ShopPurchase(
								UserUID,
								ProductNum,
								PurFlag,
								PurDate,
								PurChgDate,
								GiftedBy
							) VALUES (
								@nUserID,
								@nProductNum,
								0,
								GETDATE(),
								GETDATE(),
								@UserName
							)

							INSERT INTO ItemMallLogBuy(
								UserID,
								ProductNum,
								PremPoints,
								AfterPremPoints,
								VotePoints,
								AfterVotePoints,
								DateBought,
								GiftedTo,
								ItemDiscount,
								CurrencyType
							) VALUES (
								@UserName,
								@nProductNum,
								@EPoint,
								@EPoint-@ItemPrice,
								@GPoint,
								@GPoint,
								GETDATE(),
								@nUserID,
								@nItemDiscount,
								@CurrencyType

							)
						END
						ELSE
						BEGIN
							INSERT INTO ShopPurchase(
								UserUID,
								ProductNum,
								PurFlag,
								PurDate,
								PurChgDate
							) VALUES (
								@UserName,
								@nProductNum,
								0,
								GETDATE(),
								GETDATE()
							)

							INSERT INTO ItemMallLogBuy(
								UserID,
								ProductNum,
								PremPoints,
								AfterPremPoints,
								VotePoints,
								AfterVotePoints,
								DateBought,
								CurrencyType
							) VALUES (
								@UserName,
								@nProductNum,
								@EPoint,
								@EPoint-@ItemPrice,
								@GPoint,
								@GPoint,
								GETDATE(),
								@CurrencyType
							)
						END
						
						SET @nReturn = 1;
						RETURN @nReturn;
					END
					ELSE 
					BEGIN
						SET @nReturn = 0;
						RETURN @nReturn;
					END
				END
			END
		END
		ELSE
		IF @CurrencyType = 1 
		BEGIN
			if @bGift = 1 
				begin
					SET @nReturn = -1;
				RETURN @nReturn;
			end
				UPDATE ShopItemMap SET Itemstock = Itemstock - 1 WHERE ProductNum=@nProductNum	
					
				INSERT INTO ShopPurchase(
					UserUID,
					ProductNum,
					PurFlag,
					PurDate,
					PurChgDate
				) VALUES (
					@UserName,
					@nProductNum,
					0,
					GETDATE(),
					GETDATE()
				)

				INSERT INTO ItemMallLogBuy(
					UserID,
					ProductNum,
					PremPoints,
					AfterPremPoints,
					VotePoints,
					AfterVotePoints,
					DateBought,
					CurrencyType
				) VALUES (
					@UserName,
					@nProductNum,
					@EPoint,
					@EPoint,
					@GPoint,
					@GPoint,
					GETDATE(),
					@CurrencyType
				)

				SET @nReturn = 1;
				RETURN @nReturn;
		END
		ELSE 
		BEGIN
			SET @nReturn = 0;
			RETURN @nReturn;
		END
	END
	ELSE BEGIN
		SET @nReturn = 0;
		RETURN @nReturn;
	END

	SET NOCOUNT OFF;

	RETURN @nReturn







GO
/****** Object:  StoredProcedure [dbo].[sp_purchase_change_state]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_purchase_change_state]
	@purkey varchar(22),
    @purflag int,
	@nReturn int OUTPUT
AS
	DECLARE 	
		@error_var int, -- Declare variables used in error checking.
		@rowcount_var int,
		@nFlag int

	SET NOCOUNT ON
	
	SET @nFlag = 0
	
	SELECT @nFlag=PurFlag 
	FROM ShopPurchase 
	WHERE PurKey=@purkey
	
	IF @error_var <> 0 OR @rowcount_var = 0
	BEGIN
	    SET @nReturn = 0
	    SET NOCOUNT OFF
	    RETURN @nReturn	
	END
	
	-- 官操妨绰 内靛客 泅犁 内靛啊 鞍篮 版快 俊矾
	IF @nFlag = @purflag
	BEGIN
	    SET @nReturn = 0
	    SET NOCOUNT OFF
	    RETURN @nReturn	
	END
	
	-- 沥犬窍霸 官曹荐 乐绰 版快
    UPDATE ShopPurchase
    SET PurFlag=@purflag, PurChgDate=getdate() 
    WHERE PurKey=@purkey

	SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
	IF @error_var <> 0 OR @rowcount_var = 0
	BEGIN
	    -- 角菩
	    SET @nReturn = 0
	END
    ELSE
    BEGIN
	    -- 己傍
	    INSERT INTO LogShopPurchase (PurKey, PurFlag) 
	    VALUES (@purkey, @purflag)
	    SET @nReturn = 1
    END

	SET NOCOUNT OFF

	RETURN @nReturn




GO
/****** Object:  StoredProcedure [dbo].[sp_purchase_insert_item]    Script Date: 2025/6/21 下午 05:01:54 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_purchase_insert_item]
	@purkey varchar(22),
	@useruid varchar(30),
    @productnum int,
    @purprice int,
	@nReturn int OUTPUT
AS
	DECLARE 	
		@error_var int -- Declare variables used in error checking.

	SET NOCOUNT ON

	INSERT INTO ShopPurchase (PurKey, UserUID, ProductNum, PurPrice) 
	VALUES (@purkey, @useruid, @productnum, @purprice)

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
USE [master]
GO
ALTER DATABASE [RanShop] SET  READ_WRITE 
GO
