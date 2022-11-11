
#ifndef __SBE_SSZ_H__
#define __SBE_SSZ_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "sbe_stock.h"

#ifdef WIN32
#define PACKED
#pragma pack(push,1)
#else
#define PACKED __attribute__ ((__packed__))
#endif

#define __SecurityIDSource_SSZ_                 (102)   //深交所

#ifndef __MsgType_HEARTBEAT__
#define __MsgType_HEARTBEAT__                   (1)     //心跳消息
#endif

#define __MsgType_SSZ_INSTRUMENT_SNAP__         (111)   //个股快照、可转债快照
#define __MsgType_SSZ_INDEX_SNAP__              (11)    //指数快照
#define __MsgType_SSZ_ORDER__                   (192)   //个股逐笔委托、可转债逐笔委托
#define __MsgType_SSZ_EXECUTION__               (191)   //个股逐笔成交、可转债逐笔成交
#define __MsgType_SSZ_OPTION_SNAP__             (13)    //期权快照
#define __MsgType_SSZ_FUND_SNAP__               (12)    //基金快照

#define __MsgType_SSZ_BOND_SNAP__               (211)   //债券现券快照、逆回购快照
#define __MsgType_SSZ_BOND_ORDER__              (92)    //债券现券逐笔委托、逆回购逐笔委托
#define __MsgType_SSZ_BOND_EXECUTION__          (91)    //债券现券逐笔委托、逆回购逐笔委托


typedef union SSZ_TradingPhaseCodePack_t 
{
    uint8_t             Value;
    struct unpack{
        uint8_t         Code0 : 4;  //映射自交易阶段代码第0位
        uint8_t         Code1 : 4;  //映射自交易阶段代码第1位
    } unpack;
}SSZ_TradingPhaseCodePack_t;

//Stock ShenZhen Unified header
struct SBE_SSZ_header_t
{
    uint8_t     SecurityIDSource;                      //交易所代码:102=深交所;101=上交所.
    uint8_t     MsgType;                               //消息类型:111=快照行情;191=逐笔成交;192=逐笔委托.
    uint16_t    MsgLen;                                //消息总字节数，含消息头.
    char        SecurityID[9];                         //证券代码，6或8字符后加'\0'
    uint16_t    ChannelNo;                             //通道号
    uint64_t    ApplSeqNum;                            //消息序列号，仅对逐笔成交和逐笔委托有效.
    SSZ_TradingPhaseCodePack_t     TradingPhase;       //交易阶段代码映射，仅对行情快照有效（深交所和上交所具体映射方式不同）.
}PACKED;


//map from MsgType=300111
struct SBE_SSZ_instrument_snap_t
{
    struct SBE_SSZ_header_t  Header;    //msgType=111

    int64_t         NumTrades;          //成交笔数
    int64_t         TotalVolumeTrade;   //成交总量, Qty,N15(2)
    int64_t         TotalValueTrade;    //成交总金额, Amt,N18(4)
    int32_t         PrevClosePx;        //昨收价, Price,N13(4)

    int32_t         LastPx;             //最近价, MDEntryPx,N18(6)
    int32_t         OpenPx;             //开盘价, MDEntryPx,N18(6)
    int32_t         HighPx;             //最高价, MDEntryPx,N18(6)
    int32_t         LowPx;              //最低价, MDEntryPx,N18(6)

    int32_t         BidWeightPx;        //买方委托数量加权平均价, MDEntryPx,N18(6)
    int64_t         BidWeightSize;      //买方委托总数量, Qty,N15(2)
    int32_t         AskWeightPx;        //卖方委托数量加权平均价, MDEntryPx,N18(6)
    int64_t         AskWeightSize;      //卖方委托总数量, Qty,N15(2)
    int32_t         UpLimitPx;          //涨停价, MDEntryPx,N18(6) 无涨停价格限制 = 0x7fffffff (转自999999999.999900)
    int32_t         DnLimitPx;          //跌停价, MDEntryPx,N18(6) 无跌停价格限制 = 0x2710 (转自0.010000) 或 0x80000000 (转自-999999999.999900)
    struct price_level_t   BidLevel[10];//十档买盘，价格 MDEntryPx,N18(6)，数量 Qty,N15(2)
    struct price_level_t   AskLevel[10];//十档卖盘，价格 MDEntryPx,N18(6)，数量 Qty,N15(2)
    uint64_t         TransactTime;      //YYYYMMDDHHMMSSsss(毫秒)
    uint8_t          Resv[4];
}PACKED;

//map from MsgType=309011
struct SBE_SSZ_index_snap_t
{
    struct SBE_SSZ_header_t  Header;    //msgType=11

    int64_t         NumTrades;
    int64_t         TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int64_t         PrevClosePx;
    int64_t         LastPx;
    int64_t         OpenPx;
    int64_t         HighPx;
    int64_t         LowPx;
    uint64_t        TransactTime;
}PACKED;

//map from MsgType=300192
struct SBE_SSZ_ord_t
{
    struct SBE_SSZ_header_t  Header;    //msgType=192

    int32_t         Price;          //委托价格, Price,N13(4)
    int64_t         OrderQty;       //委托数量, Qty,N15(2)
    int8_t          Side;           //买卖方向: '1'=买, '2'=卖, 'G'=借入, 'F'=出借
    int8_t          OrdType;        //订单类别: '1'=市价, '2'=限价, 'U'=本方最优
    uint64_t        TransactTime;   //YYYYMMDDHHMMSSsss(毫秒)
    uint8_t         Resv[2];
}PACKED;

//map from MsgType=300191
struct SBE_SSZ_exe_t
{
    struct SBE_SSZ_header_t  Header;    //msgType=191

    int64_t         BidApplSeqNum;  //买方委托索引 *
    int64_t         OfferApplSeqNum;//卖方委托索引 *
    int32_t         LastPx;         //成交价格, Price,N13(4)
    int64_t         LastQty;        //成交数量, Qty,N15(2)
    int8_t          ExecType;       //成交类别: '4'=撤销, 'F'=成交
    uint64_t        TransactTime;   //YYYYMMDDHHMMSSsss(毫秒)
    uint8_t         Resv[3];
}PACKED;


//map from MsgType=300111, channel in [1050, 1059]
struct SBE_SSZ_option_snap_t
{
    struct SBE_SSZ_header_t  Header;    //msgType=13

    int64_t         NumTrades;
    int64_t         TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int32_t         PrevClosePx;

    int32_t         LastPx;
    int32_t         OpenPx;
    int32_t         HighPx;
    int32_t         LowPx;

    int32_t         BidWeightPx;
    int64_t         BidWeightSize;
    int32_t         AskWeightPx;
    int64_t         AskWeightSize;
    int32_t         UpLimitPx;  
    int32_t         DnLimitPx;  
    int64_t         ContractPos;        //合约持仓量
    int32_t         RefPx;              //参考价
    struct price_level_t   BidLevel[10];
    struct price_level_t   AskLevel[10];
    uint64_t         TransactTime;
}PACKED;

//map from MsgType=300111, channel in [1020, 1029]
struct SBE_SSZ_fund_snap_t
{
    struct SBE_SSZ_header_t  Header;    //msgType=12

    int64_t         NumTrades;
    int64_t         TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int32_t         PrevClosePx;

    int32_t         LastPx;
    int32_t         OpenPx;
    int32_t         HighPx;
    int32_t         LowPx;

    int32_t         BidWeightPx;
    int64_t         BidWeightSize;
    int32_t         AskWeightPx;
    int64_t         AskWeightSize;
    int32_t         UpLimitPx;  //无涨停价格限制 = 0x7fffffff (转自999999999.999900)
    int32_t         DnLimitPx;  //无跌停价格限制 = 0x2710 (转自0.010000) 或 0x80000000 (转自-999999999.999900)
    struct price_level_t   BidLevel[10];
    struct price_level_t   AskLevel[10];
    uint64_t        TransactTime;
    int32_t         IOPV;
}PACKED;


//map from MsgType=300211
struct SBE_SSZ_bond_snap_t
{
    struct SBE_SSZ_header_t  Header;        //msgType=211

    int64_t         NumTrades;
    int64_t         TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int32_t         PrevClosePx;

    int32_t         LastPx;
    int32_t         OpenPx;
    int32_t         HighPx;
    int32_t         LowPx;

    int32_t         BidWeightPx;
    int64_t         BidWeightSize;
    int32_t         AskWeightPx;
    int64_t         AskWeightSize;
    int32_t         LastPxTradeType;    //产生该最近价的成交方式
    int32_t         MatchTradeLastPx;   //匹配成交最近价
    int64_t         AuctionVolumeTrade; //匹配成交成交量
    int64_t         AuctionValueTrade;  //匹配成交成交金额
    struct price_level_t   BidLevel[10];
    struct price_level_t   AskLevel[10];
    uint64_t        TransactTime;
    uint8_t         Resv[4];
}PACKED;

//map from MsgType=300292
struct SBE_SSZ_bond_ord_t
{
    struct SBE_SSZ_header_t  Header;        //msgType=92

    int32_t         Price;
    int64_t         OrderQty;
    int8_t          Side;
    int8_t          OrdType;
    uint64_t        TransactTime;
    uint8_t         Resv[2];
}PACKED;

//map from MsgType=300291
struct SBE_SSZ_bond_exe_t
{
    struct SBE_SSZ_header_t  Header;        //msgType=91

    int64_t         BidApplSeqNum;
    int64_t         OfferApplSeqNum;
    int32_t         LastPx;
    int64_t         LastQty;
    int8_t          ExecType;
    uint64_t        TransactTime;
    uint8_t         Resv[3];
}PACKED;













#ifdef WIN32
#pragma pack(pop)
#undef PACKED
#else
#undef PACKED
#endif


#ifdef __cplusplus
}
#endif
#endif /*__SBE_SSZ_H__*/
