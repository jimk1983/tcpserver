/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : default_def.h
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 
  ����޸�   :
  ��������   : Ĭ�ϴ�����һ�����ص�����
  �����б�   :
  �޸���ʷ   :

******************************************************************************/
    
/**************************************************************************************/
/* �������Ͽ���Ϊͬһ������ */
/*�����ص�Ĭ�ϵ�����������������ӿ�����*/
#define DEFAULT_GATEWAY_IFNAME                      "eth0"
/*ϵͳ������Ĭ�ϴ���һ������*/
#define DEFAULT_GWTEWAY_SSLPORT                     443
#define DEFAULT_GWTEWAY_UDPSPORT                    443
/*�����ص�Ĭ�ϵ�����*/
#define DEFAULT_GATEWAY_NAME                        "defaultfm"

#define DEFAULT_CERT_ROOTCA                         "/usr/local/cert/RootCA.crt"        /*Ĭ�ϵĿͻ�����֤CA֤��*/
#define DEFAULT_CERT_SERVERC_CERT                   "/usr/local/cert/server1.crt"       /*Ĭ�Ϸ�����֤��*/
#define DEFAULT_CERT_SERVERC_KEY                    "/usr/local/cert/server1.pem"       /*Ĭ�Ϸ�����˽Կ*/
#define DEFAULT_LOGFILE                             "/usr/local/log/syslogfile"         /*Ĭ�ϵ�ϵͳ��־�ļ�*/
#define DEFAULT_RCTLOGFILE                          "/usr/local/log/rctlogfile"         /*Ĭ�ϵ�RCT ��־�ļ�*/


/*����Ĭ�ϵļ�ͥ��Ϣ, �����ظ�*/
#define DEFAULT_GWFMINFO_DEFNAME                    "dfltfm.com"

/********************************��ǽģʽ************************************/
/*�Ƿ������·����ģʽ�����Ƿ���������ʹ��һ������
��ģʽһ������NAT����ǽӳ�������ģʽ
���������������Ĳ���ģʽ*/
#define DEFAULT_GATEWAY_ACCESS_INNER_BYPASS     0


/*���ڸ��ͻ������õ�DNS��������ַ,(Windows 7֮ǰ�Ĳ������������汾����Ҫʹ��)*/
#define DEFAULT_GATEWAY_DNSPRIM                       "10.143.23.42"
#define DEFAULT_GATEWAY_DNSSEC                        "100.100.2.138"


/******************  ϵͳ����ʱĬ�ϵ�������������                  *********************/
/*���ô�����������������*/
#define DEFAULT_GATEWAY_VTAPNAME                    "tunbridges"
/*���ô��������������ĵ�ַ���õ�ַ����ת���ͻ��˱���
��ǽģʽ�£���������������Ϊ�ͻ��˵����ص�ַ*/
/*���������ĵ�ַ, ����������ת�������ص�ַ��
����ͻ���������������ģʽ����ôֱ�����õ����������ϣ���Ϊ���ص�ַʹ�ã�
����ͻ�����SDK��ģʽ����ô��Ҫ���ͻ��˵���ʵ��ַ��ӳ��Ϊ�����ַ���ϵĵ�ַ����ΪԴIP��ַ����SNATӳ��
���磺 192.168.1.11:339 ---SNAT-->10.10.10.11:339, --->Gwд�룬��������ض�Ӧ�ˣ��ǾͲ���ӳ���� */

/*�����ǰ�汾�������ͻ��˵�SNAT,��ôSDK����ʵ���û�������ʵ������ͬ���μ���*/
#define DEFAULT_GATEWAY_VTAPIP                      "192.168.3.1"    
#define DEFAULT_GATEWAY_VTAPIPMASK                  "255.255.255.0"

//��Ҫ����: -- ����һ��natӳ�䣬�����ַӳ��Ϊeth0���ڵ�ַ
//iptables -t nat -A POSTROUTING -s 192.168.3.0/24 -o eth0 -j MSAQUERADE
//����һ������: /etc/sysctl.conf �е�net_ipv4_forward=1, ִ��sysctl -p���ɡ�

/*Ĭ�����õ������ַ����ʼ��ַ*/
#define DEFAULT_GATEWAY_NETPOOL_START               "192.168.3.100"
#define DEFAULT_GATEWAY_NETPOOL_END                 "192.168.3.200"

/********************************����������Ĭ������************************************/
/*��Ĭ�����ص����������ӿ�����*/
#define DEFAULT_GATEWAY_INNER_IFNAME                "eth0"



