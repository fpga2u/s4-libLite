﻿#include "network/L2_udp_recver_th_native.h"
#include "queue/shared_array.h"
#include "common/s4time.h"
#include "sbe_ssz.h"
#include "sbe_ssh.h"
#include "common/s4logger.h"

CREATE_LOCAL_LOGGER("L2Udp-Native")

namespace S4
{
namespace NW
{

bool L2_udp_recver_th_native::start(const char* pMultiCastIp, const char* pLocalIp, const uint16_t port, bool UDPlite)
{
    std::lock_guard<std::mutex> l(_mux);
    if (_pThread)
        return false;   //no dual start

    memset(&_stats, 0, sizeof(_stats));
    _localIp.clear();
    _multicastIp.clear();
    int rc;
#ifdef WIN32
    _fd = SockUtil::bindUdpSock(port, pLocalIp, UDPlite);
    LCL_INFO("created fd={} at {}:{}  udp-lite={}", _fd, pLocalIp, port, UDPlite);
    if (SockUtil::isMulticastAddress(pMultiCastIp)){
        rc = SockUtil::joinMultiAddr(_fd, pMultiCastIp);
        if (rc != 0) {
            LCL_ERR("joinMulticast {} fail = {}!", pMultiCastIp, rc);
            return false;
        }
	    LCL_INFO("joined Multicast {}.", pMultiCastIp);
    }
#else
    if (SockUtil::isMulticastAddress(pMultiCastIp)){
        _fd = SockUtil::bindUdpSock(port, "0.0.0.0", UDPlite);  //Linux 组播若要绑定网卡需通过route命令
        if (_fd<0){
            LCL_ERR("bindUdpSock {} fail = {}!", pMultiCastIp, _fd);
            return false;
        }
            
        _localIp = pLocalIp;
        _multicastIp = pMultiCastIp;
        
        rc = SockUtil::addRouteViaIP(pMultiCastIp, pLocalIp);
        if (rc != 0){
            LCL_ERR("addRouteViaIP {} err = {}!", pMultiCastIp, rc);
            _localIp.clear();
            _multicastIp.clear();
            return false;
        }

        rc = SockUtil::joinMultiAddr(_fd, pMultiCastIp);
        if (rc != 0) {
            LCL_ERR("joinMulticast {} fail = {}!", pMultiCastIp, rc);
            return false;
        }
	    LCL_INFO("joined Multicast {}  udp-lite={}.", pMultiCastIp, UDPlite);
    }else {
        _fd = SockUtil::bindUdpSock(port, pLocalIp, UDPlite);
        LCL_INFO("created fd={} at {}:{}  udp-lite={}", _fd, pLocalIp, port, UDPlite);
    }
#endif
	if (_fd < 0){
		return false;
    }

    _localIp = pLocalIp;
    _multicastIp = pMultiCastIp;

    if (SockUtil::setRecvBuf(_fd, 16*1024*1024) != 0){
        LCL_ERR("setRecvBuf fail!");
    }
    

    _pThread = std::make_shared<std::thread>(
        [&](){
            recv_thread();
        }
    );

    return true;
}

bool L2_udp_recver_th_native::stop()
{
    std::lock_guard<std::mutex> l(_mux);
    if (_pThread){
        _stop = true;
        _pThread->join();
        _pThread.reset();
        _stop = false;

        if (_multicastIp.size() && SockUtil::delRouteViaIP(_multicastIp.c_str(), _localIp.c_str()) != 0){
            LCL_ERR("delRouteViaIP {} err = {}!", _multicastIp, SockUtil::getSockError(_fd));
        }
        _fd = -1;
    }
    return true;
}

void L2_udp_recver_th_native::recv_thread()
{

    LCL_INFO("START");

    char_array_t recv_buffer(4096);
    int recv_len;
    do {
        recv_len = recv(_fd, recv_buffer.get(), (int)recv_buffer.size(), 0);
        if (recv_len > 0){
            _stats.recv_frame_cnt++;
            _stats.last_frame_time_ms = nowTime_ms();

            //统计类型，检查是否需要转发
            int len = recv_len;
            const char* p = recv_buffer.get();
            
            do{
                const SBE_SSH_header_t* pH = (const SBE_SSH_header_t*)p;
                switch (pH->MsgType)
                {
                case __MsgType_SSH_INSTRUMENT_SNAP__:
                    if ((pH->SecurityIDSource == __SecurityIDSource_SSH_ && pH->MsgLen == sizeof(SBE_SSH_instrument_snap_t) && len >= (int)sizeof(SBE_SSH_instrument_snap_t)) ||
                        (pH->SecurityIDSource == __SecurityIDSource_SSZ_ && pH->MsgLen == sizeof(SBE_SSZ_instrument_snap_t) && len >= (int)sizeof(SBE_SSZ_instrument_snap_t))) {
                        _stats.recv_instrument_snap_cnt++;
                        _stats.live_instrument_snap_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        p += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_SSH_INDEX_SNAP__:
                    if ((pH->SecurityIDSource == __SecurityIDSource_SSH_ && pH->MsgLen == sizeof(SBE_SSH_index_snap_t) && len >= (int)sizeof(SBE_SSH_index_snap_t)) ||
                        (pH->SecurityIDSource == __SecurityIDSource_SSZ_ && pH->MsgLen == sizeof(SBE_SSZ_index_snap_t) && len >= (int)sizeof(SBE_SSZ_index_snap_t))) {
                        _stats.recv_index_snap_cnt++;
                        _stats.live_index_snap_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        p += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_SSH_ORDER__:
                    if ((pH->SecurityIDSource == __SecurityIDSource_SSH_ && pH->MsgLen == sizeof(SBE_SSH_ord_t) && len >= (int)sizeof(SBE_SSH_ord_t)) ||
                        (pH->SecurityIDSource == __SecurityIDSource_SSZ_ && pH->MsgLen == sizeof(SBE_SSZ_ord_t) && len >= (int)sizeof(SBE_SSZ_ord_t))) {
                        _stats.recv_order_cnt++;
                        _stats.live_order_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        p += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_SSH_EXECUTION__:
                    if ((pH->SecurityIDSource == __SecurityIDSource_SSH_ && pH->MsgLen == sizeof(SBE_SSH_exe_t) && len >= (int)sizeof(SBE_SSH_exe_t)) ||
                        (pH->SecurityIDSource == __SecurityIDSource_SSZ_ && pH->MsgLen == sizeof(SBE_SSZ_exe_t) && len >= (int)sizeof(SBE_SSZ_exe_t))) {
                        _stats.recv_exec_cnt++;
                        _stats.live_exec_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        p += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_SSH_INSTRUMENT_SNAPx5__:
                    if ((pH->SecurityIDSource == __SecurityIDSource_SSH_ && pH->MsgLen == sizeof(SBE_SSH_instrument_snapx5_t) && len >= (int)sizeof(SBE_SSH_instrument_snapx5_t))) {
                        _stats.recv_instrument_snapx5_cnt++;
                        _stats.live_instrument_snapx5_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        p += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_SSH_FUND_SNAPx5__:
                    if ((pH->SecurityIDSource == __SecurityIDSource_SSH_ && pH->MsgLen == sizeof(SBE_SSH_fund_snapx5_t) && len >= (int)sizeof(SBE_SSH_fund_snapx5_t))) {
                        _stats.recv_fund_snapx5_cnt++;
                        _stats.live_fund_snapx5_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        p += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_SSH_INDEX_SNAPx5__:
                    if ((pH->SecurityIDSource == __SecurityIDSource_SSH_ && pH->MsgLen == sizeof(SBE_SSH_index_snapx5_t) && len >= (int)sizeof(SBE_SSH_index_snapx5_t))) {
                        _stats.recv_index_snapx5_cnt++;
                        _stats.live_index_snapx5_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        p += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_SSH_OPTION_SNAPx5__:
                    if ((pH->SecurityIDSource == __SecurityIDSource_SSH_ && pH->MsgLen == sizeof(SBE_SSH_option_snapx5_t) && len >= (int)sizeof(SBE_SSH_option_snapx5_t))) {
                        _stats.recv_option_snapx5_cnt++;
                        _stats.live_option_snapx5_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        p += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_HEARTBEAT__:
                    if ((pH->SecurityIDSource == __SecurityIDSource_SSH_ || pH->SecurityIDSource == __SecurityIDSource_SSZ_) && pH->MsgLen == sizeof(SBE_SSH_header_t) && len >= (int)sizeof(SBE_SSH_header_t)){
                        _stats.recv_heartbeat_cnt++;
                        len -= sizeof(SBE_SSH_header_t);
                        p += sizeof(SBE_SSH_header_t);
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                
                default:
                    _stats.recv_unknown_cnt++;
                    len = 0;
                    break;
                }
            }while(len > 0);
        }else if(recv_len == -1){
            report_stats();
            proc_cmdQ();
        }else if (recv_len < -1){
            break;
        }
    } while (!_stop);
    close(_fd);
    report_stats(true);

    LCL_INFO("STOP");

}

bool L2_udp_recver_th_native::liveTrans(char* pH)
{
    SBE_SSH_header_t* pHeader = (SBE_SSH_header_t*)pH;
    mktCodeI_t code;
    code = atoi((pHeader)->SecurityID);
    if ((pHeader)->SecurityIDSource == __SecurityIDSource_SSH_){
        code += SH_PRB;
    }else{
        code += SZ_PRB;
    }

    if (_live_list.count(code)){
        L2Data_arPtr_t p;
        _pL2DataQ->P_get_tryBest(p);
        p->pQdata->info.data_len = (pHeader)->MsgLen;
        p->pQdata->info.local_time_ms = _stats.last_frame_time_ms;
        p->pQdata->info.type = L2DataType::MARKET_DATA;
        memcpy(p->pQdata->pBuffer, pH, (pHeader)->MsgLen);
        return true;
    }
    return false;
}

void L2_udp_recver_th_native::proc_cmdQ()
{
    if (!_pCmdQ || _pCmdQ->size_approx() == 0)
        return;

    std::vector<std::shared_ptr<live_cmd_t>> cmds(8);
    size_t s = _pCmdQ->try_dequeue_bulk(*_pCtok_cmdQ, cmds.begin(), 8);
    size_t n = 0;
    for (auto& cmd : cmds){
        if (cmd->add){
            _live_list.insert(cmd->code);
        }else{
            _live_list.erase(cmd->code);
        }
        if (++n >= s)break;
    }
    _stats.live_code_nb = _live_list.size();
    
}


} // namespace NW
} // namespace S4
