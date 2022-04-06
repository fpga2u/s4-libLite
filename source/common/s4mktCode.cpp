﻿#include "common/s4mktCode.h"
#include "common/s4exceptions.h"
#include "types/s4convertors.h"
#include "common/s4logger.h"
#define  SH_CODE_MIN 600000
#define  SH_CODE_MAX 699999
#define  SZ_CODE_MIN 000000
#define  SZ_CODE_MAX 399999


using namespace std;

namespace S4{

//"600997" : "sh600997", "000001":"sz000001"
std::string pureCodeStr_to_mktCodeStr(const std::string& pureCode)
{
	if (pureCode.size() == 6){
		if (pureCode.c_str()[0] == '6') {
			return move(std::string("sh").append(pureCode));
		}
		else {
			return move(std::string("sz").append(pureCode));
		}
	}else{
		mktCodeI_t code = pureCodeStr_to_mktCode(pureCode);	//may throw exception
		return mktCodeInt_to_mktCodeStr(code);
	}
}
//"600997" : 170600997, "000001": 330000001
mktCodeI_t pureCodeStr_to_mktCode(const std::string& pureCode)
{
	int c = 0;
	for (size_t i = 0; i < pureCode.size(); ++i) {
		c = c * 10 + pureCode.c_str()[i] - '0';	//no check format
	}
	return pureCodeInt_to_mktCodeInt(c);
}
//600997 : "sh600997", 1:"sz000001"
std::string pureCodeInt_to_mktCodeStr(const int pureCode)
{
	char c[9];
	if (pureCode >= SH_CODE_MIN && pureCode <= SH_CODE_MAX) {
#ifdef _MSC_VER
		sprintf_s(c, "sh%06d", pureCode);
#else
		snprintf(c, sizeof(c), "sh%06d", pureCode);
#endif
	}
	else if (pureCode >= SZ_CODE_MIN && pureCode <= SZ_CODE_MAX) {
#ifdef _MSC_VER
		sprintf_s(c, "sz%06d", pureCode);
#else
		snprintf(c, sizeof(c), "sz%06d", pureCode);
#endif
	}else{
		throw CodeNameConvertError("unknow market for pureCode:"+ std::to_string(pureCode));
	}
	return move(std::string(c));
}
//600997 : "600997", 1:"000001"
std::string pureCodeInt_to_pureCodeStr(const int pureCode)
{
	char c[9];
	if ((pureCode >= SH_CODE_MIN && pureCode <= SH_CODE_MAX) ||
	    (pureCode >= SZ_CODE_MIN && pureCode <= SZ_CODE_MAX)) {
#ifdef _MSC_VER
		sprintf_s(c, "%06d", pureCode);
#else
		snprintf(c, sizeof(c), "%06d", pureCode);
#endif
	}else{
		throw CodeNameConvertError("unknow market for pureCode:"+ std::to_string(pureCode));
	}
	return move(std::string(c));
}

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 )   //unreachable returen
#endif
//600997 : 1700600997, 1: 3300000001
mktCodeI_t pureCodeInt_to_mktCodeInt(pureCodeI_t pureCode)
{
	if (pureCode >= SH_CODE_MIN && pureCode<=SH_CODE_MAX) {
		return SH_PRB + pureCode;
	}
	else if(pureCode >= SZ_CODE_MIN && pureCode <= SZ_CODE_MAX){
		return SZ_PRB + pureCode;
	}
	throw CodeNameConvertError("unknow market for pureCode:"+ std::to_string(pureCode));
	return pureCode;
}
#ifdef _MSC_VER
#pragma warning( pop )
#endif

mktCodeI_t mktCodeStr_to_mktCodeInt(const std::string& mktCode)
{
	mktCodeI_t c = 0;
	for (size_t i = 2; i < mktCode.size(); ++i) {
		c = c * 10 + mktCode.c_str()[i] - '0';
	}
	if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'h') {
		return SH_PRB + c;
	}
	else 
	if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'z') {
		return SZ_PRB + c;
	}
	else{
        throw CodeNameConvertError("unknow code :" + mktCode);
	}
}
std::string mktCodeInt_to_mktCodeStr(mktCodeI_t mktCode)
{
	mktCodeI_t c;
	char ch[9];
	if (mktCode >= SH_PRB && mktCode <= SH_PRB + PRB_MK) {
		c = mktCode - SH_PRB;
#ifdef _MSC_VER
		sprintf_s(ch, "sh%06d", c);
#else
		snprintf(ch, sizeof(ch), "sh%06d", c);
#endif
	}
	else if (mktCode >= SZ_PRB && mktCode <= SZ_PRB + PRB_MK) {
		c = mktCode - SZ_PRB;
#ifdef _MSC_VER
		sprintf_s(ch, "sz%06d", c);
#else
		snprintf(ch, sizeof(ch), "sz%06d", c);
#endif
	}
	else {
		throw CodeNameConvertError("unkonw code :" + IntConvertor::convert(mktCode));
	}
	return move(std::string(ch));
}

//"sh600997" : "600997", "sz000001":"000001"; [ "sh000001":ERROR ]
std::string mktCodeStr_to_pureCodeStr(const std::string& mktCode)
{
	if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'h') {
		if (mktCode.c_str()[3] == '6') {
			return move(mktCode.substr(2));
		}
		else {
			throw CodeNameConvertError("sh code error:" + mktCode);
		}
	}
	else if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'z') {
		if (mktCode.c_str()[3] == '0' || mktCode.c_str()[3] == '3') {
			return move(mktCode.substr(2));
		}
		else {
			throw CodeNameConvertError("sz code error:" + mktCode);
		}
	}
	else {
		throw CodeNameConvertError("unkonw code :" + mktCode);
	}
}
//"sh600997" : 600997, "sz000001":1; [ "sh000001":ERROR ]
pureCodeI_t mktCodeStr_to_pureCodeInt(const std::string& mktCode)
{
	pureCodeI_t c = 0;
	for (size_t i = 2; i < mktCode.size(); ++i) {
		c = c * 10 + mktCode.c_str()[i] - '0';
	}
	if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'h') {
		if (c>=SH_CODE_MIN) {
			return c;
		}
		else {
			throw CodeNameConvertError("sh code error:" + mktCode);
		}
	}
	else if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'z') {
		if (c<SH_CODE_MIN) {
			return c;
		}
		else {
			throw CodeNameConvertError("sz code error:" + mktCode);
		}
	}
	else {
		throw CodeNameConvertError("unkonw code :" + mktCode);
	}

}

//170600997 : "600997", 330000001: "000001"
std::string mktCodeInt_to_pureCodeStr(mktCodeI_t mktCode)
{
	pureCodeI_t c;
	char ch[7];
	if (mktCode >= SH_PRB && mktCode <= SH_PRB + PRB_MK) {
		c = mktCode - SH_PRB;
		if (c >= SH_CODE_MIN) {
#ifdef _MSC_VER
			sprintf_s(ch, "%06d", c);
#else
			snprintf(ch, sizeof(ch), "%06d", c);
#endif
		}
		else {
			throw CodeNameConvertError("sh code error:" + IntConvertor::convert(mktCode));
		}
	}
	else if (mktCode >= SZ_PRB && mktCode <= SZ_PRB + PRB_MK) {
		c = mktCode - SZ_PRB;
		if (c < SH_CODE_MIN) {
#ifdef _MSC_VER
			sprintf_s(ch, "%06d", c);
#else
			snprintf(ch, sizeof(ch), "%06d", c);
#endif
		}
		else {
			throw CodeNameConvertError("sz code error:" + IntConvertor::convert(mktCode));
		}
	}
	else {
		throw CodeNameConvertError("unkonw code :" + IntConvertor::convert(mktCode));
	}
	return move(std::string(ch));
}

//170600997 : 600997, 330000001: 1
pureCodeI_t mktCodeInt_to_pureCodeInt(mktCodeI_t mktCode)
{
	pureCodeI_t c;
	if (mktCode >= SH_PRB && mktCode <= SH_PRB + PRB_MK) {
		c = mktCode - SH_PRB;
		if (c < SH_CODE_MIN) {
			throw CodeNameConvertError("sh code error:" + IntConvertor::convert(mktCode));
		}
	}
	else if (mktCode >= SZ_PRB && mktCode <= SZ_PRB + PRB_MK) {
		c = mktCode - SZ_PRB;
		if (c >= SH_CODE_MIN) {
			throw CodeNameConvertError("sz code error:" + IntConvertor::convert(mktCode));
		}
	}
	else {
		throw CodeNameConvertError("unkonw code :" + IntConvertor::convert(mktCode));
	}
	return c;
	
}
/*
SH:
001×××国债现货；
110×××
120×××企业债券；
129×××
100×××可转换债券；
201×××国债回购；
310×××国债期货；
500×××
550×××基金；
600×××A股；
688xxx科创板；
700×××配股；
710×××转配股；
701×××转配股再配股；
711×××转配股再转配股；
720×××红利；
730×××新股申购；
735×××新基金申购；
737×××新股配售；
900×××B股

SZ:
第1位	第二位	第3-6位	含义
  0       0       XXXX  A股证券
  0       3       XXXX  A股A2权证
  0       7       XXXX  A股增发
  0       8       XXXX  A股A1权证
  0       9       XXXX  A股转配
  1       0       XXXX  国债现货
  1       1       XXXX  债券
  1       2       XXXX  可转换债券
  1       3       XXXX  国债回购
  1       7       XXXX  原有投资基金
  1       8       XXXX  证券投资基金
  2       0       XXXX  B股证券
  2       7       XXXX  B股增发
  2       8       XXXX  B股权证
  3       0       XXXX  创业板证券
  3       7       XXXX  创业板增发
  3       8       XXXX  创业板权证
  3       9       XXXX  综合指数/成份指数
  159801~159999 etf
*/
bool isStk(const std::string & MktCodeStr)
{
	if (MktCodeStr.size() != 8)
		return false;
	std::string leading = MktCodeStr.substr(0, 4);
	if (leading == "sz00" || leading == "sz30" || leading == "sh60")
		return true;

	return false;
}
bool isIdx(const std::string & MktCodeStr)
{
	if (MktCodeStr.size() != 8)
		return false;
	std::string leading = MktCodeStr.substr(0, 5);
	if (leading == "sz399" || leading == "sh000")
		return true;
    leading = MktCodeStr.substr(0, 4);
	if (leading == "sz98")
		return true;

	return false;
}
bool isFund(const std::string & MktCodeStr)
{
	if (MktCodeStr.size() != 8)
		return false;
	std::string leading = MktCodeStr.substr(0, 5);
	if (   leading == "sh501" 
        || leading == "sh502"
        || leading == "sh510"
        || leading == "sh511"
        || leading == "sh512"
        || leading == "sh513"
        || leading == "sh515"
        || leading == "sh516"
        || leading == "sh518"
        || leading == "sh560"
        || leading == "sh561"
        || leading == "sh562"
        || leading == "sh563"
        || leading == "sh588"
        || leading == "sz159"
        )
		return true;
    leading = MktCodeStr.substr(0, 4);
	if (leading == "sz16")
		return true;

	return false;
}


bool isSZmkt(mktCodeI_t MktCodeInt)
{
	if (MktCodeInt > SZ_PRB && MktCodeInt < SZ_PRB + PRB_MK)
		return true;
	return false;
}
bool isSHmkt(mktCodeI_t MktCodeInt)
{
	if (MktCodeInt > SH_PRB && MktCodeInt < SH_PRB + PRB_MK)
		return true;
	return false;
}
bool isCYmkt(mktCodeI_t MktCodeInt)
{
	if (MktCodeInt > CY_PRB && MktCodeInt < CY_PRB + CY_PRB_MK)
		return true;
	return false;
}
bool isKCmkt(mktCodeI_t MktCodeInt)
{
	if (MktCodeInt > KC_PRB && MktCodeInt < KC_PRB + KC_PRB_MK)
		return true;
	return false;
}


bool isStk(mktCodeI_t MktCodeInt)
{
	if (isSHmkt(MktCodeInt)) {
		if (MktCodeInt >= SH_PRB + 600000 && MktCodeInt <= SH_PRB + 609999)
			return true;
		else
			return false;
	}
	else if (isSZmkt(MktCodeInt)) {
		if( (MktCodeInt >= SZ_PRB && MktCodeInt <= SZ_PRB + 9999) || 
			(MktCodeInt >= SZ_PRB + 300000 && MktCodeInt <= SZ_PRB + 309999) )
			return true;
		else
			return false;
	}

	return false;
}
//指数
bool isIdx(mktCodeI_t MktCodeInt)
{
	if (isSHmkt(MktCodeInt)) {
		if (MktCodeInt >= SH_PRB && MktCodeInt <= SH_PRB + 999)
			return true;
		else
			return false;
	}
	else if (isSZmkt(MktCodeInt)) {
		if ((MktCodeInt >= SZ_PRB + 390000 && MktCodeInt <= SH_PRB + 399999))
			return true;
		else
			return false;
	}

	return false;
}
//基金
bool isFund(mktCodeI_t MktCodeInt)
{
	if (isSHmkt(MktCodeInt)) {
        int code = MktCodeInt-SH_PRB;
		if (  (code >= 501000 && code <= 502999)
            ||(code >= 506000 && code <= 506099)
            ||(code >= 510000 && code <= 513999)
            ||(code >= 515000 && code <= 516999)
            ||(code >= 518000 && code <= 518999)
            ||(code >= 560000 && code <= 563999)
            ||(code >= 588000 && code <= 588499)
           )
			return true;
		else
			return false;
	}
	else if (isSZmkt(MktCodeInt)) {
        int code = MktCodeInt-SZ_PRB;
		if (  (code >= 160000 && code <= 169999)
            ||(code >= 159000 && code <= 159999)
           )
			return true;
		else
			return false;
	}

	return false;
}


bool pureCodeIsStk(int pureCode)
{
		if ((pureCode >= 600000 && pureCode <= 609999)||
			(pureCode >= 0 && pureCode <= 9999) ||
			(pureCode >= 300000 && pureCode <= 309999))
			return true;
	return false;
}

}//namespace S4
