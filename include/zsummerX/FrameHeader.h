/*
 * zsummerX License
 * -----------
 * 
 * zsummerX is licensed under the terms of the MIT license reproduced below.
 * This means that zsummerX is free software and can be used for both academic
 * and commercial purposes at absolutely no cost.
 * 
 * 
 * ===============================================================================
 * 
 * Copyright (C) 2010-2014 YaweiZhang <yawei_zhang@foxmail.com>.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * ===============================================================================
 * 
 * (end of COPYRIGHT)
 */


//! 公共头文件

#ifndef ZSUMMER_HEADER_H_
#define ZSUMMER_HEADER_H_

#include <zsummerX/zsummer.h>
#include <zsummerX/tcpaccept.h>
#include <zsummerX/tcpsocket.h>
#include <zsummerX/udpsocket.h>

#include <iostream>
#include <queue>
#include <iomanip>
#include <string.h>
#include <signal.h>
#include <unordered_map>
#include <log4z/log4z.h>
#include <proto4z/proto4z.h>
using namespace std;

//! frame封装在网络部分使用单例模式, 如果需要在zsummerX的网络部分使用多线程 请参考tcpTest实例调用zsummerX的原始接口实现.


typedef unsigned int SessionID;
const SessionID InvalidSeesionID = -1;
typedef unsigned int AccepterID;
const AccepterID InvalidAccepterID = -1;
typedef unsigned int ConnectorID;
const ConnectorID InvalidConnectorID = -1;
typedef unsigned short ProtocolID;
const ProtocolID InvalidProtocolID = -1;


enum ProtoType
{
	PT_TCP,
	PT_HTTP,
};

//! 监听器配置
struct tagAcceptorConfigTraits
{
	AccepterID aID = InvalidAccepterID;
	std::string listenIP = "0.0.0.0";
	unsigned short listenPort = 81;
	ProtoType protoType = PT_TCP;
	std::string rc4TcpEncryption = ""; //empty is not encryption
	unsigned int pulseInterval = 30000;
	unsigned int maxSessions = 5000;
	std::vector<std::string> whitelistIP;

};

struct tagAcceptorInfo
{
	unsigned long long totalAcceptCount = 0; //累计接收客户端连接请求
	unsigned long long currentLinked = 0; //当前客户端并发个数
};



//连接器配置
struct tagConnctorConfigTraits
{
	ConnectorID cID = InvalidConnectorID;
	std::string remoteIP = "127.0.0.1";
	unsigned short remotePort = 81;
	ProtoType protoType = PT_TCP;
	std::string rc4TcpEncryption = ""; //empty is not encryption
	unsigned int pulseInterval = 30000;
	unsigned int reconnectMaxCount = 0; // try reconnect max count
	unsigned int reconnectInterval =5000; //million seconds;
	bool         reconnectCleanAllData = true ;//clean all data when reconnect;
};

struct tagConnctorInfo
{
	unsigned long long totalConnectCount = 0; //累计连接次数
	unsigned long long curReconnectCount = 0; //当前重连次数
};




//类型定义
//----------------------------------------
class CTcpSession;

typedef std::shared_ptr<zsummer::network::CTcpSocket> CTcpSocketPtr;
typedef std::shared_ptr<zsummer::network::CTcpAccept> CTcpAcceptPtr;
typedef std::shared_ptr<zsummer::network::CZSummer> CZSummerPtr;
typedef std::shared_ptr<CTcpSession> CTcpSessionPtr;


//public method
#define  NOW_TIME (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())

//接收包缓冲大小
const unsigned int SEND_RECV_CHUNK_SIZE = 64 * 1024 -1;





//包头特性
struct FrameStreamTraits
{
	typedef unsigned short Integer;
	const static Integer PreOffset = 0;  
	const static Integer PostOffset = 0;  
	const static Integer MaxPackLen = (Integer)16*1024; 
	const static bool	 PackLenIsContainHead = true; 
	const static zsummer::proto4z::ZSummer_EndianType EndianType = zsummer::proto4z::LittleEndian;
	const static Integer IntegerTypeSize = sizeof(Integer);
	const static Integer HeadLen = PreOffset + IntegerTypeSize + PostOffset; 
};




typedef zsummer::proto4z::ReadStream<FrameStreamTraits> ReadStreamPack;
typedef zsummer::proto4z::WriteStream<FrameStreamTraits> WriteStreamPack;

//!注册原始消息的封包 如果存在回调并且回调的返回值为false则直接处理下一个封包.
typedef std::function < bool(AccepterID, SessionID, const char * /*blockBegin*/, typename FrameStreamTraits::Integer /*blockSize*/) > OnSessionOrgMessageFunction;
typedef std::function < bool(ConnectorID, const char * /*blockBegin*/, typename FrameStreamTraits::Integer /*blockSize*/) > OnConnectorOrgMessageFunction;

//!注册消息
typedef std::function < void(AccepterID, SessionID, ProtocolID, ReadStreamPack &) > OnSessionMessageFunction;
typedef std::function < void(ConnectorID, ProtocolID, ReadStreamPack &) > OnConnectorMessageFunction;

//注册事件
typedef std::function < void(AccepterID, SessionID) > OnSessionEstablished;
typedef std::function < void(ConnectorID) > OnConnectorEstablished;

//注册事件
typedef std::function < void(AccepterID, SessionID) > OnSessionDisconnect;
typedef std::function < void(ConnectorID) > OnConnectorDisconnect;

//注册HTTP消息
typedef std::function < bool(AccepterID, SessionID, const zsummer::proto4z::HTTPHeadMap& /*head*/, const std::string & /*body*/) > OnSessionHTTPMessageFunction;

typedef std::function < bool(ConnectorID, const zsummer::proto4z::HTTPHeadMap & /*head*/, const std::string & /*body*/) > OnConnectorHTTPMessageFunction;


//注册心跳
typedef std::function < void(AccepterID, SessionID, unsigned int/*pulse interval*/) > OnSessionPulseTimer;
typedef std::function < void(ConnectorID, unsigned int/*pulse interval*/) > OnConnectorPulseTimer;


//! print log
template<class OS>
OS & operator <<(OS & os, const tagAcceptorConfigTraits & traits)
{
	os << "[AccepterID=" << traits.aID << "; listenIP=" << traits.listenIP << "; listenPort=" << traits.listenPort
		<< "; maxSessions=" << traits.maxSessions << "; whitelistIP=";
	for (auto x : traits.whitelistIP)
	{
		os << x << ",";
	}
	os << "]";
	return os;
}

//! print log
template<class OS>
OS & operator <<(OS & os, const tagConnctorConfigTraits & traits)
{
	os << "[ConnectorID=" << traits.cID << "; remoteIP=" << traits.remoteIP << "; remotePort=" << traits.remotePort
		<< "; reconnectMaxCount=" << traits.reconnectMaxCount << "; reconnectInterval=" << traits.reconnectInterval;
	return os;
}

#endif

