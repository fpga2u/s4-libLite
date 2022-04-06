#pragma once
#include <stdint.h>
#include <string>

//整数价格，精度=分
typedef int32_t price_t;
typedef double fprice_t;
#define iPrice_precision 100

//float price to int price: 15.199999 -> 15.20499 -> 1520
#define fPrice_to_iPrice(x) ((price_t)(((x)+(0.5/iPrice_precision))*iPrice_precision))
#define iPrice_to_fPrice(x) ((x)*(1.0/iPrice_precision))

/*** L2 精度 ***/
//深圳股票
//深圳交易所L2 逐笔价格精度
#define SSZ_L2_iPrice_tick_precision 10000
//深圳交易所L2 快照价格精度
#define SSZ_L2_iPrice_snap_precision 1000000
//深圳交易所L2 数量精度
#define SSZ_L2_Qty_precision 100
//深圳交易所L2 成交额精度
#define SSZ_L2_Amt_precision 10000

#define SSZ_L2_iPrice_tick_to_fPrice(x) ((x)*(1.0/SSZ_L2_iPrice_tick_precision))
#define SSZ_L2_iPrice_snap_to_fPrice(x) ((x)*(1.0/SSZ_L2_iPrice_snap_precision))
#define SSZ_L2_Qty_to_hand(x)   ((x)/(100.0*SSZ_L2_Qty_precision))

//上海股票 价格3位，数量3位，金额5位
//上海交易所L2 逐笔价格精度
#define SSH_L2_iPrice_tick_precision 1000
//上海交易所L2 快照价格精度
#define SSH_L2_iPrice_snap_precision 1000
//上海交易所L2 数量精度
#define SSH_L2_Qty_precision 1000
//上海交易所L2 成交额精度
#define SSH_L2_Amt_precision 100000

//上海交易所L2 指数快照价格精度 5位小数
#define SSH_L2_INDEX_PX_precision 100000
//上海交易所L2 指数快照交易数量精度 5位小数
#define SSH_L2_INDEX_VOL_precision 100000
//上海交易所L2 指数快照交易金额精度 1位小数
#define SSH_L2_INDEX_AMT_precision 10

#define SSH_L2_iPrice_tick_to_fPrice(x) ((x)*(1.0/SSH_L2_iPrice_tick_precision))
#define SSH_L2_iPrice_snap_to_fPrice(x) ((x)*(1.0/SSH_L2_iPrice_snap_precision))
#define SSH_L2_Qty_to_hand(x)   ((x)/(100.0*SSH_L2_Qty_precision))


//上海交易所L1 Binary 快照价格精度 5位小数
#define SSH_L1Bin_PX_precision 100000
//上海交易所L1 Binary 快照交易数量精度 0位小数
#define SSH_L1Bin_VOL_precision 1
//上海交易所L1 Binary 快照交易金额精度 2位小数
#define SSH_L1Bin_AMT_precision 100

//打印价格，按2位小数
inline
std::string priceString(float p){
    char price_s[64];
    sprintf(price_s, "%0.2f", p);
    return price_s;
}

//5% 板价格
#define UP_5p(x) ((price_t)((x)*1.05+0.5))
#define DN_5p(x) ((price_t)((x)*0.95+0.5))

//10% 板价格
#define UP_10p(x) ((price_t)((x)*1.1+0.5))
#define DN_10p(x) ((price_t)((x)*0.9+0.5))

//20% 板价格
#define UP_20p(x) ((price_t)((x)*1.2+0.5))
#define DN_20p(x) ((price_t)((x)*0.8+0.5))

//涨幅 %
#define CALC_R_PERCENT(x,y) ((x) * 100.0f / (y) - 100.0f)

//涨幅 千分比
#define CALC_R_PERMILL(x,y) ((x) * 1000.0f / (y) - 1000.0f)

//涨幅 % 整数计算
#define CALC_R_PERCENT_INT(x,y) ((x) * 100 / (y) - 100)

//成交额，精度=元
typedef float amount_t;

//for amount
#define _KW ((amount_t)1E7)

//成交量
typedef long long vol_share_t;
typedef long long vol_board_t;
#define vBoard_unit (100)
// #define vShare_to_vBoard(x)  ((x)/vBoard_unit)
#define vBoard_to_vShare(x)  ((x)*vBoard_unit)

//dsx快照 成交量精度
typedef int32_t vol_dsx_t;

//换手率
typedef float turnover_t;

//复权
typedef double fq_index_t;
typedef double fq_share_t;


#define _MAX_(x,y) (((x)>(y))?(x):(y))
#define _MIN_(x,y) (((x)<(y))?(x):(y))
#define _ABS_(x) ((x)<0?-(x):(x))
