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
* Copyright (C) 2013 YaweiZhang <yawei_zhang@foxmail.com>.
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
#ifndef _ZSUMMERX_UDPSOCKET_IMPL_H_
#define _ZSUMMERX_UDPSOCKET_IMPL_H_

#include "common_impl.h"
#include "../zsummer.h"

namespace zsummer
{
	namespace network
	{
		class CUdpSocketImpl
		{
		public:
			CUdpSocketImpl();
			~CUdpSocketImpl();
			bool Initialize(CZSummerPtr summer, const char *localIP, unsigned short localPort);
			bool DoSend(char * buf, unsigned int len, const char *dstip, unsigned short dstport);
			bool OnIOCPMessage(BOOL bSuccess, DWORD dwTranceCount, unsigned char cType);
			bool DoRecv(char * buf, unsigned int len, const _OnRecvFromHandler &handler);
		public:
			//private
			CZSummerPtr m_summer;
			
			SOCKET		m_socket;
			SOCKADDR_IN	m_addr;

			//recv
			tagReqHandle m_recvHandle;
			WSABUF		 m_recvWSABuf;
			sockaddr_in  m_recvFrom;
			int			 m_recvFromLen;
			_OnRecvFromHandler m_onRecvHander;
			bool		 m_recvLock;

			LINK_STATUS m_nLinkStatus;
		};
	}
}
























#endif











