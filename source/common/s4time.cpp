#include "common/s4time.h"
#include "common/s4logger.h"
#include "common/offtime.h"

namespace S4{

uint64_t nowTime_ms(void)
{
    timeb tb;
    ftime(&tb);
    uint64_t t0 = tb.time * 1000 + tb.millitm;
    return t0;
}

time_minuSec_t nowMinuSec(void) {
    time_t sysClk;
    time(&sysClk);
    sysClk += 3600 * 8; //fix asia:shanghai

    int minu = 0;
    utc_to_date(sysClk, &minu);

    return minu;
}

time_date_t nowDate(void)
{
    time_t sysClk;
    time(&sysClk);
    sysClk += 3600 * 8; //fix asia:shanghai

    return utc_to_date(sysClk);
}


time_date_t utc_to_date(time_utcSec_t utc, time_minuSec_t* minuSec)
{
    //struct tm m_tmTime = *gmtime(&utc);  //fix asia:shanghai inside
    struct tm m_tmTime;
    if (0 != __offtime(utc, 0, &m_tmTime)){   //fail
        return 19700101;
    }

    time_date_t dateTime = (m_tmTime.tm_year + 1900) * 100 * 100 +
                            (m_tmTime.tm_mon + 1) * 100 +
                            (m_tmTime.tm_mday);

    if (minuSec != NULL)
        *minuSec = m_tmTime.tm_hour * 10000 + m_tmTime.tm_min * 100 + m_tmTime.tm_sec;

    return dateTime;
}

std::string utc_to_str(time_utcSec_t utc)
{
    char s[64];
    //struct tm ltm = *gmtime(&utc);  //fix asia:shanghai inside
    struct tm ltm;
    if (0 != __offtime(utc, 0, &ltm))   //fail
        return "1970_01_01_00_00_00";
    strftime(s, 64, "%Y_%m_%d__%H_%M_%S", &ltm);
    return (s);
}
std::string utc_str_now()
{
    return utc_to_str(time(NULL) + 8 * 3600);
}

std::string date_to_str(time_date_t date)
{
    char s[64];
    int year = date / 10000;
    int month = date % 10000;
    int day = month % 100;
    month = month / 100;
#ifdef _MSC_VER
    sprintf_s(s, sizeof(s), "%d_%02d_%02d", year, month, day);
#else
    snprintf(s, sizeof(s), "%d_%02d_%02d", year, month, day);
#endif
    return (s);
}

std::string utc_to_strMinu(time_utcSec_t utc)
{
    char s[64];
    //struct tm ltm = *gmtime(&utc);  //fix asia:shanghai inside
    struct tm ltm;
    if (0 != __offtime(utc, 0, &ltm))   //fail
        return "00:00:00";
    strftime(s, 64, "%H:%M:%S", &ltm);
    return (s);
}

//date:YYYYMMDD  minuSec=HHMMSS
time_utcSec_t date_to_utc(time_date_t date, time_minuSec_t minuSec)
{
    if (date < 0 || minuSec <0) {
        ERR("date_to_utc({:d}, {}) input illegal!\n", date, minuSec);
        return 0;
    }
    tm tmTime;
    int t = minuSec / 100;

    tmTime.tm_sec = minuSec % 100;
    tmTime.tm_min = t % 100;
    t /= 100;
    tmTime.tm_hour = t;
    if (tmTime.tm_sec > 60 || tmTime.tm_min >= 60 || tmTime.tm_hour >= 24) {
        ERR("date_to_utc({:d}, {}) input minuSec illegal!\n", date, minuSec);
    }

    t = date / 100;
    tmTime.tm_mday = date % 100;
    tmTime.tm_mon = t % 100 - 1;
    t = t / 100;
    tmTime.tm_year = t - 1900;
    assert(t > 1980 && "date_to_utc: date out of range!");

    return tm_to_utc(&tmTime);
}

int days_in(time_date_t date1, time_date_t date2)
{
    time_utcSec_t t1 = date_to_utc(date1);
    time_utcSec_t t2 = date_to_utc(date2);
    int difference = t2 > t1 ? (int)(t2 - t1) : (int)(t1 - t2);
    difference /= (60 * 60 * 24);
    return difference;
}


//TODO: check format
void str_to_tm(char *mdate, char *mtime, struct tm* mtm)
{
    char *pstr;
    long year, month, day, hour, minu, sec;

    year = strtol(mdate, &pstr, 10); while (!isdigit(*pstr)) ++pstr;
    month = strtol(pstr, &pstr, 10); while (!isdigit(*pstr)) ++pstr;
    day = strtol(pstr, &pstr, 10);

    hour = strtol(mtime, &pstr, 10); while (!isdigit(*pstr)) ++pstr;
    minu = strtol(pstr, &pstr, 10); while (!isdigit(*pstr)) ++pstr;
    sec = strtol(pstr, &pstr, 10);

    mtm->tm_sec = sec;
    mtm->tm_min = minu;
    mtm->tm_hour = hour;
    mtm->tm_mday = day;
    mtm->tm_mon = month - 1;
    mtm->tm_year = year - 1900;
}

//TODO: check format
time_minuSec_t str_to_minuSec(const char *mtime)
{
    char *pstr;
    long hour, minu, sec;
    hour = strtol(mtime, &pstr, 10); while (!isdigit(*pstr)) ++pstr;
    minu = strtol(pstr, &pstr, 10); while (!isdigit(*pstr)) ++pstr;
    sec = strtol(pstr, &pstr, 10);
    return hour * 10000 + minu * 100 + sec;
}

//TODO: check format
time_minuSec_t str_to_date(const char *mdate)
{
    char *pstr;
    long year, month, day;

    year = strtol(mdate, &pstr, 10); while (!isdigit(*pstr)) ++pstr;
    month = strtol(pstr, &pstr, 10); while (!isdigit(*pstr)) ++pstr;
    day = strtol(pstr, &pstr, 10);
    return year * 10000 + month * 100 + day;
}

time_utcSec_t tm_to_utc(const struct tm *ltm, int utcdiff)
{
    static const int mon_days[] =
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    static const int mon_days_leap[] =
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    time_t tdays;
    int tyears, leaps, leap_this_year, utc_hrs;
    int i;

    tyears = ltm->tm_year - 70; // tm->tm_year is from 1900.
    leaps = (tyears + 2) / 4; // no of next two lines until year 2100.
    //i = (ltm->tm_year ¨C 100) / 100;
    //leaps -= ( (i/4)*3 + i%4 );
    leap_this_year = (tyears + 2) % 4 == 0;

    tdays = 0;
    if (leap_this_year) {
        leaps--;
        for (i = 0; i < ltm->tm_mon; i++) tdays += mon_days_leap[i];
    }
    else {
        for (i = 0; i < ltm->tm_mon; i++) tdays += mon_days[i];
    }

    tdays += ltm->tm_mday - 1; // days of month passed.
    tdays = tdays + (tyears * 365) + leaps;

    utc_hrs = ltm->tm_hour + utcdiff; // for your time zone.
    return (tdays * 86400) + (utc_hrs * 3600) + (ltm->tm_min * 60) + ltm->tm_sec;
}

bool chk_stk_date_legal(time_date_t date)
{
    const int mon_days_leap[] =
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    int year = date / 10000;
    int month = date % 10000;
    int day = month % 100;
    month = month / 100;

    if (year >= 1990 && year < 2030) {
        if (month >= 1 && month <= 12) {
            if (day >= 1 && day <= mon_days_leap[month - 1]) {
                return true;
            }
        }
    }
    return false;
}


std::string ms_to_str(uint64_t ms)
{
    time_t utc = ms / 1000;
    int _ms = (int)(ms % 1000);
    utc += 3600 *8;
    char s[64];
    //struct tm ltm = *gmtime(&utc);  //fix asia:shanghai inside
    struct tm ltm;
    if (0 != __offtime(utc, 0, &ltm))   //fail
        return "00_00_00.0";
    strftime(s, 64, "%H_%M_%S", &ltm);
    sprintf(s + strlen(s), "'%d", _ms);
    return (s);
}

std::string ms_to_str_r(uint64_t ms)
{
    time_t utc = ms / 1000;
    int _ms = (int)(ms % 1000);
    utc += 3600 *8;
    char s[64];
    //struct tm ltm = *gmtime(&utc);  //fix asia:shanghai inside
    struct tm ltm;
    if (0 != __offtime(utc, 0, &ltm))   //fail
        return "00:00:00.0";
    strftime(s, 64, "%H:%M:%S", &ltm);
    sprintf(s + strlen(s), ".%d", _ms);
    return (s);

}


/**** L2 帧数据中的时间戳转换工具 ***/
std::string ssz_L2_timeString(uint64_t TransactTime)
{
    int ms = int(TransactTime % 1000);
    int ss = int((TransactTime % (1000 * 100)) / 1000);
    int mm = int((TransactTime % (1000 * 100 * 100)) / (1000 * 100));
    int hh = int((TransactTime % (1000 * 100 * 100 * 100)) / (1000 * 100 * 100));
    char s[128];
    sprintf(s, "%02d:%02d:%02d.%d", hh, mm, ss, ms);
    return s;
}

std::string ssh_L2_timeString(uint64_t TransactTime)
{
    int ms = int(TransactTime % 100);
    int ss = int((TransactTime % (100 * 100)) / 100);
    int mm = int((TransactTime % (100 * 100 * 100)) / (100 * 100));
    int hh = int((TransactTime % (100 * 100 * 100 * 100)) / (100 * 100 * 100));
    char s[128];
    sprintf(s, "%02d:%02d:%02d.%d", hh, mm, ss, ms);
    return s;
}
std::string ssh_L1Bin_timeString(uint64_t TransactTime)
{
    int ms = int(TransactTime % 1000);
    int ss = int((TransactTime % (100 * 1000)) / 1000);
    int mm = int((TransactTime % (100 * 100 * 1000)) / (100 * 1000));
    int hh = int((TransactTime % (100 * 100 * 100 * 1000)) / (100 * 100 * 1000));
    char s[128];
    sprintf(s, "%02d:%02d:%02d.%d", hh, mm, ss, ms);
    return s;
}

rptTime::rptTime() :
    m_target(""),
    _rptGate(-1)
{
    ftime(&tb);
}

rptTime::rptTime(const std::string& target) :
    m_target(target),
    _rptGate(-1)
{
    ftime(&tb);
}

rptTime::rptTime(const std::string& target, double rptGate) :
    m_target(target),
    _rptGate(rptGate)
{
    ftime(&tb);
}
rptTime::~rptTime() {
    timeb te;
    ftime(&te);
    uint64_t t0 = tb.time * 1000 + tb.millitm;
    uint64_t t1 = te.time * 1000 + te.millitm;
    double dlt = (double)(t1 - t0) / 1000.0;
    if(dlt>=_rptGate)
        INFO("{} used {:.3f} seconds.", m_target, dlt);
}

void rptTime::estimate(size_t now, size_t size, size_t gap)
{
    if (now==0 || now%gap != 0)
        return;
    timeb te;
    ftime(&te);
    uint64_t t0 = tb.time * 1000 + tb.millitm;
    uint64_t t1 = te.time * 1000 + te.millitm;
    double dlt = (double)(t1 - t0) / 1000.0;
    double tall = dlt * size / now;
    double tres = tall - dlt;
    INFO("{} progress:  [ {:d}/{:d} ]  used={:0.2f}s  ET(all={:0.2f}s; res={:0.2f}s)", m_target, now, size, dlt, tall, tres);
}

bool isDealTime_stock(time_minuSec_t minuSec, mktCodeI_t mktCode)
{
    //TODO: snapshot's time is not so precise.
    if (minuSec >= 91500 && minuSec <= 92500) {
        return true;
    }
    if (minuSec >= 93000 && minuSec <= 113000) {
        return true;
    }
    if (minuSec >= 130000 && minuSec <= 150000) {
        return true;
    }
    return false;
}

}//namespace S4
