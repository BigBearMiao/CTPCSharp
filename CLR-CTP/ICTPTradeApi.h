using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

#include "eventDelegate.h"

namespace CTPCLR
{
	public interface class ICTPTradeApi
	{
		/// <summary>
		/// ǰ�õ�ַ
		/// </summary>
		property  System::String ^ FrontAddr;
		/// <summary>
		/// ���͹�˾����
		/// </summary>
		property  System::String ^ BrokerID;
		/// <summary>
		/// Ͷ���ߴ���
		/// </summary>
		property  System::String ^ InvestorID;
		property  System::String ^ passWord;
		property  System::String ^ FlowPath;

		/// <summary>
		/// ǰ�ñ��
		/// </summary>
		property int FrontID;

		/// <summary>
		/// �Ự���
		/// </summary>
		property int SessionID;

		/// <summary>
		/// ��󱨵�����
		/// </summary>
		property int MaxOrderRef;


		//Ͷ���߽�����ȷ��
		int SettlementInfoConfirm([Out]  int %nRequestID);


		/// <summary>
		///����¼������
		/// </summary>
		int ReqOrderInsert(CTPCommon::CThostFtdcInputOrderField pInputOrder, [Out]  int %nRequestID);

		/// <summary>
		///������������
		/// </summary>
		int ReqOrderAction(CTPCommon::CThostFtdcInputOrderActionField pInputOrderAction, [Out]  int %nRequestID);



		/// <summary>
		///�ͻ�����֤����
		/// </summary>
		int ReqAuthenticate(CTPCommon::CThostFtdcReqAuthenticateField pReqAuthenticateField, [Out]  int %nRequestID);

		/// <summary>
		///�û���¼����
		/// </summary>
		int ReqUserLogin(CTPCommon::CThostFtdcReqUserLoginField pReqUserLoginField, [Out]  int %nRequestID);

		/// <summary>
		///�ǳ�����
		/// </summary>
		int ReqUserLogout(CTPCommon::CThostFtdcUserLogoutField pUserLogout, [Out]  int %nRequestID);

		/// <summary>
		///�û������������
		/// </summary>
		int ReqUserPasswordUpdate(CTPCommon::CThostFtdcUserPasswordUpdateField pUserPasswordUpdate, [Out]  int %nRequestID);

		/// <summary>
		///�ʽ��˻������������
		/// </summary>
		int ReqTradingAccountPasswordUpdate(CTPCommon::CThostFtdcTradingAccountPasswordUpdateField pTradingAccountPasswordUpdate, [Out]  int %nRequestID);



		/// <summary>
		///Ԥ��¼������
		/// </summary>
		int ReqParkedOrderInsert(CTPCommon::CThostFtdcParkedOrderField pParkedOrder, [Out]  int %nRequestID);

		/// <summary>
		///Ԥ�񳷵�¼������
		/// </summary>
		int ReqParkedOrderAction(CTPCommon::CThostFtdcParkedOrderActionField pParkedOrderAction, [Out]  int %nRequestID);



		/// <summary>
		///��ѯ��󱨵���������
		/// </summary>
		int ReqQueryMaxOrderVolume(CTPCommon::CThostFtdcQueryMaxOrderVolumeField pQueryMaxOrderVolume, [Out]  int %nRequestID);

		/// <summary>
		///Ͷ���߽�����ȷ��
		/// </summary>
		int ReqSettlementInfoConfirm(CTPCommon::CThostFtdcSettlementInfoConfirmField pSettlementInfoConfirm, [Out]  int %nRequestID);

		/// <summary>
		///����ɾ��Ԥ��
		/// </summary>
		int ReqRemoveParkedOrder(CTPCommon::CThostFtdcRemoveParkedOrderField pRemoveParkedOrder, [Out]  int %nRequestID);

		/// <summary>
		///����ɾ��Ԥ�񳷵�
		/// </summary>
		int ReqRemoveParkedOrderAction(CTPCommon::CThostFtdcRemoveParkedOrderActionField pRemoveParkedOrderAction, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ����
		/// </summary>
		int ReqQryOrder(CTPCommon::CThostFtdcQryOrderField pQryOrder, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ�ɽ�
		/// </summary>
		int ReqQryTrade(CTPCommon::CThostFtdcQryTradeField pQryTrade, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯͶ���ֲ߳�
		/// </summary>
		int ReqQryInvestorPosition(CTPCommon::CThostFtdcQryInvestorPositionField pQryInvestorPosition, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ�ʽ��˻�
		/// </summary>
		int ReqQryTradingAccount(CTPCommon::CThostFtdcQryTradingAccountField pQryTradingAccount, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯͶ����
		/// </summary>
		int ReqQryInvestor(CTPCommon::CThostFtdcQryInvestorField pQryInvestor, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ���ױ���
		/// </summary>
		int ReqQryTradingCode(CTPCommon::CThostFtdcQryTradingCodeField pQryTradingCode, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ��Լ��֤����
		/// </summary>
		int ReqQryInstrumentMarginRate(CTPCommon::CThostFtdcQryInstrumentMarginRateField pQryInstrumentMarginRate, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ��Լ��������
		/// </summary>
		int ReqQryInstrumentCommissionRate(CTPCommon::CThostFtdcQryInstrumentCommissionRateField pQryInstrumentCommissionRate, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ������
		/// </summary>
		int ReqQryExchange(CTPCommon::CThostFtdcQryExchangeField pQryExchange, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ��Լ
		/// </summary>
		int ReqQryInstrument(CTPCommon::CThostFtdcQryInstrumentField pQryInstrument, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ����
		/// </summary>
		int ReqQryDepthMarketData(CTPCommon::CThostFtdcQryDepthMarketDataField pQryDepthMarketData, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯͶ���߽�����
		/// </summary>
		int ReqQrySettlementInfo(CTPCommon::CThostFtdcQrySettlementInfoField pQrySettlementInfo, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯת������
		/// </summary>
		int ReqQryTransferBank(CTPCommon::CThostFtdcQryTransferBankField pQryTransferBank, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯͶ���ֲ߳���ϸ
		/// </summary>
		int ReqQryInvestorPositionDetail(CTPCommon::CThostFtdcQryInvestorPositionDetailField pQryInvestorPositionDetail, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ�ͻ�֪ͨ
		/// </summary>
		int ReqQryNotice(CTPCommon::CThostFtdcQryNoticeField pQryNotice, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ������Ϣȷ��
		/// </summary>
		int ReqQrySettlementInfoConfirm(CTPCommon::CThostFtdcQrySettlementInfoConfirmField pQrySettlementInfoConfirm, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯͶ���ֲ߳���ϸ
		/// </summary>
		int ReqQryInvestorPositionCombineDetail(CTPCommon::CThostFtdcQryInvestorPositionCombineDetailField pQryInvestorPositionCombineDetail, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
		/// </summary>
		int ReqQryCFMMCTradingAccountKey(CTPCommon::CThostFtdcQryCFMMCTradingAccountKeyField pQryCFMMCTradingAccountKey, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ�ֵ��۵���Ϣ
		/// </summary>
		int ReqQryEWarrantOffset(CTPCommon::CThostFtdcQryEWarrantOffsetField pQryEWarrantOffset, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯͶ����Ʒ��/��Ʒ�ֱ�֤��
		/// </summary>
		int ReqQryInvestorProductGroupMargin(CTPCommon::CThostFtdcQryInvestorProductGroupMarginField pQryInvestorProductGroupMargin, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ��������֤����
		/// </summary>
		int ReqQryExchangeMarginRate(CTPCommon::CThostFtdcQryExchangeMarginRateField pQryExchangeMarginRate, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ������������֤����
		/// </summary>
		int ReqQryExchangeMarginRateAdjust(CTPCommon::CThostFtdcQryExchangeMarginRateAdjustField pQryExchangeMarginRateAdjust, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯת����ˮ
		/// </summary>
		int ReqQryTransferSerial(CTPCommon::CThostFtdcQryTransferSerialField pQryTransferSerial, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ����ǩԼ��ϵ
		/// </summary>
		int ReqQryAccountregister(CTPCommon::CThostFtdcQryAccountregisterField pQryAccountregister, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯǩԼ����
		/// </summary>
		int ReqQryContractBank(CTPCommon::CThostFtdcQryContractBankField pQryContractBank, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯԤ��
		/// </summary>
		int ReqQryParkedOrder(CTPCommon::CThostFtdcQryParkedOrderField pQryParkedOrder, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯԤ�񳷵�
		/// </summary>
		int ReqQryParkedOrderAction(CTPCommon::CThostFtdcQryParkedOrderActionField pQryParkedOrderAction, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ����֪ͨ
		/// </summary>
		int ReqQryTradingNotice(CTPCommon::CThostFtdcQryTradingNoticeField pQryTradingNotice, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ���͹�˾���ײ���
		/// </summary>
		int ReqQryBrokerTradingParams(CTPCommon::CThostFtdcQryBrokerTradingParamsField pQryBrokerTradingParams, [Out]  int %nRequestID);

		/// <summary>
		///�����ѯ���͹�˾�����㷨
		/// </summary>
		int ReqQryBrokerTradingAlgos(CTPCommon::CThostFtdcQryBrokerTradingAlgosField pQryBrokerTradingAlgos, [Out]  int %nRequestID);

		/// <summary>
		///�ڻ����������ʽ�ת�ڻ�����
		/// </summary>
		int ReqFromBankToFutureByFuture(CTPCommon::CThostFtdcReqTransferField pReqTransfer, [Out]  int %nRequestID);

		/// <summary>
		///�ڻ������ڻ��ʽ�ת��������
		/// </summary>
		int ReqFromFutureToBankByFuture(CTPCommon::CThostFtdcReqTransferField pReqTransfer, [Out]  int %nRequestID);

		/// <summary>
		///�ڻ������ѯ�����������
		/// </summary>
		int ReqQueryBankAccountMoneyByFuture(CTPCommon::CThostFtdcReqQueryAccountField pReqQueryAccount, [Out]  int %nRequestID);




		/// <summary>
		/// ���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
		/// </summary>
		event FrontConnect ^OnFrontConnect;

		/// <summary>
		/// ���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
		/// </summary>
		event DisConnected ^OnDisConnected;

		/// <summary>
		/// ������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
		/// </summary>
		event HeartBeatWarning ^OnHeartBeatWarning;

		/// <summary>
		/// �ڻ����������ʽ�ת�ڻ�����ر�
		/// </summary>
		event ErrRtnBankToFutureByFuture ^OnErrRtnBankToFutureByFuture;

		/// <summary>
		/// �ڻ������ڻ��ʽ�ת���д���ر�
		/// </summary>
		event ErrRtnFutureToBankByFuture ^OnErrRtnFutureToBankByFuture;

		/// <summary>
		/// ������������ر�
		/// </summary>
		event ErrRtnOrderAction ^OnErrRtnOrderAction;

		/// <summary>
		/// ����¼�����ر�
		/// </summary>
		event ErrRtnOrderInsert ^OnErrRtnOrderInsert;

		/// <summary>
		/// �ڻ������ѯ����������ر�
		/// </summary>
		event ErrRtnQueryBankBalanceByFuture ^OnErrRtnQueryBankBalanceByFuture;

		/// <summary>
		/// ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ�����ر�
		/// </summary>
		event ErrRtnRepealBankToFutureByFutureManual ^OnErrRtnRepealBankToFutureByFutureManual;

		/// <summary>
		/// ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת���д���ر�
		/// </summary>
		event ErrRtnRepealFutureToBankByFutureManual ^OnErrRtnRepealFutureToBankByFutureManual;

		/// <summary>
		/// ����Ӧ��
		/// </summary>
		event RspError ^OnRspError;

		/// <summary>
		/// �ڻ����������ʽ�ת�ڻ�Ӧ��
		/// </summary>
		event RspFromBankToFutureByFuture ^OnRspFromBankToFutureByFuture;

		/// <summary>
		/// �ڻ������ڻ��ʽ�ת����Ӧ��
		/// </summary>
		event RspFromFutureToBankByFuture ^OnRspFromFutureToBankByFuture;

		/// <summary>
		/// ��������������Ӧ
		/// </summary>
		event RspOrderAction ^OnRspOrderAction;

		/// <summary>
		/// ����¼��������Ӧ
		/// </summary>
		event RspOrderInsert ^OnRspOrderInsert;

		/// <summary>
		/// Ԥ�񳷵�¼��������Ӧ
		/// </summary>
		event RspParkedOrderAction ^OnRspParkedOrderAction;

		/// <summary>
		/// Ԥ��¼��������Ӧ
		/// </summary>
		event RspParkedOrderInsert ^OnRspParkedOrderInsert;

		/// <summary>
		/// �����ѯ���͹�˾�����㷨��Ӧ
		/// </summary>
		event RspQryBrokerTradingAlgos ^OnRspQryBrokerTradingAlgos;

		/// <summary>
		/// �����ѯ���͹�˾���ײ�����Ӧ
		/// </summary>
		event RspQryBrokerTradingParams ^OnRspQryBrokerTradingParams;

		/// <summary>
		/// ��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ
		/// </summary>
		event RspQryCFMMCTradingAccountKey ^OnRspQryCFMMCTradingAccountKey;

		/// <summary>
		/// �����ѯǩԼ������Ӧ
		/// </summary>
		event RspQryContractBank ^OnRspQryContractBank;

		/// <summary>
		/// �����ѯ������Ӧ
		/// </summary>
		event RspQryDepthMarketData ^OnRspQryDepthMarketData;

		/// <summary>
		/// �����ѯ��������Ӧ
		/// </summary>
		event RspQryExchange ^OnRspQryExchange;

		/// <summary>
		/// �����ѯ��Լ��Ӧ
		/// </summary>
		event RspQryInstrument ^OnRspQryInstrument;

		/// <summary>
		/// �����ѯ��Լ����������Ӧ
		/// </summary>
		event RspQryInstrumentCommissionRate ^OnRspQryInstrumentCommissionRate;

		/// <summary>
		/// �����ѯ��Լ��֤������Ӧ
		/// </summary>
		event RspQryInstrumentMarginRate ^OnRspQryInstrumentMarginRate;

		/// <summary>
		/// �����ѯͶ������Ӧ
		/// </summary>
		event RspQryInvestor ^OnRspQryInvestor;

		/// <summary>
		/// �����ѯͶ���ֲ߳���Ӧ
		/// </summary>
		event RspQryInvestorPosition ^OnRspQryInvestorPosition;

		/// <summary>
		/// �����ѯͶ���ֲ߳���ϸ��Ӧ
		/// </summary>
		event RspQryInvestorPositionCombineDetail ^OnRspQryInvestorPositionCombineDetail;

		/// <summary>
		/// �����ѯͶ���ֲ߳���ϸ��Ӧ
		/// </summary>
		event RspQryInvestorPositionDetail ^OnRspQryInvestorPositionDetail;

		/// <summary>
		/// �����ѯ�ͻ�֪ͨ��Ӧ
		/// </summary>
		event RspQryNotice ^OnRspQryNotice;

		/// <summary>
		/// �����ѯ������Ӧ
		/// </summary>
		event RspQryOrder ^OnRspQryOrder;

		/// <summary>
		/// �����ѯԤ����Ӧ
		/// </summary>
		event RspQryParkedOrder ^OnRspQryParkedOrder;

		/// <summary>
		/// �����ѯԤ�񳷵���Ӧ
		/// </summary>
		event RspQryParkedOrderAction ^OnRspQryParkedOrderAction;

		/// <summary>
		/// �����ѯͶ���߽�������Ӧ
		/// </summary>
		event RspQrySettlementInfo ^OnRspQrySettlementInfo;

		/// <summary>
		/// �����ѯ������Ϣȷ����Ӧ
		/// </summary>
		event RspQrySettlementInfoConfirm ^OnRspQrySettlementInfoConfirm;

		/// <summary>
		/// �����ѯ�ɽ���Ӧ
		/// </summary>
		event RspQryTrade ^OnRspQryTrade;

		/// <summary>
		/// �����ѯ�ʽ��˻���Ӧ
		/// </summary>
		event RspQryTradingAccount ^OnRspQryTradingAccount;

		/// <summary>
		/// �����ѯ���ױ�����Ӧ
		/// </summary>
		event RspQryTradingCode ^OnRspQryTradingCode;

		/// <summary>
		/// �����ѯ����֪ͨ��Ӧ
		/// </summary>
		event RspQryTradingNotice ^OnRspQryTradingNotice;

		/// <summary>
		/// �����ѯת��������Ӧ
		/// </summary>
		event RspQryTransferBank ^OnRspQryTransferBank;

		/// <summary>
		/// �����ѯת����ˮ��Ӧ
		/// </summary>
		event RspQryTransferSerial ^OnRspQryTransferSerial;

		/// <summary>
		/// �����ѯת����ˮ��Ӧ
		/// </summary>
		event RspQryAccountregister ^OnRspQryAccountregister;

		/// <summary>
		/// �ڻ������ѯ�������Ӧ��
		/// </summary>
		event RspQueryBankAccountMoneyByFuture ^OnRspQueryBankAccountMoneyByFuture;

		/// <summary>
		/// ��ѯ��󱨵�������Ӧ
		/// </summary>
		event RspQueryMaxOrderVolume ^OnRspQueryMaxOrderVolume;

		/// <summary>
		/// ɾ��Ԥ����Ӧ
		/// </summary>
		event RspRemoveParkedOrder ^OnRspRemoveParkedOrder;

		/// <summary>
		/// ɾ��Ԥ�񳷵���Ӧ
		/// </summary>
		event RspRemoveParkedOrderAction ^OnRspRemoveParkedOrderAction;

		/// <summary>
		/// Ͷ���߽�����ȷ����Ӧ
		/// </summary>
		event RspSettlementInfoConfirm ^OnRspSettlementInfoConfirm;

		/// <summary>
		/// �ʽ��˻��������������Ӧ
		/// </summary>
		event RspTradingAccountPasswordUpdate ^OnRspTradingAccountPasswordUpdate;


		/// <summary>
		/// 
		/// </summary>
		event RspAuthenticate ^OnRspAuthenticate;

		/// <summary>
		/// ��¼������Ӧ
		/// </summary>
		event RspUserLogin ^OnRspUserLogin;

		/// <summary>
		/// �ǳ�������Ӧ
		/// </summary>
		event RspUserLogout ^OnRspUserLogout;

		/// <summary>
		/// �û��������������Ӧ
		/// </summary>
		event RspUserPasswordUpdate ^OnRspUserPasswordUpdate;

		/// <summary>
		/// ��ʾ������У�����
		/// </summary>
		event RtnErrorConditionalOrder ^OnRtnErrorConditionalOrder;

		/// <summary>
		/// ���з��������ʽ�ת�ڻ�֪ͨ
		/// </summary>
		event RtnFromBankToFutureByBank ^OnRtnFromBankToFutureByBank;

		/// <summary>
		/// �ڻ����������ʽ�ת�ڻ�֪ͨ
		/// </summary>
		event RtnFromBankToFutureByFuture ^OnRtnFromBankToFutureByFuture;

		/// <summary>
		/// ���з����ڻ��ʽ�ת����֪ͨ
		/// </summary>
		event RtnFromFutureToBankByBank ^OnRtnFromFutureToBankByBank;

		/// <summary>
		/// �ڻ������ڻ��ʽ�ת����֪ͨ
		/// </summary>
		event RtnFromFutureToBankByFuture ^OnRtnFromFutureToBankByFuture;

		/// <summary>
		/// ��Լ����״̬֪ͨ
		/// </summary>
		event RtnInstrumentStatus ^OnRtnInstrumentStatus;

		/// <summary>
		/// ����֪ͨ
		/// </summary>
		event RtnOrder ^OnRtnOrder;

		/// <summary>
		/// �ڻ������ѯ�������֪ͨ
		/// </summary>
		event RtnQueryBankBalanceByFuture ^OnRtnQueryBankBalanceByFuture;

		/// <summary>
		/// ���з����������ת�ڻ�֪ͨ
		/// </summary>
		event RtnRepealFromBankToFutureByBank ^OnRtnRepealFromBankToFutureByBank;

		/// <summary>
		/// �ڻ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
		/// </summary>
		event RtnRepealFromBankToFutureByFuture ^OnRtnRepealFromBankToFutureByFuture;

		/// <summary>
		/// ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
		/// </summary>
		event RtnRepealFromBankToFutureByFutureManual ^OnRtnRepealFromBankToFutureByFutureManual;

		/// <summary>
		/// ���з�������ڻ�ת����֪ͨ
		/// </summary>
		event RtnRepealFromFutureToBankByBank ^OnRtnRepealFromFutureToBankByBank;

		/// <summary>
		/// �ڻ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
		/// </summary>
		event RtnRepealFromFutureToBankByFuture ^OnRtnRepealFromFutureToBankByFuture;

		/// <summary>
		/// ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
		/// </summary>
		event RtnRepealFromFutureToBankByFutureManual ^OnRtnRepealFromFutureToBankByFutureManual;

		/// <summary>
		/// �ɽ�֪ͨ
		/// </summary>
		event RtnTrade ^OnRtnTrade;

		/// <summary>
		/// ����֪ͨ
		/// </summary>
		event RtnTradingNotice ^OnRtnTradingNotice;

	};
}