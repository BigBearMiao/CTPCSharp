
#include "stdafx.h"
#include "MdApi.h"
#include <string>


using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

namespace CTPCLR
{
	public ref class MdApiclr
	{
	private:

		System::Collections::Generic::List<GCHandle> ^ gchlist = gcnew 	System::Collections::Generic::List<GCHandle>();

		std::string MarshalString(String ^ s) {
			using namespace Runtime::InteropServices;
			const char* chars =
				(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
			std::string os = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
			return os;
		}

		CMdSpi *pUserSpi = nullptr;
		CThostFtdcMdApi * pUserApi = nullptr;
		int lRequestID = 0;

	public:


		MdApiclr(System::String ^_investor, System::String ^_pwd, System::String ^_broker,
			System::String ^_addr, System::String ^_FlowPath)
		{
			FrontAddr = _addr;

			pUserSpi = new CMdSpi();
			// ��ʼ��UserApi
			pUserApi = CThostFtdcMdApi::CreateFtdcMdApi(MarshalString(_FlowPath).c_str());		// ����UserApi

			EventInit();

			pUserApi->RegisterSpi(pUserSpi);						// ע���¼���
			pUserApi->RegisterFront((char*)MarshalString(FrontAddr).c_str());					// connect
			pUserApi->Init();
		}

		virtual ~MdApiclr(){

			for each (GCHandle var in gchlist)
			{
				var.Free();
			}
		}

		/// <summary>
		/// ǰ�õ�ַ
		/// </summary>
		property  System::String ^ FrontAddr;


		/// <summary>
		///�������顣
		/// </summary>
		int SubscribeMarketData(array<String^>^ ppInstrumentID, int nCount)
		{
			char** ppi = new char*[nCount];
			System::Collections::Generic::List<IntPtr> ^ ppIntPtrs = gcnew 	System::Collections::Generic::List<IntPtr>();
			try
			{
				for (int i = 0; i < nCount; i++)
				{
					IntPtr tp = Marshal::StringToHGlobalAnsi(ppInstrumentID[i]);
					ppIntPtrs->Add(tp);
					ppi[i] = static_cast<char*>(tp.ToPointer());
				}
				return pUserApi->SubscribeMarketData(ppi, nCount);
			}
			finally
			{
				for each (IntPtr var in ppIntPtrs)
				{
					Marshal::FreeHGlobal(var);
				}
			}
		}


		/// <summary>
		///�˶����顣
		/// </summary>
		int UnSubscribeMarketData(array<String^>^ ppInstrumentID, int nCount)
		{
			char** ppi = new char*[nCount];
			System::Collections::Generic::List<IntPtr> ^ ppIntPtrs = gcnew 	System::Collections::Generic::List<IntPtr>();
			try
			{
				for (int i = 0; i < nCount; i++)
				{
					IntPtr tp = Marshal::StringToHGlobalAuto(ppInstrumentID[i]);
					ppIntPtrs->Add(tp);
					ppi[i] = static_cast<char*>(tp.ToPointer());
				}
				return pUserApi->UnSubscribeMarketData(ppi, nCount);
			}
			finally
			{
				for each (IntPtr var in ppIntPtrs)
				{
					Marshal::FreeHGlobal(var);
				}
			}
		}

		/// <summary>
		///�û���¼����
		/// </summary>
		int ReqUserLogin(CTPCommon::CThostFtdcReqUserLoginField pReqUserLoginField, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pReqUserLoginField);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pReqUserLoginField, bufferIntPtr, false);
				CThostFtdcReqUserLoginField* temp = static_cast<CThostFtdcReqUserLoginField*>(bufferIntPtr.ToPointer());
				return pUserApi->ReqUserLogin(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�ǳ�����
		/// </summary>
		int ReqUserLogout(CTPCommon::CThostFtdcUserLogoutField pUserLogout, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pUserLogout);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pUserLogout, bufferIntPtr, false);
				CThostFtdcUserLogoutField* temp = static_cast<CThostFtdcUserLogoutField*>(bufferIntPtr.ToPointer());
				return pUserApi->ReqUserLogout(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		void EventInit()
		{

			{
				cppFrontConnect^ fp = gcnew cppFrontConnect(this, &MdApiclr::cppcbFrontConnect);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBOnFrontConnected cb = static_cast<CBOnFrontConnected>(ip.ToPointer());
				pUserSpi->cbOnFrontConnected = cb;
			}

		{
			cppDisConnected^ fp = gcnew cppDisConnected(this, &MdApiclr::cppcbDisConnected);
			GCHandle gch = GCHandle::Alloc(fp);
			gchlist->Add(gch);
			IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
			CBOnFrontDisconnected cb = static_cast<CBOnFrontDisconnected>(ip.ToPointer());
			pUserSpi->cbOnFrontDisconnected = cb;
		}
		{
			cppRspUserLogin^ fp = gcnew cppRspUserLogin(this, &MdApiclr::cppcbRspUserLogin);
			GCHandle gch = GCHandle::Alloc(fp);
			gchlist->Add(gch);
			IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
			CBOnRspUserLogin cb = static_cast<CBOnRspUserLogin>(ip.ToPointer());
			pUserSpi->cbOnRspUserLogin = cb;
		}
		{
			cppRspUserLogout^ fp = gcnew cppRspUserLogout(this, &MdApiclr::cppcbRspUserLogout);
			GCHandle gch = GCHandle::Alloc(fp);
			gchlist->Add(gch);
			IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
			CBOnRspUserLogout cb = static_cast<CBOnRspUserLogout>(ip.ToPointer());
			pUserSpi->cbOnRspUserLogout = cb;
		}
		{
			cppOnRtnDepthMarketData^ fp = gcnew cppOnRtnDepthMarketData(this, &MdApiclr::cppcbRtnDepthMarketData);
			GCHandle gch = GCHandle::Alloc(fp);
			gchlist->Add(gch);
			IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
			CBOnRtnDepthMarketData cb = static_cast<CBOnRtnDepthMarketData>(ip.ToPointer());
			pUserSpi->cbOnRtnDepthMarketData = cb;
		}

		}


	public:
		delegate void FrontConnect();
	private:
		delegate void cppFrontConnect();
		void cppcbFrontConnect()
		{

			OnFrontConnect();
		}
	public:
		/// <summary>
		/// ���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
		/// </summary>
		event FrontConnect ^OnFrontConnect;


	public:
		delegate void DisConnected(int reason);
	private:
		delegate void cppDisConnected(int reason);
		void cppcbDisConnected(int reason)
		{

			OnDisConnected(reason);
		}
	public:
		/// <summary>
		/// ���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
		/// </summary>
		event DisConnected ^OnDisConnected;



	public:
		delegate void RspUserLogin(CTPCommon::CThostFtdcRspUserLoginField pRspUserLogin, CTPCommon::CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
	private:
		delegate void cppRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcRspUserLoginField  cspRspUserLogin = (CTPCommon::CThostFtdcRspUserLoginField) Marshal::PtrToStructure(IntPtr(pRspUserLogin), CTPCommon::CThostFtdcRspUserLoginField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(pRspInfo), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspUserLogin(cspRspUserLogin, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// ��¼������Ӧ
		/// </summary>
		event RspUserLogin ^OnRspUserLogin;


	public:
		delegate void RspUserLogout(CTPCommon::CThostFtdcUserLogoutField pUserLogout, CTPCommon::CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
	private:
		delegate void cppRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcUserLogoutField  cspUserLogout = (CTPCommon::CThostFtdcUserLogoutField) Marshal::PtrToStructure(IntPtr(pUserLogout), CTPCommon::CThostFtdcUserLogoutField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(pRspInfo), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspUserLogout(cspUserLogout, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �ǳ�������Ӧ
		/// </summary>
		event RspUserLogout ^OnRspUserLogout;




	public:
		delegate void RtnDepthMarketData(CTPCommon::CThostFtdcDepthMarketDataField pDepthMarketData);
	private:
		delegate void cppOnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
		void cppcbRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
		{
			CTPCommon::CThostFtdcDepthMarketDataField  cspDepthMarketData = (CTPCommon::CThostFtdcDepthMarketDataField) Marshal::PtrToStructure(IntPtr(pDepthMarketData), CTPCommon::CThostFtdcDepthMarketDataField::typeid);
			OnRtnDepthMarketData(cspDepthMarketData);
		}
	public:
		/// <summary>
		/// �ڻ����������ʽ�ת�ڻ�����ر�
		/// </summary>
		event RtnDepthMarketData ^OnRtnDepthMarketData;

	};
}