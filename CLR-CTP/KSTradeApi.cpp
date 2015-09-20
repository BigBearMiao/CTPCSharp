#include "stdafx.h"
#include "KSTradeApi.h"
#include "ICTPTradeApi.h"
#include <string>


namespace CTPTradeApi
{
#include "../api/ThostFtdcTraderApi.h"
}

#include "TradeApiStructChange.h"
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

namespace CTPCLR
{
	public ref class KSTradeApiclr :ICTPTradeApi
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


	public:


		KSTradeApiclr(System::String ^_investor, System::String ^_pwd, System::String ^_broker,
			System::String ^_addr, System::String ^_FlowPath)
		{
			FrontAddr = _addr;
			BrokerID = _broker;
			InvestorID = _investor;
			passWord = _pwd;
			FlowPath = _FlowPath;

			pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(MarshalString(_FlowPath).c_str());			// ����UserApi
			pUserSpi = new KSTraderSpi();
			pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
			pUserApi->SubscribePublicTopic(THOST_TERT_QUICK/*THOST_TERT_RESTART*/);					// ע�ṫ����
			pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK/*THOST_TERT_RESTART*/);					// ע��˽����
			pUserApi->RegisterFront((char*)MarshalString(_addr).c_str());							// connect
			EventInit();
			pUserApi->Init();
		}

		virtual ~KSTradeApiclr() {

			for each (GCHandle var in gchlist)
			{
				var.Free();
			}
		}

		/// <summary>
		/// ǰ�õ�ַ
		/// </summary>
		virtual property  System::String ^ FrontAddr;
		/// <summary>
		/// ���͹�˾����
		/// </summary>
		virtual property  System::String ^ BrokerID;
		/// <summary>
		/// Ͷ���ߴ���
		/// </summary>
		virtual property  System::String ^ InvestorID;
		virtual property  System::String ^ passWord;
		virtual property  System::String ^ FlowPath;

		/// <summary>
		/// ǰ�ñ��
		/// </summary>
		virtual property int FrontID;

		/// <summary>
		/// �Ự���
		/// </summary>
		virtual property int SessionID;

		/// <summary>
		/// ��󱨵�����
		/// </summary>
		virtual property int MaxOrderRef;


		//Ͷ���߽�����ȷ��
		virtual int SettlementInfoConfirm([Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			CThostFtdcSettlementInfoConfirmField req;
			memset(&req, 0, sizeof(req));
			strcpy_s(req.BrokerID, MarshalString(BrokerID).c_str());
			strcpy_s(req.InvestorID, MarshalString(InvestorID).c_str());
			return pUserApi->ReqSettlementInfoConfirm(&req, nRequestID);
		}

		/// <summary>
		///����¼������
		/// </summary>
		virtual int ReqOrderInsert(CTPCommon::CThostFtdcInputOrderField pInputOrder, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			pInputOrder.RequestID = nRequestID;
			int size = Marshal::SizeOf(pInputOrder);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pInputOrder, bufferIntPtr, false);
				CThostFtdcInputOrderField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcInputOrderField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqOrderInsert(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///������������
		/// </summary>
		virtual int ReqOrderAction(CTPCommon::CThostFtdcInputOrderActionField pInputOrderAction, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			pInputOrderAction.RequestID = nRequestID;
			int size = Marshal::SizeOf(pInputOrderAction);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pInputOrderAction, bufferIntPtr, false);
				CThostFtdcInputOrderActionField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcInputOrderActionField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqOrderAction(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		//�Զ�����------------------------------------------------------------------------

		/// <summary>
		///�ͻ�����֤����
		/// </summary>
		virtual int ReqAuthenticate(CTPCommon::CThostFtdcReqAuthenticateField pReqAuthenticateField, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pReqAuthenticateField);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pReqAuthenticateField, bufferIntPtr, false);
				CThostFtdcReqAuthenticateField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcReqAuthenticateField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqAuthenticate(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�û���¼����
		/// </summary>
		virtual int ReqUserLogin(CTPCommon::CThostFtdcReqUserLoginField pReqUserLoginField, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pReqUserLoginField);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pReqUserLoginField, bufferIntPtr, false);
				CThostFtdcReqUserLoginField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcReqUserLoginField*>(bufferIntPtr.ToPointer()));
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
		virtual int ReqUserLogout(CTPCommon::CThostFtdcUserLogoutField pUserLogout, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pUserLogout);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pUserLogout, bufferIntPtr, false);
				CThostFtdcUserLogoutField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcUserLogoutField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqUserLogout(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�û������������
		/// </summary>
		virtual int ReqUserPasswordUpdate(CTPCommon::CThostFtdcUserPasswordUpdateField pUserPasswordUpdate, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pUserPasswordUpdate);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pUserPasswordUpdate, bufferIntPtr, false);
				CThostFtdcUserPasswordUpdateField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcUserPasswordUpdateField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqUserPasswordUpdate(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�ʽ��˻������������
		/// </summary>
		virtual int ReqTradingAccountPasswordUpdate(CTPCommon::CThostFtdcTradingAccountPasswordUpdateField pTradingAccountPasswordUpdate, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pTradingAccountPasswordUpdate);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pTradingAccountPasswordUpdate, bufferIntPtr, false);
				CThostFtdcTradingAccountPasswordUpdateField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcTradingAccountPasswordUpdateField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqTradingAccountPasswordUpdate(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}



		/// <summary>
		///Ԥ��¼������
		/// </summary>
		virtual int ReqParkedOrderInsert(CTPCommon::CThostFtdcParkedOrderField pParkedOrder, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pParkedOrder);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pParkedOrder, bufferIntPtr, false);
				CThostFtdcParkedOrderField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcParkedOrderField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqParkedOrderInsert(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///Ԥ�񳷵�¼������
		/// </summary>
		virtual int ReqParkedOrderAction(CTPCommon::CThostFtdcParkedOrderActionField pParkedOrderAction, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pParkedOrderAction);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pParkedOrderAction, bufferIntPtr, false);
				CThostFtdcParkedOrderActionField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcParkedOrderActionField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqParkedOrderAction(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}



		/// <summary>
		///��ѯ��󱨵���������
		/// </summary>
		virtual int ReqQueryMaxOrderVolume(CTPCommon::CThostFtdcQueryMaxOrderVolumeField pQueryMaxOrderVolume, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQueryMaxOrderVolume);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQueryMaxOrderVolume, bufferIntPtr, false);
				CThostFtdcQueryMaxOrderVolumeField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQueryMaxOrderVolumeField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQueryMaxOrderVolume(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///Ͷ���߽�����ȷ��
		/// </summary>
		virtual int ReqSettlementInfoConfirm(CTPCommon::CThostFtdcSettlementInfoConfirmField pSettlementInfoConfirm, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pSettlementInfoConfirm);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pSettlementInfoConfirm, bufferIntPtr, false);
				CThostFtdcSettlementInfoConfirmField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcSettlementInfoConfirmField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqSettlementInfoConfirm(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///����ɾ��Ԥ��
		/// </summary>
		virtual int ReqRemoveParkedOrder(CTPCommon::CThostFtdcRemoveParkedOrderField pRemoveParkedOrder, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pRemoveParkedOrder);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pRemoveParkedOrder, bufferIntPtr, false);
				CThostFtdcRemoveParkedOrderField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcRemoveParkedOrderField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqRemoveParkedOrder(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///����ɾ��Ԥ�񳷵�
		/// </summary>
		virtual int ReqRemoveParkedOrderAction(CTPCommon::CThostFtdcRemoveParkedOrderActionField pRemoveParkedOrderAction, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pRemoveParkedOrderAction);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pRemoveParkedOrderAction, bufferIntPtr, false);
				CThostFtdcRemoveParkedOrderActionField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcRemoveParkedOrderActionField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqRemoveParkedOrderAction(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ����
		/// </summary>
		virtual int ReqQryOrder(CTPCommon::CThostFtdcQryOrderField pQryOrder, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryOrder);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryOrder, bufferIntPtr, false);
				CThostFtdcQryOrderField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryOrderField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryOrder(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ�ɽ�
		/// </summary>
		virtual int ReqQryTrade(CTPCommon::CThostFtdcQryTradeField pQryTrade, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryTrade);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryTrade, bufferIntPtr, false);
				CThostFtdcQryTradeField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryTradeField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryTrade(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯͶ���ֲ߳�
		/// </summary>
		virtual int ReqQryInvestorPosition(CTPCommon::CThostFtdcQryInvestorPositionField pQryInvestorPosition, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryInvestorPosition);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryInvestorPosition, bufferIntPtr, false);
				CThostFtdcQryInvestorPositionField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryInvestorPositionField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryInvestorPosition(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ�ʽ��˻�
		/// </summary>
		virtual int ReqQryTradingAccount(CTPCommon::CThostFtdcQryTradingAccountField pQryTradingAccount, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryTradingAccount);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryTradingAccount, bufferIntPtr, false);
				CThostFtdcQryTradingAccountField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryTradingAccountField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryTradingAccount(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯͶ����
		/// </summary>
		virtual int ReqQryInvestor(CTPCommon::CThostFtdcQryInvestorField pQryInvestor, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryInvestor);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryInvestor, bufferIntPtr, false);
				CThostFtdcQryInvestorField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryInvestorField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryInvestor(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ���ױ���
		/// </summary>
		virtual int ReqQryTradingCode(CTPCommon::CThostFtdcQryTradingCodeField pQryTradingCode, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryTradingCode);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryTradingCode, bufferIntPtr, false);
				CThostFtdcQryTradingCodeField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryTradingCodeField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryTradingCode(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ��Լ��֤����
		/// </summary>
		virtual int ReqQryInstrumentMarginRate(CTPCommon::CThostFtdcQryInstrumentMarginRateField pQryInstrumentMarginRate, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryInstrumentMarginRate);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryInstrumentMarginRate, bufferIntPtr, false);
				CThostFtdcQryInstrumentMarginRateField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryInstrumentMarginRateField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryInstrumentMarginRate(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ��Լ��������
		/// </summary>
		virtual int ReqQryInstrumentCommissionRate(CTPCommon::CThostFtdcQryInstrumentCommissionRateField pQryInstrumentCommissionRate, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryInstrumentCommissionRate);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryInstrumentCommissionRate, bufferIntPtr, false);
				CThostFtdcQryInstrumentCommissionRateField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryInstrumentCommissionRateField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryInstrumentCommissionRate(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ������
		/// </summary>
		virtual int ReqQryExchange(CTPCommon::CThostFtdcQryExchangeField pQryExchange, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryExchange);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryExchange, bufferIntPtr, false);
				CThostFtdcQryExchangeField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryExchangeField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryExchange(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ��Լ
		/// </summary>
		virtual int ReqQryInstrument(CTPCommon::CThostFtdcQryInstrumentField pQryInstrument, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryInstrument);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryInstrument, bufferIntPtr, false);
				CThostFtdcQryInstrumentField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryInstrumentField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryInstrument(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ����
		/// </summary>
		virtual int ReqQryDepthMarketData(CTPCommon::CThostFtdcQryDepthMarketDataField pQryDepthMarketData, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryDepthMarketData);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryDepthMarketData, bufferIntPtr, false);
				CThostFtdcQryDepthMarketDataField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryDepthMarketDataField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryDepthMarketData(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯͶ���߽�����
		/// </summary>
		virtual int ReqQrySettlementInfo(CTPCommon::CThostFtdcQrySettlementInfoField pQrySettlementInfo, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQrySettlementInfo);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQrySettlementInfo, bufferIntPtr, false);
				CThostFtdcQrySettlementInfoField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQrySettlementInfoField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQrySettlementInfo(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯת������
		/// </summary>
		virtual int ReqQryTransferBank(CTPCommon::CThostFtdcQryTransferBankField pQryTransferBank, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryTransferBank);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryTransferBank, bufferIntPtr, false);
				CThostFtdcQryTransferBankField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryTransferBankField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryTransferBank(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯͶ���ֲ߳���ϸ
		/// </summary>
		virtual int ReqQryInvestorPositionDetail(CTPCommon::CThostFtdcQryInvestorPositionDetailField pQryInvestorPositionDetail, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryInvestorPositionDetail);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryInvestorPositionDetail, bufferIntPtr, false);
				CThostFtdcQryInvestorPositionDetailField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryInvestorPositionDetailField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryInvestorPositionDetail(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ�ͻ�֪ͨ
		/// </summary>
		virtual int ReqQryNotice(CTPCommon::CThostFtdcQryNoticeField pQryNotice, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryNotice);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryNotice, bufferIntPtr, false);
				CThostFtdcQryNoticeField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryNoticeField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryNotice(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ������Ϣȷ��
		/// </summary>
		virtual int ReqQrySettlementInfoConfirm(CTPCommon::CThostFtdcQrySettlementInfoConfirmField pQrySettlementInfoConfirm, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQrySettlementInfoConfirm);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQrySettlementInfoConfirm, bufferIntPtr, false);
				CThostFtdcQrySettlementInfoConfirmField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQrySettlementInfoConfirmField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQrySettlementInfoConfirm(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯͶ���ֲ߳���ϸ
		/// </summary>
		virtual int ReqQryInvestorPositionCombineDetail(CTPCommon::CThostFtdcQryInvestorPositionCombineDetailField pQryInvestorPositionCombineDetail, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryInvestorPositionCombineDetail);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryInvestorPositionCombineDetail, bufferIntPtr, false);
				CThostFtdcQryInvestorPositionCombineDetailField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryInvestorPositionCombineDetailField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryInvestorPositionCombineDetail(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
		/// </summary>
		virtual int ReqQryCFMMCTradingAccountKey(CTPCommon::CThostFtdcQryCFMMCTradingAccountKeyField pQryCFMMCTradingAccountKey, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryCFMMCTradingAccountKey);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryCFMMCTradingAccountKey, bufferIntPtr, false);
				CThostFtdcQryCFMMCTradingAccountKeyField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryCFMMCTradingAccountKeyField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryCFMMCTradingAccountKey(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ�ֵ��۵���Ϣ
		/// </summary>
		virtual int ReqQryEWarrantOffset(CTPCommon::CThostFtdcQryEWarrantOffsetField pQryEWarrantOffset, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryEWarrantOffset);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryEWarrantOffset, bufferIntPtr, false);
				CThostFtdcQryEWarrantOffsetField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryEWarrantOffsetField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryEWarrantOffset(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯͶ����Ʒ��/��Ʒ�ֱ�֤��
		/// </summary>
		virtual int ReqQryInvestorProductGroupMargin(CTPCommon::CThostFtdcQryInvestorProductGroupMarginField pQryInvestorProductGroupMargin, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryInvestorProductGroupMargin);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryInvestorProductGroupMargin, bufferIntPtr, false);
				CThostFtdcQryInvestorProductGroupMarginField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryInvestorProductGroupMarginField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryInvestorProductGroupMargin(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ��������֤����
		/// </summary>
		virtual int ReqQryExchangeMarginRate(CTPCommon::CThostFtdcQryExchangeMarginRateField pQryExchangeMarginRate, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryExchangeMarginRate);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryExchangeMarginRate, bufferIntPtr, false);
				CThostFtdcQryExchangeMarginRateField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryExchangeMarginRateField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryExchangeMarginRate(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ������������֤����
		/// </summary>
		virtual int ReqQryExchangeMarginRateAdjust(CTPCommon::CThostFtdcQryExchangeMarginRateAdjustField pQryExchangeMarginRateAdjust, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryExchangeMarginRateAdjust);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryExchangeMarginRateAdjust, bufferIntPtr, false);
				CThostFtdcQryExchangeMarginRateAdjustField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryExchangeMarginRateAdjustField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryExchangeMarginRateAdjust(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯת����ˮ
		/// </summary>
		virtual int ReqQryTransferSerial(CTPCommon::CThostFtdcQryTransferSerialField pQryTransferSerial, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryTransferSerial);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryTransferSerial, bufferIntPtr, false);
				CThostFtdcQryTransferSerialField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryTransferSerialField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryTransferSerial(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ����ǩԼ��ϵ
		/// </summary>
		virtual int ReqQryAccountregister(CTPCommon::CThostFtdcQryAccountregisterField pQryAccountregister, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryAccountregister);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryAccountregister, bufferIntPtr, false);
				CThostFtdcQryAccountregisterField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryAccountregisterField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryAccountregister(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯǩԼ����
		/// </summary>
		virtual int ReqQryContractBank(CTPCommon::CThostFtdcQryContractBankField pQryContractBank, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryContractBank);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryContractBank, bufferIntPtr, false);
				CThostFtdcQryContractBankField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryContractBankField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryContractBank(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯԤ��
		/// </summary>
		virtual int ReqQryParkedOrder(CTPCommon::CThostFtdcQryParkedOrderField pQryParkedOrder, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryParkedOrder);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryParkedOrder, bufferIntPtr, false);
				CThostFtdcQryParkedOrderField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryParkedOrderField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryParkedOrder(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯԤ�񳷵�
		/// </summary>
		virtual int ReqQryParkedOrderAction(CTPCommon::CThostFtdcQryParkedOrderActionField pQryParkedOrderAction, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryParkedOrderAction);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryParkedOrderAction, bufferIntPtr, false);
				CThostFtdcQryParkedOrderActionField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryParkedOrderActionField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryParkedOrderAction(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ����֪ͨ
		/// </summary>
		virtual int ReqQryTradingNotice(CTPCommon::CThostFtdcQryTradingNoticeField pQryTradingNotice, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryTradingNotice);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryTradingNotice, bufferIntPtr, false);
				CThostFtdcQryTradingNoticeField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryTradingNoticeField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryTradingNotice(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ���͹�˾���ײ���
		/// </summary>
		virtual int ReqQryBrokerTradingParams(CTPCommon::CThostFtdcQryBrokerTradingParamsField pQryBrokerTradingParams, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryBrokerTradingParams);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryBrokerTradingParams, bufferIntPtr, false);
				CThostFtdcQryBrokerTradingParamsField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryBrokerTradingParamsField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryBrokerTradingParams(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�����ѯ���͹�˾�����㷨
		/// </summary>
		virtual int ReqQryBrokerTradingAlgos(CTPCommon::CThostFtdcQryBrokerTradingAlgosField pQryBrokerTradingAlgos, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pQryBrokerTradingAlgos);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pQryBrokerTradingAlgos, bufferIntPtr, false);
				CThostFtdcQryBrokerTradingAlgosField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcQryBrokerTradingAlgosField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQryBrokerTradingAlgos(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�ڻ����������ʽ�ת�ڻ�����
		/// </summary>
		virtual int ReqFromBankToFutureByFuture(CTPCommon::CThostFtdcReqTransferField pReqTransfer, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pReqTransfer);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pReqTransfer, bufferIntPtr, false);
				CThostFtdcReqTransferField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcReqTransferField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqFromBankToFutureByFuture(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�ڻ������ڻ��ʽ�ת��������
		/// </summary>
		virtual int ReqFromFutureToBankByFuture(CTPCommon::CThostFtdcReqTransferField pReqTransfer, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pReqTransfer);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pReqTransfer, bufferIntPtr, false);
				CThostFtdcReqTransferField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcReqTransferField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqFromFutureToBankByFuture(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}

		/// <summary>
		///�ڻ������ѯ�����������
		/// </summary>
		virtual int ReqQueryBankAccountMoneyByFuture(CTPCommon::CThostFtdcReqQueryAccountField pReqQueryAccount, [Out]  int %nRequestID)
		{
			nRequestID = Interlocked::Increment(lRequestID);
			int size = Marshal::SizeOf(pReqQueryAccount);
			IntPtr bufferIntPtr = Marshal::AllocHGlobal(size);
			try
			{
				Marshal::StructureToPtr(pReqQueryAccount, bufferIntPtr, false);
				CThostFtdcReqQueryAccountField* temp = &CTPtoKSChange(static_cast<CTPTradeApi::CThostFtdcReqQueryAccountField*>(bufferIntPtr.ToPointer()));
				return pUserApi->ReqQueryBankAccountMoneyByFuture(temp, nRequestID);
			}
			finally
			{
				Marshal::FreeHGlobal(bufferIntPtr);
			}
		}


	private:
		void EventInit()
		{

			{
				cppFrontConnect^ fp = gcnew cppFrontConnect(this, &KSTradeApiclr::cppcbFrontConnect);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBOnFrontConnected cb = static_cast<CBOnFrontConnected>(ip.ToPointer());
				pUserSpi->cbOnFrontConnected = cb;
			}

			{
				cppDisConnected^ fp = gcnew cppDisConnected(this, &KSTradeApiclr::cppcbDisConnected);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBOnFrontDisconnected cb = static_cast<CBOnFrontDisconnected>(ip.ToPointer());
				pUserSpi->cbOnFrontDisconnected = cb;
			}

			{
				cppHeartBeatWarning^ fp = gcnew cppHeartBeatWarning(this, &KSTradeApiclr::cppcbHeartBeatWarning);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBOnHeartBeatWarning cb = static_cast<CBOnHeartBeatWarning>(ip.ToPointer());
				pUserSpi->cbOnHeartBeatWarning = cb;
			}

			{
				cppErrRtnBankToFutureByFuture^ fp = gcnew cppErrRtnBankToFutureByFuture(this, &KSTradeApiclr::cppcbErrRtnBankToFutureByFuture);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBErrRtnBankToFutureByFuture cb = static_cast<CBErrRtnBankToFutureByFuture>(ip.ToPointer());
				pUserSpi->cbErrRtnBankToFutureByFuture = cb;
			}

			{
				cppErrRtnFutureToBankByFuture^ fp = gcnew cppErrRtnFutureToBankByFuture(this, &KSTradeApiclr::cppcbErrRtnFutureToBankByFuture);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBErrRtnFutureToBankByFuture cb = static_cast<CBErrRtnFutureToBankByFuture>(ip.ToPointer());
				pUserSpi->cbErrRtnFutureToBankByFuture = cb;
			}

			{
				cppErrRtnOrderAction^ fp = gcnew cppErrRtnOrderAction(this, &KSTradeApiclr::cppcbErrRtnOrderAction);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBErrRtnOrderAction cb = static_cast<CBErrRtnOrderAction>(ip.ToPointer());
				pUserSpi->cbErrRtnOrderAction = cb;
			}

			{
				cppErrRtnOrderInsert^ fp = gcnew cppErrRtnOrderInsert(this, &KSTradeApiclr::cppcbErrRtnOrderInsert);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBErrRtnOrderInsert cb = static_cast<CBErrRtnOrderInsert>(ip.ToPointer());
				pUserSpi->cbErrRtnOrderInsert = cb;
			}

			{
				cppErrRtnQueryBankBalanceByFuture^ fp = gcnew cppErrRtnQueryBankBalanceByFuture(this, &KSTradeApiclr::cppcbErrRtnQueryBankBalanceByFuture);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBErrRtnQueryBankBalanceByFuture cb = static_cast<CBErrRtnQueryBankBalanceByFuture>(ip.ToPointer());
				pUserSpi->cbErrRtnQueryBankBalanceByFuture = cb;
			}

			{
				cppErrRtnRepealBankToFutureByFutureManual^ fp = gcnew cppErrRtnRepealBankToFutureByFutureManual(this, &KSTradeApiclr::cppcbErrRtnRepealBankToFutureByFutureManual);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBErrRtnRepealBankToFutureByFutureManual cb = static_cast<CBErrRtnRepealBankToFutureByFutureManual>(ip.ToPointer());
				pUserSpi->cbErrRtnRepealBankToFutureByFutureManual = cb;
			}

			{
				cppErrRtnRepealFutureToBankByFutureManual^ fp = gcnew cppErrRtnRepealFutureToBankByFutureManual(this, &KSTradeApiclr::cppcbErrRtnRepealFutureToBankByFutureManual);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBErrRtnRepealFutureToBankByFutureManual cb = static_cast<CBErrRtnRepealFutureToBankByFutureManual>(ip.ToPointer());
				pUserSpi->cbErrRtnRepealFutureToBankByFutureManual = cb;
			}

			{
				cppRspError^ fp = gcnew cppRspError(this, &KSTradeApiclr::cppcbRspError);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspError cb = static_cast<CBRspError>(ip.ToPointer());
				pUserSpi->cbRspError = cb;
			}

			{
				cppRspFromBankToFutureByFuture^ fp = gcnew cppRspFromBankToFutureByFuture(this, &KSTradeApiclr::cppcbRspFromBankToFutureByFuture);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspFromBankToFutureByFuture cb = static_cast<CBRspFromBankToFutureByFuture>(ip.ToPointer());
				pUserSpi->cbRspFromBankToFutureByFuture = cb;
			}

			{
				cppRspFromFutureToBankByFuture^ fp = gcnew cppRspFromFutureToBankByFuture(this, &KSTradeApiclr::cppcbRspFromFutureToBankByFuture);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspFromFutureToBankByFuture cb = static_cast<CBRspFromFutureToBankByFuture>(ip.ToPointer());
				pUserSpi->cbRspFromFutureToBankByFuture = cb;
			}

			{
				cppRspOrderAction^ fp = gcnew cppRspOrderAction(this, &KSTradeApiclr::cppcbRspOrderAction);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspOrderAction cb = static_cast<CBRspOrderAction>(ip.ToPointer());
				pUserSpi->cbRspOrderAction = cb;
			}

			{
				cppRspOrderInsert^ fp = gcnew cppRspOrderInsert(this, &KSTradeApiclr::cppcbRspOrderInsert);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspOrderInsert cb = static_cast<CBRspOrderInsert>(ip.ToPointer());
				pUserSpi->cbRspOrderInsert = cb;
			}

			{
				cppRspParkedOrderAction^ fp = gcnew cppRspParkedOrderAction(this, &KSTradeApiclr::cppcbRspParkedOrderAction);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspParkedOrderAction cb = static_cast<CBRspParkedOrderAction>(ip.ToPointer());
				pUserSpi->cbRspParkedOrderAction = cb;
			}

			{
				cppRspParkedOrderInsert^ fp = gcnew cppRspParkedOrderInsert(this, &KSTradeApiclr::cppcbRspParkedOrderInsert);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspParkedOrderInsert cb = static_cast<CBRspParkedOrderInsert>(ip.ToPointer());
				pUserSpi->cbRspParkedOrderInsert = cb;
			}

			{
				cppRspQryBrokerTradingAlgos^ fp = gcnew cppRspQryBrokerTradingAlgos(this, &KSTradeApiclr::cppcbRspQryBrokerTradingAlgos);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryBrokerTradingAlgos cb = static_cast<CBRspQryBrokerTradingAlgos>(ip.ToPointer());
				pUserSpi->cbRspQryBrokerTradingAlgos = cb;
			}

			{
				cppRspQryBrokerTradingParams^ fp = gcnew cppRspQryBrokerTradingParams(this, &KSTradeApiclr::cppcbRspQryBrokerTradingParams);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryBrokerTradingParams cb = static_cast<CBRspQryBrokerTradingParams>(ip.ToPointer());
				pUserSpi->cbRspQryBrokerTradingParams = cb;
			}

			{
				cppRspQryCFMMCTradingAccountKey^ fp = gcnew cppRspQryCFMMCTradingAccountKey(this, &KSTradeApiclr::cppcbRspQryCFMMCTradingAccountKey);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryCFMMCTradingAccountKey cb = static_cast<CBRspQryCFMMCTradingAccountKey>(ip.ToPointer());
				pUserSpi->cbRspQryCFMMCTradingAccountKey = cb;
			}

			{
				cppRspQryContractBank^ fp = gcnew cppRspQryContractBank(this, &KSTradeApiclr::cppcbRspQryContractBank);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryContractBank cb = static_cast<CBRspQryContractBank>(ip.ToPointer());
				pUserSpi->cbRspQryContractBank = cb;
			}

			{
				cppRspQryDepthMarketData^ fp = gcnew cppRspQryDepthMarketData(this, &KSTradeApiclr::cppcbRspQryDepthMarketData);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryDepthMarketData cb = static_cast<CBRspQryDepthMarketData>(ip.ToPointer());
				pUserSpi->cbRspQryDepthMarketData = cb;
			}

			{
				cppRspQryExchange^ fp = gcnew cppRspQryExchange(this, &KSTradeApiclr::cppcbRspQryExchange);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryExchange cb = static_cast<CBRspQryExchange>(ip.ToPointer());
				pUserSpi->cbRspQryExchange = cb;
			}

			{
				cppRspQryInstrument^ fp = gcnew cppRspQryInstrument(this, &KSTradeApiclr::cppcbRspQryInstrument);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryInstrument cb = static_cast<CBRspQryInstrument>(ip.ToPointer());
				pUserSpi->cbRspQryInstrument = cb;
			}

			{
				cppRspQryInstrumentCommissionRate^ fp = gcnew cppRspQryInstrumentCommissionRate(this, &KSTradeApiclr::cppcbRspQryInstrumentCommissionRate);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryInstrumentCommissionRate cb = static_cast<CBRspQryInstrumentCommissionRate>(ip.ToPointer());
				pUserSpi->cbRspQryInstrumentCommissionRate = cb;
			}

			{
				cppRspQryInstrumentMarginRate^ fp = gcnew cppRspQryInstrumentMarginRate(this, &KSTradeApiclr::cppcbRspQryInstrumentMarginRate);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryInstrumentMarginRate cb = static_cast<CBRspQryInstrumentMarginRate>(ip.ToPointer());
				pUserSpi->cbRspQryInstrumentMarginRate = cb;
			}

			{
				cppRspQryInvestor^ fp = gcnew cppRspQryInvestor(this, &KSTradeApiclr::cppcbRspQryInvestor);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryInvestor cb = static_cast<CBRspQryInvestor>(ip.ToPointer());
				pUserSpi->cbRspQryInvestor = cb;
			}

			{
				cppRspQryInvestorPosition^ fp = gcnew cppRspQryInvestorPosition(this, &KSTradeApiclr::cppcbRspQryInvestorPosition);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryInvestorPosition cb = static_cast<CBRspQryInvestorPosition>(ip.ToPointer());
				pUserSpi->cbRspQryInvestorPosition = cb;
			}

			{
				cppRspQryInvestorPositionCombineDetail^ fp = gcnew cppRspQryInvestorPositionCombineDetail(this, &KSTradeApiclr::cppcbRspQryInvestorPositionCombineDetail);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryInvestorPositionCombineDetail cb = static_cast<CBRspQryInvestorPositionCombineDetail>(ip.ToPointer());
				pUserSpi->cbRspQryInvestorPositionCombineDetail = cb;
			}

			{
				cppRspQryInvestorPositionDetail^ fp = gcnew cppRspQryInvestorPositionDetail(this, &KSTradeApiclr::cppcbRspQryInvestorPositionDetail);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryInvestorPositionDetail cb = static_cast<CBRspQryInvestorPositionDetail>(ip.ToPointer());
				pUserSpi->cbRspQryInvestorPositionDetail = cb;
			}

			{
				cppRspQryNotice^ fp = gcnew cppRspQryNotice(this, &KSTradeApiclr::cppcbRspQryNotice);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryNotice cb = static_cast<CBRspQryNotice>(ip.ToPointer());
				pUserSpi->cbRspQryNotice = cb;
			}

			{
				cppRspQryOrder^ fp = gcnew cppRspQryOrder(this, &KSTradeApiclr::cppcbRspQryOrder);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryOrder cb = static_cast<CBRspQryOrder>(ip.ToPointer());
				pUserSpi->cbRspQryOrder = cb;
			}

			{
				cppRspQryParkedOrder^ fp = gcnew cppRspQryParkedOrder(this, &KSTradeApiclr::cppcbRspQryParkedOrder);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryParkedOrder cb = static_cast<CBRspQryParkedOrder>(ip.ToPointer());
				pUserSpi->cbRspQryParkedOrder = cb;
			}

			{
				cppRspQryParkedOrderAction^ fp = gcnew cppRspQryParkedOrderAction(this, &KSTradeApiclr::cppcbRspQryParkedOrderAction);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryParkedOrderAction cb = static_cast<CBRspQryParkedOrderAction>(ip.ToPointer());
				pUserSpi->cbRspQryParkedOrderAction = cb;
			}

			{
				cppRspQrySettlementInfo^ fp = gcnew cppRspQrySettlementInfo(this, &KSTradeApiclr::cppcbRspQrySettlementInfo);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQrySettlementInfo cb = static_cast<CBRspQrySettlementInfo>(ip.ToPointer());
				pUserSpi->cbRspQrySettlementInfo = cb;
			}

			{
				cppRspQrySettlementInfoConfirm^ fp = gcnew cppRspQrySettlementInfoConfirm(this, &KSTradeApiclr::cppcbRspQrySettlementInfoConfirm);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQrySettlementInfoConfirm cb = static_cast<CBRspQrySettlementInfoConfirm>(ip.ToPointer());
				pUserSpi->cbRspQrySettlementInfoConfirm = cb;
			}

			{
				cppRspQryTrade^ fp = gcnew cppRspQryTrade(this, &KSTradeApiclr::cppcbRspQryTrade);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryTrade cb = static_cast<CBRspQryTrade>(ip.ToPointer());
				pUserSpi->cbRspQryTrade = cb;
			}

			{
				cppRspQryTradingAccount^ fp = gcnew cppRspQryTradingAccount(this, &KSTradeApiclr::cppcbRspQryTradingAccount);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryTradingAccount cb = static_cast<CBRspQryTradingAccount>(ip.ToPointer());
				pUserSpi->cbRspQryTradingAccount = cb;
			}

			{
				cppRspQryTradingCode^ fp = gcnew cppRspQryTradingCode(this, &KSTradeApiclr::cppcbRspQryTradingCode);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryTradingCode cb = static_cast<CBRspQryTradingCode>(ip.ToPointer());
				pUserSpi->cbRspQryTradingCode = cb;
			}

			{
				cppRspQryTradingNotice^ fp = gcnew cppRspQryTradingNotice(this, &KSTradeApiclr::cppcbRspQryTradingNotice);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryTradingNotice cb = static_cast<CBRspQryTradingNotice>(ip.ToPointer());
				pUserSpi->cbRspQryTradingNotice = cb;
			}

			{
				cppRspQryTransferBank^ fp = gcnew cppRspQryTransferBank(this, &KSTradeApiclr::cppcbRspQryTransferBank);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryTransferBank cb = static_cast<CBRspQryTransferBank>(ip.ToPointer());
				pUserSpi->cbRspQryTransferBank = cb;
			}

			{
				cppRspQryTransferSerial^ fp = gcnew cppRspQryTransferSerial(this, &KSTradeApiclr::cppcbRspQryTransferSerial);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryTransferSerial cb = static_cast<CBRspQryTransferSerial>(ip.ToPointer());
				pUserSpi->cbRspQryTransferSerial = cb;
			}

			{
				cppRspQryAccountregister^ fp = gcnew cppRspQryAccountregister(this, &KSTradeApiclr::cppcbRspQryAccountregister);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQryAccountregister cb = static_cast<CBRspQryAccountregister>(ip.ToPointer());
				pUserSpi->cbRspQryAccountregister = cb;
			}

			{
				cppRspQueryBankAccountMoneyByFuture^ fp = gcnew cppRspQueryBankAccountMoneyByFuture(this, &KSTradeApiclr::cppcbRspQueryBankAccountMoneyByFuture);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQueryBankAccountMoneyByFuture cb = static_cast<CBRspQueryBankAccountMoneyByFuture>(ip.ToPointer());
				pUserSpi->cbRspQueryBankAccountMoneyByFuture = cb;
			}

			{
				cppRspQueryMaxOrderVolume^ fp = gcnew cppRspQueryMaxOrderVolume(this, &KSTradeApiclr::cppcbRspQueryMaxOrderVolume);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspQueryMaxOrderVolume cb = static_cast<CBRspQueryMaxOrderVolume>(ip.ToPointer());
				pUserSpi->cbRspQueryMaxOrderVolume = cb;
			}

			{
				cppRspRemoveParkedOrder^ fp = gcnew cppRspRemoveParkedOrder(this, &KSTradeApiclr::cppcbRspRemoveParkedOrder);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspRemoveParkedOrder cb = static_cast<CBRspRemoveParkedOrder>(ip.ToPointer());
				pUserSpi->cbRspRemoveParkedOrder = cb;
			}

			{
				cppRspRemoveParkedOrderAction^ fp = gcnew cppRspRemoveParkedOrderAction(this, &KSTradeApiclr::cppcbRspRemoveParkedOrderAction);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspRemoveParkedOrderAction cb = static_cast<CBRspRemoveParkedOrderAction>(ip.ToPointer());
				pUserSpi->cbRspRemoveParkedOrderAction = cb;
			}

			{
				cppRspSettlementInfoConfirm^ fp = gcnew cppRspSettlementInfoConfirm(this, &KSTradeApiclr::cppcbRspSettlementInfoConfirm);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspSettlementInfoConfirm cb = static_cast<CBRspSettlementInfoConfirm>(ip.ToPointer());
				pUserSpi->cbRspSettlementInfoConfirm = cb;
			}

			{
				cppRspTradingAccountPasswordUpdate^ fp = gcnew cppRspTradingAccountPasswordUpdate(this, &KSTradeApiclr::cppcbRspTradingAccountPasswordUpdate);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspTradingAccountPasswordUpdate cb = static_cast<CBRspTradingAccountPasswordUpdate>(ip.ToPointer());
				pUserSpi->cbRspTradingAccountPasswordUpdate = cb;
			}

			{
				cppRspAuthenticate^ fp = gcnew cppRspAuthenticate(this, &KSTradeApiclr::cppcbRspAuthenticate);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspAuthenticate cb = static_cast<CBRspAuthenticate>(ip.ToPointer());
				pUserSpi->cbRspAuthenticate = cb;
			}


			{
				cppRspUserLogin^ fp = gcnew cppRspUserLogin(this, &KSTradeApiclr::cppcbRspUserLogin);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspUserLogin cb = static_cast<CBRspUserLogin>(ip.ToPointer());
				pUserSpi->cbRspUserLogin = cb;
			}

			{
				cppRspUserLogout^ fp = gcnew cppRspUserLogout(this, &KSTradeApiclr::cppcbRspUserLogout);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspUserLogout cb = static_cast<CBRspUserLogout>(ip.ToPointer());
				pUserSpi->cbRspUserLogout = cb;
			}

			{
				cppRspUserPasswordUpdate^ fp = gcnew cppRspUserPasswordUpdate(this, &KSTradeApiclr::cppcbRspUserPasswordUpdate);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRspUserPasswordUpdate cb = static_cast<CBRspUserPasswordUpdate>(ip.ToPointer());
				pUserSpi->cbRspUserPasswordUpdate = cb;
			}

			{
				cppRtnErrorConditionalOrder^ fp = gcnew cppRtnErrorConditionalOrder(this, &KSTradeApiclr::cppcbRtnErrorConditionalOrder);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnErrorConditionalOrder cb = static_cast<CBRtnErrorConditionalOrder>(ip.ToPointer());
				pUserSpi->cbRtnErrorConditionalOrder = cb;
			}

			{
				cppRtnFromBankToFutureByBank^ fp = gcnew cppRtnFromBankToFutureByBank(this, &KSTradeApiclr::cppcbRtnFromBankToFutureByBank);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnFromBankToFutureByBank cb = static_cast<CBRtnFromBankToFutureByBank>(ip.ToPointer());
				pUserSpi->cbRtnFromBankToFutureByBank = cb;
			}

			{
				cppRtnFromBankToFutureByFuture^ fp = gcnew cppRtnFromBankToFutureByFuture(this, &KSTradeApiclr::cppcbRtnFromBankToFutureByFuture);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnFromBankToFutureByFuture cb = static_cast<CBRtnFromBankToFutureByFuture>(ip.ToPointer());
				pUserSpi->cbRtnFromBankToFutureByFuture = cb;
			}

			{
				cppRtnFromFutureToBankByBank^ fp = gcnew cppRtnFromFutureToBankByBank(this, &KSTradeApiclr::cppcbRtnFromFutureToBankByBank);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnFromFutureToBankByBank cb = static_cast<CBRtnFromFutureToBankByBank>(ip.ToPointer());
				pUserSpi->cbRtnFromFutureToBankByBank = cb;
			}

			{
				cppRtnFromFutureToBankByFuture^ fp = gcnew cppRtnFromFutureToBankByFuture(this, &KSTradeApiclr::cppcbRtnFromFutureToBankByFuture);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnFromFutureToBankByFuture cb = static_cast<CBRtnFromFutureToBankByFuture>(ip.ToPointer());
				pUserSpi->cbRtnFromFutureToBankByFuture = cb;
			}

			{
				cppRtnInstrumentStatus^ fp = gcnew cppRtnInstrumentStatus(this, &KSTradeApiclr::cppcbRtnInstrumentStatus);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnInstrumentStatus cb = static_cast<CBRtnInstrumentStatus>(ip.ToPointer());
				pUserSpi->cbRtnInstrumentStatus = cb;
			}

			{
				cppRtnOrder^ fp = gcnew cppRtnOrder(this, &KSTradeApiclr::cppcbRtnOrder);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnOrder cb = static_cast<CBRtnOrder>(ip.ToPointer());
				pUserSpi->cbRtnOrder = cb;
			}

			{
				cppRtnQueryBankBalanceByFuture^ fp = gcnew cppRtnQueryBankBalanceByFuture(this, &KSTradeApiclr::cppcbRtnQueryBankBalanceByFuture);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnQueryBankBalanceByFuture cb = static_cast<CBRtnQueryBankBalanceByFuture>(ip.ToPointer());
				pUserSpi->cbRtnQueryBankBalanceByFuture = cb;
			}

			{
				cppRtnRepealFromBankToFutureByBank^ fp = gcnew cppRtnRepealFromBankToFutureByBank(this, &KSTradeApiclr::cppcbRtnRepealFromBankToFutureByBank);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnRepealFromBankToFutureByBank cb = static_cast<CBRtnRepealFromBankToFutureByBank>(ip.ToPointer());
				pUserSpi->cbRtnRepealFromBankToFutureByBank = cb;
			}

			{
				cppRtnRepealFromBankToFutureByFuture^ fp = gcnew cppRtnRepealFromBankToFutureByFuture(this, &KSTradeApiclr::cppcbRtnRepealFromBankToFutureByFuture);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnRepealFromBankToFutureByFuture cb = static_cast<CBRtnRepealFromBankToFutureByFuture>(ip.ToPointer());
				pUserSpi->cbRtnRepealFromBankToFutureByFuture = cb;
			}

			{
				cppRtnRepealFromBankToFutureByFutureManual^ fp = gcnew cppRtnRepealFromBankToFutureByFutureManual(this, &KSTradeApiclr::cppcbRtnRepealFromBankToFutureByFutureManual);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnRepealFromBankToFutureByFutureManual cb = static_cast<CBRtnRepealFromBankToFutureByFutureManual>(ip.ToPointer());
				pUserSpi->cbRtnRepealFromBankToFutureByFutureManual = cb;
			}

			{
				cppRtnRepealFromFutureToBankByBank^ fp = gcnew cppRtnRepealFromFutureToBankByBank(this, &KSTradeApiclr::cppcbRtnRepealFromFutureToBankByBank);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnRepealFromFutureToBankByBank cb = static_cast<CBRtnRepealFromFutureToBankByBank>(ip.ToPointer());
				pUserSpi->cbRtnRepealFromFutureToBankByBank = cb;
			}

			{
				cppRtnRepealFromFutureToBankByFuture^ fp = gcnew cppRtnRepealFromFutureToBankByFuture(this, &KSTradeApiclr::cppcbRtnRepealFromFutureToBankByFuture);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnRepealFromFutureToBankByFuture cb = static_cast<CBRtnRepealFromFutureToBankByFuture>(ip.ToPointer());
				pUserSpi->cbRtnRepealFromFutureToBankByFuture = cb;
			}

			{
				cppRtnRepealFromFutureToBankByFutureManual^ fp = gcnew cppRtnRepealFromFutureToBankByFutureManual(this, &KSTradeApiclr::cppcbRtnRepealFromFutureToBankByFutureManual);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnRepealFromFutureToBankByFutureManual cb = static_cast<CBRtnRepealFromFutureToBankByFutureManual>(ip.ToPointer());
				pUserSpi->cbRtnRepealFromFutureToBankByFutureManual = cb;
			}

			{
				cppRtnTrade^ fp = gcnew cppRtnTrade(this, &KSTradeApiclr::cppcbRtnTrade);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnTrade cb = static_cast<CBRtnTrade>(ip.ToPointer());
				pUserSpi->cbRtnTrade = cb;
			}

			{
				cppRtnTradingNotice^ fp = gcnew cppRtnTradingNotice(this, &KSTradeApiclr::cppcbRtnTradingNotice);
				GCHandle gch = GCHandle::Alloc(fp);
				gchlist->Add(gch);
				IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
				CBRtnTradingNotice cb = static_cast<CBRtnTradingNotice>(ip.ToPointer());
				pUserSpi->cbRtnTradingNotice = cb;
			}

		}





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
		virtual event FrontConnect ^OnFrontConnect;




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
		virtual event DisConnected ^OnDisConnected;




	private:
		delegate void cppHeartBeatWarning(int pTimeLapes);
		void cppcbHeartBeatWarning(int pTimeLapes)
		{

			OnHeartBeatWarning(pTimeLapes);
		}
	public:
		/// <summary>
		/// ������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
		/// </summary>
		virtual event HeartBeatWarning ^OnHeartBeatWarning;




	private:
		delegate void cppErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo);
		void cppcbErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
		{
			CTPCommon::CThostFtdcReqTransferField  cspReqTransfer = (CTPCommon::CThostFtdcReqTransferField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pReqTransfer)), CTPCommon::CThostFtdcReqTransferField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnErrRtnBankToFutureByFuture(cspReqTransfer, cspRspInfo);
		}
	public:
		/// <summary>
		/// �ڻ����������ʽ�ת�ڻ�����ر�
		/// </summary>
		virtual event ErrRtnBankToFutureByFuture ^OnErrRtnBankToFutureByFuture;




	private:
		delegate void cppErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo);
		void cppcbErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
		{
			CTPCommon::CThostFtdcReqTransferField  cspReqTransfer = (CTPCommon::CThostFtdcReqTransferField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pReqTransfer)), CTPCommon::CThostFtdcReqTransferField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnErrRtnFutureToBankByFuture(cspReqTransfer, cspRspInfo);
		}
	public:
		/// <summary>
		/// �ڻ������ڻ��ʽ�ת���д���ر�
		/// </summary>
		virtual event ErrRtnFutureToBankByFuture ^OnErrRtnFutureToBankByFuture;




	private:
		delegate void cppErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
		void cppcbErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
		{
			CTPCommon::CThostFtdcOrderActionField  cspOrderAction = (CTPCommon::CThostFtdcOrderActionField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pOrderAction)), CTPCommon::CThostFtdcOrderActionField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnErrRtnOrderAction(cspOrderAction, cspRspInfo);
		}
	public:
		/// <summary>
		/// ������������ر�
		/// </summary>
		virtual event ErrRtnOrderAction ^OnErrRtnOrderAction;




	private:
		delegate void cppErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
		void cppcbErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
		{
			CTPCommon::CThostFtdcInputOrderField  cspInputOrder = (CTPCommon::CThostFtdcInputOrderField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pInputOrder)), CTPCommon::CThostFtdcInputOrderField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnErrRtnOrderInsert(cspInputOrder, cspRspInfo);
		}
	public:
		/// <summary>
		/// ����¼�����ر�
		/// </summary>
		virtual event ErrRtnOrderInsert ^OnErrRtnOrderInsert;




	private:
		delegate void cppErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo);
		void cppcbErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo)
		{
			CTPCommon::CThostFtdcReqQueryAccountField  cspReqQueryAccount = (CTPCommon::CThostFtdcReqQueryAccountField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pReqQueryAccount)), CTPCommon::CThostFtdcReqQueryAccountField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnErrRtnQueryBankBalanceByFuture(cspReqQueryAccount, cspRspInfo);
		}
	public:
		/// <summary>
		/// �ڻ������ѯ����������ر�
		/// </summary>
		virtual event ErrRtnQueryBankBalanceByFuture ^OnErrRtnQueryBankBalanceByFuture;




	private:
		delegate void cppErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo);
		void cppcbErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
		{
			CTPCommon::CThostFtdcReqRepealField  cspReqRepeal = (CTPCommon::CThostFtdcReqRepealField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pReqRepeal)), CTPCommon::CThostFtdcReqRepealField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnErrRtnRepealBankToFutureByFutureManual(cspReqRepeal, cspRspInfo);
		}
	public:
		/// <summary>
		/// ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ�����ر�
		/// </summary>
		virtual event ErrRtnRepealBankToFutureByFutureManual ^OnErrRtnRepealBankToFutureByFutureManual;




	private:
		delegate void cppErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo);
		void cppcbErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
		{
			CTPCommon::CThostFtdcReqRepealField  cspReqRepeal = (CTPCommon::CThostFtdcReqRepealField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pReqRepeal)), CTPCommon::CThostFtdcReqRepealField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnErrRtnRepealFutureToBankByFutureManual(cspReqRepeal, cspRspInfo);
		}
	public:
		/// <summary>
		/// ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת���д���ر�
		/// </summary>
		virtual event ErrRtnRepealFutureToBankByFutureManual ^OnErrRtnRepealFutureToBankByFutureManual;




	private:
		delegate void cppRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspError(cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// ����Ӧ��
		/// </summary>
		virtual event RspError ^OnRspError;




	private:
		delegate void cppRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcReqTransferField  cspReqTransfer = (CTPCommon::CThostFtdcReqTransferField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pReqTransfer)), CTPCommon::CThostFtdcReqTransferField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspFromBankToFutureByFuture(cspReqTransfer, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �ڻ����������ʽ�ת�ڻ�Ӧ��
		/// </summary>
		virtual event RspFromBankToFutureByFuture ^OnRspFromBankToFutureByFuture;




	private:
		delegate void cppRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcReqTransferField  cspReqTransfer = (CTPCommon::CThostFtdcReqTransferField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pReqTransfer)), CTPCommon::CThostFtdcReqTransferField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspFromFutureToBankByFuture(cspReqTransfer, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �ڻ������ڻ��ʽ�ת����Ӧ��
		/// </summary>
		virtual event RspFromFutureToBankByFuture ^OnRspFromFutureToBankByFuture;




	private:
		delegate void cppRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcInputOrderActionField  cspInputOrderAction = (CTPCommon::CThostFtdcInputOrderActionField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pInputOrderAction)), CTPCommon::CThostFtdcInputOrderActionField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspOrderAction(cspInputOrderAction, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// ��������������Ӧ
		/// </summary>
		virtual event RspOrderAction ^OnRspOrderAction;




	private:
		delegate void cppRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcInputOrderField  cspInputOrder = (CTPCommon::CThostFtdcInputOrderField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pInputOrder)), CTPCommon::CThostFtdcInputOrderField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspOrderInsert(cspInputOrder, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// ����¼��������Ӧ
		/// </summary>
		virtual event RspOrderInsert ^OnRspOrderInsert;




	private:
		delegate void cppRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcParkedOrderActionField  cspParkedOrderAction = (CTPCommon::CThostFtdcParkedOrderActionField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pParkedOrderAction)), CTPCommon::CThostFtdcParkedOrderActionField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspParkedOrderAction(cspParkedOrderAction, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// Ԥ�񳷵�¼��������Ӧ
		/// </summary>
		virtual event RspParkedOrderAction ^OnRspParkedOrderAction;




	private:
		delegate void cppRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcParkedOrderField  cspParkedOrder = (CTPCommon::CThostFtdcParkedOrderField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pParkedOrder)), CTPCommon::CThostFtdcParkedOrderField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspParkedOrderInsert(cspParkedOrder, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// Ԥ��¼��������Ӧ
		/// </summary>
		virtual event RspParkedOrderInsert ^OnRspParkedOrderInsert;




	private:
		delegate void cppRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcBrokerTradingAlgosField  cspBrokerTradingAlgos = (CTPCommon::CThostFtdcBrokerTradingAlgosField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pBrokerTradingAlgos)), CTPCommon::CThostFtdcBrokerTradingAlgosField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryBrokerTradingAlgos(cspBrokerTradingAlgos, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ���͹�˾�����㷨��Ӧ
		/// </summary>
		virtual event RspQryBrokerTradingAlgos ^OnRspQryBrokerTradingAlgos;




	private:
		delegate void cppRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcBrokerTradingParamsField  cspBrokerTradingParams = (CTPCommon::CThostFtdcBrokerTradingParamsField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pBrokerTradingParams)), CTPCommon::CThostFtdcBrokerTradingParamsField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryBrokerTradingParams(cspBrokerTradingParams, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ���͹�˾���ײ�����Ӧ
		/// </summary>
		virtual event RspQryBrokerTradingParams ^OnRspQryBrokerTradingParams;




	private:
		delegate void cppRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcCFMMCTradingAccountKeyField  cspCFMMCTradingAccountKey = (CTPCommon::CThostFtdcCFMMCTradingAccountKeyField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pCFMMCTradingAccountKey)), CTPCommon::CThostFtdcCFMMCTradingAccountKeyField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryCFMMCTradingAccountKey(cspCFMMCTradingAccountKey, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// ��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ
		/// </summary>
		virtual event RspQryCFMMCTradingAccountKey ^OnRspQryCFMMCTradingAccountKey;




	private:
		delegate void cppRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcContractBankField  cspContractBank = (CTPCommon::CThostFtdcContractBankField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pContractBank)), CTPCommon::CThostFtdcContractBankField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryContractBank(cspContractBank, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯǩԼ������Ӧ
		/// </summary>
		virtual event RspQryContractBank ^OnRspQryContractBank;




	private:
		delegate void cppRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcDepthMarketDataField  cspDepthMarketData = (CTPCommon::CThostFtdcDepthMarketDataField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pDepthMarketData)), CTPCommon::CThostFtdcDepthMarketDataField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryDepthMarketData(cspDepthMarketData, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ������Ӧ
		/// </summary>
		virtual event RspQryDepthMarketData ^OnRspQryDepthMarketData;




	private:
		delegate void cppRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcExchangeField  cspExchange = (CTPCommon::CThostFtdcExchangeField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pExchange)), CTPCommon::CThostFtdcExchangeField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryExchange(cspExchange, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ��������Ӧ
		/// </summary>
		virtual event RspQryExchange ^OnRspQryExchange;




	private:
		delegate void cppRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcInstrumentField  cspInstrument = (CTPCommon::CThostFtdcInstrumentField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pInstrument)), CTPCommon::CThostFtdcInstrumentField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryInstrument(cspInstrument, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ��Լ��Ӧ
		/// </summary>
		virtual event RspQryInstrument ^OnRspQryInstrument;




	private:
		delegate void cppRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcInstrumentCommissionRateField  cspInstrumentCommissionRate = (CTPCommon::CThostFtdcInstrumentCommissionRateField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pInstrumentCommissionRate)), CTPCommon::CThostFtdcInstrumentCommissionRateField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryInstrumentCommissionRate(cspInstrumentCommissionRate, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ��Լ����������Ӧ
		/// </summary>
		virtual event RspQryInstrumentCommissionRate ^OnRspQryInstrumentCommissionRate;




	private:
		delegate void cppRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcInstrumentMarginRateField  cspInstrumentMarginRate = (CTPCommon::CThostFtdcInstrumentMarginRateField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pInstrumentMarginRate)), CTPCommon::CThostFtdcInstrumentMarginRateField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryInstrumentMarginRate(cspInstrumentMarginRate, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ��Լ��֤������Ӧ
		/// </summary>
		virtual event RspQryInstrumentMarginRate ^OnRspQryInstrumentMarginRate;




	private:
		delegate void cppRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcInvestorField  cspInvestor = (CTPCommon::CThostFtdcInvestorField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pInvestor)), CTPCommon::CThostFtdcInvestorField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryInvestor(cspInvestor, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯͶ������Ӧ
		/// </summary>
		virtual event RspQryInvestor ^OnRspQryInvestor;




	private:
		delegate void cppRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcInvestorPositionField  cspInvestorPosition = (CTPCommon::CThostFtdcInvestorPositionField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pInvestorPosition)), CTPCommon::CThostFtdcInvestorPositionField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryInvestorPosition(cspInvestorPosition, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯͶ���ֲ߳���Ӧ
		/// </summary>
		virtual event RspQryInvestorPosition ^OnRspQryInvestorPosition;




	private:
		delegate void cppRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcInvestorPositionCombineDetailField  cspInvestorPositionCombineDetail = (CTPCommon::CThostFtdcInvestorPositionCombineDetailField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pInvestorPositionCombineDetail)), CTPCommon::CThostFtdcInvestorPositionCombineDetailField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryInvestorPositionCombineDetail(cspInvestorPositionCombineDetail, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯͶ���ֲ߳���ϸ��Ӧ
		/// </summary>
		virtual event RspQryInvestorPositionCombineDetail ^OnRspQryInvestorPositionCombineDetail;




	private:
		delegate void cppRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{

			CTPCommon::CThostFtdcInvestorPositionDetailField  cspInvestorPositionDetail = (CTPCommon::CThostFtdcInvestorPositionDetailField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pInvestorPositionDetail)), CTPCommon::CThostFtdcInvestorPositionDetailField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryInvestorPositionDetail(cspInvestorPositionDetail, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯͶ���ֲ߳���ϸ��Ӧ
		/// </summary>
		virtual event RspQryInvestorPositionDetail ^OnRspQryInvestorPositionDetail;




	private:
		delegate void cppRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcNoticeField  cspNotice = (CTPCommon::CThostFtdcNoticeField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pNotice)), CTPCommon::CThostFtdcNoticeField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryNotice(cspNotice, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ�ͻ�֪ͨ��Ӧ
		/// </summary>
		virtual event RspQryNotice ^OnRspQryNotice;




	private:
		delegate void cppRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcOrderField  cspOrder = (CTPCommon::CThostFtdcOrderField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pOrder)), CTPCommon::CThostFtdcOrderField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryOrder(cspOrder, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ������Ӧ
		/// </summary>
		virtual event RspQryOrder ^OnRspQryOrder;




	private:
		delegate void cppRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcParkedOrderField  cspParkedOrder = (CTPCommon::CThostFtdcParkedOrderField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pParkedOrder)), CTPCommon::CThostFtdcParkedOrderField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryParkedOrder(cspParkedOrder, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯԤ����Ӧ
		/// </summary>
		virtual event RspQryParkedOrder ^OnRspQryParkedOrder;




	private:
		delegate void cppRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcParkedOrderActionField  cspParkedOrderAction = (CTPCommon::CThostFtdcParkedOrderActionField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pParkedOrderAction)), CTPCommon::CThostFtdcParkedOrderActionField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryParkedOrderAction(cspParkedOrderAction, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯԤ�񳷵���Ӧ
		/// </summary>
		virtual event RspQryParkedOrderAction ^OnRspQryParkedOrderAction;




	private:
		delegate void cppRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcSettlementInfoField  cspSettlementInfo = (CTPCommon::CThostFtdcSettlementInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pSettlementInfo)), CTPCommon::CThostFtdcSettlementInfoField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQrySettlementInfo(cspSettlementInfo, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯͶ���߽�������Ӧ
		/// </summary>
		virtual event RspQrySettlementInfo ^OnRspQrySettlementInfo;




	private:
		delegate void cppRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcSettlementInfoConfirmField  cspSettlementInfoConfirm = (CTPCommon::CThostFtdcSettlementInfoConfirmField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pSettlementInfoConfirm)), CTPCommon::CThostFtdcSettlementInfoConfirmField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQrySettlementInfoConfirm(cspSettlementInfoConfirm, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ������Ϣȷ����Ӧ
		/// </summary>
		virtual event RspQrySettlementInfoConfirm ^OnRspQrySettlementInfoConfirm;




	private:
		delegate void cppRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcTradeField  cspTrade = (CTPCommon::CThostFtdcTradeField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pTrade)), CTPCommon::CThostFtdcTradeField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryTrade(cspTrade, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ�ɽ���Ӧ
		/// </summary>
		virtual event RspQryTrade ^OnRspQryTrade;




	private:
		delegate void cppRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcTradingAccountField  cspTradingAccount = (CTPCommon::CThostFtdcTradingAccountField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pTradingAccount)), CTPCommon::CThostFtdcTradingAccountField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryTradingAccount(cspTradingAccount, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ�ʽ��˻���Ӧ
		/// </summary>
		virtual event RspQryTradingAccount ^OnRspQryTradingAccount;




	private:
		delegate void cppRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcTradingCodeField  cspTradingCode = (CTPCommon::CThostFtdcTradingCodeField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pTradingCode)), CTPCommon::CThostFtdcTradingCodeField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryTradingCode(cspTradingCode, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ���ױ�����Ӧ
		/// </summary>
		virtual event RspQryTradingCode ^OnRspQryTradingCode;




	private:
		delegate void cppRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcTradingNoticeField  cspTradingNotice = (CTPCommon::CThostFtdcTradingNoticeField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pTradingNotice)), CTPCommon::CThostFtdcTradingNoticeField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryTradingNotice(cspTradingNotice, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯ����֪ͨ��Ӧ
		/// </summary>
		virtual event RspQryTradingNotice ^OnRspQryTradingNotice;




	private:
		delegate void cppRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcTransferBankField  cspTransferBank = (CTPCommon::CThostFtdcTransferBankField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pTransferBank)), CTPCommon::CThostFtdcTransferBankField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryTransferBank(cspTransferBank, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯת��������Ӧ
		/// </summary>
		virtual event RspQryTransferBank ^OnRspQryTransferBank;




	private:
		delegate void cppRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcTransferSerialField  cspTransferSerial = (CTPCommon::CThostFtdcTransferSerialField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pTransferSerial)), CTPCommon::CThostFtdcTransferSerialField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryTransferSerial(cspTransferSerial, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯת����ˮ��Ӧ
		/// </summary>
		virtual event RspQryTransferSerial ^OnRspQryTransferSerial;




	private:
		delegate void cppRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcAccountregisterField  cspAccountregister = (CTPCommon::CThostFtdcAccountregisterField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pAccountregister)), CTPCommon::CThostFtdcAccountregisterField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQryAccountregister(cspAccountregister, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �����ѯת����ˮ��Ӧ
		/// </summary>
		virtual event RspQryAccountregister ^OnRspQryAccountregister;




	private:
		delegate void cppRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcReqQueryAccountField  cspReqQueryAccount = (CTPCommon::CThostFtdcReqQueryAccountField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pReqQueryAccount)), CTPCommon::CThostFtdcReqQueryAccountField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQueryBankAccountMoneyByFuture(cspReqQueryAccount, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �ڻ������ѯ�������Ӧ��
		/// </summary>
		virtual event RspQueryBankAccountMoneyByFuture ^OnRspQueryBankAccountMoneyByFuture;




	private:
		delegate void cppRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcQueryMaxOrderVolumeField  cspQueryMaxOrderVolume = (CTPCommon::CThostFtdcQueryMaxOrderVolumeField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pQueryMaxOrderVolume)), CTPCommon::CThostFtdcQueryMaxOrderVolumeField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspQueryMaxOrderVolume(cspQueryMaxOrderVolume, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// ��ѯ��󱨵�������Ӧ
		/// </summary>
		virtual event RspQueryMaxOrderVolume ^OnRspQueryMaxOrderVolume;




	private:
		delegate void cppRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcRemoveParkedOrderField  cspRemoveParkedOrder = (CTPCommon::CThostFtdcRemoveParkedOrderField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRemoveParkedOrder)), CTPCommon::CThostFtdcRemoveParkedOrderField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspRemoveParkedOrder(cspRemoveParkedOrder, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// ɾ��Ԥ����Ӧ
		/// </summary>
		virtual event RspRemoveParkedOrder ^OnRspRemoveParkedOrder;




	private:
		delegate void cppRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcRemoveParkedOrderActionField  cspRemoveParkedOrderAction = (CTPCommon::CThostFtdcRemoveParkedOrderActionField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRemoveParkedOrderAction)), CTPCommon::CThostFtdcRemoveParkedOrderActionField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspRemoveParkedOrderAction(cspRemoveParkedOrderAction, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// ɾ��Ԥ�񳷵���Ӧ
		/// </summary>
		virtual event RspRemoveParkedOrderAction ^OnRspRemoveParkedOrderAction;




	private:
		delegate void cppRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcSettlementInfoConfirmField  cspSettlementInfoConfirm = (CTPCommon::CThostFtdcSettlementInfoConfirmField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pSettlementInfoConfirm)), CTPCommon::CThostFtdcSettlementInfoConfirmField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspSettlementInfoConfirm(cspSettlementInfoConfirm, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// Ͷ���߽�����ȷ����Ӧ
		/// </summary>
		virtual event RspSettlementInfoConfirm ^OnRspSettlementInfoConfirm;




	private:
		delegate void cppRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcTradingAccountPasswordUpdateField  cspTradingAccountPasswordUpdate = (CTPCommon::CThostFtdcTradingAccountPasswordUpdateField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pTradingAccountPasswordUpdate)), CTPCommon::CThostFtdcTradingAccountPasswordUpdateField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspTradingAccountPasswordUpdate(cspTradingAccountPasswordUpdate, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �ʽ��˻��������������Ӧ
		/// </summary>
		virtual event RspTradingAccountPasswordUpdate ^OnRspTradingAccountPasswordUpdate;






	private:
		delegate void cppRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcRspAuthenticateField  cspRspAuthenticate = (CTPCommon::CThostFtdcRspAuthenticateField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspAuthenticateField)), CTPCommon::CThostFtdcRspUserLoginField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspAuthenticate(cspRspAuthenticate, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>

		/// 
		/// </summary>
		virtual event RspAuthenticate ^OnRspAuthenticate;




	private:
		delegate void cppRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcRspUserLoginField  cspRspUserLogin = (CTPCommon::CThostFtdcRspUserLoginField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspUserLogin)), CTPCommon::CThostFtdcRspUserLoginField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			FrontID = cspRspUserLogin.FrontID;
			SessionID = cspRspUserLogin.SessionID;
			OnRspUserLogin(cspRspUserLogin, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// ��¼������Ӧ
		/// </summary>
		virtual event RspUserLogin ^OnRspUserLogin;




	private:
		delegate void cppRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcUserLogoutField  cspUserLogout = (CTPCommon::CThostFtdcUserLogoutField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pUserLogout)), CTPCommon::CThostFtdcUserLogoutField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspUserLogout(cspUserLogout, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �ǳ�������Ӧ
		/// </summary>
		virtual event RspUserLogout ^OnRspUserLogout;




	private:
		delegate void cppRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void cppcbRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			CTPCommon::CThostFtdcUserPasswordUpdateField  cspUserPasswordUpdate = (CTPCommon::CThostFtdcUserPasswordUpdateField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pUserPasswordUpdate)), CTPCommon::CThostFtdcUserPasswordUpdateField::typeid); CTPCommon::CThostFtdcRspInfoField  cspRspInfo = (CTPCommon::CThostFtdcRspInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspInfo)), CTPCommon::CThostFtdcRspInfoField::typeid);
			OnRspUserPasswordUpdate(cspUserPasswordUpdate, cspRspInfo, nRequestID, bIsLast);
		}
	public:
		/// <summary>
		/// �û��������������Ӧ
		/// </summary>
		virtual event RspUserPasswordUpdate ^OnRspUserPasswordUpdate;




	private:
		delegate void cppRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder);
		void cppcbRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder)
		{
			CTPCommon::CThostFtdcErrorConditionalOrderField  cspErrorConditionalOrder = (CTPCommon::CThostFtdcErrorConditionalOrderField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pErrorConditionalOrder)), CTPCommon::CThostFtdcErrorConditionalOrderField::typeid);
			OnRtnErrorConditionalOrder(cspErrorConditionalOrder);
		}
	public:
		/// <summary>
		/// ��ʾ������У�����
		/// </summary>
		virtual event RtnErrorConditionalOrder ^OnRtnErrorConditionalOrder;




	private:
		delegate void cppRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer);
		void cppcbRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer)
		{
			CTPCommon::CThostFtdcRspTransferField  cspRspTransfer = (CTPCommon::CThostFtdcRspTransferField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspTransfer)), CTPCommon::CThostFtdcRspTransferField::typeid);
			OnRtnFromBankToFutureByBank(cspRspTransfer);
		}
	public:
		/// <summary>
		/// ���з��������ʽ�ת�ڻ�֪ͨ
		/// </summary>
		virtual event RtnFromBankToFutureByBank ^OnRtnFromBankToFutureByBank;




	private:
		delegate void cppRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer);
		void cppcbRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer)
		{
			CTPCommon::CThostFtdcRspTransferField  cspRspTransfer = (CTPCommon::CThostFtdcRspTransferField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspTransfer)), CTPCommon::CThostFtdcRspTransferField::typeid);
			OnRtnFromBankToFutureByFuture(cspRspTransfer);
		}
	public:
		/// <summary>
		/// �ڻ����������ʽ�ת�ڻ�֪ͨ
		/// </summary>
		virtual event RtnFromBankToFutureByFuture ^OnRtnFromBankToFutureByFuture;




	private:
		delegate void cppRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer);
		void cppcbRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer)
		{
			CTPCommon::CThostFtdcRspTransferField  cspRspTransfer = (CTPCommon::CThostFtdcRspTransferField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspTransfer)), CTPCommon::CThostFtdcRspTransferField::typeid);
			OnRtnFromFutureToBankByBank(cspRspTransfer);
		}
	public:
		/// <summary>
		/// ���з����ڻ��ʽ�ת����֪ͨ
		/// </summary>
		virtual event RtnFromFutureToBankByBank ^OnRtnFromFutureToBankByBank;




	private:
		delegate void cppRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer);
		void cppcbRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer)
		{
			CTPCommon::CThostFtdcRspTransferField  cspRspTransfer = (CTPCommon::CThostFtdcRspTransferField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspTransfer)), CTPCommon::CThostFtdcRspTransferField::typeid);
			OnRtnFromFutureToBankByFuture(cspRspTransfer);
		}
	public:
		/// <summary>
		/// �ڻ������ڻ��ʽ�ת����֪ͨ
		/// </summary>
		virtual event RtnFromFutureToBankByFuture ^OnRtnFromFutureToBankByFuture;




	private:
		delegate void cppRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);
		void cppcbRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
		{
			CTPCommon::CThostFtdcInstrumentStatusField  cspInstrumentStatus = (CTPCommon::CThostFtdcInstrumentStatusField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pInstrumentStatus)), CTPCommon::CThostFtdcInstrumentStatusField::typeid);
			OnRtnInstrumentStatus(cspInstrumentStatus);
		}
	public:
		/// <summary>
		/// ��Լ����״̬֪ͨ
		/// </summary>
		virtual event RtnInstrumentStatus ^OnRtnInstrumentStatus;




	private:
		delegate void cppRtnOrder(CThostFtdcOrderField *pOrder);
		void cppcbRtnOrder(CThostFtdcOrderField *pOrder)
		{
			CTPCommon::CThostFtdcOrderField  cspOrder = (CTPCommon::CThostFtdcOrderField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pOrder)), CTPCommon::CThostFtdcOrderField::typeid);
			OnRtnOrder(cspOrder);
		}
	public:
		/// <summary>
		/// ����֪ͨ
		/// </summary>
		virtual event RtnOrder ^OnRtnOrder;




	private:
		delegate void cppRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount);
		void cppcbRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount)
		{
			CTPCommon::CThostFtdcNotifyQueryAccountField  cspNotifyQueryAccount = (CTPCommon::CThostFtdcNotifyQueryAccountField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pNotifyQueryAccount)), CTPCommon::CThostFtdcNotifyQueryAccountField::typeid);
			OnRtnQueryBankBalanceByFuture(cspNotifyQueryAccount);
		}
	public:
		/// <summary>
		/// �ڻ������ѯ�������֪ͨ
		/// </summary>
		virtual event RtnQueryBankBalanceByFuture ^OnRtnQueryBankBalanceByFuture;




	private:
		delegate void cppRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal);
		void cppcbRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal)
		{
			CTPCommon::CThostFtdcRspRepealField  cspRspRepeal = (CTPCommon::CThostFtdcRspRepealField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspRepeal)), CTPCommon::CThostFtdcRspRepealField::typeid);
			OnRtnRepealFromBankToFutureByBank(cspRspRepeal);
		}
	public:
		/// <summary>
		/// ���з����������ת�ڻ�֪ͨ
		/// </summary>
		virtual event RtnRepealFromBankToFutureByBank ^OnRtnRepealFromBankToFutureByBank;




	private:
		delegate void cppRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal);
		void cppcbRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal)
		{
			CTPCommon::CThostFtdcRspRepealField  cspRspRepeal = (CTPCommon::CThostFtdcRspRepealField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspRepeal)), CTPCommon::CThostFtdcRspRepealField::typeid);
			OnRtnRepealFromBankToFutureByFuture(cspRspRepeal);
		}
	public:
		/// <summary>
		/// �ڻ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
		/// </summary>
		virtual event RtnRepealFromBankToFutureByFuture ^OnRtnRepealFromBankToFutureByFuture;




	private:
		delegate void cppRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal);
		void cppcbRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
		{
			CTPCommon::CThostFtdcRspRepealField  cspRspRepeal = (CTPCommon::CThostFtdcRspRepealField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspRepeal)), CTPCommon::CThostFtdcRspRepealField::typeid);
			OnRtnRepealFromBankToFutureByFutureManual(cspRspRepeal);
		}
	public:
		/// <summary>
		/// ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
		/// </summary>
		virtual event RtnRepealFromBankToFutureByFutureManual ^OnRtnRepealFromBankToFutureByFutureManual;




	private:
		delegate void cppRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal);
		void cppcbRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal)
		{
			CTPCommon::CThostFtdcRspRepealField  cspRspRepeal = (CTPCommon::CThostFtdcRspRepealField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspRepeal)), CTPCommon::CThostFtdcRspRepealField::typeid);
			OnRtnRepealFromFutureToBankByBank(cspRspRepeal);
		}
	public:
		/// <summary>
		/// ���з�������ڻ�ת����֪ͨ
		/// </summary>
		virtual event RtnRepealFromFutureToBankByBank ^OnRtnRepealFromFutureToBankByBank;




	private:
		delegate void cppRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal);
		void cppcbRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal)
		{
			CTPCommon::CThostFtdcRspRepealField  cspRspRepeal = (CTPCommon::CThostFtdcRspRepealField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspRepeal)), CTPCommon::CThostFtdcRspRepealField::typeid);
			OnRtnRepealFromFutureToBankByFuture(cspRspRepeal);
		}
	public:
		/// <summary>
		/// �ڻ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
		/// </summary>
		virtual event RtnRepealFromFutureToBankByFuture ^OnRtnRepealFromFutureToBankByFuture;




	private:
		delegate void cppRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal);
		void cppcbRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
		{
			CTPCommon::CThostFtdcRspRepealField  cspRspRepeal = (CTPCommon::CThostFtdcRspRepealField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pRspRepeal)), CTPCommon::CThostFtdcRspRepealField::typeid);
			OnRtnRepealFromFutureToBankByFutureManual(cspRspRepeal);
		}
	public:
		/// <summary>
		/// ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
		/// </summary>
		virtual event RtnRepealFromFutureToBankByFutureManual ^OnRtnRepealFromFutureToBankByFutureManual;




	private:
		delegate void cppRtnTrade(CThostFtdcTradeField *pTrade);
		void cppcbRtnTrade(CThostFtdcTradeField *pTrade)
		{
			CTPCommon::CThostFtdcTradeField  cspTrade = (CTPCommon::CThostFtdcTradeField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pTrade)), CTPCommon::CThostFtdcTradeField::typeid);
			OnRtnTrade(cspTrade);
		}
	public:
		/// <summary>
		/// �ɽ�֪ͨ
		/// </summary>
		virtual event RtnTrade ^OnRtnTrade;




	private:
		delegate void cppRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo);
		void cppcbRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo)
		{
			CTPCommon::CThostFtdcTradingNoticeInfoField  cspTradingNoticeInfo = (CTPCommon::CThostFtdcTradingNoticeInfoField) Marshal::PtrToStructure(IntPtr(&KStoCTPChange(pTradingNoticeInfo)), CTPCommon::CThostFtdcTradingNoticeInfoField::typeid);
			OnRtnTradingNotice(cspTradingNoticeInfo);
		}
	public:
		/// <summary>
		/// ����֪ͨ
		/// </summary>
		virtual event RtnTradingNotice ^OnRtnTradingNotice;






	private:
		KSTraderSpi *pUserSpi = nullptr;
		CThostFtdcTraderApi * pUserApi = nullptr;
		int lRequestID = 0;
	};

}