#pragma once
/*********************************************************************************
 * 交易所部分代码段分类表：
 * -----------+-------------------------+----------------------------------+-----------
 * type       | SZSE                    | SSE                             | comment
 * -----------+-------------------------+----------------------------------+-----------
 * index      | 399xxx, 98xxxx, 395xxx  | 000xxx(used=0~993)               | 指数
 * -----------+-------------------------+----------------------------------+-----------
 * stock      | 000~001xxx              | 600~601xxx, 603xxx, 605xxx       | 股票主板
 * stock      | 002~004xxx              |                                  | 股票中小板
 * stock      | 300~309xxx              |                                  | 股票创业板
 * stock      | 200~209xxx              | 900xxx                           | 股票B股
 * stock      |                         |                                  | 股票H股
 * stock      |                         | 688xxx                           | 股票科创板
 * -----------+-------------------------+----------------------------------+-----------
 * fund       | 160~169xxx              | 501~502xxx, 5060xx               | LOF基金
 * fund       | 159xxx                  | 510~513xxx, 515~516xxx, 518xxx   | ETF基金
 * fund       |                         | 560000-563999, 588000-588499     | ETF基金
 * -----------+-------------------------+----------------------------------+-----------
 * bond       | 100~107xxx              | 010xxx, 018~019xxx               | 国债
 * bond       | 120~129xxx              | 110xxx, 113xxx, 132xxx, 719xxx   | 可转债
 * -----------+-------------------------+----------------------------------+-----------
 * repurchase | 1318xx                  | 204xxx                           | 国债逆回购
 * -----------+-------------------------+----------------------------------+-----------
 * option     | 92000001~99999999       | 20000001~30000000                | 股票期权
 * option     | 90000001~91999999       | 10000001~20000000                | ETF期权
 * -----------+-------------------------+----------------------------------+-----------
 *********************************************************************************/

#define PRB_MK 99999999
#define SH_PRB 1700000000
#define KC_PRB 1700688000
#define KC_PRB_MK 1000

#define SZ_PRB 3300000000
#define CY_PRB 3300300000
#define CY_PRB_MK 10000


#define CODE_INT_STK_INDEX_SH (1700000001)
#define CODE_INT_STK_INDEX_SZ (3300399001)
#define CODE_INT_STK_INDEX_CY (3300399006)

#define CODE_STR_STK_INDEX_SH ("sh000001")
#define CODE_STR_STK_INDEX_SZ ("sz399001")
#define CODE_STR_STK_INDEX_CY ("sz399006")


#include <string>
/***********************************
 * mktCodeInt  : 320000001, 330000997, 170600004;  map_gbbd_t, map_fhps_t;
 * mktCodeStr  : "sh000001", "sz000997", "sh600004"; (interface)
 * pureCodeInt : 1, 997, 600004;  ??
 * pureCodeStr : "000001", "000997", "600004"; basic (tushare/ol)
 * *********************************/

namespace S4{

typedef uint32_t mktCodeI_t;
typedef uint32_t pureCodeI_t;

//"600997" : "sh600997", "000001":"sz000001"
std::string pureCodeStr_to_mktCodeStr(const std::string& pureCode);
//"600997" : 170600997, "000001": 330000001
mktCodeI_t pureCodeStr_to_mktCode(const std::string& pureCode);
//600997 : "sh600997", 1:"sz000001"
std::string pureCodeInt_to_mktCodeStr(pureCodeI_t pureCode);
//600997 : "600997", 1:"000001"
std::string pureCodeInt_to_pureCodeStr(pureCodeI_t pureCode);
//600997 : 170600997, 1: 330000001
mktCodeI_t pureCodeInt_to_mktCodeInt(pureCodeI_t pureCode);

//"sh600997" : "600997", "sz000001":"000001"; [ "sh000001":ERROR ]
std::string mktCodeStr_to_pureCodeStr(const std::string& mktCode);
//"sh600997" : 600997, "sz000001":1; [ "sh000001":ERROR ]
pureCodeI_t mktCodeStr_to_pureCodeInt(const std::string& mktCode);
//170600997 : "600997", 330000001: "000001"
std::string mktCodeInt_to_pureCodeStr(mktCodeI_t mktCode);
//170600997 : 600997, 330000001: 1
pureCodeI_t mktCodeInt_to_pureCodeInt(mktCodeI_t mktCode);

//"sh600997" : 170600997, "sz000001":330000001; [ "sh000001":170000001 ]
mktCodeI_t mktCodeStr_to_mktCodeInt(const std::string& mktCode);

//170600997 : "sh600997", 330000001: "sz000001"
std::string mktCodeInt_to_mktCodeStr(mktCodeI_t mktCode);

bool isSZmkt(mktCodeI_t MktCodeInt);
bool isSHmkt(mktCodeI_t MktCodeInt);
bool isCYmkt(mktCodeI_t MktCodeInt);
bool isKCmkt(mktCodeI_t MktCodeInt);

bool isStk(mktCodeI_t MktCodeInt);
bool isIdx(mktCodeI_t MktCodeInt);
bool isFund(mktCodeI_t MktCodeInt);

bool isStk(const std::string & MktCodeStr);
bool isIdx(const std::string & MktCodeStr);
bool isFund(const std::string & MktCodeStr);

bool pureCodeIsStk(int pureCode);



}//namespace S4