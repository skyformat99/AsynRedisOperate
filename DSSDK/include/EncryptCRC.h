//----------------------------------------------------------------------------------------------------------------
//	���ݵ�ʵʱѹ����CRC32У�鹦��
//
//----------------------------------------------------------------------------------------------------------------

#ifndef __ENCRYPTCRC_H__
#define __ENCRYPTCRC_H__

#ifdef __WINDOWS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // ��ֹwindows.h�����winsock.h

# include <windows.h>
#else
# include "BaseType.h"
# include <string.h>
#define strcpy_s strcpy
#endif

#ifdef _MSC_VER

#include <stdlib.h>
#pragma intrinsic(_lrotr,_lrotl)
#define rotr(x,n) _lrotr(x,n)
#define rotl(x,n) _lrotl(x,n)

#else

#define rotr(x,n)   (((x) >> ((int)(n))) | ((x) << (32 - (int)(n))))
#define rotl(x,n)   (((x) << ((int)(n))) | ((x) >> (32 - (int)(n))))

#endif

class CEncryptCRC  
{
public:
	CEncryptCRC();
	virtual ~CEncryptCRC();
private:
    unsigned int	m_dwkey[44];
	int     m_cninit;
private:
	void	Encrypt(const BYTE in_blk[16], BYTE out_blk[16]);
	void	Decrypt(const BYTE in_blk[16], BYTE out_blk[16]);
public:

	//�õ������㷨��
	char *name(void);

	//����ָ��pData��CRC32ֵ
	//dwInitCrc32Ϊ��ǰ��CRC32ֵ ���԰���ǰCalcCrc32������ͽ���
	//����������һ���ڴ滺�����ݵ�CRC32ֵ�������Ϳ���һ��������ݿ��Ϊ���CalcCrc32
	//�����õ�һ��CRC32ֵ
	unsigned int CalcCrc32(LPVOID pData,int cnDataLen,unsigned int dwInitCrc32=0);

	//���ü��ܵ���Կ  pszKeyΪ�ַ�����Կ  dwKeySizeΪ��Կ����,(����λ  һ���ַ�8λ)
	void SetCryptKey(const char	*pszKey,const unsigned int dwKeySize);

	//����ָ������,�������ؼ��ܺ�����ݳ���
	//Data����������㹻�Ŀռ����ɼ��ܺ�����,
	//ע����ܺ�ĳ��Ȳ���
	//Ҫ�ﵽ���õļ���Ч��DataLen����>16���ַ�
	int EncryptData(void *Data,int DataLen);
    
	//����ָ������,�������ؽ��ܺ�����ݳ���
	//Data����������㹻�Ŀռ����ɽ��ܺ�����,ע����ܺ�ĳ��Ȳ���
	int DecryptData(void *Data,int DataLen);
};

#endif // __ENCRYPTCRC_H__

