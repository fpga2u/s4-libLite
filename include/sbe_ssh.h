
#ifndef __SBE_SSH_H__
#define __SBE_SSH_H__
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

#define __SecurityIDSource_SSH_             (101)

#ifndef __MsgType_HEARTBEAT__
#define __MsgType_HEARTBEAT__               (1)
#endif

#define __MsgType_SSH_INSTRUMENT_SNAP__         (111)   //竞价行情快照数据
#define __MsgType_SSH_INDEX_SNAP__              (11)    //指数行情数据
#define __MsgType_SSH_ORDER__                   (192)   //竞价逐笔委托数据
#define __MsgType_SSH_EXECUTION__               (191)   //竞价逐笔成交数据

#define __MsgType_SSH_INSTRUMENT_SNAPx5__       (2)     //L1  FPGA目前不支持
#define __MsgType_SSH_FUND_SNAPx5__             (4)     //L1  FPGA目前不支持
#define __MsgType_SSH_INDEX_SNAPx5__            (10)    //L1  FPGA目前不支持
#define __MsgType_SSH_OPTION_SNAPx5__           (31)    //L1  FPGA目前不支持

//Stock ShangHai Unified header
struct SBE_SSH_header_t
{
    uint8_t     SecurityIDSource;   //=101
    uint8_t     MsgType;            //
    uint16_t    MsgLen;             //include this header, until ending byte of SBE message
    char        SecurityID[9];      // c8 + '\0'
    uint16_t    ChannelNo;          //111, 11, 192, 191:Channel;  2:MDStreamID(2,3,101.102); others:0
    uint64_t    ApplSeqNum;         //
    uint8_t     TradingPhase;       //
}PACKED;

typedef union TradingPhaseCodePack_t 
{
    uint8_t             Value;
    struct unpack{
        uint8_t         B1 : 2; //映射自交易阶段代码第1Byte
        uint8_t         B2 : 4; //映射自交易阶段代码第2Byte
        uint8_t         B3 : 2; //映射自交易阶段代码第3Byte
    } unpack;
}TradingPhaseCodePack_t;

//map from MsgType=UA3202
struct SBE_SSH_instrument_snap_t
{
    struct SBE_SSH_header_t  Header;    //msgType=111

    int32_t         NumTrades;          //成交笔数
    int64_t         TotalVolumeTrade;   //成交总量, 3位小数
    int64_t         TotalValueTrade;    //成交总金额, 5位小数
    int32_t         PrevClosePx;        //昨收盘价格, 3位小数
    int32_t         LastPx;             //成交价格, 3位小数
    int32_t         OpenPx;             //开盘价格, 3位小数
    int32_t         HighPx;             //最高价格, 3位小数
    int32_t         LowPx;              //最低价格, 3位小数
    int32_t         BidWeightPx;        //加权平均委买价格, 3位小数
    int64_t         BidWeightSize;      //委托买入总量, 3位小数
    int32_t         AskWeightPx;        //加权平均委卖价格, 3位小数
    int64_t         AskWeightSize;      //委托卖出总量, 3位小数
    uint32_t        DataTimeStamp;      //最新订单时间(秒), 143025表示14:30:25
    struct price_level_t   BidLevel[10];//价格3位小数; 申买量3位小数
    struct price_level_t   AskLevel[10];//价格3位小数; 申卖量3位小数
    TradingPhaseCodePack_t TradingPhaseCodePack;
    uint8_t          Resv[3];
}PACKED;

//map from MsgType=UA3113
struct SBE_SSH_index_snap_t
{
    struct SBE_SSH_header_t  Header;    //msgType=11

    int64_t         TotalVolumeTrade;   //参与计算相应指数的交易数量, 5位小数
    int64_t         TotalValueTrade;    //参与计算相应指数的成交金额（元），1位小数
    int64_t         PrevClosePx;        //前收盘指数, 5位小数
    int64_t         LastPx;             //最新指数, 5位小数
    int64_t         OpenPx;             //今开盘指数, 5位小数
    int64_t         HighPx;             //最高指数, 5位小数
    int64_t         LowPx;              //最低指数, 5位小数
    uint32_t        DataTimeStamp;      //行情时间（秒）143025 表示 14:30:25
    uint8_t         Resv[4];
}PACKED;

//map from MsgType=UA5801
struct SBE_SSH_ord_t
{
    struct SBE_SSH_header_t  Header;    //msgType=192

    int64_t         OrderNo;            //原始订单号 *
    int32_t         Price;              //委托价格（元）, 3位小数
    int64_t         OrderQty;           //委托数量, 3位小数
    int8_t          OrdType;            //订单类型: 'A'=新增委托订单, 'D'=删除委托订单
    int8_t          Side;               //买卖单标志: 'B'=买单, 'S'=卖单
    uint32_t        OrderTime;          //委托时间(百分之一秒), 14302506表示14:30:25.06
    uint8_t         Resv[6];
}PACKED;

//map from MsgType=UA3201
struct SBE_SSH_exe_t
{
    struct SBE_SSH_header_t  Header;    //msgType=191

    int64_t         TradeBuyNo;         //买方订单号
    int64_t         TradeSellNo;        //卖方订单号
    int32_t         LastPx;             //成交价格（元）, 3位小数
    int64_t         LastQty;            //成交数量, 3位小数 *
    int8_t          TradeBSFlag;        //内外盘标志: 'B'=外盘，主动买; 'S'=内盘，主动卖; 'N'=未知 **
    uint32_t        TradeTime;          //委托时间(百分之一秒), 14302506表示14:30:25.06
    uint8_t         Resv[7];
}PACKED;


struct trade_level_t
{
    int32_t    Price;
    int64_t    Qty;
    uint32_t   TimeStamp;
}PACKED;


//map from MsgType=W & MDStreamID=MD002;
struct SBE_SSH_instrument_snapx5_t
{
    struct SBE_SSH_header_t  Header;    //msgType=2 FPGA目前不支持

    uint64_t        NumTrades;
    uint64_t        TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int32_t         LastPx;             //
    int64_t         LastQty;            //
    int32_t         OpenPx;             //
    int32_t         HighPx;             //
    int32_t         LowPx;              //
    struct price_level_t   BidLevel[5]; //
    struct price_level_t   AskLevel[5]; //
    int32_t         DataTimeStamp;      //
    TradingPhaseCodePack_t TradingPhaseCodePack;
    uint8_t         Resv[3];
}PACKED;

//map from MsgType=W & MDStreamID=MD004
struct SBE_SSH_fund_snapx5_t
{
    struct SBE_SSH_header_t  Header;    //msgType=4 FPGA目前不支持

    uint64_t        NumTrades;
    uint64_t        TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int32_t         LastPx;             //
    uint64_t        LastQty;            //
    int32_t         OpenPx;             //
    int32_t         HighPx;             //
    int32_t         LowPx;              //
    uint64_t        IOPV;               //
    struct price_level_t   BidLevel[5]; //
    struct price_level_t   AskLevel[5]; //
    int32_t         DataTimeStamp;      //
    TradingPhaseCodePack_t TradingPhaseCodePack;
    uint8_t         Resv[3];
}PACKED;

//map from MsgType=W & MDStreamID=MD001
struct SBE_SSH_index_snapx5_t
{
    struct SBE_SSH_header_t  Header;    //msgType=10 FPGA目前不支持

    uint64_t        TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int64_t         LastPx;             //MDEntryType=3
    int64_t         OpenPx;             //MDEntryType=4
    int64_t         HighPx;             //MDEntryType=7
    int64_t         LowPx;              //MDEntryType=8
    int32_t         DataTimeStamp;      //MDEntryType=3
    uint8_t         Resv[4];
}PACKED;

//map from L1-Binary.MDStreamID=MD301
struct SBE_SSH_option_snapx5_t
{
    struct SBE_SSH_header_t  Header;    //msgType=31 FPGA目前不支持

    uint64_t        NumTrades;
    uint64_t        TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int32_t         LastPx;             //
    uint64_t        LastQty;            //
    int32_t         OpenPx;             //
    int32_t         HighPx;             //
    int32_t         LowPx;              //
    int32_t         SettlPx;            //结算价
    uint64_t        TotalLongPosition;  //总持仓量
    int32_t         RefPx;              //动态参考价格
    uint64_t        QuantityMatching;   //虚拟匹配数量
    struct price_level_t   BidLevel[5]; //
    struct price_level_t   AskLevel[5]; //
    int32_t         DataTimeStamp;      //
    TradingPhaseCodePack_t TradingPhaseCodePack;
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
#endif /*__SBE_SSH_H__*/
