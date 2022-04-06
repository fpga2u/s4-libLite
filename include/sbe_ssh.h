
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

#define __MsgType_SSH_INSTRUMENT_SNAP__         (111)
#define __MsgType_SSH_INDEX_SNAP__              (11)
#define __MsgType_SSH_ORDER__                   (192)
#define __MsgType_SSH_EXECUTION__               (191)

#define __MsgType_SSH_INSTRUMENT_SNAPx5__       (2)     //L1
#define __MsgType_SSH_FUND_SNAPx5__             (4)     //L1
#define __MsgType_SSH_INDEX_SNAPx5__            (10)    //L1
#define __MsgType_SSH_OPTION_SNAPx5__           (31)    //L1

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
		uint8_t         B1 : 2;
		uint8_t         B2 : 4;
		uint8_t         B3 : 2;
    } unpack;
}TradingPhaseCodePack_t;

//map from MsgType=UA3202
struct SBE_SSH_instrument_snap_t
{
    struct SBE_SSH_header_t  Header;    //msgType=111

    int32_t         NumTrades;
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
    uint32_t        DataTimeStamp;
    struct price_level_t   BidLevel[10];
    struct price_level_t   AskLevel[10];
    TradingPhaseCodePack_t TradingPhaseCodePack;
    uint8_t          Resv[3];
}PACKED;

//map from MsgType=UA3113
struct SBE_SSH_index_snap_t
{
    struct SBE_SSH_header_t  Header;    //msgType=11

    int64_t         TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int64_t         PrevClosePx;
    int64_t         LastPx;
    int64_t         OpenPx;
    int64_t         HighPx;
    int64_t         LowPx;
    uint32_t        DataTimeStamp;
    uint8_t         Resv[4];
}PACKED;

//map from MsgType=UA5801
struct SBE_SSH_ord_t
{
    struct SBE_SSH_header_t  Header;    //msgType=192

    int64_t         OrderNo;    //
    int32_t         Price;      //
    int64_t         OrderQty;
    int8_t          OrdType;    //'A', 'D'
    int8_t          Side;       //'B', 'S'
    uint32_t        OrderTime;
    uint8_t         Resv[6];
}PACKED;

//map from MsgType=UA3201
struct SBE_SSH_exe_t
{
    struct SBE_SSH_header_t  Header;    //msgType=191

    int64_t         TradeBuyNo;
    int64_t         TradeSellNo;
    int32_t         LastPx;
    int64_t         LastQty;
    int8_t          TradeBSFlag;    //'B', 'S', 'N'
    uint32_t        TradeTime;
    uint8_t         Resv[7];
}PACKED;


struct trade_level_t
{
    int32_t    Price;
    int64_t    Qty;
    uint32_t   TimeStamp;
}PACKED;


//map from MsgType=W & MDStreamID=MD002; TODO:MD003/MD101/MD102?
struct SBE_SSH_instrument_snapx5_t
{
    struct SBE_SSH_header_t  Header;    //msgType=2

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
    struct SBE_SSH_header_t  Header;    //msgType=4

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
    struct SBE_SSH_header_t  Header;    //msgType=10

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
    struct SBE_SSH_header_t  Header;    //msgType=31

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
